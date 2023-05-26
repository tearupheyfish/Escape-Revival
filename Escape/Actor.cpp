#include "actor.hpp"

extern ResourcePool image_pool_;
extern SDL_Renderer *renderer;

/*演员/实体*/
Actor::Actor(double x,double y,SDL_Texture *clo,int type):            //演员基类构造
posit_x(x),
posit_y(y),
direction(0),
face_direc(0),
clothes(clo),
find_way_req(false),
actor_type(type),
buff{}
{
    
}

void Actor::TurnAround()
{
    double distant=direction-face_direc;
    if(distant>180)distant=-360+distant;
    else if(distant<-180)distant=360+distant;
    
    if(fabs(distant)>0.1)face_direc+=distant*0.2;
}

void Actor::TrunCircle(double degree)
{
    face_direc+=degree;
    if(face_direc>=360)face_direc-=360;
    else if(face_direc<0)face_direc+=360;
}

void Actor::Walk()
{
    posit_x+=speed*sin(direction/360*PI*2);
    posit_y+=-speed*cos(direction/360*PI*2);
}

Player::Player(double x,double y):          //玩家类 构造
Actor(x,y,image_pool_.FindResource(( "./source/images/man.png")),PLAYER)
{
    physique=0.4;
    speed=0.05*iscontrolled;
}

void Player::Action()
{
    speed=0.05*iscontrolled;
    if(buff[0])
    {
        speed*=1.5;
        --buff[0];
    }
    TurnAround();
    Walk();
}

Bean::Bean(SDL_Texture *sp,double x,double y)://豆豆类 构造
Actor(x,y,sp,BEAN)
{
    physique=0.1;
    
}

void Bean::Action()
{
    face_direc+=1;
    if(face_direc>355)face_direc-=360;
}

Dog::Dog(double x,double y):                //小狗类 构造
Actor(x,y,image_pool_.FindResource( "./source/images/dog.png"),DOG),
req_tick(0),
posit(0)
{
    speed=0.04;
    physique=0.35;
}

void Dog::Action()                          //小狗行动
{
    if(path[posit].x==(int)posit_x&&path[posit].y==(int)posit_y)
        ++posit;
    
    if(req_tick>90||path[posit].x==-1){find_way_req=true;req_tick=0;posit=0;}
    else
    {
        ++req_tick;
        find_way_req=false;
        direction=atan2(path[posit].y+0.5-posit_y,path[posit].x+0.5-posit_x)/2/PI*360+90;
    }
    
    TurnAround();
    Walk();
}

void Dog::ProvidePath(SDL_Point *&p_p, SDL_Point &point, int &mode, int &path_len)//小狗路径搜索提交
{
    point.x=posit_x,point.y=posit_y;
    p_p=path;
    mode=0;
    path_len=10;
}

Cat::Cat(double x,double y):                //小猫类 构造
Actor(x,y,image_pool_.FindResource( "./source/images/cat.png"),CAT),
posit(0),
req_tick(0)
{
    speed=0.04;
    physique=0.3;
}

void Cat::Action()
{
    if(path[posit].x==(int)posit_x&&path[posit].y==(int)posit_y)
        ++posit;

    if(req_tick > 90 || path[posit].x == -1)
    {
        find_way_req = true;
        req_tick = 0;
        posit = 0;
    }
    else
    {
        ++req_tick;
        find_way_req=false;
        direction=atan2(path[posit].y+0.5-posit_y,path[posit].x+0.5-posit_x)/2/PI*360+90;
    }
    
    TurnAround();
    Walk();
}

void Cat::ProvidePath(SDL_Point *&list, SDL_Point &point, int &mode, int &path_len)//小猫路劲搜索提交
{
    point.x=posit_x,point.y=posit_y;
    list=path;
    mode=0;
    path_len=10;
}
/*场景构件*/
Furniture::Furniture(double x,double y,SDL_Texture *p,int type):
picture(p),
posit_x(x),posit_y(y),
fn_type(type),
bedraw(true)
{
    
}


Cage::Cage(double x,double y,int actor_type,int time):
Furniture(x, y,image_pool_.FindResource( "./source/images/cage.png"),1),
content(actor_type),
is_break(false),
tick(time)
{
    width=height=0.3;
}

void Cage::Action()
{
    if(tick<=0)
    {
        is_break=true;
    }
    else
        --tick;
}
