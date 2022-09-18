/*
-----------------
 Persistent info
-----------------
(C) 2012-2022 Zavod3D project. Marat Sungatullin

.........
License:
.........

is in the "LICENSE" file.

......
 Web:
......

 + https://gamedev.ru/community/trash_team/forum/?id=192020 (for questions and help)

------
 Desc
------
Purpose: .

----------------------
 for developers notes
----------------------

*/

#include "ZvdSysPlatform.h"

#ifdef ZVD_SYS_OS_WIN
#include <windows.h>
#endif

#include "ZvdSysStaticAllocatedSingleton.h"
#include "ZvdSysCommonMacros.h"
#include "Debug/ZvdSysAssert.h"
#include "ZvdSystemDefs.h"
#include "Debug/ZvdSysDebugDefs.h"

#include <new>
#include <vector>
#include <algorithm>
#include <memory>

namespace zvd
{

//-----------------------------------------------------------------------------
class MemoryBlock
{
public:
    MemoryBlock(void* ptr, ZvdSysUInt32 nBytes)
        : ptr_(ptr)
        , nBytes_(nBytes)
        , headerEndMarker_(ZVD_FOURCC('Z', 'v', '3', 'D'))
    {
        ZVD_ASSERT_HIGH_NOMSG(ptr);
        ZVD_ASSERT_HIGH_NOMSG(nBytes);
    }

    ~MemoryBlock()
    {
        RemoveFromRegList();
        ptr_ = nullptr;
        nBytes_ = 0;
        headerEndMarker_ = 0;
    }

    void RemoveFromRegList()
    {
        if (regNext_)
        {
            regNext_->regPrev_ = regPrev_;
        }

        if (regPrev_)
        {
            regPrev_->regNext_ = regNext_;
        }

        regPrev_ = regNext_ = nullptr;

    }
    void* ptr_{};
    ZvdSysUInt32 nBytes_{};


    // for list
    MemoryBlock* regNext_{};
    MemoryBlock* regPrev_{};
    ZvdSysUInt32 headerEndMarker_;
};


//=============================================================================
// Low level routines
static ZvdSysUInt8* NewBytes(ZvdSysUInt32 nBytes)
{
    return new(std::nothrow) ZvdSysUInt8[nBytes]{};
}

static void DeleteBytes(ZvdSysUInt8* pBytes)
{
    ZVD_ASSERT_HIGH_NOMSG(pBytes);
    delete [] pBytes;
}

static void DeleteBlock(MemoryBlock* pBlock, bool fromPool = false)
{
    ZVD_ASSERT_HIGH_NOMSG(pBlock);
    if (fromPool)
    {
        return;
    }
    ZvdSysUInt8* pBytes = reinterpret_cast<ZvdSysUInt8*>(pBlock);
    pBlock->RemoveFromRegList();
    pBlock->~MemoryBlock();
    DeleteBytes(pBytes);
}

//-----------------------------------------------------------------------------
class MemoryManager;
struct MemoryManagerCtor
{
    MemoryManager* operator()(void* p);
};
static const ZvdSysUInt32 kMemoryManagerInstanceSize = 2 * (sizeof(std::vector<int>) + sizeof(MemoryBlock*));
//-----------------------------------------------------------------------------
class MemoryManager : public StaticAllocatedSingleton<MemoryManager, kMemoryManagerInstanceSize, MemoryManagerCtor>
{
    typedef std::shared_ptr<MemoryBlock> BlockSPtr;
    struct Pool
    {
        Pool(ZvdSysUInt32 nBlockSize)
            : nBlockSize_(nBlockSize)
        {
            ZVD_ASSERT_HIGH_NOMSG(nBlockSize >= 4);
        }
        std::vector<BlockSPtr> blocks_;
        ZvdSysUInt32 nBlockSize_{};
    };

    typedef std::shared_ptr<Pool> PoolSPtr;

