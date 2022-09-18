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
Purpose: implementation of debug tools.

----------------------
 for developers notes
----------------------

*/

#include <string>
#include <assert.h>

#ifndef ZVD_USE_TEMPLATE_ARGS_FOR_PRINT
#   include <cstdarg>
#else
#   error "Feature is not implemented yet"
#endif

#include "Debug/ZvdSysAssert.h"

#ifdef ZVD_USE_MSGBOX_FOR_ASSERT
#   ifndef ZVD_SYS_OS_WIN
#       error "message box for assert messaging is unsupported on nowindows platforms"
#   endif
#   include <windows.h>
#endif // ZVD_USE_MSGBOX_FOR_ASSERT

#include <new>
namespace zvd
{

namespace debug
{

AssertPoint* AssertPointCtor::operator()(void* p)
{
    return ::new(p) AssertPoint();
}

typedef std::basic_string<AssertPoint::CharType> CStdString;

//-----------------------------------------------------------------------------
// Implementation of output assert message function.
#ifndef ZVD_USE_TEMPLATE_ARGS_FOR_PRINT
void OutputAssertMessage(AssertPoint::CStringType fmt, ...)
{

    if (!AssertPoint::Instance().exp())
    {
        return;
    }

    CStdString strMsg = ZVD_ASSERT_TEXT("[ASSERT]: \"");
    strMsg += AssertPoint::Instance().exp();
    strMsg += ZVD_ASSERT_TEXT("\" failed in \"");
    strMsg += AssertPoint::Instance().fileAndLine();
    strMsg += ZVD_ASSERT_TEXT(".\n");
    AssertPoint::Instance().Clear();

    if (fmt)
    {
        va_list args;
        va_start(args, fmt);

        // evaluating buffer size for "details"
        va_list args2;
        va_copy(args2, args);
#ifdef ZVD_DEBUG_CHARTYPE_WIDE
        int nBufSize = _vscwprintf(fmt, args2) + 1;
#else
        int nBufSize = _vscprintf(fmt, args2) + 1;
#endif
        va_end(args2);

        const int kStaticBufSize = 256;
        AssertPoint::CharType static_buf[kStaticBufSize] = {};
        AssertPoint::CharType* p = static_buf;
        bool fStaticBuf = true;
        if (nBufSize > kStaticBufSize)
        {
            p = new AssertPoint::CharType[nBufSize];
            fStaticBuf = false;
        }
#ifdef ZVD_DEBUG_CHARTYPE_WIDE
        nBufSize = _vsnwprintf(p, nBufSize, fmt, args);
#else
        nBufSize = _vsnprintf(p, nBufSize, fmt, args);
#endif
        va_end(args);

        if (nBufSize == -1)
        {
            if (!fStaticBuf)
                delete [] p;
            return;
        }

        strMsg += ZVD_ASSERT_TEXT("Details: ");
        strMsg += p;
        strMsg += ZVD_ASSERT_TEXT("\n");
        if (!fStaticBuf)
            delete [] p;
    }

#ifdef ZVD_USE_MSGBOX_FOR_ASSERT
#   ifdef ZVD_DEBUG_CHARTYPE_WIDE
    ::MessageBoxW(0, strMsg.c_str(), ZVD_ASSERT_TEXT("Assert failed!"), MB_ICONERROR | MB_OK | MB_TOPMOST);
#   else
    ::MessageBoxA(0, strMsg.c_str(), ZVD_ASSERT_TEXT("Assert failed!"), MB_ICONERROR | MB_OK | MB_TOPMOST);
#   endif
#else
#   ifdef ZVD_DEBUG_CHARTYPE_WIDE
    wprintf(strMsg.c_str());
#   else
    printf(strMsg.c_str());
#   endif
#endif // ZVD_USE_MSGBOX_FOR_ASSERT

}
#endif // ZVD_USE_TEMPLATE_ARGS_FOR_PRINT

} // end of debug
} // end of zvd
