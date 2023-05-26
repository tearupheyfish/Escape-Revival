
#include "view.hpp"

extern const int WINDOWS_WIDTH;
extern const int WINDOWS_HEIGHT;
extern SDL_Renderer *renderer;
extern TTF_Font *def_font_;
extern SDL_Color def_clr;
extern int h_score[5];
extern ResourcePool image_pool_;

const double half_display_width=3.75;
const int base_len=120;

Cutscenes::Cutscenes(int type,int usedtimes): 
view(model)
{
    SDL_Event event;
    const Uint8 *keystate=nullptr;
    switch (type/10) {
        case 1://分数结算
        {
            model.bu_ist.push_back(new Button(0,400,100,400,425,NULL,NULL,
                                              ("用时: "+std::to_string(usedtimes/3600)+":"+std::to_string(usedtimes/60%60)).c_str(),def_font_,def_clr,def_clr));
            SDL_Color clr{255,0,0,255};
            int targetScore=0;
            switch(type%10)
            {
                case 0:
                    if(usedtimes<5400)usedtimes=5400;
                    else if(usedtimes>18000)usedtimes=18000;
                    targetScore=(pow(2,(18000-usedtimes)/(double)6300)-1)/3*100;
                    break;
            }
            int tick(0);
            double u_score=(double)targetScore/180;
            model.bu_ist.push_back(new Button(1,400,100,400,625,NULL,NULL,
                                              ("得分: "+std::to_string(0)).c_str(),def_font_,clr,clr));
            SDL_Texture *replace[2]={nullptr,nullptr};
            
            while(tick++<180)
            {
                SDL_PollEvent(&event);
                
                tick++;
                if(!(tick%10))
                {
                    clr.g=255*tick*u_score/100,clr.r=255-clr.g;
                    replace[0]=SDL_CreateTextureFromSurface(renderer,TTF_RenderUTF8_Blended(def_font_, ("得分:  "+std::to_string((int)(u_score*tick))).c_str(),clr));
                    model.bu_ist[1]->changeSTT(&replace);
                }
                
                view.DrawWorld();
            }
            clr={255,211,0,255};
            std::string com;
            switch (targetScore/5) {
                case 20:
                case 19:
                    com+="SSS";
                    break;
                case 18:
                case 17:
                    com+="SS ";
                    break;
                case 16:
                case 15:
                    com+=" S ";
                    break;
                case 14:
                    com+=" A+";
                    break;
                case 13:
                case 12:
                    com+=" A ";
                    break;
                case 11:
                case 10:
                case 9:
                    com+=" B ";
                    break;
                case 8:
                case 7:
                case 6:
                    com+=" C ";
                    break;
                default:
                    com+="PASS";
                    break;
            }
            
            if(targetScore>75)clr={255,211,0,255};
            else if(targetScore>60)clr={0,212,0,255};
            else clr=def_clr;
            
            model.bu_ist.push_back(new Button(1,450,225,375,100,NULL,NULL,
            com.c_str(),def_font_,clr,clr));
            
            view.DrawWorld();
            
            h_score[type%10]=targetScore;
            
            while(true)
            {
                SDL_PumpEvents();
                keystate=SDL_GetKeyboardState(NULL);
                if(keystate[SDL_SCANCODE_RETURN])
                    break;
            }
        }
            break;
        case -1://黑屏过渡
        {
            int tick=usedtimes;
            while(tick--)
            {
                SDL_RenderClear(renderer);
                SDL_PollEvent(NULL);
                SDL_RenderPresent(renderer);
            }
        }
            break;
        default:
            break;
    }
}

View::View(Model &mod):                 //View基类构造函数
model(&mod),
little_beans(image_pool_.FindResource("./source/images/little_bean.png"))
{
    
}

bool View::Inscreen(double p_x,double p_y)
{
    double std_x=model->cast[0]->posit_x,std_y=model->cast[0]->posit_y;
    if(fabs(p_x-std_x)<ceil(half_display_width)&&fabs(p_y-std_y)<ceil(half_display_width))
        return true;
    return false;
}

