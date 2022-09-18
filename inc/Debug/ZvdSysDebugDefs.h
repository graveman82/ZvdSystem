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
Purpose: debug subsystem definitions.

----------------------
 for developers notes
----------------------

*/

#ifndef ZVD_SYS_DEBUG_DEBUGDEFS_H
#define ZVD_SYS_DEBUG_DEBUGDEFS_H


#include "ZvdSysPlatform.h"
#include "ZvdSysStringify.h"
#include "ZvdSysUnion.h"
#include "ZvdSystemDefs.h"

//-----------------------------------------------------------------------------
/// Stringifies class or other type name.
#define ZVD_CLASSNAME(cls) ZVD_STRINGIFY(cls, ZVD_NARROWCHAR)


//-----------------------------------------------------------------------------
//
#define kZVD_ERROR_DESC_TEXTUAL_INFO_SIZE 512
class ZvdErrorDesc
{
public:
    typedef ZvdUnion3<int, float, void*> Data;
    ZvdErrorDesc(eZvdSysErrorCode errorCode = kZVD_SYS_ERRC_OK,
                 const char* initiatorName = "",
                 const char* textualInfo = "",
                 ZvdSysUInt32 textualInfoLen = 0,
                 const Data& data = Data());

    eZvdSysErrorCode ErrorCode() const { return errorCode_; }
    const char* InitiatorName() const { return initiatorName_; }
    const char* TextualInfo() const { return &textualInfo_[0]; }
    const Data& GetData() const { return data_; }

    ZvdErrorDesc& operator=(const ZvdErrorDesc& oth);
private:
    eZvdSysErrorCode errorCode_;
    const char* initiatorName_;
    char textualInfo_[kZVD_ERROR_DESC_TEXTUAL_INFO_SIZE];
    Data data_;
};

void ZvdErrorMessageBox(const char* msg, ...);
#endif // ZVD_SYS_DEBUG_DEBUGDEFS_H
