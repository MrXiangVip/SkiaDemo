//
// Created by xshx on 2023/2/21.
//
#include <stdio.h>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
//#include "SDL_ttf.h"

#endif


SDL_Window *window;
SDL_Renderer *pRenderer;

int main()
{

    int width = 800;
    int height = 480;
    //初始化SDL为视频显示
    SDL_Init(SDL_INIT_VIDEO);
    //创建窗口
    window = SDL_CreateWindow("Hello Skia", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (window == NULL) {
        return -1;
    }
    pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* bmp = SDL_LoadBMP("../res/64x48.bmp");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(pRenderer, bmp);
    SDL_RenderCopy(pRenderer, tex, NULL, NULL);
    //显示到窗口
    SDL_RenderPresent(pRenderer);
    SDL_Delay(2000);

//    SDL_Color color = { 255, 255, 255, 255 };
//    TTF_Font* font = TTF_OpenFont("DS-DIGIB.TTF", 18);
//    SDL_Surface* surf = TTF_RenderText_Blended(font, "are you OK ?", color);
//    SDL_Texture* ftex = SDL_CreateTextureFromSurface(pRenderer, surf);
//    SDL_RenderCopy(pRenderer, ftex, NULL, NULL);
    //显示到窗口
//    SDL_RenderPresent(pRenderer);

    SDL_FreeSurface(bmp);
    SDL_DestroyTexture(tex);

}