void View::DrawWorld()                  //View绘制函数
{
    double std_x(0),std_y(0);   //准线坐标
    if(model->cast.begin()!=model->cast.end())
        std_x=model->cast[0]->posit_x,
        std_y=model->cast[0]->posit_y;
    
    SDL_RenderClear(renderer);  //清空缓冲区
    
    //若模型有背景则先显示背景
    if(model->bg_picture)SDL_RenderCopy(renderer,model->bg_picture, NULL, NULL);
    SDL_Rect print{0,0,base_len,base_len},sticker{};//场景贴图、豆子贴图
    //显示地图
    if(model->map)
    {
        DrawMap(std_x, std_y,print,sticker);
    }
        
    //显示实体
    for(auto i:model->cast)
    {
        
        if(Inscreen(i->posit_x, i->posit_y))
        {
            print.x=(i->posit_x-std_x-i->physique+half_display_width)*base_len,
            print.y=(i->posit_y-std_y-i->physique+half_display_width)*base_len;
            print.w=print.h=i->physique*2*base_len;
            SDL_RenderCopyEx(renderer,i->clothes, nullptr,&print,i->face_direc, nullptr, SDL_FLIP_NONE);
        }
    }
    
    //显示按钮
    for(auto &i:model->bu_ist)
    {
        if(i->beDraw)
        {
            SDL_RenderCopy (renderer,i->back_ground[i->selected_title], NULL, &i->ofPicture);
            SDL_RenderCopy (renderer, i->title[i->selected_title], NULL, &i->ofTitle);
        }
    }
    
    SDL_Rect to;
    to.x=1059,to.y=355,to.w=55;
    //技能显示
    for(int i=0;i<2;++i)
    {
        if(model->skills[i])
        {
            to.x=140*i+939;
            to.h=model->skills[i]->rate*55;
            SDL_RenderCopy(renderer, model->skills[i]->skill_picture, &model->skills[i]->from, &to);
        }
    }
    if(model->map)
    {
        DrawMiniMap(std_x, std_y);
    }
    
    
    SDL_RenderPresent(renderer);        //交换缓冲区
}

void View::DrawMap(double std_x,double std_y,SDL_Rect print,SDL_Rect sticker)
{
    Actor *bean;
    Furniture *mob;
    
    int width=model->width;
    int i=std_y-half_display_width,e_i=std_y+half_display_width;
    int j,e_j;
    if(i<0)i=0;if(e_i>width-1)e_i=width-1;
    
    print.y=(i-std_y)*base_len+450;
    while(i<=e_i)
    {
        j=std_x-half_display_width,e_j=std_x+half_display_width;
        if(j<0)j=0;if(e_j>width-1)e_j=width-1;
        
        print.x=(j-std_x)*base_len+450;
        while(j<=e_j)
        {
            SDL_RenderCopy(renderer,(model->map LOCATE(i,j,model->width))->skin, NULL, &print);
            if((bean=(model->map LOCATE(i,j,model->width))->blind))//显示场实体
            {
                bean->Action();                     //豆子表现
                sticker.x=print.x+60,sticker.y=print.y+60,
                sticker.h=sticker.w=bean->physique*2*base_len;
                SDL_RenderCopyEx(renderer, bean->clothes, NULL, &sticker, bean->face_direc, NULL, SDL_FLIP_NONE);
            }
            else if((mob=(model->map LOCATE(i,j,model->width))->mob)&&mob->bedraw)//显示场景器具
            {
                sticker.x=print.x+(0.5-mob->width)*base_len,sticker.y=print.y+(0.5-mob->height)*base_len;
                sticker.w=(int)(mob->width*base_len)<<1,sticker.h=(int)(mob->height*base_len)<<1;
                SDL_RenderCopy(renderer, mob->picture, NULL ,&sticker);
            }
            j++,print.x+=base_len;
        }
        
        i++,print.y+=base_len;
    }
}
//迷你地图绘制
void View::DrawMiniMap(double std_x,double std_y)
{
    SDL_Rect print;
    print.w=print.h=10;
    
    int width=model->width;
    int i=std_y-10,e_i=std_y+10;
    int j,e_j;
    if(i<0)i=0;if(e_i>width-1)e_i=width-1;
    
    print.y=(i-std_y)*10+680;
    while(i<=e_i)
    {
        j=std_x-10,e_j=std_x+10;
        if(j<0)j=0;if(e_j>width-1)e_j=width-1;
        
        print.x=(j-std_x)*10+1040;
        while(j<=e_j)
        {
            if((model->map LOCATE(i,j,model->width))->blind&&
               (model->map LOCATE(i,j,model->width))->blind->actor_type==BEAN)
                SDL_RenderCopy(renderer,little_beans, NULL, &print);
            
            ++j,print.x+=10;
        }
        
        ++i,print.y+=10;
    }
    print.x=1035,print.y=675;
    SDL_RenderCopyEx(renderer, image_pool_.FindResource("./source/images/arrow.png"), NULL, &print, model->cast[0]->face_direc, NULL, SDL_FLIP_NONE);
}
