//
//  tool.cpp
//  Escape
//
//  Created by Digger on 2020/6/15.
//  Copyright © 2020 Digger. All rights reserved.
//

#include "tool.hpp"

Skill::Skill(int tp):
type(tp),
rate(1.0)
{
    from.x=from.y=0;
    from.w=114,
    from.h=rate*114;
    switch (tp)
    {
        case 0:
            fix_speed=1./1200;
            break;
        default:
            fix_speed=0.01;
            break;
    }
}
