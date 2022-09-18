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
Purpose: static assert macro definition.

----------------------
 for developers notes
----------------------

*/

#ifndef ZV3D_DEBUG_STATICASSERT_H
#define ZV3D_DEBUG_STATICASSERT_H

//=============================================================================
// static assert
#define ZVD_STATIC_ASSERT_CONCAT_(x,y) x ## y
#define ZVD_STATIC_ASSERT_CONCAT(x,y) ZVD_STATIC_ASSERT_CONCAT_(x,y)
#define ZVD_STATIC_ASSERT_CONCAT_WITH_LINE(x) \
    ZVD_STATIC_ASSERT_CONCAT(x, __LINE__)

/// Static assert macro. Failed if false expression is passed when compile time.
#define ZVD_STATIC_ASSERT(expr) namespace zvdStaticAssertNamespace { \
typedef char ZVD_STATIC_ASSERT_CONCAT_WITH_LINE(buf_t) [(!!(expr) * 2) - 1]; \
}

#define ZVD_STATIC_ASSERT_T(expr, name) struct ZvdStaticAssert_Unused_##name \
{typedef char buf_t[(!!(expr) * 2) - 1];}

#endif // ZV3D_DEBUG_STATICASSERT_H
