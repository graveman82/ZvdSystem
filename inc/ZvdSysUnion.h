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

#ifndef ZV3D_SYS_UNION_H
#define ZV3D_SYS_UNION_H

#include "ZvdSysPlatform.h"
#include "ZvdSysCommonMacros.h"

template <typename T1, typename T2, typename T3>
class ZvdUnion3
{
public:
    enum { kSIZE = ZVD_MAX3(sizeof(T1), sizeof(T2), sizeof(T3)) };

    T1& getT1() { return *reinterpret_cast<T1*>(&data_[0]); }
    T2& getT2() { return *reinterpret_cast<T2*>(&data_[0]); }
    T3& getT3() { return *reinterpret_cast<T3*>(&data_[0]); }
private:
    ZvdSysUInt8 data_[kSIZE];
};

#endif // ZV3D_SYS_UNION_H
