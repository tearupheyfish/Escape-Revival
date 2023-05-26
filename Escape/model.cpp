#include "model.hpp"

#include <random>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font* def_font_;
extern SDL_Color def_clr;
extern ResourcePool image_pool_;

SDL_Texture* ResourcePool::FindResource(std::string key)        //加载纹理
{
    u_m::iterator it=DOM.find(key);
    if(it!=DOM.end())
        return it->second;
    else
    {
        surf=IMG_Load(key.c_str());
        if(!surf)
        {
            std::cerr<<"surface read failure: "<<key<<".ErrorInfo: "<<SDL_GetError();
        }
        tet=SDL_CreateTextureFromSurface(renderer, surf);
        DOM.insert(std::pair<std::string,SDL_Texture*>{key,tet});
        SDL_FreeSurface(surf);
        return tet;
    }
};

Model::Model(int type,block *mp):                         //模型基类构造
bg_picture(nullptr),
flag(type),
map(mp),
skills{nullptr,nullptr}
{
}

//路径搜索
void Model::FindWay(Actor *atr,block *map,Uint8 width)  //可优化
{
    int mode;                       //A*模式
    SDL_Point *path,start,target;   //路径、起始点、目标点
    int path_len;                   //路径长度
    atr->ProvidePath(path, start, mode, path_len);

    SDL_Point caculating_point,t;
//    fee temp_map[width][width];
//    memset(temp_map, -1, width*width*sizeof(fee));
    std::vector<std::vector<fee>> temp_map(width,std::vector<fee>(width,{-1,-1})); //用于计算的临时地图
    std::vector<SDL_Point> best_list;//根据优先度排序的待扩展的点列表

    //起点所需步数设为0，填入到玩家的距离
#define PLAYER_ACTOR cast[0]
    temp_map[start.y][start.x].step=0;
    temp_map[start.y][start.x].distant=abs(start.y-PLAYER_ACTOR->posit_y)+abs(start.x-PLAYER_ACTOR->posit_x);
    //将起点加入列表
    best_list.push_back(start);
    while(!best_list.empty())
    {
        //取出最新的路径点
        caculating_point = best_list.back();
        best_list.erase(best_list.end() - 1);
        //如果已经达成路径长度要求、或者已经追溯到玩家身上则退出循环
        if(temp_map[caculating_point.y][caculating_point.x].step >= path_len ||
           caculating_point.x == (int) PLAYER_ACTOR->posit_x && caculating_point.y == (int) PLAYER_ACTOR->posit_y)
            break;
        //如果y-1的点可达 而且 临时地图中该点的步数为-1 //表示未计算
        if(IsPassable(map LOCATE(caculating_point.y - 1, caculating_point.x, width)) && temp_map[caculating_point.y - 1][caculating_point.x].step == -1)
        {
            t.y = caculating_point.y - 1, t.x = caculating_point.x;
            temp_map[t.y][t.x].step = temp_map[caculating_point.y][caculating_point.x].step + 1, //临时地图该点步数为最新路径点+1，距离为到玩家距离
                    temp_map[t.y][t.x].distant =
                            abs(t.y - PLAYER_ACTOR->posit_y) + abs(t.x - PLAYER_ACTOR->posit_x);
            InsertPoint(best_list, t, temp_map, width); //将点插入列表
        }
        if(IsPassable(map LOCATE(caculating_point.y + 1, caculating_point.x, width)) && temp_map[caculating_point.y + 1][caculating_point.x].step == -1)
        {
            t.y = caculating_point.y + 1, t.x = caculating_point.x;
            temp_map[t.y][t.x].step = temp_map[caculating_point.y][caculating_point.x].step + 1,
                    temp_map[t.y][t.x].distant =
                            abs(t.y - PLAYER_ACTOR->posit_y) + abs(t.x - PLAYER_ACTOR->posit_x);
            InsertPoint(best_list, t, temp_map, width);
        }
        if(IsPassable(map LOCATE(caculating_point.y, caculating_point.x + 1, width)) && temp_map[caculating_point.y][caculating_point.x + 1].step == -1)
        {
            t.y = caculating_point.y, t.x = caculating_point.x + 1;
            temp_map[t.y][t.x].step = temp_map[caculating_point.y][caculating_point.x].step + 1,
                    temp_map[t.y][t.x].distant =
                            abs(t.y - PLAYER_ACTOR->posit_y) + abs(t.x - PLAYER_ACTOR->posit_x);
            InsertPoint(best_list, t, temp_map, width);
        }
        if(IsPassable(map LOCATE(caculating_point.y, caculating_point.x - 1, width)) && temp_map[caculating_point.y][caculating_point.x - 1].step == -1)
        {
            t.y = caculating_point.y, t.x = caculating_point.x - 1;
            temp_map[t.y][t.x].step = temp_map[caculating_point.y][caculating_point.x].step + 1,
                    temp_map[t.y][t.x].distant =
                            abs(t.y - PLAYER_ACTOR->posit_y) + abs(t.x - PLAYER_ACTOR->posit_x);
            InsertPoint(best_list, t, temp_map, width);
        }
    }

    //路径的尾后节点设为-1,-1，表示结束
    path[temp_map[caculating_point.y][caculating_point.x].step]={ -1, -1};
    //从最后一个计算的点开始
    int posit= temp_map[caculating_point.y][caculating_point.x].step - 1;
    //逆向回溯到起点为止
    auto& nxt_point = caculating_point;
    while(caculating_point.x != start.x || caculating_point.y != start.y)
    {
        auto &cur_point= path[posit] = caculating_point;
        //依次计算点上下左右四个点
        //如果可达且步数小于当前点则将下一步设为该点
        if(temp_map[cur_point.y - 1][cur_point.x].step != -1 && temp_map[cur_point.y - 1][cur_point.x].step < temp_map[cur_point.y][cur_point.x].step)
            caculating_point.y= cur_point.y - 1, caculating_point.x=cur_point.x;
        if(temp_map[cur_point.y + 1][cur_point.x].step != -1 && temp_map[cur_point.y + 1][cur_point.x].step < temp_map[cur_point.y][cur_point.x].step)
            caculating_point.y= cur_point.y + 1, caculating_point.x=cur_point.x;
        if(temp_map[cur_point.y][cur_point.x - 1].step != -1 && temp_map[cur_point.y][cur_point.x - 1].step < temp_map[cur_point.y][cur_point.x].step)
            caculating_point.y=cur_point.y, caculating_point.x= cur_point.x - 1;
        if(temp_map[cur_point.y][cur_point.x + 1].step != -1 && temp_map[cur_point.y][cur_point.x + 1].step < temp_map[cur_point.y][cur_point.x].step)
            caculating_point.y=cur_point.y, caculating_point.x= cur_point.x + 1;
        --posit;
    }
}

