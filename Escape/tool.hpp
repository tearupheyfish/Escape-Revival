#ifndef tool_hpp
#define tool_hpp

#include <SDL2/SDL.h>
#include <iostream>

class Tool
{
};

class Skill:public Tool
{
public:
    int type;
    double rate;
    double fix_speed;
    SDL_Rect from;
    SDL_Texture *skill_picture;     //资源池，不用管
    
    Skill(int tp);
};

#endif /* tool_hpp */