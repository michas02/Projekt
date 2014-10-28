#include "Photo.h"


Photo::Photo(SDL_Window *window)
{
	this->window=window;
}

void Photo::makeBW()
{
	this->lockTexture();
	Uint32* pixels=(Uint32*)this->pixels;
	int pixelCount=(this->pitch/4)*this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		Uint8 color=(red+green+blue)/3;
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, color, color, color); 
	}

	this->unlockTexture();
}

void Photo::makeSepia(int colorSaturation)
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		Uint8 color = (red+green+blue)/3;
		if(color+2*colorSaturation>255)
		{
			red=255;
		}
		else
		{
			red=color+2*colorSaturation;
		}
		if(color+colorSaturation>255)
		{
			green=255;
		}
		else
		{
			green=color+colorSaturation;
		}
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, red, green, color); 
	}

	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Photo::highContrast()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		red=contrastChange(1.1,red);
		green=contrastChange(1.1,green);
		blue=contrastChange(1.1,blue);
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, red, green, blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Photo::lowContrast()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		red=contrastChange(0.9,red);
		green=contrastChange(0.9,green);
		blue=contrastChange(0.9,blue);
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, red, green, blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

Uint8 Photo::contrastChange(double param, Uint8 color)
{
	if((param*(color-255/2)+255/2)<0)
	{
		return 0;
	}
	else if((param*(color-255/2)+255/2)<255)
	{
		return (Uint8)(param*(color-255/2)+255/2);
	}
	return 255;
}

void Photo::filterImage()
{
	int valArray[3][3]={{1,1,1},{1,4,1},{1,1,1}};
	//int valArray[3][3]={{0,-1,0},{0,2,0},{0,-1,0}};
	//int valArray[3][3]={{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};
	//int valArray[3][3]={{1,0,-1},{1,1,-1},{1,0,-1}};
	//int valArray[3][3]={{-1,-1,-1},{1,-2,1},{1,1,1}};
	Uint32 color = SDL_MapRGBA( SDL_GetWindowSurface(window)->format,0xFF,0xFF,0xFF,0);
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	Uint32 *newPixels = new Uint32[pixelCount];
	
	for(int x=2;x<this->pitch/4-1;x++)
	{
		for(int y=1;y<this->imageH-1;y++)
		{
			Uint8 red[3][3], green[3][3], blue[3][3];
			SDL_GetRGB(pixels[x-1+(y-1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[0][0], &green[0][0], &blue[0][0]);
			SDL_GetRGB(pixels[x+(y-1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[0][1], &green[0][1], &blue[0][1]);
			SDL_GetRGB(pixels[x+1+(y-1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[0][2], &green[0][2], &blue[0][2]);

			SDL_GetRGB(pixels[x-1+(y)*pitch/4],SDL_GetWindowSurface(window)->format,&red[1][0], &green[1][0], &blue[1][0]);
			SDL_GetRGB(pixels[x+(y)*pitch/4],SDL_GetWindowSurface(window)->format,&red[1][1], &green[1][1], &blue[1][1]);
			SDL_GetRGB(pixels[x+1+(y)*pitch/4],SDL_GetWindowSurface(window)->format,&red[1][2], &green[1][2], &blue[1][2]);

			SDL_GetRGB(pixels[x-1+(y+1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[2][0], &green[2][0], &blue[2][0]);
			SDL_GetRGB(pixels[x+(y+1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[2][1], &green[2][1], &blue[2][1]);
			SDL_GetRGB(pixels[x+1+(y+1)*pitch/4],SDL_GetWindowSurface(window)->format,&red[2][2], &green[2][2], &blue[2][2]);

			int sr=valArray[0][0]*red[0][0];
			sr+=valArray[0][1]*red[0][1];
			sr+=valArray[0][2]*red[0][2];

			sr+=valArray[1][0]*red[1][0];
			sr+=valArray[1][1]*red[1][1];
			sr+=valArray[1][2]*red[1][2];

			sr+=valArray[2][0]*red[2][0];
			sr+=valArray[2][1]*red[2][1];
			sr+=valArray[2][2]*red[2][2];

			int sg=valArray[0][0]*green[0][0];
			sg+=valArray[0][1]*green[0][1];
			sg+=valArray[0][2]*green[0][2];

			sg+=valArray[1][0]*green[1][0];
			sg+=valArray[1][1]*green[1][1];
			sg+=valArray[1][2]*green[1][2];

			sg+=valArray[2][0]*green[2][0];
			sg+=valArray[2][1]*green[2][1];
			sg+=valArray[2][2]*green[2][2];

			int sb=valArray[0][0]*blue[0][0];
			sb+=valArray[0][1]*blue[0][1];
			sb+=valArray[0][2]*blue[0][2];

			sb+=valArray[1][0]*blue[1][0];
			sb+=valArray[1][1]*blue[1][1];
			sb+=valArray[1][2]*blue[1][2];

			sb+=valArray[2][0]*blue[2][0];
			sb+=valArray[2][1]*blue[2][1];
			sb+=valArray[2][2]*blue[2][2];

			int arraySum=0;

			for(int i=0;i<3;i++)
			{
				for(int j=0;j<3;j++)
				{
					arraySum+=valArray[i][j];
				}
			}
			if(arraySum!=0)
			{
				sr/=arraySum;
				sg/=arraySum;
				sb/=arraySum;
				if(sr<0)sr=0;
				if(sg<0)sr=0;
				if(sb<0)sr=0;
				if(sr>255)sr=255;
				if(sg>255)sr=255;
				if(sb>255)sb=255;
			}
			//pixels[x+y*this->pitch/4]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, sr, sg, sb); 
			newPixels[x+y*this->pitch/4]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, sr, sg, sb);
		}
	}

	for(int x=2;x<this->pitch/4-1;x++)
	{
		for(int y=1;y<this->imageH-1;y++)
		{
			pixels[x+y*this->pitch/4]=newPixels[x+y*this->pitch/4];
		}
	}

	this->unlockTexture();
}

void Photo::lockTexture()
{
	SDL_LockTexture(texture,NULL,&pixels,&pitch);
}

void Photo::unlockTexture()
{
	SDL_UnlockTexture(texture);
}

void Photo::highContrast(int iterations)
{
	for(int i=0;i<iterations;i++)
	{
		this->highContrast();
	}
}

void Photo::lowContrast(int iterations)
{
	for(int i=0;i<iterations;i++)
	{
		this->lowContrast();
	}
}

Uint8 Photo::brightnessChange(int param, Uint8 color)
{
	if(param+color<0)
	{
		return 0;
	}
	if(param+color<255)
	{
		return param+color;
	}
	return 255;
}

void Photo::lowBrightness()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		red=brightnessChange(-10,red);
		green=brightnessChange(-10,green);
		blue=brightnessChange(-10,blue);
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, red, green, blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Photo::highBrightness()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		red=brightnessChange(10,red);
		green=brightnessChange(10,green);
		blue=brightnessChange(10,blue);
		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, red, green, blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}