bool Model::canSeeable(block *map, Uint8 width, double begin_x, double begin_y, double target_x, double target_y)
{
    double lenth=sqrt(pow(target_x-begin_x,2)+pow(target_y-begin_y,2));
    double u_y=(target_y-begin_y)/lenth,u_x=(target_x-begin_x)/lenth;
    int t(0);
    while(t++<lenth&&IsPassable(map LOCATE((int)begin_y,(int)begin_x,(int)width)))begin_x+=u_x,begin_y+=u_y;
    if(t>=lenth)return true;
    return false;
}

//插入待选点队列
void Model::InsertPoint(std::vector<SDL_Point> &list,SDL_Point t,const std::vector<std::vector<fee>>& temp_map,int width)
{
/*    auto i=list.begin();
    while(i != list.end() &&
    temp_map[i->y][i->x].step + temp_map[i->y][i->x].distant >
    temp_map[t.y][t.x].step + temp_map[t.y][t.x].distant
    ) ++i;
    list.insert(i, t);*/
    //将该点保序插入列表，排序依据是到达该点的步数+该点到玩家的距离，逆序
    //insert: 向迭代器指向的元素前插入
    //lower_bound: 第一个<=的位置，提供的比较器看做<
    list.insert(std::lower_bound(list.cbegin(),list.cend(), t,
                                 [&temp_map](const SDL_Point & left, decltype(left) right){
        return
        temp_map[left .y][left .x].step+temp_map[left .y][left .x].distant >
        temp_map[right.y][right.x].step+temp_map[right.y][right.x].distant ;
    }),
                t);
}

