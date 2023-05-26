#ifndef tool_hpp
#define tool_hpp

#include <SDL2/SDL.h>
#include <iostream>

class Skill
{
public:
    int type;
    double rate;
    double fix_speed;
    SDL_Rect from;
    SDL_Texture *skill_picture;
    
    Skill(int tp);
};

#endif /* tool_hpp */