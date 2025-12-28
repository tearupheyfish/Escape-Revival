

#ifndef pool_hpp
#define pool_hpp

#include <SDL_ttf.h>

#include <unordered_map>
#include <string>

class ResourcePool                      //资源池
{
    using u_m=std::unordered_map<std::string,SDL_Texture*>;
    u_m DOM;
public:
    ~ResourcePool()
    {
        for (auto &pair : DOM)
            SDL_DestroyTexture(pair.second);
    }
    SDL_Texture* FindResource(std::string key);
};

extern TTF_Font *def_font_;
extern int progress;                                //全局进度
extern int h_score[5];                              //全局关卡分数
extern SDL_Color def_clr;                           //全局默认色

//全局窗口长宽
extern const int WINDOWS_WIDTH;
extern const int WINDOWS_HEIGHT;
extern SDL_Window* windows;                         //全局窗口
extern SDL_Renderer *renderer;                      //全局渲染器
extern ResourcePool image_pool_;                    //图片资源池
extern bool isRunning;                              //全局游戏运行标记
extern int game_state,last_state;                   //全局游戏模式标记、上一帧模式标记


#endif /* pool_hpp */
