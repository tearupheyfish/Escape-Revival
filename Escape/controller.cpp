#include "controller.hpp"

extern bool isRunning;
extern int game_state,last_state;
extern SDL_Renderer *renderer;
extern int progress;
extern int h_score[5];
extern TTF_Font *def_font_;
extern SDL_Color def_clr;
Uint8 last_press;                         //上一按键

int repeat=0;

bool isRunInto(Actor &a,Actor &b)
{
    if(pow(a.posit_x-b.posit_x,2)+pow(a.posit_y-b.posit_y,2)<pow(a.physique+b.physique,2))
        return true;
    return false;
}

Controller::Controller(View &vw):           //Controller基类构造
view(vw),
keystate{SDL_GetKeyboardState(nullptr)},
LeftClicked{false},
mouse_x{0}, mouse_y{0}
{
    event = new SDL_Event;
}

void Controller::TickControll()             //基类Tick函数
{
    while(SDL_PollEvent(event))
    {
        switch(event->type)
        {
            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&mouse_x, &mouse_y);
                break;
            case SDL_KEYDOWN:
                keystate = SDL_GetKeyboardState(nullptr);
                break;
            case SDL_MOUSEBUTTONDOWN:
                LeftClicked=SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON(SDL_BUTTON_LEFT);
                break;
            default:;
        }
    }
//    SDL_GetMouseState(&mouse_x, &mouse_y);
//    keystate = SDL_GetKeyboardState(nullptr);
//    LeftClicked=SDL_GetMouseState(nullptr, nullptr)&SDL_BUTTON(SDL_BUTTON_LEFT);
}

Controller::~Controller()
{
    delete event;
}

MM_Controller::MM_Controller(MM_Model &mod,View &vw)://主菜单 控制器构造
Controller(vw),
model(mod)
{
    view.DrawWorld();
}

