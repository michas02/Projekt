#include "Libs.h"
#include "Photo.h"
#include "Button.h"

SDL_Window *window = NULL;


SDL_Renderer *renderer = NULL;
SDL_Surface *screenSurface = NULL;
SDL_Texture *texture = NULL;
SDL_Texture *panelTexture = NULL;
Button **buttons = NULL;
const int buttonLimit = 26;

Photo *image = NULL;
Texture *shatter = NULL;
Texture **photoPanel = NULL;
const Uint8 *state;

SDL_Event e;

bool red=false,green=false,blue=false,alpha=false;
int redVal=255,greenVal=255,blueVal=255,alphaVal=255;

int ages=4;
int currentAge=0;

bool init()
{
	if(TTF_Init()<0)
	{
		cout<<"Failed to initialize TTF: "<<SDL_GetError()<<endl;
		system("PAUSE");
		return false;
	}
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
	SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);
	state = SDL_GetKeyboardState(NULL);
	return true;
}

bool loadMedia()
{
	/*
	string defaultButton[4] = {"Images\\Buttons\\defaultButton.jpg","Images\\Buttons\\blank-wood-plaque.gif","Images\\Buttons\\steelButton.jpg","Images\\Buttons\\rustButton.jpg"};
	string changeButton[4] = {"Images\\Buttons\\defaultButton.jpg","Images\\Buttons\\woodChangeButton.gif","Images\\Buttons\\steelButton.jpg","Images\\Buttons\\rustButton.jpg"};
	string panelImages[4] = {"Images\\Background\\defaultWall.jpg","Images\\Background\\wood.jpg","Images\\Background\\stoneWall.jpg","Images\\Background\\rustWall.jpg"};
	*/
	int ages=4;
	string defaultButton[4] = {"Image\\Button\\defaultButton.jpg","Image\\Button\\woodButton.gif","Image\\Button\\steelButton.jpg","Image\\Button\\rustButton.jpg"};
	string changeButton[4] = {"Image\\Button\\defaultButton.jpg","Image\\Button\\woodChange.gif","Image\\Button\\steelButton.jpg","Image\\Button\\rustButton.jpg"};
	string panelImages[4] = {"Image\\Background\\defaultWall.jpg","Image\\Background\\wood.jpg","Image\\Background\\stoneWall.jpg","Image\\Background\\rustWall.jpg"};
	string fonts[4] = {"Fonts\\Montserrat-Regular.ttf","Fonts\\CREAMPUF.ttf","Fonts\\uwch.ttf","Fonts\\FancyPantsNF.ttf"};

	image = new Photo(window,renderer);
	if(!image->loadTexture("bear.jpg",PIXEL,renderer))
	{
		return false;
	}
	if(!image->init(renderer))
	{
		return false;
	}
	image->setBlendMode(SDL_BLENDMODE_BLEND);
	photoPanel = new Texture *[ages];
	for(int i=0;i<ages;i++)
	{
		photoPanel[i]=new Texture(window);
		if(!photoPanel[i]->loadTexture(panelImages[i],COLOR_KEY,renderer))
		{
			return false;
		}
	}

	shatter = new Texture(window);
	if(!shatter->loadTexture("shattered.jpg",COLOR_KEY,renderer))
	{
		return false;
	}
	shatter->setBlendMode(SDL_BLENDMODE_BLEND);


	buttons = new Button*[buttonLimit];
	for(int i=0;i<buttonLimit;i++)
	{
		buttons[i]=new Button();
	}
	if(!buttons[0]->init(40,60,120,40,defaultButton,"1830'","Preset 1",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}

	if(!buttons[1]->init(40,120,120,40,defaultButton,"1880'","Preset 2",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}

	if(!buttons[2]->init(40,180,120,40,defaultButton,"1940'","Preset 3",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}
	if(!buttons[3]->init(40,240,120,40,defaultButton,"Reset","Reset",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}
	if(!buttons[4]->init(40,300,120,40,defaultButton,"Koniec","Koniec",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}
	if(!buttons[5]->init(screenWidth-40-photoPanelWidth,0,40,40,defaultButton,"X","X",renderer,fonts,true,BASIC|ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[6]->init(20,0,200,40,defaultButton,"Ustawienia zaawansowane","Ustawienia zaawansowane",renderer,fonts,true,BASIC,ages))
	{
		return false;
	}
	if(!buttons[7]->init(20,0,200,40,defaultButton,"Ustawienia podstawowe","Ustawienia podstawowe",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[8]->init(40,60,120,40,defaultButton,"Kontrast","Kontrast",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[9]->init(40,100,40,40,changeButton,"-","Kontrast-",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[10]->init(120,100,40,40,changeButton,"+","Kontrast+",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[11]->init(40,160,120,40,defaultButton,"Gamma","",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[12]->init(40,200,40,40,changeButton,"-","Gamma-",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[13]->init(120,200,40,40,changeButton,"+","Gamma+",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[14]->init(40,260,120,40,defaultButton,"Saturacja","",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[15]->init(40,300,40,40,changeButton,"-","Saturacja-",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[16]->init(120,300,40,40,changeButton,"+","Saturacja+",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[17]->init(40,460,120,40,defaultButton,"Czarno-Biale","BW",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[18]->init(40,520,120,40,defaultButton,"Sepia","Sepia",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[19]->init(40,360,120,40,defaultButton,"Rozmycie","Rozmycie",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[20]->init(90,110,20,20,defaultButton,"r","ResetKontrast",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[21]->init(90,210,20,20,defaultButton,"r","ResetGamma",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[22]->init(90,310,20,20,defaultButton,"r","ResetSaturacja",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[23]->init(40,400,40,40,changeButton,"-","Rozmycie-",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[24]->init(120,400,40,40,changeButton,"+","Rozmycie+",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	if(!buttons[25]->init(90,410,20,20,defaultButton,"r","ResetRozmycie",renderer,fonts,false,ADVANCED,ages))
	{
		return false;
	}
	return true;
}
void close()
{
	delete buttons;
	image->free();
	for(int i=0;i<ages;i++)
	{
		photoPanel[i]->free();
	}

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
			image->reset(renderer);
			break;
			default:break;
		case SDLK_d:
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
			image->filterImage(LOW);
			break;
		case SDLK_i:
			image->highBrightness();
			break;
		case SDLK_k:
			image->lowBrightness();
			break;
		case SDLK_u:
			image->highSaturation();
			break;
		case SDLK_j:
			image->lowSaturation();
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

					for(int i=0;i<buttonLimit;i++)
					{
						if(buttons[i]->isVisible())
						{
							buttons[i]->collision(e,state);
						}
					}
				}
				image->render(renderer);
				photoPanel[currentAge]->renderPanel(renderer);

				for(int i=0;i<buttonLimit;i++)
				{
					if(buttons[i]->isVisible())
					{
						buttons[i]->render(renderer);
					}
				}
				
				SDL_RenderPresent(renderer);
				SDL_RenderClear(renderer);

				for(int i=0;i<buttonLimit;i++)
				{
					if(buttons[i]->isVisible())
					{
						if((buttons[i]->getClicked())&&(!buttons[i]->getSelected()))
						{
							string button=buttons[i]->getFunction();
							if(button=="Preset 1")
							{
								image->reset(renderer);
								image->makeBW();
								image->filterImage(LOW);
								image->lowContrast(10);
								image->setBorder(true);
								image->setScratches(true);
								for(int i=0;i<buttonLimit;i++)buttons[i]->changeAge(1);
								currentAge=1;
							}

							if(button=="Preset 2")
							{
								image->reset(renderer);
								image->makeSepia(40);
								image->lowContrast(2);
								image->filterImage(LOW);
								image->filterImage(LOW);
								image->setBorder(false);
								image->setScratches(true);
								for(int i=0;i<buttonLimit;i++)buttons[i]->changeAge(3);
								currentAge=3;
							}
							if(button=="Preset 3")
							{
								image->reset(renderer);
								image->filterImage(LOW);
								image->lowSaturation();
								image->lowSaturation();
								image->lowSaturation();
								image->lowSaturation();
								image->lowContrast();
								image->highBrightness();
								image->highBrightness();
								image->highBrightness();
								image->highBrightness();
								image->setBorder(false);
								image->setScratches(false);
								for(int i=0;i<buttonLimit;i++)buttons[i]->changeAge(2);
								currentAge=2;
							}
							if(button=="Reset")
							{
								image->reset(renderer);
								for(int i=0;i<buttonLimit;i++)buttons[i]->changeAge(0);
								currentAge=0;
							}
							if(button=="Koniec")
							{
								quit=true;
							}
							if(button=="X")
							{
								quit=true;
							}
							if(button=="Ustawienia zaawansowane")
							{
								for(int i=0;i<buttonLimit;i++)
								{
									buttons[i]->tabChange(ADVANCED);
								}
								break;
							}

							if(button=="Ustawienia podstawowe")
							{
								for(int i=0;i<buttonLimit;i++)
								{
									//buttons[i]->tabChange(BASIC);
								}
								break;
							}
							if(button=="Kontrast+")
							{
								image->highContrast();
								image->incCounter(CONTRAST);
							}
							if(button=="Kontrast-")
							{
								image->lowContrast();
								image->decCounter(CONTRAST);
							}
							if(button=="Gamma+")
							{
								image->highBrightness();
								image->incCounter(BRIGHTNESS);
							}
							if(button=="Gamma-")
							{
								image->lowBrightness();
								image->decCounter(BRIGHTNESS);
							}
							if(button=="Saturacja+")
							{
								image->highSaturation();
								image->incCounter(SATURATION);
							}
							if(button=="Saturacja-")
							{
								image->lowSaturation();
								image->decCounter(SATURATION);
							}
							if(button=="BW")
							{
								image->reset(renderer);
								image->makeBW();
							}
							if(button=="Sepia")
							{
								image->reset(renderer);
								image->makeSepia(40);
							}
							if(button=="Rozmycie")
							{
								image->filterImage(LOW);
							}
							if(button=="Rozmycie-")
							{
								image->filterImage(LOW);
								image->decCounter(FILTER);
							}
							if(button=="Rozmycie+")
							{
								image->filterImage(HIGH);
								image->incCounter(FILTER);
							}
							if(button=="ResetRozmycie")
							{
								image->reset(renderer);
								image->setFilterCounter(0);
								image->restoreEffects();
							}
							if(button=="ResetSaturacja")
							{
								image->reset(renderer);
								image->setSaturationCounter(0);
								image->restoreEffects();
							}
							if(button=="ResetGamma")
							{
								image->reset(renderer);
								image->setBrightnessCounter(0);
								image->restoreEffects();
							}
							if(button=="ResetKontrast")
							{
								image->reset(renderer);
								image->setContrastCounter(0);
								image->restoreEffects();
							}
							buttons[i]->setClicked();
						}
					}
				}
			}
		}
	}
	close();
	return 0;
}