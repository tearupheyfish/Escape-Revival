#include "Game.hpp"

MainMenu::MainMenu():                   //主菜单构造
view(model),
controller{model,view}
{
    
}

void MainMenu::RunGame()                //主菜单运行
{
    controller.TickControll();
}

PauseMenu::PauseMenu():                 //暂停菜单构造
view(model),
controller{model,view}
{
    
}
void PauseMenu::RunGame()               //暂停菜单运行
{
    controller.TickControll();
}

LoadMenu::LoadMenu():
view(model),
controller(model, view)
{
    
}

void LoadMenu::RunGame()
{
    controller.TickControll();
}

Dawn::Dawn():                           //Level I 构造
view(model),
controller(model, view)
{
    
}

void Dawn::RunGame()                    //Level I 运行
{
    controller.TickControll();
}


Love::Love():
controller(model,view),
view(model)
{
    
}

void Love::RunGame()
{
    controller.TickControll();
}

Home::Home():
controller(model, view),
view(model)
{
    
}

void Home::RunGame()
{
    controller.TickControll();
}
