#include <stdio.h>
#include <stdlib.h>
#include <SDL.h> // Using SDL
#include "SDL2_gfxPrimitives.h" // Using SDL2_gfx
#include "SDL_image.h"  // Using SDL2 image extension library 
#include "SDL_mixer.h" 
#include "SDL_ttf.h"
#include <time.h>
#include <thread>


//Screen dimension constants
const int WIDTH = 1280;
const int HEIGHT = 780;

const int pika1_FRAME = 12;
const int pika1_jump_FRAME = 12;
const int pika2_FRAME = 12;
const int pika2_jump_FRAME = 12;
int pika1_jump_index = 0;
int pika2_jump_index = 0;

int ballFlag = 0;
bool colFlag = false;
float netx = WIDTH / 2 - 11;
int viY = 100;
float ballx = 100;
float ball2x = ballx;
float bally = -96;
float ball2y = bally;
int velocity_x = 10;
int velocity_y = 15;
int fisrttimecollision = 0;
int ballincloudtime = 1;
int colopen = 5;
//道具出現
int prop_exist = 0;
float prop_x = 0;
//1 p1 win ： 2 p2 win
int end_game = 0;
//移動
int pika1_V = 0;
float pika1_jump_X = 300;
float pika1_X = 300;

int pika2_V = 0;
float pika2_jump_X = 300;
float pika2_X = 900;

bool pika1_jump_flag = false;
int  pika_jump_power = 0;
float pika1_jump_y = 460;
bool pika2_jump_flag = false;
float pika2_jump_y = 460;

static const int M_WIDTH = 20;
static const int VELOCITY = 10;
const int SOLID = 100;
const int SHADED = 101;
const int BLENDED = 102;


char pika1_grade1 = '0';
char pika2_grade1 = '0';
char pika1_grade_time = 0;
char pika2_grade_time = 0;
char pika1_grade2 = '\0';
char pika2_grade2 = '\0';
int kill_ballx = 3;
int kill_bally = 3;
bool kill_ball_open = false;
bool kill_ball_flag = false;
int ball_alpha = 255;
int ball2_alpha = 0;
int col_alpha = 0;
//bool col_flag = false;
bool teeoff_pika1 = true;
bool teeoff_pika2 = false;
//int kill_bally_open = 3;
float colx = ballx;
float coly = bally;
bool movenet = false;
bool pika1alpha = false;
bool pika2alpha = false;
bool pikafast = false;
bool manyball = false;
bool pika1fast = false;
bool pika2fast = false;
bool openhole = false;
bool pikaalpha = false;//
int netuser = 0;//0為左邊 1為右邊
int alphauser = 0;//0為左邊 1為右邊
int tmpnetuser = 0;
int holeuser = 0;
int fastuser = 0;
int pika1_alpha = 255;
int pika2_alpha = 255;
float pika1faster = 1;
float pika2faster = 1;
int pika1alphatime = 0;
int pika2alphatime = 0;
int fast1time = 0;
int fast2time = 0;
int killer = 0;
int manyuser = 0;
int holex = 0;
int holealpha = 0;
int holetime = 0;
int holerand = 200;
int balltoosmalltime = 0;

//ImageData col;
//選分數、速度
int point_select = 1, speed_select = 1;
int propalpha = 0;
int propuser = 0;
bool propflag = false;

struct ImageData
{
	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
};

struct cloud
{
	SDL_Texture *texture;
	ImageData cl;
	int clx;
	int cly;
}cloud[10];

struct TextData
{
	char path[100];
	SDL_Texture *texture;
	int width;
	int height;
};

enum MouseState
{
	NONE = 0,
	OUT = 1, // Mouse out
	IN_LB_SC = 2,  // Inside, Left Button, Single Click
	IN_RB_SC = 3,  // Inside, RIGHT Button, Single Click
	IN_LB_DC = 4,  // Inside, Left Button, Double Click
	IN_RB_DC = 5,  // Inside, RIGHT Button, Double Click
	IN_LB_PR = 6,  // Inside, Left Button, Press
	IN_RB_PR = 7,  // Inside, Left Button, Press
	IN_WU = 8,  // Inside, Wheel UP
	IN_WD = 9,  // Inside, Wheel DOWN
	HOVER = 10 // Mouse hover
};

void mouseHandleEvent(SDL_Event* e, MouseState *mouseState, int *x, int *y)
{
	// If mouse event happened
	// https://wiki.libsdl.org/SDL_Event
	// https://wiki.libsdl.org/SDL_MouseMotionEvent
	// https://wiki.libsdl.org/SDL_MouseButtonEvent
	// https://wiki.libsdl.org/SDL_MouseWheelEvent
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP || e->type == SDL_MOUSEWHEEL)
	{
		// Get mouse position
		// https://wiki.libsdl.org/SDL_GetMouseState		
		SDL_GetMouseState(x, y);
		//printf("(%d, %d)\n", *x, *y)	

		switch (e->type)
		{
		case SDL_MOUSEBUTTONDOWN:
			break;
		case SDL_MOUSEBUTTONUP:
			if (e->button.button == SDL_BUTTON_LEFT && e->button.clicks == 1)
			{
				*mouseState = IN_LB_SC;
			}
			else if (e->button.button == SDL_BUTTON_RIGHT && e->button.clicks == 1)
			{
				*mouseState = IN_RB_SC;
			}
			break;
		}
	}
}


int initSDL(); // Starts up SDL and creates window
void closeSDL(); // Frees media and shuts down SDL
TextData loadTextTexture(const char *str, const char *fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb);
void textRender(SDL_Renderer *renderer, TextData text, int posX, int posY, int cx, int cy, double angle, SDL_RendererFlip flip);
bool setTextureAlpha(SDL_Texture *texture, Uint8 alpha);

ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b);
void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY);
bool loadAudio();
void draw_select(SDL_Renderer *renderer);
Uint32 pika2_action(Uint32 interval, void* param); // Timer callback function
Uint32 pika2_jump(Uint32 interval, void *param);
Uint32 prop_action(Uint32 interval, void *param);
Uint32 manyballaction(Uint32 interval, void *param);
Uint32 holeaction(Uint32 interval, void *param);
Uint32 netaction(Uint32 interval, void *param);


SDL_Window *window = NULL; // The window we'll be rendering to
SDL_Renderer *renderer = NULL; // The window renderer


Mix_Music *music1 = NULL;
Mix_Chunk *effect1 = NULL;



int initSDL()
{
	// Initialize SDL	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		// Error Handling		
		printf("SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	// Create window	
	// SDL_WINDOWPOS_UNDEFINED: Used to indicate that you don't care what the window position is.
	window = SDL_CreateWindow("OOP SDL Tutorial", 50, 50, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 2;
	}

	// Initialize PNG loading	
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image failed: %s\n", IMG_GetError());
		return 3;
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return 4;
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return 5;
	}


	// Create renderer	
	// VSync allows the rendering to update at the same time as when your monitor updates during vertical refresh.
	// For this tutorial it will make sure the animation doesn't run too fast. 
	// Most monitors run at about 60 frames per second and that's the assumption we're making here. 
	// If you have a different monitor refresh rate, that would explain why the animation is running too fast or slow.

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 3;
	}

	return 0;
}


