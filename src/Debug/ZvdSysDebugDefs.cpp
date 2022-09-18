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

#include "Debug/ZvdSysDebugDefs.h"
#include "Debug/ZvdSysAssert.h"
#include <string.h>

#ifdef ZVD_SYS_OS_WIN
#include <windows.h>
#endif
#include <stdio.h>
#include <stdarg.h>

//-----------------------------------------------------------------------------

ZvdErrorDesc::ZvdErrorDesc( eZvdSysErrorCode errorCode,
                             const char* initiatorName,
                             const char* textualInfo,
                             ZvdSysUInt32 textualInfoLen,
                             const Data& data)
    : errorCode_(errorCode)
    , initiatorName_(initiatorName)
    , data_(data)
{
    strncpy(textualInfo_, textualInfo,
            ZVD_MIN(textualInfoLen, (ZvdSysUInt32)kZVD_ERROR_DESC_TEXTUAL_INFO_SIZE));
}

//-----------------------------------------------------------------------------
ZvdErrorDesc& ZvdErrorDesc::operator=(const ZvdErrorDesc& oth)
{
    if (this != &oth)
    {
        errorCode_ = oth.ErrorCode();
        initiatorName_ = oth.InitiatorName();
        strcpy(textualInfo_, oth.TextualInfo());
        data_ = oth.GetData();
    }
    return *this;
}

//-----------------------------------------------------------------------------
void ZvdErrorMessageBox(const char* fmt, ...)
{
    va_list args1;
    va_start(args1, fmt);

    va_list args2;
    va_copy(args2, args1);

    static char buf[1024] = {};
    int needSize = 1 + vsnprintf(NULL, 0, fmt, args1);
    ZVD_ASSERT_HIGH_NOMSG(needSize <= 1024);
    va_end(args1);

    vsnprintf(buf, sizeof(buf), fmt, args2);
    va_end(args2);

    ::MessageBoxA(0, buf, "ZvdSystem Error", MB_OK | MB_TOPMOST | MB_ICONERROR);
}
