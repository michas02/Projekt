#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(SDL_Window *window)
{
	this->window=window;
	red=255;
	green=255;
	blue=255;
	alpha=255;
	imageW = 0;
	imageH = 0;
	frameW=0;
	frameNumber=0;
	frameCounter=0;
	texture = NULL;
	backupTexture = NULL;
	frameCounter2=0;
}

Texture::~Texture()
{
	free();
}

void Texture::free()
{
	if(texture!=NULL){
	SDL_DestroyTexture(texture);
	texture=NULL;}
	imageW=0;
	imageH=0;
}

bool Texture::loadTexture(string file, SDL_Renderer *renderer,int mode)
{
	//free();
	//SDL_Texture* loadTexture(string path);
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loadedSurface = IMG_Load(file.c_str());

	if(loadedSurface==NULL)
	{
		cout<<"Failed to load file: "<<SDL_GetError()<<endl;
		return false;
	}
	else
	{
		
		if(mode==COLOR_KEY)
		{
			SDL_SetColorKey(loadedSurface,SDL_TRUE,SDL_MapRGB(loadedSurface->format,0xFF,0,0xFF));
			newTexture = SDL_CreateTextureFromSurface(renderer,loadedSurface);
			imageW = loadedSurface->w;
			imageH = loadedSurface->h;
			texture=newTexture;
		}
		if(mode==PIXEL)
		{
			SDL_Surface* formattedSurface = SDL_ConvertSurface( loadedSurface, SDL_GetWindowSurface( window )->format, NULL );
			newTexture = SDL_CreateTexture( renderer, SDL_GetWindowPixelFormat( window ), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );
			if(newTexture==NULL)
			{
				cout<<"Failed to convert surface: "<<SDL_GetError()<<endl;
				return false;
			}
			else
			{
				SDL_LockTexture( newTexture, NULL, &pixels, &pitch );
				memcpy( pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h );
				SDL_UnlockTexture( newTexture );

				pixels = NULL;
				imageW = formattedSurface->w;
				imageH = formattedSurface->h;
				texture=newTexture;
			}
			SDL_FreeSurface(loadedSurface);
			SDL_FreeSurface(formattedSurface);
		}
	}
	//this->colorKey(window);
	return true;
}


void Texture::render(int x,int y,SDL_Renderer *renderer)
{
	SDL_Rect renderQuad = {x,y,imageW,imageH};
	SDL_RenderCopy(renderer,texture,NULL,&renderQuad);
}

void Texture::render(int x,int y,SDL_Renderer *renderer, double angle)
{

	if(frameNumber==0)
	{
		SDL_Rect renderQuad = {x,y,imageW,imageH};
		SDL_RenderCopyEx(renderer,texture,NULL,&renderQuad,angle,NULL,SDL_FLIP_NONE);
	}
	else
	{
		if(frameCounter==frameNumber){frameCounter=0;}
		SDL_Rect renderQuad = {x,y,frameW,imageH};
		SDL_Rect renderQuadClip = imageClip[frameCounter];
		SDL_RenderCopyEx(renderer,texture,&renderQuadClip,&renderQuad,angle,NULL,SDL_FLIP_NONE);
		frameCounter2++;
		if(frameCounter2==6){
			frameCounter2=0;
			frameCounter++;}
	}
}

void Texture::renderStreched(SDL_Renderer *renderer)
{
	SDL_Rect clip;
	
	clip.x=0;
	clip.y=0;
	clip.w=this->imageW;
	clip.h=this->imageH;

	this->render(0,0,renderer,clip);
}

int Texture::getHeight()
{
	return imageH;
}

int Texture::getWidth()
{
	if(frameW!=0){
		return frameW;
	}
	return imageW;
}

void Texture::setAlpha(int alpha)
{
	/*if(alpha<0){alpha=0;}
	if(alpha>100){alpha=100;}

	double tempAlpha=0;
	if(alpha!=0)
	{
		tempAlpha = 255 / (100/alpha);
	}*/
	SDL_SetTextureAlphaMod(texture,alpha);
}

void Texture::setBlendMode(SDL_BlendMode blend)
{
	SDL_SetTextureBlendMode(texture,blend);
}

void Texture::modifyColor(int red,int green, int blue)
{
	SDL_SetTextureColorMod(texture,red,green,blue);
}

void Texture::render(int x, int y, SDL_Renderer *renderer,SDL_Rect &clip)
{
	SDL_Rect displayRect = {0,0,800,600};
	SDL_RenderCopyEx(renderer,texture,&clip,&displayRect,NULL,NULL,SDL_FLIP_NONE);
}


/*
void Texture::colorKey(SDL_Window *window)
{
	 //Get pixel data
	this->lockTexture();
	Uint32* pixels=(Uint32*)this->pixels;
	int pixelCount=(this->pitch/4)*this->imageH;

    Uint32 colorKey = SDL_MapRGB( SDL_GetWindowSurface(window)->format,0xFF,0, 0xFF);
    Uint32 transparent = SDL_MapRGBA( SDL_GetWindowSurface(window)->format,0xFF,0xFF,0xFF,0);

            //Color key pixels
           for(int i=0;i<pixelCount;++i)
           {
               if(pixels[i]==colorKey)
               {
                   pixels[i]=transparent;
               }
           }

            //Unlock texture
	this->unlockTexture();
}*/


void Texture::increaseColor(int color)
{
	switch(color)
	{
		case RED:
			if(red<255){red++;}
			break;
		case GREEN:
			if(green<255){green++;}
			break;
		case BLUE:
			if(blue<255){blue++;}
			break;
		default:
			break;
	}
	
	this->modifyColor(red,green,blue);
}

void Texture::decreaseColor(int color)
{
	switch(color)
	{
	case RED:
		if(red>0){red--;}
		break;
	case GREEN:
		if(green>0){green--;}
		break;
	case BLUE:
		if(blue>0){blue--;}
		break;
	default:
		break;
	}
	this->modifyColor(red,green,blue);
}

void Texture::increaseAlpha()
{
	if(alpha<255){alpha++;}
	this->setAlpha(alpha);
}

void Texture::decreaseAlpha()
{
	if(alpha>0){alpha--;}
	this->setAlpha(alpha);
}