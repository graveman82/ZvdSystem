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

#ifndef ZV3D_SYS_COMMONMACROS_H
#define ZV3D_SYS_COMMONMACROS_H

#define ZVD_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define ZVD_MAX(a,b) (((a) > (b)) ? (a) : (b))

#define ZVD_MAX3(a,b,c) ZVD_MAX(ZVD_MAX((a),(b)), (c))

//-----------------------------------------------------------------------------
/// Unused variable wrapper.
#define ZVD_UNUSED(x) ((void)sizeof(x))

//-----------------------------------------------------------------------------
/// FOURCC - packs characters into 32-bit unsigned int.
#define ZVD_FOURCC(ch0, ch1, ch2, ch3)      \
   (((ZvdSysUInt32(ch0) & 0xFF)      ) |       \
    ((ZvdSysUInt32(ch1) & 0xFF) << 8 ) |       \
    ((ZvdSysUInt32(ch2) & 0xFF) << 16) |       \
    ((ZvdSysUInt32(ch3) & 0xFF) << 24) )

#endif // ZV3D_SYS_COMMONMACROS_H
