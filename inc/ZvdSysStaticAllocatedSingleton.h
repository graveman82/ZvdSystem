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
Purpose: static singleton pattern definition.

----------------------
 for developers notes
----------------------

*/

#ifndef ZVD_SYS_STATICALLOCATEDSINGLETON_H
#define ZVD_SYS_STATICALLOCATEDSINGLETON_H

#include "ZvdSysPlatform.h"


namespace zvd
{

//-----------------------------------------------------------------------------
/** @class StaticAllocatedSingleton
    An object instance is allocated in static buffer.
*/
template <typename T, ZvdSysUInt32 kBufferSize, typename TPseudoCtor>
class StaticAllocatedSingleton
{
public:
    StaticAllocatedSingleton() = default;

    /// Returns reference to a single instance of given template type.
    static T& Instance()
    {
        if (!sInstance_)
        {
            sInstance_ = TPseudoCtor() (&sBuffer_[0]);
        }
        return *sInstance_;
    }

    /// Destroys the instance explicitly.
    static void Destroy()
    {
        if (sInstance_)
        {
            sInstance_->~T();
            sInstance_ = nullptr;
        }
    }

private:

    // not allowed
    StaticAllocatedSingleton(const StaticAllocatedSingleton<T, kBufferSize, TPseudoCtor>&) = delete;
    StaticAllocatedSingleton& operator=(const StaticAllocatedSingleton<T, kBufferSize, TPseudoCtor>&) = delete;

    static T* sInstance_;
    static ZvdSysInt8 sBuffer_[kBufferSize];
};

//-----------------------------------------------------------------------------
template <typename T, ZvdSysUInt32 kBufferSize, typename TPseudoCtor>
T* StaticAllocatedSingleton<T, kBufferSize, TPseudoCtor>::sInstance_ = nullptr;

//-----------------------------------------------------------------------------
template <typename T, ZvdSysUInt32 kBufferSize, typename TPseudoCtor>
ZvdSysInt8 StaticAllocatedSingleton<T, kBufferSize, TPseudoCtor>::sBuffer_[kBufferSize];

} // end of zvd

#endif // ZVD_SYS_STATICALLOCATEDSINGLETON_H
