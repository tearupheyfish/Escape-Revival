//
//  pool.cpp
//  Escape
//
//  Created by Digger on 2020/6/11.
//  Copyright © 2020 Digger. All rights reserved.
//

#include "pool.hpp"

#include "SDL_image.h"
#include "iostream"

SDL_Texture* ResourcePool::FindResource(std::string key)        //加载纹理
{
    u_m::iterator it=DOM.find(key);
    if(it!=DOM.end())
        return it->second;
    else
    {
        SDL_Surface *surf;
        surf=IMG_Load(key.c_str());
        if(!surf)
        {
            std::cerr<<"surface read failure: "<<key<<".ErrorInfo: "<<SDL_GetError();
        }
        auto tet=SDL_CreateTextureFromSurface(renderer, surf);
        DOM.insert(std::pair<std::string,SDL_Texture*>{key,tet});
        SDL_FreeSurface(surf);
        return tet;
    }
};