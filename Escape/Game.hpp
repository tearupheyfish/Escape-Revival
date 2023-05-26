#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include "controller.hpp"

class Game                          //--游戏基类
{
public:
    virtual void RunGame()=0;
    virtual ~Game()=default;
    
    /*friend*/
};

class MainMenu:public Game          //-主菜单
{
    MM_Model model;
    View view;
    MM_Controller controller;
public:
    MainMenu();
    void RunGame() override;
    ~MainMenu()=default;
    
};

class PauseMenu:public Game         //-暂停菜单
{
    PA_Model model;
    View view;
    PA_Controller controller;
public:
    PauseMenu();
    void RunGame() override;
    void setLastState(int state);
};

class LoadMenu:public Game          //选择关卡窗口
{
    LD_Model model;
    LD_Controller controller;
    View view;
public:
    LoadMenu();
    void RunGame() override;
};

class Dawn:public Game              //第一关 Level I
{
public:
    DN_Model model;
    View view;
    DN_Controller controller;
    
    Dawn();
    void RunGame() override;
};

class Love:public Game              //第二关 Level II
{
public:
    LV_Model model;
    LV_Controller controller;
    View view;
    Love();
    ~Love()=default;
    void RunGame() override;
};

class Home:public Game              //第三关 Level III
{
public:
    HM_Model model;
    HM_Controller controller;
    View view;
    
    Home();
    ~Home()=default;
    void RunGame() override;
};
#endif /* Game_hpp */
