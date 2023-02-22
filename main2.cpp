//
// Created by xshx on 2023/2/21.
//

#include <stdio.h>
#ifdef _WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

int main()
{
    SDL_Window *window = NULL; // 窗口
    SDL_Renderer *render = NULL;// 渲染器
    SDL_Texture *texture = NULL; // 纹理
    SDL_Event event; // 监听事件
    int quit = 0; // 是否退出
    int win_w = 640; // 窗口宽度
    int win_h = 480; // 窗口高度
    SDL_Rect rect; // 绘制的矩形
    rect.w = 40;
    rect.h = 40;

    // 初始化
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口，参数分别是窗口的标题，x,y,w,h，最后一个参数是一些flag(SDL_WINDOW_SHOWN表示将窗口显示出来)
    window = SDL_CreateWindow("窗口标题",300,300,win_w,win_h,SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("创建窗口失败\n");
        goto __EXIT;
    }

    // 窗口渲染
    // 创建渲染器
    render = SDL_CreateRenderer(window,-1,0);// 创建渲染器
    if (!render)
    {
        printf("创渲染器口失败\n");
        goto __DESTROY;
    }
    // SDL_SetRenderDrawColor(render,255,0,0,255); // 设置窗口颜色，后面几个参数分别是RGBA
    // SDL_RenderClear(render); // 清空窗口
    // SDL_RenderPresent(render); // 将窗口展示出来

    // 创建纹理（第二个参数是指明像素格式，可以是YUV或RGB，第3个参数是指明纹理类型，可以是stream(流)或target(一般类型)）
    texture = SDL_CreateTexture(render,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,win_w,win_h);
    if (!texture)
    {
        printf("纹理创建失败!\n");
        goto __DESTROY;
    }


    // SDL_Delay(10000);
    // 监听退出事件
    do
    {
        // SDL_WaitEvent(&event);
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
            quit = 1;
        }else{
            printf("事件类型:%d\n", event.type);
        }

        // 随机设置矩形位置
        rect.x = rand()%600;
        rect.y = rand()%440;

        // 对纹理进行渲染
        SDL_SetRenderTarget(render,texture); // 改变渲染目标为纹理
        SDL_SetRenderDrawColor(render,255,255,255,255); // 设置纹理颜色(颜色为RGBA)
        SDL_RenderClear(render); // 清空渲染器
        // 绘制矩形并给矩形填充颜色
        SDL_RenderDrawRect(render,&rect); // 绘制矩形
        SDL_SetRenderDrawColor(render,rand()%256,rand()%256,rand()%256,255); // 给矩形设置随机色
        SDL_RenderFillRect(render,&rect); // 填充
        // 恢复成默认渲染目标
        SDL_SetRenderTarget(render,NULL);
        // 将目标输出到显卡
        SDL_RenderCopy(render,texture,NULL,NULL);
        SDL_RenderPresent(render); // 显卡计算完后进行显示
        SDL_Delay(500);

    } while (!quit);

    __DESTROY:
    if (window) // 销毁window
    {
        SDL_DestroyWindow(window);
    }
    if (render) // 销毁渲染器
    {
        SDL_DestroyRenderer(render);
    }
    if (texture) // 销毁纹理
    {
        SDL_DestroyTexture(texture);
    }


    __EXIT:
    // 退出
    SDL_Quit();
    return 0;
}