void closeSDL()
{
	// Free loaded image
	Mix_FreeChunk(effect1);
	Mix_FreeMusic(music1);


	// Destroy renderer	
	// Destroy window	
	// Quit Image subsystem
	// Quit SDL subsystems
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}
TextData loadTextTexture(const char *str, const char *fontPath, int fontSize, Uint8 fr, Uint8 fg, Uint8 fb, int textType, Uint8 br, Uint8 bg, Uint8 bb)
{
	TextData text;

	// TTF Font sturct
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_Font *ttfFont = NULL;

	// Open the font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	ttfFont = TTF_OpenFont(fontPath, fontSize); //("../fonts/lazy.ttf", 28);
	if (ttfFont == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	// A structure that represents a color.
	// https://wiki.libsdl.org/SDL_Color
	SDL_Color textFgColor = { fr, fg, fb }, textBgColor = { br, bg, bb };

	// Render text surface
	// The way SDL_ttf works is that you create a new image from a font and color. 
	// We're going to be loading our image from text rendered by SDL_ttf instead of a file.
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html#SEC42		

	SDL_Surface *textSurface = NULL;

	// Creates a solid/shaded/blended color surface from the font, text, and color given.
	// https://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html
	if (textType == SOLID)
	{
		// Quick and Dirty
		textSurface = TTF_RenderText_Solid(ttfFont, str, textFgColor);
	}
	else if (textType == SHADED)
	{
		// Slow and Nice, but with a Solid Box
		textSurface = TTF_RenderText_Shaded(ttfFont, str, textFgColor, textBgColor);
	}
	else if (textType == BLENDED)
	{
		// Slow Slow Slow, but Ultra Nice over another image
		textSurface = TTF_RenderText_Blended(ttfFont, str, textFgColor);
	}

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		// Create texture from surface pixels
		text.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (text.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get text dimensions and information
		text.width = textSurface->w;
		text.height = textSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(textSurface);
	}

	// Free font
	// https://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_frame.html
	TTF_CloseFont(ttfFont);

	//return font data
	return text;
}


void textRender(SDL_Renderer *renderer, TextData text, int posX, int posY, int cx, int cy, double angle, SDL_RendererFlip flip)
{
	SDL_Rect r;
	r.x = posX;
	r.y = posY;
	r.w = text.width;
	r.h = text.height;

	SDL_Point center = { cx, cy };

	SDL_RenderCopyEx(renderer, text.texture, NULL, &r, angle, &center, flip);
}




bool setTextureAlpha(SDL_Texture *texture, Uint8 alpha)//設置圖片透明度
{
	// Set and enable standard alpha blending mode for a texture
	// https://wiki.libsdl.org/SDL_SetTextureBlendMode
	if (SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND) == -1)
	{
		printf("SDL_SetTextureBlendMode failed: %s\n", SDL_GetError());
		return -1;
	}

	// Modulate texture alpha
	// https://wiki.libsdl.org/SDL_SetTextureAlphaMod
	if (SDL_SetTextureAlphaMod(texture, alpha) == -1)
	{
		printf("SDL_SetTextureAlphaMod failed: %s\n", SDL_GetError());
		return -1;
	}
}

/*
static char tochar(int value)
{
	return ' ' + value;//This is the part i don't understand
}*/