void MM_Controller::TickControll()          //主菜单 Tick函数
{
    Controller::TickControll();
    
    if(keystate[SDL_SCANCODE_RETURN])       //回车选择
    {
        if(last_press==SDL_SCANCODE_RETURN&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_RETURN, choiceTitle(model.chosen_button);
    }
    if(keystate[SDL_SCANCODE_W])            //W向上选择
    {
        if(last_press==SDL_SCANCODE_W&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_W,--model.chosen_button;
    }
    if(keystate[SDL_SCANCODE_S])            //S向下选择
    {
        if(last_press==SDL_SCANCODE_S&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_S,++model.chosen_button;
    }
    
    if(model.chosen_button<0)model.chosen_button=0;
    else if(model.chosen_button>3)model.chosen_button=3;
    
    for (auto &i:model.bu_ist)              //鼠标选择
    {
        if(i->Betouched(mouse_x, mouse_y)&&LeftClicked)
            choiceTitle(i->identity);
    }
    
    model.bu_ist[model.chosen_button]->changeSelected(1);
    
    view.DrawWorld();
};

void MM_Controller::choiceTitle(int titleId)//选择的行为
{
    switch (titleId) {
        case 0:
            game_state=last_state;
            break;
        case 1:
            game_state=LOADMENU;
            break;
        case 2:
            game_state=-1;
            break;
        default:
            isRunning=false;
            break;
    }
}


PA_Controller::PA_Controller(PA_Model &mod,View &vw)://暂停菜单 控制器构造
Controller(vw),
model(mod)
{
    
}

void PA_Controller::TickControll()          //暂停菜单Tick函数
{
    Controller::TickControll();
    
    if(keystate[SDL_SCANCODE_RETURN])       //回车选择
    {
        choiceTitle(model.keyboardChosen);
        last_press=SDL_SCANCODE_RETURN;
        repeat=0;
    }
    if(keystate[SDL_SCANCODE_W])            //W
    {
        if(last_press==SDL_SCANCODE_W&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_W,--model.keyboardChosen;
    }
    if(keystate[SDL_SCANCODE_S])            //S
    {
        if(last_press==SDL_SCANCODE_S&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_S,++model.keyboardChosen;
    }
    
    if(model.keyboardChosen<0)model.keyboardChosen=0;
    else if(model.keyboardChosen>1)model.keyboardChosen=1;
    
    for (auto &i:model.bu_ist)              //鼠标选择
    {
        if(i->Betouched(mouse_x, mouse_y)&&LeftClicked)
            choiceTitle(i->identity);
    }
    
    model.bu_ist[model.keyboardChosen]->changeSelected(1);
    
    view.DrawWorld();
}

void PA_Controller::choiceTitle(int titleId)
{
    switch (titleId) {
        case 0:     //继续游戏 回到上一游戏状态
            game_state=last_state;
            break;
        default:    //返回主菜单 游戏状态调整为主菜单  上一游戏状态是暂停前上一状态
            game_state=MAINMENU;
            break;
    }
}

LD_Controller::LD_Controller(LD_Model &mod,View &vw)://加载菜单控制器构造
Controller(vw),
model(&mod)
{
    
}

void LD_Controller::TickControll()          //加载菜单tick函数
{
    Controller::TickControll();
    //回到主菜单
    if(keystate[SDL_SCANCODE_ESCAPE])
    {
        game_state=MAINMENU;
        
    }
    //选择载入游戏
    if(keystate[SDL_SCANCODE_RETURN])
    {
        if(last_press==SDL_SCANCODE_RETURN&&repeat<10)
            ++repeat;
        else
        {
            repeat=0,last_press=SDL_SCANCODE_RETURN;
            game_state=model->selected_level;
        }
    }
    //左右选择关卡
    if(keystate[SDL_SCANCODE_A])
    {
        if(last_press==SDL_SCANCODE_A&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_A,--model->selected_level;
    }
    else if(keystate[SDL_SCANCODE_D])
    {
        if(last_press==SDL_SCANCODE_D&&repeat<10)
            ++repeat;
        else
            repeat=0,last_press=SDL_SCANCODE_D,++model->selected_level;
    }
    
    if(LeftClicked)
    {
        if(last_press==SDL_SCANCODE_P&&repeat<5)
            ++repeat;
        else
        {
            repeat=0,last_press=SDL_SCANCODE_P;
            if(mouse_x<160)
            {
                --model->selected_level;
            }
            else if(mouse_x>1040)
            {
                ++model->selected_level;
            }
            else if(progress>=model->selected_level)game_state=model->selected_level;
        }
    }
    
    if(model->selected_level>FIGHT)model->selected_level=FIGHT;
    else if(model->selected_level<DAWN)model->selected_level=DAWN;
    
    DrawWorld();
}

void LD_Controller::DrawWorld()
{
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, model->bg_picture, NULL, NULL);
    if(!(model->DisplayerMove()))
    {
        model->score=SDL_CreateTextureFromSurface(renderer,TTF_RenderUTF8_Blended(model->sft,std::to_string(h_score[model->selected_level]).c_str(),model->clr));
        model->sc.w=std::to_string(h_score[model->selected_level]).size()*40;
            SDL_RenderCopy (renderer,model->score, NULL, &model->sc);
    }
    
    model->print.x=model->display_posit;
    for(int i=0;i<5;++i,model->print.x+=880)
    {
        if(i+DAWN>progress)
            SDL_RenderCopy(renderer, model->undentifined, NULL, &model->print);
        else
            SDL_RenderCopy(renderer, model->picture[i], NULL, &model->print);
    }
    
    SDL_RenderPresent(renderer);
}

DN_Controller::DN_Controller(DN_Model &mod,View &vw)://LevelI 控制器构造
Controller(vw),
model(&mod)
{
    
}

void DN_Controller::TickControll()          //LevelI Tick函数
{
    ++model->usedtime;
    Controller::TickControll();
    
    //胜利检测
    if(/*keystate[SDL_SCANCODE_SPACE]&&*/model->remain_bean<=0
       &&model->cast[0]->posit_x>=13&&model->cast[0]->posit_x<17&&
       model->cast[0]->posit_y>=15&&model->cast[0]->posit_y<19)
    {
        Cutscenes e(10,model->usedtime);
        game_state=LOVE;
    }
    //Escape键
    if(keystate[SDL_SCANCODE_ESCAPE])
    {
        game_state=PAUSEMENU;
        repeat=0;
    }
    //技能冷却
    model->SkillCooling();
    //八向操控
    ((Player *)model->cast[0])->iscontrolled=true;
    if(keystate[SDL_SCANCODE_W])
    {
        if(keystate[SDL_SCANCODE_A])model->cast[0]->direction=315;
        else if(keystate[SDL_SCANCODE_D])model->cast[0]->direction=45;
        else model->cast[0]->direction=0;
    }
    else if(keystate[SDL_SCANCODE_A])
    {
        if(keystate[SDL_SCANCODE_S])model->cast[0]->direction=225;
        else model->cast[0]->direction=270;
    }
    else if(keystate[SDL_SCANCODE_S])
    {
        if(keystate[SDL_SCANCODE_D])model->cast[0]->direction=135;
        else model->cast[0]->direction=180;
    }
    else if(keystate[SDL_SCANCODE_D])model->cast[0]->direction=90;
    else ((Player *)model->cast[0])->iscontrolled=false;
    //技能检测
    if(keystate[SDL_SCANCODE_Q]&&model->skills[0]->rate>=1)
    {
        model->skills[0]->rate=0;
        model->cast[0]->buff[0]=120;
    }
    //吃豆检测
    Actor *bl;
    for(int i=(int)model->cast[0]->posit_y-1;i<(int)model->cast[0]->posit_y+1;++i)
        for(int j=(int)model->cast[0]->posit_x-1;j<(int)model->cast[0]->posit_x+1;++j)
        {
            bl=model->map[i][j].blind;
            if(bl&&isRunInto(*bl,*model->cast[0]))
            {
                delete bl;
                model->map[i][j].blind=nullptr;
                --model->remain_bean;
                
                model->r_bean_title=std::to_string(model->remain_bean);
                SDL_Texture *replace[2];//可以考虑作为长生存周期变量
                replace[0]=SDL_CreateTextureFromSurface(renderer,TTF_RenderUTF8_Blended(model->r_bean_ft, model->remain_bean?model->r_bean_title.c_str():"Run!",model->r_bean_clr));
                replace[1]=nullptr;
                model->bu_ist[2]->changeSTT(&replace);
            }
        }
    //碰撞检测
    for(auto actor=model->cast.begin()+1;actor!=model->cast.end();++actor)
    {
        if(isRunInto(**(model->cast.begin()), **actor))
            game_state=-2;
    }
    //器具处理   逻辑优化
    for(auto mob:model->mo_ist)
    {
        if((*mob)->bedraw)
        {
            if((*mob)->fn_type==1)
            {
                if(((Cage*)(*mob))->is_break)
                {
                    (*mob)->bedraw=false;
                    model->cast.push_back(new Dog((*mob)->posit_x,(*mob)->posit_y));
                }
            }
            if(*mob)(*mob)->Action();
        }
    }
    //成员行动
    for(auto actor:model->cast)
    {
        if(!(actor->buff[2]))
        {
            if(actor->find_way_req)
                model->FindWay(actor,(block *)model->map,30);   //帮助寻找路径
            actor->Action();                //让每一个演员行动，并限制位置
            PositLimit(actor->posit_x,actor->posit_y,actor->physique);
        }
        else --actor->buff[2];
    }
    view.DrawWorld();
}

void DN_Controller::PositLimit(double &p_x,double &p_y,double physique)
{
    if(model -> map[(int)p_y - 1][(int)p_x].flag == 0 && p_y - (int)p_y < physique)
        p_y = (int)p_y + physique;
    else if(model -> map[(int)p_y+1][(int)p_x].flag == 0 && 1 - p_y + (int)p_y < physique)
        p_y = (int)p_y + 1 - physique;
    
    if(model -> map[(int)p_y][(int)p_x - 1].flag == 0 && p_x - (int)p_x < physique)
        p_x = (int)p_x + physique;
    else if(model -> map[(int)p_y][(int)p_x+1].flag == 0 && 1 - p_x + (int)p_x < physique )
        p_x = (int)p_x + 1 - physique;
}

LV_Controller::LV_Controller(LV_Model &mod,View &vw)://Level II 控制器构造
model(&mod),
Controller(vw)
{
    
}

void LV_Controller::TickControll()          //Level II Tick函数
{
    Controller::TickControll();
    if(keystate[SDL_SCANCODE_ESCAPE])
    {
        game_state=PAUSEMENU;
    }
    
    if(model->bu_ist[0]->Betouched(mouse_x, mouse_y)&&LeftClicked)
    {
        SDL_SetClipboardText("https://www.immaturity.space/forever-player/escape");
        model->bu_ist.push_back(new Button(1,900,200,150,650,NULL,NULL,"Web has copied to clipboard",def_font_,def_clr,def_clr));
    }
    view.DrawWorld();
}


HM_Controller::HM_Controller(Model &mod,View &vw)://Level III 控制器构造
Controller(vw)
{
    
}

void HM_Controller::TickControll()          //Level III Tick函数
{
    Controller::TickControll();
    isRunning=false;
}

