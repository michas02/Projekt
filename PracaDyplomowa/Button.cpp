#include "Button.h"

Button::Button()
{
	
}

bool Button::init(int x,int y,int width,int height ,string *image,string text,string function,SDL_Renderer *renderer, string *fonts, bool visible,int tab, int ages)
{
	this->currentAge=0;
	this->ages=ages;
	this->tab=tab;
	this->function=function;
	this->visible=visible;
	this->selected=false;
	this->clicked=false;
	this->text=text;
	this->x=photoPanelWidth+x;
	this->y=y;
	this->width=width;
	this->height=height;
	this->defWidth=width;
	this->defHeight=height;
	this->texture=new Texture *[ages];
	this->textTexture=new Texture *[ages];
	for(int i=0;i<ages;i++)
	{
		texture[i]=new Texture();
		if(!this->texture[i]->loadTexture(image[i],COLOR_KEY,renderer))
		{
			return false;
		}
		this->texture[i]->setAlpha(150);
		int size = 72;
		if(text.length()>10)
		{
			size=32;
		}
		TTF_Font *font=TTF_OpenFont(fonts[i].c_str(),size);
		if(font==NULL)
		{
			cout<<"Couldn't open font from file: "<<SDL_GetError()<<endl;
			return false;
		}
		if(!this->initTextTexture(font,renderer,i))
		{
			return false;
		}
	}

	return true;
}

bool Button::initTextTexture(TTF_Font *font, SDL_Renderer *renderer, int index)
{
	this->textTexture[index]=new Texture();
	if(!this->textTexture[index]->loadTextTexture(font,text,renderer))
	{
		return false;
	}
	return true;
}

Button::~Button()
{
	for(int i=0;i<ages;i++)
	{
		this->texture[i]->free();
	}
}

bool Button::getClicked()
{
	return clicked;
}

void Button::setClicked()
{
	clicked=false;
}

bool Button::getSelected()
{
	return selected;
}

void Button::render(SDL_Renderer *renderer)
{
	SDL_Rect clip;
	clip.x=x;
	clip.y=y;
	clip.w=width;
	clip.h=height;
	texture[currentAge]->render(x,y,clip,renderer);
	textTexture[currentAge]->render(x,y,clip,renderer);
}

void Button::collision(SDL_Event &e, const Uint8 *state)
{
	if(state[SDL_SCANCODE_LCTRL]){selected=true;}else{selected=false;}

	if(e.type==SDL_MOUSEBUTTONDOWN)
	{
		if(e.button.button==SDL_BUTTON_LEFT)
		{
				
			int mouseX=0,mouseY=0;
			SDL_GetMouseState(&mouseX,&mouseY);
			if((x<mouseX)&&(mouseX<x+width)&&(y<mouseY)&&(mouseY<y+height)&&(!this->clicked))
			{
				this->clicked=true;
			}
			else
			{
				this->clicked=false;
				this->selected=false;
			}
		}

	}

	if(e.type==SDL_MOUSEBUTTONUP)
	{
		clicked=false;
		selected=false;
	}

	if(e.type==SDL_MOUSEWHEEL)
	{
		if((selected)&&(clicked))
		{
			if(e.wheel.y>0)
			{
				this->width+=10;
				this->height+=10;
			}
			if(e.wheel.y<0)
			{
				if(this->width>this->defWidth)
				{
				this->width-=10;
				}
				if(this->height>this->defHeight)
				this->height-=10;
			}
		}
	}

	if(selected)
	{
		if(clicked)
		{
			int mouseX=0,mouseY=0;
			SDL_GetMouseState(&mouseX,&mouseY);
			this->x=mouseX-this->width/2;
			this->y=mouseY-this->height/2;
		}
	}
}

string Button::getText()
{
	return text;
}

bool Button::isVisible()
{
	return visible;
}

void Button::setVisible(bool visible)
{
	this->visible=visible;
}

string Button::getFunction()
{
	return function;
}

void Button::tabChange(int tab)
{
	if(this->tab==tab)
	{
		visible=true;
	}
	else
	{
		visible=false;
	}
}

void Button::changeAge(int age)
{
	this->currentAge=age;
}