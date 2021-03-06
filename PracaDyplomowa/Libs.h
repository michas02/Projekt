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
#include <SDL_ttf.h>

using namespace std;

enum TEXTURE_ENUM {RED,GREEN,BLUE};

enum PHOTO_ENUM {PIXEL,COLOR_KEY};

enum FILTER {HIGH,LOW};

enum TABS {BASIC,ADVANCED};

enum COUNTERS {FILTER,CONTRAST,BRIGHTNESS,SATURATION};

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

const int screenWidth = 1366;
const int screenHeight = 768;

const int photoPanelWidth = screenWidth*4/5;
const int photoPanelHeight = screenHeight*4/5;

#endif