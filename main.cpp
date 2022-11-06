
#include "SkBitmap.h"
#include "core/SkCanvas.h"
#include "core/SkTypeface.h"

#ifdef _WIN32

#include <SDL.h>

#else

#include <SDL2/SDL.h>

#endif

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
};

//创建SkBitmap对象并在Bitmap上绘制
SkBitmap draw(int w, int h) {
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

//程序的入口点
int main(int args, char *argv[]) {
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
    //声明RGBA结构体
    RGBA rgba;
    //声明矩形
    SDL_Rect rect;
    //声明窗口的宽高
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
    //获取绘制后的Bitmap
    bitmap = draw(width, height);
    //通过Bitmap的像素数据创建表面
    surface = SDL_CreateRGBSurfaceFrom(bitmap.getPixels(), width, height, 32, width * 4, rgba.rmask, rgba.gmask,
                                       rgba.bmask, rgba.amask);
    //通过SDL_Surface创建矩形
    rect = create_rect(surface);
    //创建渲染器
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //清理渲染器
    SDL_RenderClear(renderer);
    //创建纹理
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    //辅助纹理到渲染器
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    //显示到窗口
    SDL_RenderPresent(renderer);
    //延时5秒钟
    SDL_Delay(5000);
    //释放表面
    SDL_FreeSurface(surface);
    //释放纹理
    SDL_DestroyTexture(texture);
    //释放渲染器
    SDL_DestroyRenderer(renderer);
    //释放窗口
    SDL_DestroyWindow(window);
    //结束SDL
    SDL_Quit();
    //程序退出
    return 0;
}