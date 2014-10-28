#ifndef LIBS_H
#define LIBS_H

#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <fstream>


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

enum TEXTURE_ENUM {RED,GREEN,BLUE};

enum PHOTO_ENUM {PIXEL,COLOR_KEY};

struct RGB
{
	Uint8 red;
	Uint8 green;
	Uint8 blue;
};

struct HSV
{
	double hue;
	double sat;
	double val;
};

#endif