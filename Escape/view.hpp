
#ifndef view_hpp
#define view_hpp

#include <iostream>
#include <SDL_image.h>
#include "model.hpp"

class Game;
class Button;
class Actor;

class View
{
    Model *model;
    SDL_Texture *little_beans;
public:
//    std::vector<Button *> *DropInButton(Game &gm);//???
    View(Model &game);
    
    bool Inscreen(double px,double py);
    void DrawWorld();
    void DrawMap(double stdx,double stdy,SDL_Rect print,SDL_Rect sticker);
    void DrawMiniMap(double std_x,double std_y);
};

class Cutscenes
{
public:
    Ct_Model model;
    View view;
    Cutscenes(int type,int times);
    
};

#endif /* view_hpp */
