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

#ifndef ZV3D_SYS_SYSTEM_H
#define ZV3D_SYS_SYSTEM_H

#include "ZvdSystemDefs.h"

//=============================================================================
// DLL Interface definitions --{

//ZVD_SYS_EXPORTS_BEGIN
//-----------------------------------------------------------------------------

eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysAllocate(ZvdSysUInt32 nBytes, void** pResult);

//-----------------------------------------------------------------------------
eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysDeallocate(void* p);

//-----------------------------------------------------------------------------
eZvdSysErrorCode /*ZVD_SYS_API*/ ZvdSysInitPools(ZvdSysUInt32* pSizes, ZvdSysUInt32 nSizes);

//ZVD_SYS_EXPORTS_END
// }-- DLL Interface definitions
//=============================================================================

#endif // ZV3D_SYS_SYSTEM_H