void Model::SkillCooling()
{
    if(skills[0]&&skills[0]->rate<1)
    {
        skills[0]->rate+=skills[0]->fix_speed;
        if(skills[0]->rate>1)skills[0]->rate=1;
        skills[0]->from.h=skills[0]->rate*114;
    }
    if(skills[1]&&skills[1]->rate<1)
    {
        skills[1]->rate+=skills[1]->fix_speed;
        if(skills[1]->rate>1)skills[1]->rate=1;
        skills[1]->from.h=skills[1]->rate*114;
    }
}

MM_Model::MM_Model():                   //主菜单模型构造
Model(MAINMENU,nullptr),
touched{200,45,35,255},
origin{200,45,35,150},
chosen_button(0)
{
    bg_picture=SDL_CreateTextureFromSurface(renderer,IMG_Load(("./source/images/cover1.png")));
    bu_ist.push_back(new Button(0,150,65,30,400,NULL,NULL,"开始",def_font_,origin,touched));
    bu_ist.push_back(new Button(1,150,65,30,475,NULL,NULL,"载入",def_font_,origin,touched));
    bu_ist.push_back(new Button(2,150,65,30,550,NULL,NULL,"新游戏",def_font_,origin,touched));
    bu_ist.push_back(new Button(3,150,65,30,625,NULL,NULL,"EXIT",def_font_,origin,touched));
    bu_ist.push_back(new Button
    (-1,600,250,0,0,NULL,NULL,"Escape 逃",TTF_OpenFont(("./source/ttfs/Slidefu-Regular.ttf"),200),def_clr,def_clr));
    (*(bu_ist.end()-1))->canBetouch=false;
}

PA_Model::PA_Model():                   //暂停菜单模型构造
Model(PAUSEMENU,nullptr),
touched{182,10,0,160},
origin{80,80,80,127}
{
    bg_picture=image_pool_.FindResource(("./source/images/pause_bg.png"));
    
    bu_ist.push_back(new Button
                           (0,312,136,444,342,NULL,image_pool_.FindResource(("./source/images/glay.png")),"继续游戏",def_font_,origin,touched));
    bu_ist.push_back(new Button
                           (1,312,136,444,500,NULL,image_pool_.FindResource(("./source/images/glay.png")),"回到主界面",def_font_,origin,touched));
}

LD_Model::LD_Model():
Model(LOADMENU,nullptr),
display_posit(1120),
selected_level(DAWN),
sft(TTF_OpenFont(("./source/ttfs/Slidefu-Regular.ttf"), 200)),
clr{180,180,180,255}
{
    sc.x=240,sc.y=720,sc.h=80;
    bg_picture=image_pool_.FindResource("./source/images/load.png");
    std::string a;
    for(int i=0;i<5;++i)
        picture[i]=image_pool_.FindResource((a+"./source/images/level_"+(char)(i+'0')+".png"));
    undentifined=image_pool_.FindResource("./source/images/undentifined.png");
    print.x=display_posit,print.y=180,print.w=720,print.h=540;
}

bool LD_Model::DisplayerMove()
{
    if(display_posit<240-(selected_level-DAWN)*880)display_posit+=11;
    else if(display_posit>240-(selected_level-DAWN)*880)display_posit-=11;
    else return false;
    return true;
}

Ct_Model::Ct_Model():
Model(-1,nullptr)
{
    
}

