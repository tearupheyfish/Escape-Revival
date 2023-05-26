#ifndef model_hpp
#define model_hpp

#include <SDL2/SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <fstream>
#include <string>
#include <ctime>

#include "Actor.hpp"
#include "button.hpp"

class Button;
class Actor;
class ResourcePool;

#define LOCATE(a/*行序*/,b/*列序*/,w/*边长*/) +((a))*(w)+(b) //二维方式取一维数组值，参数
enum{MAINMENU=0,PAUSEMENU,LOADMENU,DAWN,LOVE,HOME,ALONE,FIGHT};

struct block                            //基本地图单元
{
    int flag;
    int degree=0;                         //度
    SDL_Texture *skin;                  //地图贴图
    Actor *blind;
    Furniture *mob;
};

struct fee                              //搜索费用结构体
{
    int step,distant;
};

class Model                             //--模型基类
{
public:
    int flag;                           //模型标记
    int width;
    block *map;
    SDL_Texture *bg_picture;            //背景图像
    std::vector<Furniture **> mo_ist;   //器具表列
    std::vector<Actor *> cast;          //演员表列
    std::vector<Button *> bu_ist;       //按钮表列
    Skill *skills[2];
    
    Model(int type,block *map);
    virtual ~Model()=default;
    void FindWay(Actor *atr,block *map,Uint8 width);//路径搜索算法
    void InsertPoint(std::vector<SDL_Point> &list,SDL_Point t,const std::vector<std::vector<fee>>& t_map,int width);//搜索算法中插入队列
//    virtual void RecognizeMap(block &map_unit,int i,int j,std::ifstream &dawn_map)=0;//从文件造图
    virtual bool IsPassable(block *area){return true;}//某点可通过性判断
    bool canSeeable(block *map,Uint8 width/*地图、地图边长*/,
                    double begin_x,double begin_y,
                    double target_x,double target_y/*起点终点坐标*/);
    void SkillCooling();
    
    friend class View;
};

class MM_Model:public Model             //-主菜单模型
{
public:
    int chosen_button;                  //被选中的按钮
    SDL_Color touched,origin;           //两种颜色
    
    MM_Model();
    ~MM_Model()=default;
    void RecognizeMap(block &map_unit, int i, int j, std::ifstream &dawn_map) {};
};

class PA_Model:public Model             //-暂停菜单模型
{
public:
    int keyboardChosen;                 //被选中的按钮
    SDL_Color touched,origin;           //两种颜色
    
    PA_Model();
    ~PA_Model()=default;
    void RecognizeMap(block &map_unit, int i, int j, std::ifstream &dawn_map) {};
};

class LD_Model:public Model             //加载菜单模型
{
public:
    SDL_Texture *score;
    int selected_level;
    int display_posit;
    SDL_Texture* picture[5];
    SDL_Texture* undentifined;
    SDL_Rect print;
    TTF_Font *sft;
    SDL_Color clr;
    SDL_Rect sc;
    
    LD_Model();
    void RecognizeMap(block &map_unit, int i, int j, std::ifstream &dawn_map) {};
    bool DisplayerMove();
};

class Ct_Model:public Model
{
public:
    Ct_Model();
    ~Ct_Model()=default;
    void RecognizeMap(block &map_unit, int i, int j, std::ifstream &dawn_map){};
};

class DN_Model:public Model             //-Level I 模型
{
public:
    int usedtime;
    int score;
    int remain_bean;
    block map[30][30];                  //注意地图x,y和图像x,y相反
    SDL_Color r_bean_clr;               //计数器颜色
    TTF_Font *r_bean_ft;                 //计数器字体
    std::string r_bean_title;           //计数器文字
    
    DN_Model();
    ~DN_Model()=default;
    void RecognizeMap(block &map_unit,int i,int j,std::ifstream &dawn_map);
    bool IsPassable(block *area)override;
};

class LV_Model:public Model
{
public:
    LV_Model();
    ~LV_Model()=default;
    void RecognizeMap(block &map_unit,int i,int j,std::ifstream &dawn_map) {};
    bool IsPassable(block *area)override{return true;}
};

class HM_Model:public Model
{
public:
    block map[50][50];
    
    HM_Model();
    ~HM_Model()=default;
    void RecognizeMap(block &map_unit,int i,int j,std::ifstream &dawn_map) ;
    bool IsPassable(block *area)override{return true;}
};

#endif /* model_hpp */