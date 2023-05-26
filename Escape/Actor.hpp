
#ifndef actor_hpp
#define actor_hpp

#include <iostream>
#include <vector>
#include <unordered_map>
#include <SDL_image.h>
#include <cmath>
#include "tool.hpp"

class Tool;
class ResourcePool;

#define PI 3.14159

enum{BEAN,PLAYER=0,DOG,CAT};

class ResourcePool                      //资源池
{
    using u_m=std::unordered_map<std::string,SDL_Texture*>;
    u_m DOM;
    SDL_Surface *surf;
    SDL_Texture *tet;
public:
    SDL_Texture* FindResource(std::string key);
};

class ResourcePool_2
{
    using s_u_m=std::unordered_map<std::string,SDL_Texture*>;
};

class Actor                             //--演员基类
{
public:
    int actor_type;                     //类型标记
    SDL_Texture *clothes;               //俯视图片
    double posit_x,posit_y;             //相对地图的绝对坐标(演员中心)
    double direction,face_direc;        //移动方向、面朝朝向 遵从向上为0，顺时针为正，度数制
    double speed;                       //移动速度
    double physique;                    //碰撞箱半径
    bool find_way_req;                  //路径搜索请求
    int buff[5];
    
    Actor(double x,double y,SDL_Texture *clo,int type);
    virtual ~Actor()=default;
    virtual void Action()=0;
    void TurnAround();                  //调整面部朝向向行动方向靠拢
    void TrunCircle(double degree);     //转圈圈
    virtual void ProvidePath(SDL_Point *&path, SDL_Point &p, int &mode, int &path_len) =0;//提供路径要求
protected:
    void Walk();
};

class Player:public Actor               //-玩家
{
public:
    std::vector<Tool *> backpagk;       //背包
    bool iscontrolled;                  //是否被控制
    
    Player(double x,double y);
    ~Player()=default;
    void Action()override;
    void ProvidePath(SDL_Point *&path, SDL_Point &p, int &mode, int &path_len) override{}
};

class Dog:public Actor                  //—小狗
{
private:
    int req_tick;
    int posit;
public:
    SDL_Point path[11];
    
    Dog(double x,double y);
    ~Dog()=default;
    void Action()override;
    void ProvidePath(SDL_Point *&path, SDL_Point &p, int &mode, int &path_len) override;
};

class Cat:public Actor                  //-小猫
{
private:
    int req_tick;
    int posit;
public:
    SDL_Point path[11];
    
    Cat(double x,double y);
    ~Cat()=default;
    void Action()override;
    void ProvidePath(SDL_Point *&path, SDL_Point &p, int &mode, int &path_len) override;
};

class Bean:public Actor                 //-豆豆
{
public:
    Bean(SDL_Texture *sp,double x,double y);
    ~Bean()=default;
    void Action()override;
    void ProvidePath(SDL_Point *&path, SDL_Point &p, int &mode, int &path_len) override{}
private:
    
};

//class Girl:public Actor                 //-女孩
//{
//
//};
//
//class Drunk:public Actor                //-醉汉
//{
//
//};
//
//class Doll:public Actor                 //-人偶
//{
//
//};

/*场景构件*/
class Furniture
{
public:
    bool bedraw;
    int fn_type;
    SDL_Texture *picture;
    int facing;
    double posit_x,posit_y;
    double width,height;
    Furniture(double x,double y,SDL_Texture *p,int type);
    virtual void Action()=0;
};

class Cage:public Furniture             //笼子    装载实体
{
public:
    int content;
    bool is_break;
    int tick;
    
    Cage(double x,double y,int type,int time);
    void Action() override;
};

class Box:public Furniture              //盒子    装载道具
{
    void Action() override{};
};

class Barrier:public Furniture          //屏障    仅阻挡作用
{
    void Action() override{};
};

#endif /* actor_hpp */