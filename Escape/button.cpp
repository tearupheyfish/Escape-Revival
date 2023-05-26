#include "button.hpp"
#include <cstring>
extern SDL_Color def_clr;
extern SDL_Renderer *renderer;

bool Button::Betouched(int mouse_x, int mouse_y)
{
    selected_title=canBetouch&&mouse_y>ofTitle.y&&mouse_y<ofTitle.y+ofTitle.h&&mouse_x>ofTitle.x&&mouse_x<ofTitle.x+ofTitle.w;
    return selected_title;
}


Button::Button(int id/*按钮标识*/,
               int picture_width,int picture_height/*提供的纹理的显示尺寸*/,
               int p_x,int p_y/*显示位置*/,
               SDL_Texture *bg/*背景材质*/,
               SDL_Texture *bgt/*被触摸背景材质*/,
               const char *title/*按钮文字*/,
               TTF_Font *ft/*字体*/,
               SDL_Color &color/*按钮色泽*/,
               SDL_Color &touch/*被触摸按钮色泽*/
               ):                   //按钮构造
beDraw(true),canBetouch(true),
selected_title(0),
height_rate(0.7),width_rate(0.8),
title_size(strlen(title)),
identity(id)
{
    this->title[0]=SDL_CreateTextureFromSurface(renderer,TTF_RenderUTF8_Blended(ft, title,color));
    if(memcmp((const void *)&color,(const void *)&touch,sizeof(SDL_Color)))
        this->title[1]=SDL_CreateTextureFromSurface(renderer, TTF_RenderUTF8_Blended(ft,title,touch));
    else
        this->title[1]=this->title[0];
    back_ground[0]=bg,back_ground[1]=bgt;
    ofPicture.x=p_x,ofPicture.y=p_y;
    ofPicture.w=picture_width,ofPicture.h=picture_height;
    SortOut();
}

void Button::SortOut()              //按钮居中对齐
{
    ofTitle.h=ofPicture.h*height_rate,ofTitle.y=ofPicture.y+ofPicture.h*(1-height_rate)/2;
    ofTitle.w=title_size*ofTitle.h*0.33;
    if(ofTitle.w>ofPicture.w*width_rate)ofTitle.w=ofPicture.w*width_rate;
    ofTitle.x=ofPicture.x+(ofPicture.w>>1)-(ofTitle.w>>1);
}

void Button::setHeightRate(double rate)//设顶文字与按钮高比例
{
    height_rate=rate;
}

void Button::setWidthRate(double rate)//设置宽度比例上限
{
    width_rate=rate;
}

void Button::changeSelected(int s_title)//改变按钮状态
{
    selected_title=s_title;
}

void Button::setCanBeTouched(bool bl)//设置按钮是否可被触碰
{
    canBetouch=bl;
}

void Button::changeSTT(SDL_Texture *(*replace)[2])
{
    if((*replace)[0])
    {
//        delete title[0];
        title[0]=(*replace)[0];
    }
    if((*replace)[1])
    {
//        delete title[1];
        title[1]=(*replace)[1];
    }
}
