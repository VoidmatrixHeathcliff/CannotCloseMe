#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
using namespace std;

#define _FPS_ 60

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 360

#define random(a, b) (rand() % (b - a) + a)

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Imm32.lib")
#pragma comment(lib, "Version.lib")

#pragma comment( linker, "/subsystem:windows /entry:mainCRTStartup" )

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	string strFontPath = "C:/Windows/Fonts/";
	TTF_Font* pFontEnglish = TTF_OpenFont((strFontPath + "msyh.ttc").c_str(), 27);

	if (!pFontEnglish)
	{
		pFontEnglish = TTF_OpenFont("msyh.ttc", 27);
		if (!pFontEnglish)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "字体文件加载失败", "检测到字体文件默认路径错误：C:\\Windows\\Fonts\\msyh.ttc\n\n• 以下方式可能修复程序：\n      进入系统盘对应字体目录复制此字体文件或联网下载此字体文件至程序同一目录下，\n并重启程序，注意确保字体文件名为：msyh.ttc\n\n联系开发者：Voidmatrix@outlook.com", nullptr);
			TTF_Quit();
			SDL_Quit();
			return -1;
		}
		else
			strFontPath = "";
	}

	TTF_Font* pFontChinese = TTF_OpenFont((strFontPath + "msyh.ttc").c_str(), 35);

	srand((int)time(0));

	SDL_Window* pWindow = SDL_CreateWindow(
		"CANNOT CLOSE ME !!!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS
	);

	int widthDesktop, heightDesktop;
	SDL_GetWindowSize(pWindow, &widthDesktop, &heightDesktop);
	SDL_SetWindowFullscreen(pWindow, 0);

	SDL_Renderer* pWRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetWindowOpacity(pWindow, 0.90);

	SDL_Color clrBack = { 215, 215, 215, 255 }, clrFront = { 5, 5, 5, 255 };

	int unitClrBack = -1, unitClrFront = 5;

	while (true)
	{
		Uint32 _start = SDL_GetTicks();

		SDL_SetRenderDrawColor(pWRenderer, clrBack.r, clrBack.g, clrBack.b, clrBack.a);
		SDL_RenderClear(pWRenderer);

		if (clrBack.r < 125 || clrBack.r > 215)
			unitClrBack = -unitClrBack;

		clrBack.r += unitClrBack;
		clrBack.g += unitClrBack;
		clrBack.b += unitClrBack;

		if (clrFront.r < 5 || clrFront.r > 100)
			unitClrFront = -unitClrFront;

		clrFront.r += unitClrFront;
		clrFront.g += unitClrFront;
		clrFront.b += unitClrFront;

		SDL_Surface* pSurfaceText_English = TTF_RenderUTF8_Blended(pFontEnglish, "You can't close ME !", clrFront);
		SDL_Surface* pSurfaceText_Chinese = TTF_RenderUTF8_Blended(pFontChinese, "你永远也无法关掉我~", clrFront);

		SDL_Texture* pTextureText_English = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceText_English);
		SDL_Texture* pTextureText_Chinese = SDL_CreateTextureFromSurface(pWRenderer, pSurfaceText_Chinese);

		SDL_Rect rcText_English = {
			WINDOW_WIDTH / 2 - pSurfaceText_English->w / 2,
			WINDOW_HEIGHT / 2 - pSurfaceText_English->h / 2 + 50,
			pSurfaceText_English->w,
			pSurfaceText_English->h
		};
		SDL_Rect rcText_Chinese = {
			WINDOW_WIDTH / 2 - pSurfaceText_Chinese->w / 2,
			WINDOW_HEIGHT / 2 - pSurfaceText_Chinese->h / 2 - 50,
			pSurfaceText_Chinese->w,
			pSurfaceText_Chinese->h
		};

		SDL_RenderCopy(pWRenderer, pTextureText_English, nullptr, &rcText_English);
		SDL_RenderCopy(pWRenderer, pTextureText_Chinese, nullptr, &rcText_Chinese);

		SDL_DestroyTexture(pTextureText_English);
		SDL_DestroyTexture(pTextureText_Chinese);

		SDL_FreeSurface(pSurfaceText_English);
		SDL_FreeSurface(pSurfaceText_Chinese);

		SDL_Event _event;
		if (SDL_PollEvent(&_event))
		{
			if (_event.window.event == SDL_WINDOWEVENT_ENTER || _event.type == SDL_MOUSEMOTION)
				SDL_SetWindowPosition(pWindow, random(0, widthDesktop - 100), random(0, heightDesktop - 100));
		}

		SDL_RenderPresent(pWRenderer);

		Uint32 _end = SDL_GetTicks();

		if (_end - _start < 1000 / _FPS_)
			SDL_Delay(1000 / _FPS_ - (_end - _start));
	}

	return 0;
}