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
Purpose: stringify macros.

----------------------
 for developers notes
----------------------

*/

#ifndef ZVD_SYS_STRINGIFY_H
#define ZVD_SYS_STRINGIFY_H

//-----------------------------------------------------------------------------
// Widen predefined macro and stringification support
#define ZVD_WIDECHAR(x)          ZVD_WIDECHAR_(x)
#define ZVD_WIDECHAR_(x)         L ## x
#define ZVD_NARROWCHAR(x)        x

#define ZVD_STRINGIFY(x, t)     ZVD_STRINGIFY_(x, t)
#define ZVD_STRINGIFY_(x, t)    t(#x)

#define ZVD_FILE_AND_LINE_(t)  t(__FILE__) t("[") ZVD_STRINGIFY(__LINE__, t) t("]")

#ifdef ZVD_DEBUG_CHARTYPE_WIDE
#   define ZVD_FILE_AND_LINE       ZVD_FILE_AND_LINE_(ZVD_WIDECHAR)
#   define ZVD_DEBUG_STRINGIFY(x)  ZVD_STRINGIFY(x,ZVD_WIDECHAR)
#else
#   define ZVD_FILE_AND_LINE       ZVD_FILE_AND_LINE_(ZVD_NARROWCHAR)
#   define ZVD_DEBUG_STRINGIFY(x)  ZVD_STRINGIFY(x,ZVD_NARROWCHAR)
#endif

#endif // ZVD_SYS_STRINGIFY_H