//SDL_Texture *loadTexture(char *path, int *width, int *height, bool ckEnable)
ImageData loadTexture(char *path, bool ckEnable, Uint8 r, Uint8 g, Uint8 b)
{
	ImageData img;


	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
	{
		printf("IMG_Load failed: %s\n", IMG_GetError());
	}
	else
	{
		// Set the color key (transparent pixel) in a surface.
		// https://wiki.libsdl.org/SDL_SetColorKey
		// The color key defines a pixel value that will be treated as transparent in a blit. 
		// It is a pixel of the format used by the surface, as generated by SDL_MapRGB().
		// Use SDL_MapRGB() to map an RGB triple to an opaque pixel value for a given pixel format.
		// https://wiki.libsdl.org/SDL_MapRGB
		SDL_SetColorKey(loadedSurface, ckEnable, SDL_MapRGB(loadedSurface->format, r, g, b));

		// Create texture from surface pixels
		img.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (img.texture == NULL)
		{
			printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
		}

		//Get image dimensions
		img.width = loadedSurface->w;
		img.height = loadedSurface->h;

		// Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//return newTexture;
	return img;
}


void imgRender(SDL_Renderer *renderer, ImageData img, int posX, int posY)
{
	SDL_Rect r;
	r.x = posX;
	r.y = posY;
	r.w = img.width;
	r.h = img.height;
	SDL_RenderCopy(renderer, img.texture, NULL, &r);
}

/*
Uint32 ballaction(Uint32 interval, void *param)
{

	if (bally >= 530)//飛到頂就停
	{
		ballFlag = false;
		colFlag = true;
		bally = 100;
		return 0;
	}
	else
	{

		bally = bally + 10;
		return interval;
	}



}*/

Uint32 colaction(Uint32 interval, void *param)
{

	if (colFlag == true && col_alpha < 255)
	{
		col_alpha = col_alpha + 65;
		//printf("fgjkfld");

	}
	if (col_alpha >= 255 || colFlag == false)
	{
		col_alpha = 0;
		colFlag = false;
	}
	return interval;
}

Uint32 manyballaction(Uint32 interval, void *param)
{

	if (manyball == true)
	{
		ball2_alpha = 220;
		//printf("fgjkfld");
		if (manyuser == 0)
			ball2x = ballx + 120;
		else
			ball2x = ballx - 120;
		ball2y = ball2y + 10;
	}
	if (manyball == false || ball2y + 96 >= 560)
	{
		ball2y = bally;
		ball2_alpha = 0;
		manyball = false;
	}
	return interval;




}
Uint32 holeaction(Uint32 interval, void *param)
{


	if (openhole == true)
	{
		//printf("%dhole", holealpha);
		holealpha = 255;
		//printf("fgjkfld");
		if (holeuser == 0)
			holex = netx + 73 + holerand;
		else
			holex = netx - 160 - holerand;
		holetime++;
		// = ball2y + 10;
	}
	if (openhole == false || holetime == 30)
	{
		holetime = 0;
		holealpha = 0;
		openhole = false;
	}
	return interval;




}
Uint32 netaction(Uint32 interval, void *param)
{


	if (movenet == true && netx <= WIDTH / 2 + 50 && netx >= WIDTH / 2 - 72)
	{
		if (tmpnetuser != netuser)//如果另一邊在竿子移動過程也碰到竿子移動道具
			movenet = false;
		if (netuser == 0)//左方碰到道具
			netx = netx - 0.05;
		else
			netx = netx + 0.05;
		tmpnetuser = netuser;
	}
	if (netx > WIDTH / 2 + 50 && netx < WIDTH / 2 - 72)
	{
		movenet = false;
		if (netx >= WIDTH / 2 + 50)
			netx = WIDTH / 2 + 50;
		else if (netx <= WIDTH / 2 - 72)
			netx = WIDTH / 2 - 72;

	}
	return interval;



}

Uint32 killball(Uint32 interval, void *param)
{


	//cloud[0].clx = (cloud[0].clx + 10) % WIDTH;
	if (kill_ball_flag == true)//open是案件被按下 flag按下時碰撞到皮卡丘成功觸發
	{
		ball_alpha = ball_alpha - 60;
		printf("殺球\n");

	}
	if (ball_alpha <= 50 || kill_ball_flag == false)
	{
		ball_alpha = 255;
		kill_ball_flag = false;
		kill_ball_open = false;
	}
	return interval;


}
Uint32 pika1alphaact(Uint32 interval, void *param)
{
	//if (alphauser == 0)
	//{
	if (pika1alpha == true)
	{
		pika1_alpha = 10;
		//printf("dfj");
		pika1alphatime++;

	}
	else
	{
		pika1_alpha = 255;
	}
	if (pika1alphatime >= 5)
	{
		pika1_alpha = 255;
		pika1alpha = false;
		pika1alphatime = 0;
	}
	/*}
	else if (alphauser == 1)
	{
		if (pikaalpha == true)
		{
			pika2_alpha = pika2_alpha - 80;
			//printf("dfj");

		}
		if (pika2_alpha <= 5)
		{
			pika2_alpha = 255;
			pikaalpha = false;
		}
	}*/
	return interval;


}

Uint32 pika1fasteraction(Uint32 interval, void *patimeram)
{
	//if (alphauser == 0)
	//{
	if (pika1fast == true && fast1time < 10)
	{
		pika1faster = 1.2;
		fast1time++;



	}
	if (fast1time == 10)
	{
		pika1faster = 1;
		fast1time = 0;
		pika1fast = false;
	}

	return interval;


}

Uint32 pika2fasteraction(Uint32 interval, void *patimeram)
{
	//if (alphauser == 0)
	//{
	if (pika2fast == true && fast2time < 10)
	{
		pika2faster = 1.2;
		fast2time++;



	}

	if (fast2time == 10)
	{
		pika2faster = 1;
		fast2time = 0;
		pika2fast = false;
	}

	return interval;


}
Uint32 pika2alphaact(Uint32 interval, void *param)
{
	if (pika2alpha == true)
	{
		pika2_alpha = 10;
		//printf("dfj");
		pika2alphatime++;

	}
	else
	{
		pika2_alpha = 255;
	}
	if (pika2alphatime >= 5)
	{
		pika2_alpha = 255;
		pika2alpha = false;
		pika2alphatime = 0;
	}
	//if (alphauser == 0)
	//{
	/*
	if (pika2alpha == true)
	{
		pika2_alpha = pika2_alpha - 80;
		//printf("dfj");

	}
	if (pika2_alpha <= 5)
	{
		pika2_alpha = 255;
		pika2alpha = false;
	}
	*/
	return interval;


}

Uint32 ballaction(Uint32 interval, void *param)
{
	//kill_ball_flag = false;
	/*
	if (viY == 530)//飛到頂就停
	{
		ballFlag = false;
		colFlag = true;
		viY = 100;
		return 0;
	}
	else
	{

		viY = viY + 10;
		return interval;
	}
	*/
	/*
	if (fisrttimecollision == 0)
		ballx = ballx-0;
	else
	{
		fisrttimecollision++;
		//velocity_x = 10;
		ballx = ballx - velocity_x;
	}*/
	/*
	if (kill_ball_open = false)
	{
		velocity_x = velocity_x + 3;
		velocity_y = velocity_y + 3;

	}*/
	//int killtmp = 0;
	if (fisrttimecollision != 0)
		ballx = ballx + velocity_x;//一開始掉下沒有這一行
	//velocity_x = 10;
	//if (pika1_jump_flag == true)
	bally = bally + 0.5* ballincloudtime + velocity_y - pika_jump_power;//加速度為gt
	float bally_a = 0.5* ballincloudtime + velocity_y - pika_jump_power;
	//else
		//bally = bally + 0.3* ballincloudtime + velocity_y;

	if (ballx <= 0)
	{
		velocity_x = -velocity_x;
		balltoosmalltime++;
	}
	else if (ballx + 96 >= WIDTH)
	{
		velocity_x = -velocity_x;
		balltoosmalltime++;
	}
	else if (bally <= -96)
	{
		velocity_y = -velocity_y;
		printf("%lf", bally);
		balltoosmalltime++;
	}
	else
	{	//if (bally <= 0)
			//velocity_y = -velocity_y;
		balltoosmalltime = 0;
		int up = 0, left = 0, right = 0, down = 0;
		float pika1_v_abs = pika1_V;
		if (pika1_v_abs < 0)
			pika1_v_abs = -pika1_V;
		float pika2_v_abs = pika2_V;
		if (pika2_v_abs < 0)
			pika2_v_abs = -pika2_V;

		if (pika1_jump_flag == true)
		{
			if (bally + 96 >= pika1_jump_y && ((ballx + 96 >= pika1_jump_X && ballx + 96 <= pika1_jump_X + 120) || (ballx <= pika1_jump_X + 120 && ballx >= pika1_jump_X)) && bally_a > 0)/*碰到皮卡丘上界*/
				up = 1;
			if (bally >= pika1_jump_y + 126 && ((ballx + 96 >= pika1_jump_X && ballx + 96 <= pika1_jump_X + 120) || (ballx <= pika1_jump_X + 120 && ballx >= pika1_jump_X)) && bally_a < 0)/*碰到皮卡丘上界*/
				down = 1;
			if ((bally + 96 >= pika1_jump_y && bally + 96 <= 580) && ((ballx + 96) >= pika1_jump_X && (ballx + 96) <= pika1_jump_X + velocity_x + pika1_v_abs) && velocity_x > 0)/*碰到皮卡丘左界 +-pika1_v_abs是考慮相對速度*/
				left = 1;
			if ((bally + 96 >= pika1_jump_y && bally + 96 <= 580) && (ballx <= pika1_jump_X + 120 && ballx >= pika1_jump_X + 120 + velocity_x - pika1_v_abs) && velocity_x < 0)/*碰到皮卡丘又界*/
				right = 1;
		}
		else
		{
			if (bally + 96 >= 460 && ((ballx + 96 >= pika1_X && ballx + 96 <= pika1_X + 120) || (ballx <= pika1_X + 120 && ballx >= pika1_X)) && bally_a > 0)/*碰到皮卡丘上界*/
				up = 1;
			if ((bally + 96 >= 460 && bally + 96 <= 580) && ((ballx + 96) >= pika1_X && (ballx + 96) <= pika1_X + 120) && velocity_x > 0)/*碰到皮卡丘左界*/
				left = 1;
			if ((bally + 96 >= 460 && bally + 96 <= 580) && ((ballx) <= pika1_X + 120 && ballx >= pika1_X) && velocity_x < 0)/*碰到皮卡丘又界*/
				right = 1;
		}
		if (pika2_jump_flag == true)
		{
			if (bally + 96 >= pika2_jump_y && ((ballx + 96 >= pika2_jump_X && ballx + 96 <= pika2_jump_X + 120) || (ballx <= pika2_jump_X + 120 && ballx >= pika2_jump_X)) && bally_a > 0)/*碰到皮卡丘上界*/
				up = 1;
			//(bally >= pika2_jump_y + 126 && bally >= pika2_jump_y + 126 - (0.5* ballincloudtime + velocity_y - pika_jump_power)
			if ((bally >= pika2_jump_y + 126 && bally >= pika2_jump_y + 126 - 20) && ((ballx + 96 >= pika2_jump_X && ballx + 96 <= pika2_jump_X + 120) || (ballx <= pika2_jump_X + 120 && ballx >= pika2_jump_X)) && bally_a < 0)/*碰到皮卡丘上界*/
				down = 1;
			/*
			if ((bally + 96 >= pika2_jump_y && bally + 96 <= 580) && ((ballx + 96) >= pika2_X && (ballx + 96) <= pika2_X + 120))//碰到皮卡丘左界
				left = 1;
			if ((bally + 96 >= pika2_jump_y && bally + 96 <= 580) && (ballx <= pika2_X + 120 && ballx >= pika2_X))//碰到皮卡丘又界
				right = 1;
				*/
			if ((bally + 96 >= pika2_jump_y && bally + 96 <= 580) && ((ballx + 96) >= pika2_jump_X && (ballx + 96) <= pika2_jump_X + velocity_x + pika2_v_abs) && velocity_x > 0)/*碰到皮卡丘左界*/
				left = 1;
			if ((bally + 96 >= pika2_jump_y && bally + 96 <= 580) && (ballx <= pika2_jump_X + 120 && ballx >= pika2_jump_X + 120 + velocity_x - pika2_v_abs) && velocity_x < 0)/*碰到皮卡丘又界*/
				right = 1;
		}
		else
		{

			if (bally + 96 >= 460 && ((ballx + 96 >= pika2_X && ballx + 96 <= pika2_X + 120) || (ballx <= pika2_X + 120 && ballx >= pika2_X)) && bally_a > 0)/*碰到皮卡丘上界*/
				up = 1;
			if ((bally + 96 >= 460 && bally + 96 <= 580) && ((ballx + 96) >= pika2_X && (ballx + 96) <= pika2_X + 120) && velocity_x > 0)/*碰到皮卡丘左界*/
				left = 1;
			if ((bally + 96 >= 460 && bally + 96 <= 580) && (ballx <= pika2_X + 120 && ballx >= pika2_X) && velocity_x < 0)/*碰到皮卡丘又界*/
				right = 1;

		}
		//球碰到net時
		bool net = false;

		if (bally + 96 >= 415 && ((ballx + 96 >= netx + 5 && ballx + 96 <= netx + 23 - 5) || (ballx <= netx + 23 - 5 && ballx >= netx + 5)))//碰到net上界
		{
			up = 1;
			net = true;
		}
		//int netline = netx + 23 / 2;

		if ((bally + 96 >= 415 && bally + 96 <= 580) && ((ballx + 96) >= netx && (ballx + 96) <= netx + velocity_x) && velocity_x > 0)//左界
		{
			left = 1;
			net = true;
		}
		if ((bally + 96 >= 415 && bally + 96 <= 580) && (ballx <= netx + 23 && ballx >= netx + 23 + velocity_x && velocity_x < 0))//又界
		{
			right = 1;
			net = true;
		}
		if (up == 1 || left == 1 || right == 1 || down == 1)//球碰到皮卡丘
		{

			if (kill_ball_flag == true)//一隻皮卡丘殺球後另一隻皮卡丘接到
			{
				kill_ball_flag = false;
				//kill_ball_open = false;
			}


			if (net == false)
			{
				if (fisrttimecollision != 0)
					ballx = ballx - velocity_x;//一開始掉下沒有這一行
				//velocity_x = 10;
				//if (pika1_jump_flag == true)
				bally = bally - 0.4* ballincloudtime - velocity_y + pika_jump_power;//加速度為gt
			}
			if (up == 1 || down == 1)
				velocity_y = -velocity_y;
			else
				velocity_x = -velocity_x;

			if (kill_ball_open == true && net == false && ((killer == 0 && ballx + 96 / 2 < netx + 23) || (ballx + 96 / 2 >= netx + 23 && killer == 1)))
			{

				kill_ball_flag = true;
				kill_ball_open = false;
				colFlag = true;
				colx = ballx;
				coly = bally;
				SDL_TimerID timerID_kill = SDL_AddTimer(80, killball, 0);
				printf("kill");
				//velocity_y = -velocity_y;
				//printf("1 vx %d,vy %d\n ", velocity_x, velocity_y);
				if (velocity_y < 0)//往上
					velocity_y = velocity_y - 10;
				else
					velocity_y = velocity_y + 10;

				//velocity_x = -velocity_x;
				if (velocity_x < 0)//往左
					velocity_x = velocity_x - 15;
				else
					velocity_x = velocity_x + 15;
				//printf("2 vx %d,vy %d\n ", velocity_x, velocity_y);

			}
			if (fisrttimecollision == 0)
				if (left == 1 || right == 1 || up == 1)
					fisrttimecollision++;
			ballincloudtime = 1;


		}
		else//球碰到皮卡丘 球在天上or地面
		{
			if (bally + 96 <= 580)
				ballincloudtime++;
			else
				ballincloudtime = 1;
		}
	}


	pika_jump_power = 0;
	if (bally + 96 >= 550 || balltoosmalltime > 10)//580
	{
		ballFlag = false;
		colFlag = true;
		colx = ballx;
		coly = bally;
		SDL_TimerID col_timer = SDL_AddTimer(50, colaction, 0);
		kill_ball_flag = false;
		kill_ball_open = false;
		if (ballx + 96 / 2 > netx + 23)
		{
			ballx = 100;
			pika1_grade1 = pika1_grade1 + 1;
			pika1_grade_time = pika1_grade_time + 1;
			teeoff_pika1 = true;
			if (pika1_grade_time >= 10)
			{
				pika1_grade1 = '1';

				pika1_grade2 = pika1_grade2 + 1;
				if (pika1_grade_time == 10)
				{
					pika1_grade2 = '0';

				}


			}
			velocity_x = 10;

		}
		else if (ballx + 96 / 2 < netx)
		{
			ballx = WIDTH - 196;
			pika2_grade1 = pika2_grade1 + 1;
			pika2_grade_time = pika2_grade_time + 1;
			teeoff_pika2 = true;
			if (pika2_grade_time >= 10)
			{
				pika2_grade1 = '1';

				pika2_grade2 = pika2_grade2 + 1;
				if (pika2_grade_time == 10)
				{
					pika2_grade2 = '0';
				}
			}
			velocity_x = -10;
		}
		if ((pika1_grade1 == '5' || pika2_grade1 == '5') && point_select == 1)
		{
			if (pika1_grade1 == '5')
				end_game = 1;
			else
				end_game = 2;
		}
		if ((pika1_grade2 == '0' || pika2_grade2 == '0') && point_select == 2)
		{
			if (pika1_grade2 == '0')
				end_game = 1;
			else
				end_game = 2;
		}
		if ((pika1_grade2 == '5' || pika2_grade2 == '5') && point_select == 3)
		{
			if (pika1_grade2 == '5')
				end_game = 1;
			else
				end_game = 2;
		}

		bally = -96;//回到發球點

		//velocity_x = 10;
		velocity_y = 15;
		ballincloudtime = 1;

		fisrttimecollision = 0;

		//落地得分

		return 0;
	}

	//復原殺球

	//printf("3 vx %d,vy %d\n ", velocity_x, velocity_y);

	if (kill_ball_flag == false)//==false?
	{
		//printf("vx %d,vy %d\n ", velocity_x, velocity_y);
		//	if (killtmp == 0)
			//{
		//printf("3 vx %d,vy %d\n ", velocity_x, velocity_y);
		if (velocity_x < 0)
			velocity_x = -10;
		else
			velocity_x = 10;
		if (velocity_y < 0)
			velocity_y = -15;
		else
			velocity_y = 15;
		//printf("4 vx %d,vy %d\n ", velocity_x, velocity_y);

		//}
		//killtmp = 1;
	}
	//kill_ball_open = false;
	return interval;






}

Uint32 cloudaction1(Uint32 interval, void *param)
{

	cloud[1].clx = (cloud[1].clx + 10) % WIDTH;
	return interval;

}
Uint32 cloudaction2(Uint32 interval, void *param)
{

	cloud[2].clx = (cloud[2].clx + 10) % WIDTH;
	return interval;


}
Uint32 cloudaction3(Uint32 interval, void *param)
{

	cloud[3].clx = (cloud[3].clx + 10) % WIDTH;
	return interval;


}
Uint32 cloudaction4(Uint32 interval, void *param)
{
	cloud[4].clx = (cloud[4].clx + 10) % WIDTH;
	return interval;
}
Uint32 cloudaction5(Uint32 interval, void *param)
{

	cloud[5].clx = (cloud[5].clx + 10) % WIDTH;
	return interval;

}
Uint32 cloudaction6(Uint32 interval, void *param)
{

	cloud[6].clx = (cloud[6].clx + 10) % WIDTH;
	return interval;


}
Uint32 cloudaction7(Uint32 interval, void *param)
{

	cloud[7].clx = (cloud[7].clx + 10) % WIDTH;
	return interval;


}
Uint32 cloudaction8(Uint32 interval, void *param)
{


	cloud[8].clx = (cloud[8].clx + 10) % WIDTH;
	return interval;

}
Uint32 cloudaction9(Uint32 interval, void *param)
{


	cloud[9].clx = (cloud[9].clx + 10) % WIDTH;
	return interval;


}
Uint32 cloudaction0(Uint32 interval, void *param)
{


	cloud[0].clx = (cloud[0].clx + 10) % WIDTH;
	return interval;


}

//pika action
Uint32 pika1_action(Uint32 interval, void *param)
{
	int *index = (int *)param;
	(*index) = ((*index) + 1) % pika1_FRAME;

	return interval;
}

Uint32 pika2_action(Uint32 interval, void *param)
{
	int *index = (int *)param;
	(*index) = ((*index) + 1) % pika2_FRAME;

	return interval;
}

//jump


bool top1 = false;

Uint32 pika1_jump(Uint32 interval, void *param)
{
	int *index = (int *)param;
	(*index) = ((*index) + 1) % pika2_jump_FRAME;
	float speed;
	if (speed_select == 1)
		speed = 0.75;
	else if (speed_select == 2)
		speed = 1;
	else if (speed_select == 3)
		speed = 1.25;
	if (pika1_jump_y <= 160 && top1 == false)
	{
		top1 = true;
		pika1_jump_y += (2 * speed*pika1faster);
		return interval;
	}
	else if (top1 == false)
	{
		pika1_jump_y -= (2 * speed*pika1faster);
		return interval;
	}

	if (top1 == true && pika1_jump_y >= 460)
	{
		pika1_jump_flag = false;
		top1 = false;
		return 0;
	}
	else if (top1 == true)
	{
		pika1_jump_y += (2 * speed*pika1faster);
		return interval;
	}
}


bool top2 = false;

Uint32 pika2_jump(Uint32 interval, void *param)
{
	int *index = (int *)param;
	(*index) = ((*index) + 1) % pika2_jump_FRAME;
	float speed;
	if (speed_select == 1)
		speed = 0.75;
	else if (speed_select == 2)
		speed = 1;
	else if (speed_select == 3)
		speed = 1.25;
	if (pika2_jump_y <= 160 && top2 == false)
	{
		top2 = true;
		pika2_jump_y += (2 * speed*pika2faster);
		return interval;
	}
	else if (top2 == false)
	{
		pika2_jump_y -= (2 * speed*pika2faster);
		return interval;
	}

	if (top2 == true && pika2_jump_y >= 460)
	{
		pika2_jump_flag = false;
		top2 = false;
		return 0;
	}
	else if (top2 == true)
	{
		pika2_jump_y += (2 * speed*pika2faster);
		return interval;
	}
}

Uint32 prop_action(Uint32 interval, void *param)
{
	int a = rand() % 10;
	if (a > 4)//一半概率出現
	{
		prop_exist = true;
		propalpha = 255;
		prop_x = rand() % 1220;
		printf("a: %d  道具出現\n", a);
		//SDL_TimerID timerID_propexist = SDL_AddTimer(3000, propexist,0);
	}
	else
	{
		printf("a: %d  道具不出現\n", a);
		//propalpha = 0;
		prop_exist = false;
	}

	if (prop_exist == false)
	{
		propalpha = 0;
		prop_exist = false;
	}
	return interval;
}
Uint32 propflagset(Uint32 interval, void *param)
{
	if (propflag == true)
	{
		srand(time(NULL));
		int prop = rand() % 5;
		printf("\n %d propflagopen\n", prop);
		if (prop == 0)
		{
			movenet = true;
			netuser = propuser;//先隨機某一邊碰到道具
			tmpnetuser = netuser;
			//if (movenet == true)
			SDL_TimerID timerID_net = SDL_AddTimer(5, netaction, 0);
		}
		else if (prop == 1)
		{
			pikaalpha = true;

			alphauser = propuser;//
		   //printf("fmkdsmf;s\n");
			if (alphauser == 1)
			{
				pika1alpha = true;
				//printf("1");
			}
			else
			{
				pika2alpha = true;
				//printf("2");
			}
			if (pika1alpha == true)
				SDL_TimerID timerID_pika1aplha = SDL_AddTimer(400, pika1alphaact, 0);
			if (pika2alpha == true)
				SDL_TimerID timerID_pika1aplha = SDL_AddTimer(400, pika2alphaact, 0);

		}
		else if (prop == 2)
		{
			pikafast = true;
			fastuser = propuser;
			if (fastuser == 0)
				pika1fast = true;
			else if (fastuser == 1)
				pika2fast = true;



			if (fastuser == 0)
			{
				SDL_TimerID timerID_pika1fast = SDL_AddTimer(50, pika1fasteraction, 0);
				//pika1faster = 2;
			}
			else if (fastuser == 1)
				//pika2faster = 1.3;
				SDL_TimerID timerID_pika2fast = SDL_AddTimer(50, pika2fasteraction, 0);
		}
		else if (prop == 3)
		{
			manyball = true;
			manyuser = rand() % 2;
			ball2x = ballx;
			ball2y = bally;
			SDL_TimerID timerID_manyball = SDL_AddTimer(50, manyballaction, 0);
		}
		else if (prop == 4)
		{
			openhole = true;
			holeuser = propuser;
			holerand = rand() % (WIDTH / 2 - 500);
			SDL_TimerID timerID_hole = SDL_AddTimer(150, holeaction, 0);
		}
		propflag = false;
		printf("%d propflagclose\n", prop);


	}
	//else
		//printf("false");
	/*
	holeuser = propuser;
	holerand = rand() % (WIDTH / 2 - 500);
	if (openhole == true)
		SDL_TimerID timerID_hole = SDL_AddTimer(150, holeaction, 0);

	netuser = propuser;//先隨機某一邊碰到道具
	tmpnetuser = netuser;
	if (movenet == true)
		SDL_TimerID timerID_net = SDL_AddTimer(5, netaction, 0);
	if (pikafast == true)
	{
		fastuser = propuser;
		if (fastuser == 0)
			pika1fast = true;
		else if (fastuser == 1)
			pika2fast = true;



		if (fastuser == 0)
		{
			SDL_TimerID timerID_pika1fast = SDL_AddTimer(50, pika1fasteraction, 0);
			//pika1faster = 2;
		}
		else if (fastuser == 1)
			//pika2faster = 1.3;
			SDL_TimerID timerID_pika2fast = SDL_AddTimer(50, pika2fasteraction, 0);

	}



	if (manyball == true)
	{
		manyuser = rand() % 2;
		ball2x = ballx;
		ball2y = bally;
		SDL_TimerID timerID_manyball = SDL_AddTimer(50, manyballaction, 0);
	}
	if (pikaalpha == true)
	{
		alphauser = propuser;//
	   //printf("fmkdsmf;s\n");
		if (alphauser == 1)
		{
			pika1alpha = true;
			//printf("1");
		}
		else
		{
			pika2alpha = true;
			//printf("2");
		}
	}
	if (pika1alpha == true)
		SDL_TimerID timerID_pika1aplha = SDL_AddTimer(150, pika1alphaact, 0);
	if (pika2alpha == true)
		SDL_TimerID timerID_pika1aplha = SDL_AddTimer(150, pika2alphaact, 0);

	*/
	return interval;
}

void handleEvent(SDL_Event& e)
{
	float speed;
	if (speed_select == 1)
		speed = 0.75;
	else if (speed_select == 2)
		speed = 1;
	else if (speed_select == 3)
		speed = 1.25;
	/* The method for "Debunce" */

	// If a key was pressed
	// repeat: non-zero if this is a key repeat
	// https://wiki.libsdl.org/SDL_KeyboardEvent
	//if (e.type == SDL_KEYDOWN)
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			//case SDLK_UP: mVelY -= VELOCITY; break;
			//case SDLK_DOWN: mVelY += VELOCITY; break;
		case SDLK_a:
			if (teeoff_pika1 == true)
			{
				if (ballFlag == false)
				{
					ballFlag = true;
					if (speed_select == 1)
						SDL_TimerID timerID_ball = SDL_AddTimer(25, ballaction, 0);
					else if (speed_select == 2)
						SDL_TimerID timerID_ball = SDL_AddTimer(20, ballaction, 0);
					else if (speed_select == 3)
						SDL_TimerID timerID_ball = SDL_AddTimer(15, ballaction, 0);
					//virusX = kuX + 30;
					//velocity_x = 0;
					fisrttimecollision = 0;

				}
				else
				{
					ballFlag = false;
					//SDL_TimerID timerID_ball = SDL_AddTimer(50, ballaction, 0);
					//virusX = kuX + 30;
				}
				teeoff_pika1 = false;
			}
			break;
			//左邊皮卡丘
		case SDLK_l:
			if (teeoff_pika2 == true)
			{
				if (ballFlag == false)
				{
					ballFlag = true;
					if (speed_select == 1)
						SDL_TimerID timerID_ball = SDL_AddTimer(25, ballaction, 0);
					else if (speed_select == 2)
						SDL_TimerID timerID_ball = SDL_AddTimer(20, ballaction, 0);
					else if (speed_select == 3)
						SDL_TimerID timerID_ball = SDL_AddTimer(15, ballaction, 0);
					//virusX = kuX + 30;
					//velocity_x = 0;
					fisrttimecollision = 0;

				}
				else
				{
					ballFlag = false;
					//SDL_TimerID timerID_ball = SDL_AddTimer(50, ballaction, 0);
					//virusX = kuX + 30;
				}
				teeoff_pika2 = false;
			}
			break;
			//左邊皮卡丘
		case SDLK_z:
			if (ballx + 96 / 2 < netx + 23)
			{
				printf("殺球\n");

				kill_ball_open = true;
				killer = 0;
			}
			break;
		case SDLK_d: pika1_V -= (VELOCITY*speed*pika1faster); break;
		case SDLK_g: pika1_V += (VELOCITY*speed*pika1faster); break;
		case SDLK_r:
			if (pika1_jump_flag == false)
			{
				pika1_jump_flag = true;
				pika1_jump_X = pika1_X;
				pika_jump_power = 30;
				SDL_TimerID timerID_pika1_jump = SDL_AddTimer(5, pika1_jump, &pika1_jump_index);
			}
			break;
			//case SDLK_UP: mVelY -= VELOCITY; break;
			//case SDLK_DOWN: mVelY += VELOCITY; break;
			//右邊皮卡丘
		case SDLK_LEFT: pika2_V -= (VELOCITY*speed*pika2faster); break;
		case SDLK_RIGHT: pika2_V += (VELOCITY*speed*pika2faster); break;
		case SDLK_UP:
			if (pika2_jump_flag == false)
			{
				pika2_jump_flag = true;
				pika2_jump_X = pika2_X;
				SDL_TimerID timerID_pika2_jump = SDL_AddTimer(5, pika2_jump, &pika2_jump_index);
			}
			break;
		case SDLK_p:
			if (ballx + 96 / 2 >= netx + 23)
			{
				kill_ball_open = true;
				killer = 1;
				printf("殺球\n");
			}//printf("enter");
			break;
		}
	}
	//If a key was released
	//else if (e.type == SDL_KEYUP)
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			//左邊皮卡丘
		case SDLK_d: pika1_V += (VELOCITY*speed*pika1faster); break;
		case SDLK_g: pika1_V -= (VELOCITY*speed*pika1faster); break;
			//右邊皮卡丘
		case SDLK_LEFT: pika2_V += (VELOCITY*speed*pika2faster); break;
		case SDLK_RIGHT: pika2_V -= (VELOCITY*speed*pika2faster); break;
		}
	}
}



