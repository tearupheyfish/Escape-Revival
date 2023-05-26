#define SDL_MAIN_HANDLED

#include <iostream>
//#include <SDL2/SDL_filesystem.h>
#include "Game.hpp"
#include "SDL_ttf.h"
#include "SDL_image.h"

extern int progress;                                //全局进度
int progress;

extern int h_score[5];                              //全局关卡分数
int h_score[5];

extern TTF_Font *def_font_;                         //全局默认字体
TTF_Font *def_font_;

extern SDL_Color def_clr;                           //全局默认色
SDL_Color def_clr={255,255,255,200};
//全局窗口长宽
extern const int WINDOWS_WIDTH;
extern const int WINDOWS_HEIGHT;
const int WINDOWS_WIDTH=1200;
const int WINDOWS_HEIGHT=900;

extern SDL_Window* windows;                         //全局窗口
extern SDL_Renderer *renderer;                      //全局渲染器
SDL_Window *windows;
SDL_Renderer *renderer;

extern ResourcePool image_pool_;                    //图片资源池
ResourcePool image_pool_;

extern bool isRunning;                              //全局游戏运行标记
bool isRunning=true;

extern int game_state,last_state;                   //全局游戏模式标记、上一帧模式标记
int game_state=MAINMENU,last_state=DAWN;

void Init();
void GameLoop(Game *mode_list[]);
void GameQuit();

int main(/*int argc, const char * argv[]*/)
{
    Init();                 //初始化
    
    //游戏模式实例化
    Game *mode_list[6]={new MainMenu,new PauseMenu, new LoadMenu, new Dawn/*,new Love,new Home*/};
    
    GameLoop(mode_list);    //循环核心
    
    GameQuit();             //退出
    
    return 0;
}

void Init()
{
    //最高分读入
    std::ifstream state(("./source/progress.txt"));
    state>>progress;
    for(auto &i:h_score)
    {
        state>>i;
    }
    state.close();
    
    SDL_Init(SDL_INIT_EVERYTHING);  //无脑初始化
    TTF_Init();                     //文字初始化
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    //打开默认文字字体
    def_font_=TTF_OpenFont(("./source/ttfs/海派腔调摩擦简-闪.ttf"),200);
    if(!def_font_)
        std::cerr<<SDL_GetError()<<std::endl;
    //在屏幕中间的全屏窗口
    windows=SDL_CreateWindow("Escape",SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, WINDOWS_WIDTH, WINDOWS_HEIGHT, SDL_WINDOW_SHOWN/*SDL_WINDOW_BORDERLESS*/);
    //渲染器,加速|刷新率同步
    renderer=SDL_CreateRenderer(windows, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
}

void GameLoop(Game *mode_list[])
{
    while(isRunning)
    {
        switch (game_state) {
            case -2:                    //部分实例化
            {
                Cutscenes e(-10,120);
                delete mode_list[last_state];
                switch (last_state) {
                    case MAINMENU:
                        mode_list[MAINMENU]=new MainMenu;
                        break;
                    case PAUSEMENU:
                        mode_list[PAUSEMENU]=new PauseMenu;
                        break;
                    case LOADMENU:
                        mode_list[LOADMENU]=new LoadMenu;
                        break;
                    case DAWN:
                        mode_list[DAWN]=new Dawn;
                        break;
                    default:
                        break;
                }
                game_state=last_state;
                //                Cutscenes *pause=new Cutscenes;
                //                delete pause;
                //继续执行
                mode_list[game_state]->RunGame();
                break;
            }
            case -1:                        //重置
            {
                //重新实例化
                progress=DAWN;
                last_state=DAWN;
                for(int i=0;i<6;++i)
                {
                    if(mode_list[i])
                    {
                        delete mode_list[i];
                        switch (i) {
                            case MAINMENU:
                                mode_list[MAINMENU]=new MainMenu;
                                break;
                            case PAUSEMENU:
                                mode_list[PAUSEMENU]=new PauseMenu;
                                break;
                            case LOADMENU:
                                mode_list[LOADMENU]=new LoadMenu;
                                break;
                            case DAWN:
                                mode_list[DAWN]=new Dawn;
                                break;
                            default:
                                break;
                        }
                    }
                }
                //过渡画面
                //                Cutscenes *pause=new Cutscenes;
                //                delete pause;
                
                //继续执行
                mode_list[game_state=MAINMENU]->RunGame();
                break;
            }
            default:                        //正常切换
                //若切换了游戏状态
                if(last_state^game_state&&game_state^MAINMENU&&game_state^PAUSEMENU&&game_state^LOADMENU)
                {
                    if(game_state>last_state)
                        progress=game_state;
                    //销毁上一段游戏
                    delete mode_list[last_state];
                    mode_list[last_state]=nullptr;
                    //新建下一段游戏
                    switch (game_state)
                    {
                        case DAWN:
                            mode_list[DAWN]=new Dawn;
                            break;
                        case LOVE:
                            mode_list[LOVE]=new Love;
                            break;
                        case HOME:
                            mode_list[HOME]=new Home;
                            break;
                    }
                }
                last_state=game_state;
            case LOADMENU:                  //加载菜单
            case PAUSEMENU:                 //暂停
            case MAINMENU:                  //主菜单模式下保存上一次的关卡
                mode_list[game_state]->RunGame();
        }
    }
}

void GameQuit()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(windows);
    SDL_Quit();
    
    TTF_Quit();
    
    std::ofstream state(("./source/progress.txt"));
    state<<progress<<" ";
    for(int i:h_score)
        state<<i<<" ";
    state.close();
}
