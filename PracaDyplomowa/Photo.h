#ifndef PHOTO_H
#define PHOTO_H

#include "Libs.h"
#include "Texture.h"

class Photo : public Texture
{
private:
	void lockTexture();
	void unlockTexture();
	Uint8 contrastChange(double param, Uint8 color);
	Uint8 brightnessChange(int param, Uint8 color);

	HSV RGBtoHSV(RGB rgb);
	RGB HSVtoRGB(HSV hsv);
public:
	Photo();
	Photo(SDL_Window *window);
	~Photo();
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
	void filterImage();
};

#endif