
#ifndef controller_hpp
#define controller_hpp

#include <SDL2/SDL.h>
#include <string>
#include "view.hpp"

class Model;

class Controller                                //--控制器基类
{
protected:
    View &view;                                 //相关视窗
    SDL_Event *event;                           //事件
    int mouse_x,mouse_y;                        //鼠标位置
    bool LeftClicked;                           //是否左键单击
    const Uint8 *keystate;                      //按键
    
    Controller(View &vw);
    ~Controller();
    virtual void TickControll()=0;
private:
};

class MM_Controller:public Controller           //-主菜单控制器
{
private:
    void choiceTitle(int titleId);
public:
    MM_Model &model;                            //相关游戏模型
    
    MM_Controller(MM_Model &mod,View &vw);
    void TickControll() override;
    
};

class PA_Controller:public Controller           //-暂停菜单控制器
{
private:
    void choiceTitle(int titileId);
public:
    PA_Model &model;                            //相关游戏模型
    
    PA_Controller(PA_Model &mod,View &vw);
    void TickControll() override;
};

class LD_Controller:public Controller           //游戏加载菜单控制器
{
public:
    LD_Model *model;
    
    LD_Controller(LD_Model &mod,View &vw);
    void TickControll() override;
    void DrawWorld();
};

class DN_Controller:public Controller           //-LevelI控制器    Dawn
{
public:
    DN_Model *model;
    DN_Controller(DN_Model &mod,View &vw);
    void TickControll() override;
    void PositLimit(double &p_x,double &p_y,double physique);
};

class LV_Controller:public Controller           //-LevelII控制器   Love
{
public:
    LV_Model *model;
    LV_Controller(LV_Model &mod,View &vw);
    
    void TickControll() override;
};

class HM_Controller:public Controller           //LevelIII控制器   Home
{
public:
    HM_Controller(Model &mod,View &vw);
    
    void TickControll() override;
};

#endif /* controller_hpp */