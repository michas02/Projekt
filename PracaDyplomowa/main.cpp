#include "Libs.h"
//#include "Texture.h"
#include "Photo.h"

SDL_Window *window = NULL;
const int screenWidth = 800;
const int screenHeight = 600;

SDL_Renderer *renderer = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Texture *texture = NULL;


Texture *white = NULL;
Photo *image = NULL;
Texture *scratch = NULL;
const Uint8 *state;

SDL_Event e;

bool red=false,green=false,blue=false,alpha=false;
int redVal=255,greenVal=255,blueVal=255,alphaVal=255;

bool init()
{
	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		cout<<"Failed to initialize video: "<<SDL_GetError()<<endl;
		system("PAUSE");
		return false;
	}
	if( SDL_Init(SDL_INIT_AUDIO )<0 )
    {
        cout<<"Failed to initialize audio: "<<SDL_GetError()<<endl;
		system("PAUSE");
        return false;
    }
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        cout<<"Failed to initialize sound: "<<SDL_GetError()<<endl;
		system("PAUSE");
        return false;
    }
	window = SDL_CreateWindow("Edytor",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,screenWidth,screenHeight,SDL_WINDOW_SHOWN);

	if(window == NULL)
	{
		cout<<"Can't create window: "<<SDL_GetError()<<endl;
		system("PAUSE");
		return false;
	}
	else
	{
		renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        if( renderer == NULL )
        {
            cout<<"Render could not be initialized: "<<SDL_GetError()<<endl;
			system("PAUSE");
            return false;
        }
        else
        {
			SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
			int imgFlags = IMG_INIT_PNG;
			if(!( IMG_Init(imgFlags)&imgFlags))
			{
				cout<<"SDL_image could not be initialized: "<<SDL_GetError()<<endl;
				system("PAUSE");
				return false;
			}
			else
			{
				screenSurface = SDL_GetWindowSurface(window);
			}
		}
	}
	state = SDL_GetKeyboardState(NULL);
	return true;
}

bool loadMedia()
{
	image = new Photo(window);
	if(!image->loadTexture("bear.jpg",renderer))
	{
		return false;
	}
	image->setBlendMode(SDL_BLENDMODE_BLEND);
	scratch = new Texture(window);
	if(!scratch->loadTexture("scratch.jpg",renderer))
	{
		return false;
	}
	scratch->setBlendMode(SDL_BLENDMODE_BLEND);
	white = new Texture(window);
	if(!white->loadTexture("white.bmp",renderer))
	{
		return false;
	}
	white->setBlendMode(SDL_BLENDMODE_BLEND);
	return true;
}
void close()
{
	image->free();
	scratch->free();
	white->free();
	SDL_DestroyTexture(texture);
	texture = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;
	SDL_DestroyWindow(window);
	window = NULL;
	IMG_Quit();
	SDL_Quit();
}

void controls(SDL_Event &e)
{
	int color=0;

	if(e.type==SDL_KEYDOWN)
	{
		switch(e.key.keysym.sym)
		{
		case SDLK_r:
			red=!red;
			red?cout<<"Red selected\n":cout<<"Red deselected\n";
			break;
		case SDLK_g:
			green=!green;
			green?cout<<"Green selected\n":cout<<"Green deselected\n";
			break;
		case SDLK_b:
			blue=!blue;
			blue?cout<<"Blue selected\n":cout<<"Blue deselected\n";
			break;
		case SDLK_a:
			alpha=!alpha;
			alpha?cout<<"Alpha selected\n":cout<<"Alpha deselected\n";
			break;
		case SDLK_w:
			image->makeBW();
			cout<<"Black & white\n";
			break;
		case SDLK_s:
			image->makeSepia(35);
			cout<<"Sepia\n";
			break;
		case SDLK_n:
			red=false;
			green=false;
			blue=false;
			alpha=false;
			redVal=255;
			greenVal=255;
			blueVal=255;
			alphaVal=255;
			cout<<"Reset\n";
			image->free();
			image->loadTexture("bear.jpg",renderer);
			//image->modifyColor(redVal,greenVal,blueVal);
			//image->setAlpha(alphaVal);
			break;
			default:break;
		case SDLK_u:
			red=false;
			green=false;
			blue=false;
			alpha=false;
			cout<<"All deselected\n";
		case SDLK_o:
			image->highContrast();
			break;
		case SDLK_l:
			image->lowContrast();
			break;
		case SDLK_f:
			image->filterImage();
			break;
		}
	}


	if(state[SDL_SCANCODE_KP_PLUS])
	{
		if(red){image->increaseColor(RED);}
		if(green){image->increaseColor(GREEN);}
		if(blue){image->increaseColor(BLUE);}
		if(alpha){image->increaseAlpha();}
	}

		if(state[SDL_SCANCODE_KP_MINUS])
	{
		if(red){image->decreaseColor(RED);}
		if(green){image->decreaseColor(GREEN);}
		if(blue){image->decreaseColor(BLUE);}
		if(alpha){image->decreaseAlpha();}
	}
}


int main( int argc, char* args[] )
{
	bool quit=false;
	if(init())
	{
		if(loadMedia())
		{
			while(!quit)
			{

				while(SDL_PollEvent(&e)!=0)
				{
					if(e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if(e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
						case SDLK_ESCAPE: quit=true; break;
						default: break;
						}
					}
					controls(e);
				}
				SDL_Rect clip;
				clip.x=0;
				clip.y=0;
				clip.w=991;
				clip.h=678;
				SDL_Rect clip2;
				clip2.x=0;
				clip2.y=0;
				clip2.w=1024;
				clip2.h=1024;
				image->render(0,0,renderer,clip);
				//scratch->setAlpha(50);
				//scratch->render(0,0,renderer);
				//white->setAlpha(255);
				//white->render(0,0,renderer);
				SDL_RenderPresent(renderer);
				SDL_RenderClear(renderer);
			}
		}
	}
	close();
	return 0;
}