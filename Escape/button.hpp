#ifndef button_hpp
#define button_hpp

#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mouse.h>

//#include "tool.hpp"

class Button                        //--按钮类
{
    //为什么不能定义静态成员？？？？
protected:
    SDL_Texture *back_ground[2];    //按钮材质,被触碰按钮材质
    SDL_Texture *title[2];          //文字材质,被触碰时文字材质
    int selected_title;             //被选中的材质
    SDL_Rect ofPicture,ofTitle;     //显示范围
    double height_rate,width_rate;  //文字比例
    size_t title_size;              //文字长度
public:
    const int identity;             //按钮标识
    bool beDraw;                    //是否被绘制
    bool canBetouch;                //是否允许被触摸
    bool Betouched(int m_x,int m_y);//是否被触摸
    
    Button(int id,int p_w,int p_h,int p_x,int p_y,SDL_Texture *bg,SDL_Texture *bgt,const char *title,TTF_Font *ft,SDL_Color &color,SDL_Color &touch);
    void setCanBeTouched(bool bl);
    void setHeightRate(double rate);
    void setWidthRate(double rate);
    void SortOut();
    void changeSelected(int title); //改变材质选择
    void changeSTT(SDL_Texture *(*replace)[2]);
/*friend*/
    friend class View;
    
};

#endif /* button_hpp */