bool loadAudio()
{
	// Load music
	// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_55.html
	// This can load WAVE, MOD, MIDI, OGG, MP3, FLAC, and any file that you use a command to play with.
	music1 = Mix_LoadMUS("../audio/mainmusic.mp3");
	if (music1 == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}



	// Load sound effects
	// https://www.libsdl.org/projects/SDL_mixer/docs/SDL_mixer_19.html
	// This can load WAVE, AIFF, RIFF, OGG, and VOC files.
	effect1 = Mix_LoadWAV("../audio/e01.wav");
	if (effect1 == NULL)
	{
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}


}

void move()
{
	//Move the dot left or right
	pika1_X += pika1_V;

	//If the dot went too far to the left or right
	//if ((pika1_X - M_WIDTH <= -50) || (pika1_X + M_WIDTH >= WIDTH - 200))
	if ((pika1_X <= 0) || (pika1_X + 120 >= netx))
	{
		//Move back
		pika1_X -= pika1_V;
		if (pika1_X + 120 >= netx && movenet == true)
			pika1_X -= pika1_V;

	}

	pika1_jump_X += pika1_V;
	if ((pika1_jump_X <= 0) || (pika1_jump_X + 120 >= netx))
	{
		//Move back
		pika1_jump_X -= pika1_V;
		if (pika1_jump_X + 120 >= netx && movenet == true)
			pika1_X -= pika1_V;
	}

	//Move the dot left or right
	pika2_X += pika2_V;

	//If the dot went too far to the left or right
	if ((pika2_X - M_WIDTH <= netx) || (pika2_X + 120 >= WIDTH))
	{
		//Move back
		pika2_X -= pika2_V;
		if (pika2_X - M_WIDTH <= netx && movenet == true)
			pika2_X -= pika2_V;
	}

	pika2_jump_X += pika2_V;
	//if ((pika2_jump_X - M_WIDTH <= -50) || (pika2_jump_X + M_WIDTH >= WIDTH))
	if ((pika2_jump_X - M_WIDTH <= netx) || (pika2_jump_X + 120 >= WIDTH))
	{
		//Move back
		pika2_jump_X -= pika2_V;
		if (pika2_jump_X - M_WIDTH <= netx && movenet == true)
			pika2_X -= pika2_V;
	}
	if (openhole == true && holealpha == 255)
	{
		if (pika1_X >= holex && pika1_X + 96 <= holex + 160 && pika1_jump_flag == false)
			pika1_X = 0;
		if (pika2_X >= holex && pika2_X + 96 <= holex + 160 && pika2_jump_flag == false)
			pika2_X = WIDTH - 120;
	}

	if (prop_exist == true)
	{
		if (((prop_x + 60) >= pika1_X && (prop_x + 60) <= pika1_X + 120) && pika1_jump_flag == false)//皮卡丘左界
		{
			prop_exist = false;//道具出不出現的開關
			propuser = 0;//左邊觸發
			propflag = true;//道具效果觸發的開關
			printf("jfrij");
		}
		if ((prop_x <= pika1_X + 120 && prop_x >= pika1_X) && pika1_jump_flag == false)//皮卡丘右界
		{
			prop_exist = false;
			propuser = 0;
			propflag = true;//道具效果觸發的開關 
			printf("jfrij");
		}
		if (((prop_x + 60) >= pika2_X && (prop_x + 60) <= pika2_X + 120) && pika2_jump_flag == false)//碰到皮卡丘左界
		{
			prop_exist = false;
			propuser = 1;
			propflag = true;//道具效果觸發的開關 
			printf("jfrij");
		}
		if ((prop_x <= pika2_X + 120 && prop_x >= pika2_X) && pika2_jump_flag == false)//碰到皮卡丘左界
		{
			prop_exist = false;
			propuser = 1;
			propflag = true;//道具效果觸發的開關 
			printf("jfrij");
		}
	}
	if (prop_exist == false)
	{
		propalpha = 0;
		prop_exist = false;
	}

}