    friend class StaticAllocatedSingleton<MemoryManager, kMemoryManagerInstanceSize, MemoryManagerCtor>;
    friend class MemoryManagerCtor;
public:
    MemoryManager() = default;
    ~MemoryManager();
    eZvdSysErrorCode Allocate(ZvdSysUInt32 nBytes, void** pResult);
    eZvdSysErrorCode Deallocate(void* p);

    eZvdSysErrorCode InitPools(ZvdSysUInt32* pSizes, ZvdSysUInt32 nSizes);
private:
    std::vector<PoolSPtr>::iterator FindPool(MemoryBlock* pBlock, ZvdSysUInt32 nBytes = 0);
    bool TryPutInPool(MemoryBlock* pBlock);
    bool TryGetFromPool(MemoryBlock** ppBlock, ZvdSysUInt32 nBytes);

    MemoryBlock* pBlocks_{};
    std::vector<PoolSPtr> pools_;
};

//-----------------------------------------------------------------------------
MemoryManager* MemoryManagerCtor::operator()(void* p)
{
    return ::new(p) MemoryManager();
}

//-----------------------------------------------------------------------------
MemoryManager::~MemoryManager()
{
    for (auto& poolSPtr : pools_)
    {
        for (auto& blockSPtr : poolSPtr->blocks_)
        {
            DeleteBlock(blockSPtr.get());
            blockSPtr = nullptr;
        }
    }
}
//-----------------------------------------------------------------------------

eZvdSysErrorCode MemoryManager::Allocate(ZvdSysUInt32 nBytes, void** pResult)
{
    MemoryBlock* pBlock{};
    if (TryGetFromPool(&pBlock, nBytes))
    {
        *pResult = pBlock;
        return kZVD_SYS_ERRC_OK;
    }

    ZvdSysUInt32 nReqBytes = nBytes + sizeof(MemoryBlock);
    ZvdSysUInt8* pBytes = NewBytes(nReqBytes);
    if (!pBytes)
    {
        return kZVD_SYS_ERRC_NOT_ENOUGH_MEM;
    }

    pBlock = new(pBytes) MemoryBlock(pBytes + sizeof(MemoryBlock), nBytes);
    *pResult = pBlock->ptr_;

    if (!pBlocks_)
    {
        pBlocks_ = pBlock;
    }
    else
    {
        pBlock->regNext_ = pBlocks_;
        pBlocks_->regPrev_ = pBlock;
        pBlocks_ = pBlock;
    }
    return kZVD_SYS_ERRC_OK;
}

//-----------------------------------------------------------------------------
eZvdSysErrorCode MemoryManager::Deallocate(void* p)
{
    ZVD_ASSERT_HIGH_NOMSG(p);
    ZvdSysUInt8* pBytes = reinterpret_cast<ZvdSysUInt8*>(p);
    ZvdSysUInt32* pHeaderMarker = reinterpret_cast<ZvdSysUInt32*>(pBytes - sizeof(ZvdSysUInt32));
    if (pHeaderMarker[0] != ZVD_FOURCC('Z', 'v', '3', 'D'))
    {
        ZvdErrorMessageBox("Trying to free not a memory block allocated by ZvdSystem: %p\n", p);
        return kZVD_SYS_ERRC_NOT_MEMBLOCK;
    }
    pBytes -= sizeof(MemoryBlock);
    MemoryBlock* pBlock = reinterpret_cast<MemoryBlock*>(pBytes);
    if (TryPutInPool(pBlock))
    {
        return kZVD_SYS_ERRC_OK;
    }
    DeleteBlock(pBlock);
    return kZVD_SYS_ERRC_OK;
}

//-----------------------------------------------------------------------------
eZvdSysErrorCode MemoryManager::InitPools(ZvdSysUInt32* pSizes, ZvdSysUInt32 nSizes)
{
    ZVD_ASSERT_HIGH_NOMSG(pSizes);
    ZVD_ASSERT_HIGH_NOMSG(nSizes);
    try
    {
        pools_.reserve(nSizes);
    }
    catch(const std::bad_alloc& exc)
    {
        ZvdErrorMessageBox("Not enough memory on system for pools\n");
        return kZVD_SYS_ERRC_NOT_ENOUGH_MEM;
    }

    for (ZvdSysUInt32 i = 0; i < nSizes; ++i)
    {
        try
        {
            PoolSPtr poolSPtr(new Pool(pSizes[i]));
            auto it = std::lower_bound(pools_.begin(), pools_.end(), poolSPtr,
                             [](const PoolSPtr& pool1, const PoolSPtr& pool2)
                             {
                                 return pool1->nBlockSize_ < pool2->nBlockSize_;
                             });
            pools_.insert(it, poolSPtr);
        }
        catch(const std::bad_alloc& exc)
        {
            ZvdErrorMessageBox("Not enough memory on system for pools\n");
            return kZVD_SYS_ERRC_NOT_ENOUGH_MEM;
        }
    }
    return kZVD_SYS_ERRC_OK;
}

//-----------------------------------------------------------------------------
std::vector<MemoryManager::PoolSPtr>::iterator MemoryManager::FindPool(MemoryBlock* pBlock, ZvdSysUInt32 nBytes)
{
    ZvdSysUInt32 nBlockSize = nBytes;
    if (!nBlockSize)
    {
        ZVD_ASSERT_HIGH_NOMSG(pBlock->nBytes_);
        nBlockSize = pBlock->nBytes_;
    }

    auto Find = [](const PoolSPtr& pool1, ZvdSysUInt32 nBlockSize)
    {
        return pool1->nBlockSize_ < nBlockSize;
    };

    auto Find2 = [](ZvdSysUInt32 nBlockSize, const PoolSPtr& pool1)
    {
        return nBlockSize < pool1->nBlockSize_;
    };

    auto itl = std::lower_bound(pools_.begin(), pools_.end(), nBlockSize, Find);
    if (pools_.end() == itl)
    {
        return pools_.end();
    }

    auto itu = std::upper_bound(pools_.begin(), pools_.end(), nBlockSize, Find2);
    if (pools_.end() == itu)
    {
        return pools_.end();
    }

    if (itl == itu)
    {
        return pools_.end();
    }
    return itl;

}
//-----------------------------------------------------------------------------
bool MemoryManager::TryPutInPool(MemoryBlock* pBlock)
{
    auto it = FindPool(pBlock);
    if (pools_.end() == it)
    {
        return false;
    }
    (*it)->blocks_.push_back(BlockSPtr(pBlock, [](MemoryBlock* pBlock)
                                    {
                                        DeleteBlock(pBlock, true);
                                    }));
    return true;
}

//-----------------------------------------------------------------------------
bool MemoryManager::TryGetFromPool(MemoryBlock** ppBlock, ZvdSysUInt32 nBytes)
{
    auto it = FindPool(nullptr, nBytes);
    if (pools_.end() == it)
    {
        return false;
    }
    if ((*it)->blocks_.empty())
    {
        return false;
    }

    *ppBlock = (*it)->blocks_.back().get();
    (*it)->blocks_.pop_back();

    return true;
}
} // end of zvd

//=============================================================================
// DLL Interface implementation --{

//-----------------------------------------------------------------------------

eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysAllocate(ZvdSysUInt32 nBytes, void** pResult)
{
    eZvdSysErrorCode errCode = zvd::MemoryManager::Instance().Allocate(nBytes, pResult);
    if (errCode != kZVD_SYS_ERRC_OK)
    {
        if (kZVD_SYS_ERRC_NOT_ENOUGH_MEM == errCode)
        {
            ZvdErrorMessageBox("Not enough memory on system. Requested %u bytes\n", nBytes);
        }

        return errCode;
    }

    return errCode;
}

//-----------------------------------------------------------------------------
eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysDeallocate(void* p)
{
    return zvd::MemoryManager::Instance().Deallocate(p);
}


//-----------------------------------------------------------------------------
eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysInitPools(ZvdSysUInt32* pSizes, ZvdSysUInt32 nSizes)
{
    return zvd::MemoryManager::Instance().InitPools(pSizes, nSizes);
}
// }-- DLL Interface implementation
//=============================================================================
