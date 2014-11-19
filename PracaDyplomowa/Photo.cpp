#include "Photo.h"


Photo::Photo(SDL_Window *window,SDL_Renderer *renderer)
{
	this->brightnessCounter=0;
	this->contrastCounter=0;
	this->filterCounter=0;
	this->saturationCounter=0;
	this->window=window;
	this->scratched=false;
	this->border=false;
}

Photo::~Photo()
{
	scratch->free();
	white->free();
}

bool Photo::init(SDL_Renderer *renderer)
{
	scratch = new Texture(window);
	if(!scratch->loadTexture("scratch2.jpg",COLOR_KEY,renderer))
	{
		return false;
	}
	scratch->setBlendMode(SDL_BLENDMODE_BLEND);
	scratch->setAlpha(50);
	white = new Texture(window);
	if(!white->loadTexture("whiteBorder.jpg",COLOR_KEY,renderer))
	{
		return false;
	}
	white->setAlpha(120);
	white->setBlendMode(SDL_BLENDMODE_BLEND);
	return true;
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
		//Uint8 color=(red+green+blue)/3;
		Uint8 color=red*0.299+green*0.587+blue*0.114;
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

void Photo::filterImage(int filter)
{
	int valArray[3][3]={{0,0,0},{0,0,0},{0,0,0}};
	
	if(filter==LOW)
	{
		valArray[0][0]=1;
		valArray[0][1]=1;
		valArray[0][2]=1;

		valArray[1][0]=1;
		valArray[1][1]=4;
		valArray[1][2]=1;

		valArray[2][0]=1;
		valArray[2][1]=1;
		valArray[2][2]=1;
	}

		if(filter==HIGH)
	{
		valArray[0][0]=-1;
		valArray[0][1]=-1;
		valArray[0][2]=-1;

		valArray[1][0]=-1;
		valArray[1][1]=20;
		valArray[1][2]=-1;

		valArray[2][0]=-1;
		valArray[2][1]=-1;
		valArray[2][2]=-1;
	}
	//int 
	//int valArray[3][3]={{0,-1,0},{0,2,0},{0,-1,0}};
	//int valArray[3][3]={{-1,-1,-1},{-1,20,-1},{-1,-1,-1}};
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

			}
				if(sr<0)sr=0;
				if(sg<0)sg=0;
				if(sb<0)sb=0;
				if(sr>255)sr=255;
				if(sg>255)sg=255;
				if(sb>255)sb=255;
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

HSV Photo::RGBtoHSV(RGB rgb)
{
	HSV hsv={0,0,0};

	double x=0,val=0,f=0;
	int i=0;

	double red=0;
	double green=0;
	double blue=0;

	red = (double)rgb.red/255;
	green = (double)rgb.green/255;
	blue = (double)rgb.blue/255;
	
	x=red;
	if(x>green)x=green;
	if(x>blue)x=blue;

	val=red;
	if(val<green)val=green;
	if(val<blue)val=blue;

	if(x==val)
	{
		hsv.hue=0;
		hsv.sat=0;
		if(hsv.val>1)hsv.val=1;
		if(hsv.val<0)hsv.val=0;
		hsv.val=val;
		return hsv;
	}
	else if(red==x)
	{
		f=green-blue;
		i=3;
	}
	else if(green==x)
	{
		f=blue-red;
		i=5;
	}
	else
	{
		f=red-green;
		i=1;
	}
	hsv.hue = (int)((i-f/(val-x))*60)%360;
	hsv.sat = ((val-x)/val);
	hsv.val = val;

	if(hsv.val>1)hsv.val=1;
	if(hsv.val<0)hsv.val=-0;

	if(hsv.sat>1)hsv.sat=1;
	if(hsv.sat<0)hsv.sat=0;

	return hsv;
}

RGB Photo::HSVtoRGB(HSV hsv)
{
	RGB rgb={0,0,0};
	double red=0,green=0,blue=0;
	if(hsv.val==0){red=0;green=0;blue=0;}
	else
	{
		hsv.hue/=60;
		double i=0,f=0,p=0,q=0,t=0;
		i = floor(hsv.hue);
		f = hsv.hue-i;
		p = hsv.val*(1-hsv.sat);
		q = hsv.val*(1-(hsv.sat*f));
		t = hsv.val*(1-(hsv.sat*(1-f)));
		if(i==0){red=hsv.val;green=t;blue=p;}
		else if(i==1){red=q;green=hsv.val;blue=p;}
		else if(i==2){red=p;green=hsv.val;blue=t;}
		else if(i==3){red=p;green=q;blue=hsv.val;}
		else if(i==4){red=t;green=p;blue=hsv.val;}
		else if(i==5){red=hsv.val;green=p;blue=q;}
	}
	red*=255;
	green*=255;
	blue*=255;
	rgb.red=(int)red;
	rgb.green=(int)green;
	rgb.blue=(int)blue;
	if(rgb.red<0)rgb.red=0;
	if(rgb.red>255)rgb.red=255;

	if(rgb.green<0)rgb.green=0;
	if(rgb.green>255)rgb.green=255;

	if(rgb.blue<0)rgb.blue=0;
	if(rgb.blue>255)rgb.blue=255;
	return rgb;
}

void Photo::highSaturation()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		
		RGB rgb={red,green,blue};
		HSV hsv= RGBtoHSV(rgb);
		if(hsv.sat<0.95)
		{
			hsv.sat+=0.05;
		}
		rgb= HSVtoRGB(hsv);

		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, rgb.red, rgb.green, rgb.blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Photo::lowSaturation()
{
	this->lockTexture();
	Uint32* pixels = (Uint32*)this->pixels;
	int pixelCount = (this->pitch/4) * this->imageH;
	for(int i=0;i<pixelCount;++i)
	{
		Uint8 red, green, blue;
		SDL_GetRGB(pixels[i],SDL_GetWindowSurface(window)->format,&red, &green, &blue);
		
		RGB rgb={red,green,blue};
		HSV hsv= RGBtoHSV(rgb);
		if(hsv.sat>0.05)
		{
			hsv.sat-=0.05;
		}
		rgb= HSVtoRGB(hsv);

		pixels[i]=SDL_MapRGB(SDL_GetWindowSurface(window)->format, rgb.red, rgb.green, rgb.blue); 
	}
	this->unlockTexture();
	this->setBlendMode(SDL_BLENDMODE_BLEND);
}

void Photo::setBrightnessCounter(int brightnessCounter)
{
	this->brightnessCounter=brightnessCounter;
}

void Photo::setContrastCounter(int contrastCounter)
{
	this->contrastCounter=contrastCounter;
}

void Photo::setFilterCounter(int filterCounter)
{
	this->filterCounter=filterCounter;
}

void Photo::setSaturationCounter(int saturationCounter)
{
	this->saturationCounter=saturationCounter;
}

void Photo::incCounter(int counter)
{
	if(counter==FILTER)
	{
		filterCounter++;
	}
	else if(counter==CONTRAST)
	{
		contrastCounter++;
	}
	else if(counter==SATURATION)
	{
		saturationCounter++;
	}
	else if(counter==BRIGHTNESS)
	{
		brightnessCounter++;
	}
}


void Photo::decCounter(int counter)
{
	if(counter==FILTER)
	{
		filterCounter--;
	}
	else if(counter==CONTRAST)
	{
		contrastCounter--;
	}
	else if(counter==SATURATION)
	{
		saturationCounter--;
	}
	else if(counter==BRIGHTNESS)
	{
		brightnessCounter--;
	}
}

void Photo::restoreEffects()
{
	if(brightnessCounter>0)
	{
		for(int i=0;i<brightnessCounter;i++)
		{
			this->highBrightness();
		}
	}
	else
	{
		for(int i=brightnessCounter;i>0;i++)
		{
			this->lowContrast();
		}
	}



	if(saturationCounter>0)
	{
		for(int i=0;i<saturationCounter;i++)
		{
			this->highSaturation();
		}
	}
	else
	{
		for(int i=saturationCounter;i>0;i++)
		{
			this->lowSaturation();
		}
	}

	
	if(filterCounter>0)
	{
		for(int i=0;i<filterCounter;i++)
		{
			this->filterImage(HIGH);
		}
	}
	else
	{
		for(int i=filterCounter;i>0;i++)
		{
			this->filterImage(LOW);
		}
	}

	
	if(contrastCounter>0)
	{
		for(int i=0;i<contrastCounter;i++)
		{
			this->highContrast();
		}
	}
	else
	{
		for(int i=contrastCounter;i>0;i++)
		{
			this->lowContrast();
		}
	}
}

void Photo::render(SDL_Renderer *renderer)
{
	Texture::renderStreched(renderer);
	if(scratched)
	{
		scratch->renderStreched(renderer);
	}
	if(border)
	{
		white->renderStreched(renderer);
	}
}

void Photo::setScratches(bool scratched)
{
	this->scratched=scratched;
}

void Photo::setBorder(bool border)
{
	this->border=border;
}

void Photo::reset(SDL_Renderer *renderer)
{
	setBorder(false);
	setScratches(false);
	free();
	loadTexture("bear.jpg",PIXEL,renderer);
}