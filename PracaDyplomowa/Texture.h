#ifndef TEXTURE
#define TEXTURE

#include "Libs.h"

using namespace std;

class Texture
{
protected:
	SDL_Window *window;
	SDL_Texture *texture;
	SDL_Texture *backupTexture;
	int imageW;
	int imageH;

	int red;
	int green;
	int blue;
	
	int alpha;

	int frameNumber;
	int frameW;
	SDL_Rect *imageClip;
	int frameCounter;
	int frameCounter2;

	void* pixels;
    int pitch;

	//void colorKey(SDL_Window *window);
public:
	Texture(SDL_Window *window);
	Texture();
	~Texture();
	bool loadTexture(string file, SDL_Renderer *renderer);
	bool loadTexture(string file, SDL_Renderer *renderer, int frameW);

	int getWidth();
	int getHeight();

	void render(int x, int y, SDL_Renderer *renderer);
	void render(int x, int y, SDL_Renderer *renderer, double angle);
	void renderStreched(SDL_Renderer *renderer);
	void render(int x, int y, SDL_Renderer *renderer,SDL_Rect &clip);

	void free();

	void modifyColor(int red,int green, int blue);
	void setBlendMode(SDL_BlendMode blend);
	void setAlpha(int alpha);

	void increaseColor(int color);
	void decreaseColor(int color);
	void increaseAlpha();
	void decreaseAlpha();
};


#endif