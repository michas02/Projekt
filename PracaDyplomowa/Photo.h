#ifndef PHOTO_H
#define PHOTO_H

#include "Libs.h"
#include "Texture.h"

class Photo : public Texture
{
private:
	bool scratched;
	bool border;
	Texture *white;
	Texture *scratch;

	int saturationCounter;
	int brightnessCounter;
	int filterCounter;
	int contrastCounter;

	void lockTexture();
	void unlockTexture();
	Uint8 contrastChange(double param, Uint8 color);
	Uint8 brightnessChange(int param, Uint8 color);

	HSV RGBtoHSV(RGB rgb);
	RGB HSVtoRGB(HSV hsv);


public:
	Photo();
	Photo(SDL_Window *window,SDL_Renderer *renderer);
	~Photo();
	bool init(SDL_Renderer *renderer);
	void makeBW();
	void makeSepia(int colorSaturation);
	void highContrast();
	void highContrast(int iterations);
	void lowContrast();
	void lowContrast(int iterations);
	void highBrightness();
	void lowBrightness();
	void highSaturation();
	void lowSaturation();
	void filterImage(int filter);

	void setSaturationCounter(int saturationCounter);
	void setContrastCounter(int contrastCounter);
	void setFilterCounter(int filterCounter);
	void setBrightnessCounter(int brightnessCounter);

	void incCounter(int counter);
	void decCounter(int counter);
	void restoreEffects();

	void setScratches(bool scratched);
	void setBorder(bool border);

	void render(SDL_Renderer *renderer);
	void reset(SDL_Renderer *renderer);
};

#endif