DN_Model::DN_Model()://Level I 模型构造
usedtime(0),
Model(DAWN,&**map),
score(0),
r_bean_clr{149,2,139,200},
r_bean_ft(TTF_OpenFont(("./source/ttfs/HelloScarecrow.ttf"),200)),
r_bean_title(std::to_string(remain_bean))
{
    skills[0]=new Skill(0);
    skills[0]->skill_picture=image_pool_.FindResource("./source/images/speeding.png");
    width=30;
    bu_ist.push_back(new Button(0,300,900,900,0,image_pool_.FindResource((  "./source/images/black_window.png")),NULL,"",def_font_,def_clr,def_clr));
    bu_ist.push_back(new Button(1,310,792,890,27,image_pool_.FindResource((  "./source/images/table.png")),NULL,"",def_font_,def_clr,def_clr));
    bu_ist.push_back(new Button(2,160,55,960,460,NULL,NULL,r_bean_title.c_str(),r_bean_ft,r_bean_clr,r_bean_clr));
    //地图读入
    std::ifstream dawn_map((  "./source/maps/dawn.txt"));
    remain_bean = 0;
    for(int j,i=0;i<30;i++)
    {
        for(j=0;j<30;j++)
        {
            RecognizeMap(map[i][j], i, j, dawn_map);
            if(map[i][j].flag == 1 && map[i][j].blind != nullptr)
                ++remain_bean;
        }
    }
    //计算度
    for(int j,i=0;i<30;i++)
    {
        for(j=0;j<30;j++)
        {
            if(IsPassable(&map[i][j]))
            {
                if(i>0)++map[i-1][j].degree;
                if(i<29)++map[i+1][j].degree;
                if(j>0)++map[i][j-1].degree;
                if(j<29)++map[i][j+1].degree;
            }
        }
    }
    dawn_map.close();
    
    //放置生物
//    srand((int)time(0));

    std::default_random_engine raneng(time(0));
    int x,y;
    while(x=raneng()%30,y=raneng()%30,map[y][x].flag==0);
    cast.push_back(new Player(x,y));
    for(int n=4;n--;)
    {
        while(x=raneng()%30,y=raneng()%30,map[y][x].flag==0);
        if(n&1)
            cast.push_back(new Dog(x+0.5,y+0.5));
        else
            cast.push_back(new Cat(x+0.5,y+0.5));
    }
}

void DN_Model::RecognizeMap(block &map_unit,int i,int j,std::ifstream &dawn_map)
{
    int wasted;
    dawn_map>>map[i][j].flag>>wasted;
    map[i][j].blind=nullptr,map[i][j].mob=nullptr;
    switch (map_unit.flag) {
        case 0:
            map_unit.skin=image_pool_.FindResource(  "./source/images/rub_grass.png");
            break;
        case 1:
            map_unit.skin=image_pool_.FindResource(  "./source/images/stone_with_grass.png");
            switch (wasted) {
                case 1:
                    map_unit.blind=new Bean(image_pool_.FindResource(  "./source/images/boon.png"),0.5+j,0.5+i);
                    break;
                case 2:
                    map_unit.mob=new Cage(j+0.1,i+0.1,DOG,1800);
                    mo_ist.push_back(&map_unit.mob);
                default:
                    break;
            }
                
            break;
        default:
            map_unit.skin=image_pool_.FindResource(  "./source/images/brick.png");
            break;
    }
}

bool DN_Model::IsPassable(block *area)
{
    return area->flag;
}

LV_Model::LV_Model():
Model(LOVE,nullptr)
{
    bu_ist.push_back(new Button(0,600,200,300,450,NULL,image_pool_.FindResource((  "./source/images/glay.png")),"To Be Continue",TTF_OpenFont((  "./source/ttfs/MajorScare.ttf"),200),def_clr,def_clr));
}

HM_Model::HM_Model():
Model(HOME,&**map)
{
    width=50;
    
}

void HM_Model::RecognizeMap(block &map_unit, int i, int j, std::ifstream &dawn_map)
{
//    int wasted;
    dawn_map>>map[i][j].flag/*>>wasted*/;
    map[i][j].blind=nullptr,map[i][j].mob=nullptr;
    switch (map_unit.flag) {
        case 0:
            map_unit.skin=image_pool_.FindResource(  "./source/images/rub_grass.png");
            break;
        case 1:
            map_unit.skin=image_pool_.FindResource(  "./source/images/stone_with_grass.png");
            break;
        default:
            map_unit.skin=image_pool_.FindResource(  "./source/images/brick.png");
            break;
    }
}
