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

#ifndef ZV3D_SYS_SYSTEMDEFS_H
#define ZV3D_SYS_SYSTEMDEFS_H

#include "ZvdSysPlatform.h"

//-----------------------------------------------------------------------------
enum eZvdSysErrorCode
{
    kZVD_SYS_ERRC_UNKNOWN = -1,         ///< uncategorized error
    kZVD_SYS_ERRC_OK = 1,               ///< no error (success)
    kZVD_SYS_ERRC_NOT_ENOUGH_MEM,       ///< not enough memory for allocation request
    kZVD_SYS_ERRC_FILE_NOT_FOUND,       ///< file or directory not found
    kZVD_SYS_ERRC_NOT_MEMBLOCK,         ///< trying free memory not allocated by memory manager
};

#endif // ZV3D_SYS_SYSTEMDEFS_H
