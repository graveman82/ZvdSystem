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

#ifndef ZV3D_SYS_PLATFORM_H
#define ZV3D_SYS_PLATFORM_H


//=============================================================================
// Compiler detection --{

#if defined (__GNUC__)


#   define ZVD_SYS_GCC_COMPILER

#   define ZVD_SYS_GCC_VERSION_TO_INT(major,minor,patchlevel) (\
    major * 10000 + minor * 100 + patchlevel)

#   define ZVD_SYS_GCC_VERSION_GR_OR_EQ(major,minor,patchlevel) (\
    ZVD_SYS_GCC_VERSION_TO_INT(__GNUC__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__) >=\
    ZVD_SYS_GCC_VERSION_TO_INT(major,minor,patchlevel))

#if !ZVD_SYS_GCC_VERSION_GR_OR_EQ(8,1,0)
#   error "ZvdSystem: guarantied only gcc 8.1 or higher"
#endif

//elif // for easy add new compilers detection
#else
#   error "ZvdSystem: your compiler isn't supported yet. Please try another one."

#endif // Compiler detection
// }-- Compiler detection
//=============================================================================


//=============================================================================
// Operation system detection --{
#ifdef ZVD_SYS_GCC_COMPILER

#   if defined(_WIN32) || defined(__WIN32__)
#       define ZVD_SYS_OS_WIN
#       define ZVD_SYS_OS_WIN32

#   endif // WIN32


#endif

// }-- Operation system detection
//=============================================================================


//=============================================================================
// Fixed size integer typedefs --{
#ifdef ZVD_SYS_GCC_COMPILER
#   include <stdint.h>
typedef int8_t      ZvdSysInt8;
typedef int16_t     ZvdSysInt16;
typedef int32_t     ZvdSysInt32;
typedef int64_t     ZvdSysInt64;

typedef uint8_t     ZvdSysUInt8;
typedef uint16_t    ZvdSysUInt16;
typedef uint32_t    ZvdSysUInt32;
typedef uint64_t    ZvdSysUInt64;
#endif

// }-- Fixed size integer typedefs
//=============================================================================

//=============================================================================
// API macro --{
/** Example of defining export:
    @code
    ZVD_SYS_EXPORTS_BEGIN

    void ZVD_SYS_API SomeFunction(const LPCSTR sometext);
    void ZVD_SYS_API SomeFunction2(const LPCSTR sometext);
    //...
    void ZVD_SYS_API SomeFunctionN(const LPCSTR sometext);

    ZVD_SYS_EXPORTS_END
    @endcode
*/
#ifdef ZVD_SYS_OS_WIN
#   ifndef ZVD_SYS_API
#       ifdef BUILD_DLL
#           define ZVD_SYS_API __declspec(dllexport)
#       else
#           define ZVD_SYS_API __declspec(dllimport)
#       endif
#   endif // ZVD_SYS_API

#ifdef __cplusplus
#   define ZVD_SYS_EXPORTS_BEGIN extern "C" {
#else
#   define ZVD_SYS_EXPORTS_BEGIN
#endif

#ifdef __cplusplus
#   define ZVD_SYS_EXPORTS_END }
#else
#   define ZVD_SYS_EXPORTS_END
#endif

#endif

// }-- API macro
//=============================================================================

#endif // ZV3D_SYS_PLATFORM_H