//把選項圈起來
void draw_select(SDL_Renderer *renderer)
{
	switch (point_select)
	{
	case 1:
		circleRGBA(renderer, 1034.5, 569, 25, 255, 255, 255, 255);
		break;
	case 2:
		circleRGBA(renderer, 1129, 569, 25, 255, 255, 255, 255);
		break;
	case 3:
		circleRGBA(renderer, 1230, 569, 25, 255, 255, 255, 255);
		break;
	}
	switch (speed_select)
	{
	case 1:
		circleRGBA(renderer, 1031, 617.5, 25, 255, 255, 255, 255);
		break;
	case 2:
		circleRGBA(renderer, 1132, 617.5, 25, 255, 255, 255, 255);
		break;
	case 3:
		circleRGBA(renderer, 1239, 617.5, 25, 255, 255, 255, 255);
		break;
	}
}

// When using SDL, you have to use "int main(int argc, char* args[])"
// int main() and void main() are not allowed
int main(int argc, char* args[])
{
	// The working directory is where the vcxproj file is located.
	char imgPath1[100] = "../images/net.png";
	char imgPath2[100] = "../images/backgrounds.png";
	char colPath[100] = "../images/collision.png";
	char cloudPath[100] = "../images/cloud.png";
	char ballPath[100] = "../images/ball.png";
	char holePath[100] = "../images/hole.png";

	char pika1_Path[100] = "../images/pika1/";
	char pika1_jump_Path[100] = "../images/pika1_jump/";
	int pika1_Index = 0;
	char pika2_Path[100] = "../images/pika2/";
	char pika2_jump_Path[100] = "../images/pika2_jump/";

	int pika2_Index = 0;

	char dragon1_Path[100] = "../images/dragon1/";
	char dragon2_Path[100] = "../images/dragon2/";
	char bachi1_Path[100] = "../images/871/";
	char bachi1_jump_Path[100] = "../images/871_jump/";
	char bachi2_Path[100] = "../images/872/";
	char bachi2_jump_Path[100] = "../images/872_jump/";

	//UI圖
	char imgPathUI[100] = "../images/01.png";
	char dragon_path[100] = "../images/dragon.png";
	char pika_path[100] = "../images/pika.png";
	char bachi_path[100] = "../images/bachi.png";
	char propPath[100] = "../images/prop.png";
	char diePath[100] = "../images/rip.png";
	ImageData net, bg, ball, col, ball2, hole, prop, die;
	ImageData pika1[pika1_FRAME], pika1_jump[pika1_jump_FRAME], pika2[pika2_FRAME], pika2_jump[pika2_jump_FRAME];

	SDL_RendererFlip no = SDL_FLIP_NONE;
	SDL_RendererFlip ho = SDL_FLIP_HORIZONTAL;
	SDL_RendererFlip ve = SDL_FLIP_VERTICAL;
	SDL_RendererFlip hove = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);

	// Start up SDL and create window
	if (initSDL())
	{
		printf("Failed to initialize SDL!\n");
		return -1;
	}
	loadAudio();
	//const char* grade1 =(char*)pika1_grade;
	//const char* grade2 = (char*)pika2_grade;



	net = loadTexture(imgPath1, true, 0xFF, 0xFF, 0xFF);
	bg = loadTexture(imgPath2, false, 0xFF, 0xFF, 0xFF);
	ball = loadTexture(ballPath, false, 0xFF, 0xFF, 0xFF);
	ball2 = loadTexture(ballPath, false, 0xFF, 0xFF, 0xFF);
	col = loadTexture(colPath, false, 0xFF, 0xFF, 0xFF);
	hole = loadTexture(holePath, false, 0xFF, 0xFF, 0xFF);
	prop = loadTexture(propPath, true, 0xFF, 0xFF, 0xFF);
	die = loadTexture(diePath, true, 0xFF, 0xFF, 0xFF);

	Mix_PlayMusic(music1, -1);
	for (int i = 0; i < 10; i++)
		cloud[i].cl = loadTexture(cloudPath, false, 0xFF, 0xFF, 0xFF);

	for (int i = 0; i < 3; i++)
	{
		cloud[i].clx = i * 100;
		cloud[i].cly = 50 + (i % 3) * 70; //50 + (i / 3) * 70
	}
	for (int i = 0; i < 3; i++)
	{
		cloud[i + 3].clx = (i % 3) * 500;
		cloud[i + 3].cly = 50;
	}
	for (int i = 0; i < 3; i++)
	{
		cloud[i + 6].clx = 250 + (i % 3) * 300;
		cloud[i + 6].cly = 100 + (i % 2) * 70;
	}
	cloud[9].clx = 800;
	cloud[9].cly = 230;
	/*
	//讀皮卡丘圖片
	for (int i = 0; i < pika1_FRAME; i++)
	{
		char str[100];
		sprintf_s(str, 100, "%s%02d.png", pika1_Path, i + 1);
		pika1[i] = loadTexture(str, true, 71, 113, 77);
	}
	for (int i = 0; i < pika1_jump_FRAME; i++)
	{
		char str[100];
		sprintf_s(str, 100, "%s%02d.png", pika1_jump_Path, i + 1);
		pika1_jump[i] = loadTexture(str, true, 71, 113, 77);
	}
	for (int i = 0; i < pika2_FRAME; i++)
	{
		char str[100];
		sprintf_s(str, 100, "%s%02d.png", pika2_Path, i + 1);
		pika2[i] = loadTexture(str, true, 71, 113, 77);
	}
	for (int i = 0; i < pika2_jump_FRAME; i++)
	{
		char str[100];
		sprintf_s(str, 100, "%s%02d.png", pika2_jump_Path, i + 1);
		pika2_jump[i] = loadTexture(str, true, 71, 113, 77);
	}*/

	//timer
	SDL_TimerID timerID_cloud3 = SDL_AddTimer(300, cloudaction3, 0);
	SDL_TimerID timerID_cloud5 = SDL_AddTimer(300, cloudaction5, pika1_action);
	SDL_TimerID timerID_cloud9 = SDL_AddTimer(300, cloudaction9, 0);
	SDL_TimerID timerID_cloud1 = SDL_AddTimer(300, cloudaction1, 0);
	SDL_TimerID timerID_cloud2 = SDL_AddTimer(300, cloudaction2, 0);
	SDL_TimerID timerID_cloud4 = SDL_AddTimer(300, cloudaction4, 0);
	SDL_TimerID timerID_cloud6 = SDL_AddTimer(300, cloudaction6, 0);
	SDL_TimerID timerID_cloud7 = SDL_AddTimer(300, cloudaction7, 0);
	SDL_TimerID timerID_cloud8 = SDL_AddTimer(300, cloudaction8, 0);
	SDL_TimerID timerID_cloud0 = SDL_AddTimer(300, cloudaction0, 0);

	SDL_TimerID timerID_pika1 = SDL_AddTimer(200, pika1_action, &pika1_Index);
	SDL_TimerID timerID_pika2 = SDL_AddTimer(200, pika2_action, &pika2_Index);
	SDL_TimerID timerID_prop = SDL_AddTimer(3000, prop_action, 0);
	SDL_TimerID timerID_propset = SDL_AddTimer(30, propflagset, 0);
	/*
	if (propflag == true)
	{
		int prop = rand() % 5;
		printf("\n %d propflagopen\n", prop);
		if (prop == 0)
			movenet = true;
		else if (prop == 1)
		{
			pikaalpha = true;
			//pika1alpha = false;
			//pika2alpha = false;
		}
		else if (prop == 2)
			pikafast = true;
		else if (prop == 3)
			manyball = true;
		else if (prop == 4)
		{
			pikafast = true;
		}
		else if (prop == 5)
			openhole = false;
		propflag = false;
		printf("%d propflagclose\n", prop);


	}
	else
		printf("false");*/
		/*
		holeuser = propuser;
		holerand = rand() % (WIDTH / 2 - 500);
		if (openhole == true)
			SDL_TimerID timerID_hole = SDL_AddTimer(150, holeaction, 0);

		netuser = propuser;//先隨機某一邊碰到道具
		tmpnetuser = netuser;
		if (movenet == true)
			SDL_TimerID timerID_net = SDL_AddTimer(5, netaction, 0);
		if (pikafast == true)
		{
			fastuser = propuser;
			if (fastuser == 0)
				pika1fast = true;
			else if (fastuser == 1)
				pika2fast = true;



			if (fastuser == 0)
			{
				SDL_TimerID timerID_pika1fast = SDL_AddTimer(50, pika1fasteraction, 0);
				//pika1faster = 2;
			}
			else if (fastuser == 1)
				//pika2faster = 1.3;
				SDL_TimerID timerID_pika2fast = SDL_AddTimer(50, pika2fasteraction, 0);

		}



		if (manyball == true)
		{
			manyuser = rand() % 2;
			ball2x = ballx;
			ball2y = bally;
			SDL_TimerID timerID_manyball = SDL_AddTimer(50, manyballaction, 0);
		}
		if (pikaalpha == true)
		{
			alphauser = propuser;//
		   //printf("fmkdsmf;s\n");
			if (alphauser == 1)
			{
				pika1alpha = true;
				//printf("1");
			}
			else
			{
				pika2alpha = true;
				//printf("2");
			}
		}
			if (pika1alpha == true)
				SDL_TimerID timerID_pika1aplha = SDL_AddTimer(150, pika1alphaact, 0);
			if (pika2alpha == true)
				SDL_TimerID timerID_pika1aplha = SDL_AddTimer(150, pika2alphaact, 0);

		*/
		//UI圖+字
	ImageData UI, dragon_select, pika_select, bachi_select;
	TextData point = loadTextTexture("Point :", "../fonts/lazy.ttf", 40, 250, 20, 240, SHADED, 17, 253, 40);
	TextData point_select_text = loadTextTexture("5   10   15", "../fonts/lazy.ttf", 40, 0, 0, 0, SOLID, 17, 253, 40);
	TextData speed = loadTextTexture("Speed :", "../fonts/lazy.ttf", 40, 250, 20, 240, SHADED, 17, 253, 40);
	TextData speed_select_text = loadTextTexture("Slow  Middle  Fast", "../fonts/lazy.ttf", 30, 0, 0, 0, SOLID, 17, 253, 40);
	TextData start_game = loadTextTexture("Start Game", "../fonts/lazy.ttf", 40, 250, 20, 240, SHADED, 17, 253, 40);
	UI = loadTexture(imgPathUI, false, 0xFF, 0xFF, 0xFF);
	dragon_select = loadTexture(dragon_path, false, 0xFF, 0xFF, 0xFF);
	pika_select = loadTexture(pika_path, false, 0xFF, 0xFF, 0xFF);
	bachi_select = loadTexture(bachi_path, false, 0xFF, 0xFF, 0xFF);

	bool quit = false;
	bool start = true;
	const int max_character = 3;
	int select_character_left = 0, select_character_right = 0;

	//Event handler
	SDL_Event e;
	MouseState mouseState;
	int mouseX, mouseY;
	//While application is running
	while (!quit)
	{
		if (start == true)
		{
			mouseState = NONE;
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				mouseHandleEvent(&e, &mouseState, &mouseX, &mouseY);
			}
			switch (mouseState)
			{
			case IN_LB_SC:  // Inside, Left Button, Single Click
				printf("Inside, Left Button, Single Click: (%d, %d)\n", mouseX, mouseY);

				if (mouseX > 1012 && mouseX < 1057 && mouseY>554 && mouseY < 584)
					point_select = 1;//5分
				else if (mouseX > 1104 && mouseX < 1154 && mouseY>554 && mouseY < 584)
					point_select = 2;//10分
				else if (mouseX > 1208 && mouseX < 1252 && mouseY>554 && mouseY < 584)
					point_select = 3;//15分
				if (mouseX > 996 && mouseX < 1066 && mouseY>600 && mouseY < 635)
					speed_select = 1;//slow
				else if (mouseX > 1083 && mouseX < 1181 && mouseY>600 && mouseY < 635)
					speed_select = 2;//middle
				else if (mouseX > 1212 && mouseX < 1266 && mouseY>600 && mouseY < 635)
					speed_select = 3;//fast
				//選腳色
				if (mouseX > 0 && mouseX < 150 && mouseY>0 && mouseY < 150)
					select_character_left = (select_character_left + 1) % max_character;
				if (mouseX > 1130 && mouseX < 1280 && mouseY>0 && mouseY < 150)
					select_character_right = (select_character_right + 1) % max_character;

				if (mouseX > 1050 && mouseX < 1250 && mouseY>671 && mouseY < 705)//start game 轉場動畫?
				{
					const int POINTS_COUNT = 11, x = 700, y = 400;
					for (float i = 14; i >= 0; i -= 0.15)
					{
						SDL_Point points[POINTS_COUNT] = { {x, y - 37 * i}, {x + 10 * i, y - 10 * i}, {x + 35 * i, y - 10 * i},
														{x + 14 * i, y + 6 * i}, {x + 22 * i, y + 31 * i}, {x, y + 16 * i},
														{x - 23 * i, y + 31 * i}, {x - 14 * i, y + 6 * i}, {x - 36 * i, y - 10 * i},
														{x - 10 * i, y - 10 * i}, {x, y - 37 * i} };
						SDL_SetRenderDrawColor(renderer, rand() % 255 + 1, rand() % 255 + 1, rand() % 255 + 1, 0xFF);
						SDL_RenderDrawLines(renderer, points, POINTS_COUNT);
						SDL_Delay(15);
						SDL_RenderPresent(renderer);
					}
					//讀皮卡丘圖片
					for (int i = 0; i < pika1_FRAME; i++)
					{
						char str[100];
						if (select_character_left == 0)
						{
							sprintf_s(str, 100, "%s%02d.png", pika1_Path, i + 1);
							pika1[i] = loadTexture(str, true, 71, 113, 77);
						}
						if (select_character_left == 1)
						{
							sprintf_s(str, 100, "%s%02d.png", dragon1_Path, i + 1);
							pika1[i] = loadTexture(str, true, 71, 113, 77);
						}
						if (select_character_left == 2)
						{
							sprintf_s(str, 100, "%s%02d.png", bachi1_Path, i + 1);
							pika1[i] = loadTexture(str, true, 255, 255, 255);
						}

					}
					for (int i = 0; i < pika1_jump_FRAME; i++)
					{
						char str[100];
						if (select_character_left == 0)
						{
							sprintf_s(str, 100, "%s%02d.png", pika1_jump_Path, i + 1);
							pika1_jump[i] = loadTexture(str, true, 71, 113, 77);
						}

						if (select_character_left == 1)
						{
							sprintf_s(str, 100, "%s%02d.png", dragon1_Path, i + 1);
							pika1_jump[i] = loadTexture(str, true, 71, 113, 77);
						}

						if (select_character_left == 2)
						{
							sprintf_s(str, 100, "%s%02d.png", bachi1_jump_Path, i + 1);
							pika1_jump[i] = loadTexture(str, true, 255, 255, 255);

						}
					}
					for (int i = 0; i < pika2_FRAME; i++)
					{
						char str[100];
						if (select_character_right == 0)
						{
							sprintf_s(str, 100, "%s%02d.png", pika2_Path, i + 1);
							pika2[i] = loadTexture(str, true, 71, 113, 77);
						}

						if (select_character_right == 1)
						{
							sprintf_s(str, 100, "%s%02d.png", dragon2_Path, i + 1);
							pika2[i] = loadTexture(str, true, 71, 113, 77);
						}
						if (select_character_right == 2)
						{
							sprintf_s(str, 100, "%s%02d.png", bachi2_Path, i + 1);
							pika2[i] = loadTexture(str, true, 255, 255, 255);
						}
					}
					for (int i = 0; i < pika2_jump_FRAME; i++)
					{
						char str[100];
						if (select_character_right == 0)
						{
							sprintf_s(str, 100, "%s%02d.png", pika2_jump_Path, i + 1);
							pika2_jump[i] = loadTexture(str, true, 71, 113, 77);
						}
						if (select_character_right == 1)
						{
							sprintf_s(str, 100, "%s%02d.png", dragon2_Path, i + 1);
							pika2_jump[i] = loadTexture(str, true, 71, 113, 77);
						}
						if (select_character_right == 2)
						{
							sprintf_s(str, 100, "%s%02d.png", bachi2_jump_Path, i + 1);
							pika2_jump[i] = loadTexture(str, true, 255, 255, 255);
						}
					}
					start = false;
				}
				break;
			}

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(renderer);

			imgRender(renderer, UI, 0, 0);
			if (select_character_left == 0)
				imgRender(renderer, pika_select, 0, 0);
			else if (select_character_left == 1)
				imgRender(renderer, dragon_select, 0, 0);
			else if (select_character_left == 2)
				imgRender(renderer, bachi_select, 0, 0);
			if (select_character_right == 0)
				imgRender(renderer, pika_select, 1130, 0);
			else if (select_character_right == 1)
				imgRender(renderer, dragon_select, 1130, 0);
			else if (select_character_right == 2)
				imgRender(renderer, bachi_select, 1130, 0);

			textRender(renderer, point, 850, 550, 0, 0, 0, no);
			textRender(renderer, point_select_text, 1025, 550, 0, 0, 0, no);
			textRender(renderer, speed, 850, 600, 0, 0, 0, no);
			textRender(renderer, speed_select_text, 1000, 605, 0, 0, 0, no);
			textRender(renderer, start_game, 1050, 670, 0, 0, 0, no);
			draw_select(renderer);
			SDL_RenderPresent(renderer);
		}


		//Handle events on queue
		else
		{
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				handleEvent(e);
			}
			move();
			// Clear screen
			/*
			if (propflag == true)
			{
				int prop = rand() % 5;
				printf("%d propflagopen", prop);
				if (prop == 0)
					movenet = true;
				else if (prop == 1)
				{
					pikaalpha = true;
					//pika1alpha = false;
					//pika2alpha = false;
				}
				else if (prop == 2)
					pikafast = false;
				else if (prop == 3)
					manyball = false;
				else if (prop == 4)
				{
					pikafast = true;
				}
				else if (prop == 5)
					openhole = false;
				propflag = false;

			}
			*/

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
			SDL_RenderClear(renderer);

			imgRender(renderer, bg, 0, 0);
			imgRender(renderer, net, netx, 415);

			for (int i = 0; i < 10; i++)
				imgRender(renderer, cloud[i].cl, cloud[i].clx, cloud[i].cly); //50 + (i / 3) * 70
			//if(kill_ball_flag==true)
				//SDL_TimerID timerID_kill = SDL_AddTimer(150, killball, 0);

			if (ballFlag == true)
				imgRender(renderer, ball, ballx, bally);
			//if (manyball == true)
			//{
			imgRender(renderer, ball2, ball2x, ball2y);
			setTextureAlpha(ball2.texture, ball2_alpha);
			//printf("ball2 %d\n", ball2_alpha);
		//}
			imgRender(renderer, prop, prop_x, 530);
			setTextureAlpha(prop.texture, propalpha);
			/*if (kill_ball_open)
			imgRender(renderer, ball, ballx, bally);*/
			setTextureAlpha(ball.texture, ball_alpha);
			//imgRender(renderer, ball, 100, viY);

			//if (colFlag == true)
			//{
				//SDL_TimerID col_timer = SDL_AddTimer(300, colaction, 0);
				//colopen = 5;
			imgRender(renderer, col, colx, coly);
			setTextureAlpha(col.texture, col_alpha);

			imgRender(renderer, hole, holex, 580);

			setTextureAlpha(hole.texture, holealpha);
			//
		//	//colFlag = false;


		/*
			imgRender(renderer, col, ballx, 530);
			setTextureAlpha(col.texture, col_alpha);
		*/
		// Update screen
		//左邊皮卡丘
			if (pika1_jump_flag == false)
				imgRender(renderer, pika1[pika1_Index], pika1_X, 460);
			if (pika1_jump_flag == true)
				imgRender(renderer, pika1_jump[pika1_jump_index], pika1_jump_X, pika1_jump_y);
			//右邊皮卡丘
			if (pika2_jump_flag == false)
				imgRender(renderer, pika2[pika2_Index], pika2_X, 460);
			if (pika2_jump_flag == true)
				imgRender(renderer, pika2_jump[pika2_jump_index], pika2_jump_X, pika2_jump_y);
			for (int i = 0; i < pika1_FRAME; i++)
			{
				setTextureAlpha(pika1[i].texture, pika1_alpha);
			}
			for (int i = 0; i < pika1_jump_FRAME; i++)
			{
				setTextureAlpha(pika1_jump[i].texture, pika1_alpha);
			}
			for (int i = 0; i < pika2_FRAME; i++)
			{
				setTextureAlpha(pika2[i].texture, pika2_alpha);
			}
			for (int i = 0; i < pika2_jump_FRAME; i++)
			{
				setTextureAlpha(pika2_jump[i].texture, pika2_alpha);
			}
			char grade1[3] = { pika1_grade1,pika1_grade2 };
			char grade2[3] = { pika2_grade1, pika2_grade2 };
			TextData textsolid1 = loadTextTexture(grade1, "../fonts/lazy.ttf", 100, 255, 0, 0, SOLID, NULL, NULL, NULL);
			TextData textsolid2 = loadTextTexture(grade2, "../fonts/lazy.ttf", 100, 255, 0, 0, SOLID, NULL, NULL, NULL);
			textRender(renderer, textsolid1, 100, 100, NULL, NULL, NULL, no);
			textRender(renderer, textsolid2, WIDTH - 150, 100, NULL, NULL, NULL, no);

			if (end_game == 1)//p1 win
			{
				pika1_jump_flag = false;
				pika2_jump_flag = false;
				imgRender(renderer, die, pika2_X - 15, 440);
				//Sleep(2000);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				end_game = 999;
			}
			else if (end_game == 2)
			{
				pika1_jump_flag = false;
				pika2_jump_flag = false;
				imgRender(renderer, die, pika1_X - 10, 440);
				//Sleep(2000);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				end_game = 999;
			}
			else if (end_game == 999)
				system("pause");

			SDL_RenderPresent(renderer);
		}
	}

	//Free resources and close SDL
	closeSDL();

	return 0;
}
