//
// Created by xshx on 2023/2/21.
//

#include <SkBitmap.h>
#include <core/SkCanvas.h>
#include <core/SkTypeface.h>
#include <string.h>

#ifdef _WIN32

#pragma warning(disable:4996)
#include <SDL.h>
#include <SDL_thread.h>

#else

#include <SDL2/SDL.h>

#endif

//声明窗口
SDL_Window *window;
//声明绘图表面
SDL_Surface *surface;
//声明渲染器
SDL_Renderer *renderer;
//声明纹理
SDL_Texture *texture;
//声明Bitmap
SkBitmap bitmap;
//声明矩形
SDL_Rect rect;
//声明窗口的宽高
int width = 800;
int height = 480;
//线程循环标记
bool quit = false;
//帧数
int fps;
//待绘制的FPS数据
char chars[15];

//创建RGBA结构体
struct RGBA {
    //设置Red
    Uint32 rmask = 0x00ff0000;
    //设置Green
    Uint32 gmask = 0x0000ff00;
    //设置Blue
    Uint32 bmask = 0x000000ff;
    //设置Alpha
    Uint32 amask = 0xff000000;
}RGBA;


//创建SkBitmap对象并在Bitmap上绘制
SkBitmap draw(int w, int h) {
    printf("draw width %d , height %d\n", w, h);
    //声明
    SkBitmap bitmap;
    //设置Bitmap的配置信息
    bitmap.setInfo(SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
    //转换为像素填充
    bitmap.allocPixels();
    //创建画布
    SkCanvas canvas(bitmap);
    //创建画笔
    SkPaint paint;
    //设置画布颜色
    canvas.clear(SK_ColorWHITE);
    //设置画笔抗锯齿
    paint.setAntiAlias(true);
    //设置画笔颜色(此处为红色)
    paint.setARGB(255, 255, 0, 0);
    //绘制圆形
    canvas.drawCircle(80, 80, 40, paint);
    //绘制线段
    canvas.drawLine(0, 280, w, 280, paint);
    //设置字体大小
    paint.setTextSize(60);
    //绘制字体
    canvas.drawString("Hello Skia", 300, 150, paint);
    //设置字体大小
    paint.setTextSize(20);
    //绘制FPS
    printf("chars %s\n", chars);
    canvas.drawText(chars, sizeof((char*)chars) - 1, 10, 20, paint);
    //返回SkBitmap对象
    return bitmap;
}

//通过SDL_Surface创建SDL_Rect
SDL_Rect create_rect(SDL_Surface *surface) {
    //创建SDL_Rect对象
    SDL_Rect src = { 0, 0, surface->w, surface->h };
    //返回SDL_Rect对象
    return src;
}

//SDL线程调用的方法
int update(void *data) {
    while (!quit) {
        //判断表面是否为空
        if (surface != NULL) {
            //释放表面
            SDL_FreeSurface(surface);
        }
        //判断纹理是否为空
        if (texture != NULL) {
            //释放纹理
            SDL_DestroyTexture(texture);
        }
        //绘制位图
        bitmap = draw(rect.w, rect.h);
        //创建纹理
        surface = SDL_CreateRGBSurfaceFrom(bitmap.getPixels(), rect.w, rect.h, 32, rect.w * 4, RGBA.rmask, RGBA.gmask,
                                           RGBA.bmask, RGBA.amask);
        //创建纹理
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        //复制纹理
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        //提交渲染
        SDL_RenderPresent(renderer);
        //休眠16.6毫秒
        SDL_Delay(16.6);
        //FPS自增长
        fps++;
    }
    return 0;
}

//FPS更新线程方法
int updateFPS(void *data) {
    while (!quit){
        //拼接字符串
        sprintf(chars, "FPS: %d", fps);
        //重置FPS
        fps = 0;
        //每秒获取一次刷新速率
        SDL_Delay(1000);
    }
    return 0;
}

//初始化
void init() {
    //初始化SDL为视频显示
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    //创建窗口
    window = SDL_CreateWindow("Hello Skia", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //清理渲染器
    SDL_RenderClear(renderer);
    //获取窗口表面
    surface = SDL_GetWindowSurface(window);
    //通过SDL_Surface创建矩形
    rect = create_rect(surface);
}

//程序的入口点
int main(int args, char *argv[]) {
    //初始化
    init();
    //创建并启动线程
    SDL_Thread *thread = SDL_CreateThread(update, "update", (void *)NULL);
    //创建并启动FPS记录线程
    SDL_Thread *fpsThread = SDL_CreateThread(updateFPS, "fps", (void *)NULL);
    //事件队列
    SDL_Event e;
    //事件处理
    while (!quit) {
        //轮询事件
        while (SDL_PollEvent(&e) != 0) {
            printf("event %d \n", e.type);
            //用户退出事件
            if (e.type == SDL_QUIT) {
                //设置循环标记
                quit = true;
            }
        }
    }
    //判断表面是否为空
    if (surface != NULL) {
        //释放表面
        SDL_FreeSurface(surface);
    }
    //判断纹理是否为空
    if (texture != NULL) {
        //释放纹理
        SDL_DestroyTexture(texture);
    }
    //判断渲染器是否为空
    if (renderer != NULL) {
        //释放渲染器
        SDL_DestroyRenderer(renderer);
    }
    //释放窗口
    SDL_DestroyWindow(window);
    //结束SDL
    SDL_Quit();
    //程序退出
    return 0;
}