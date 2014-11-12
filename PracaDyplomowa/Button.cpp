#include "Button.h"

Button::Button()
{
	
}

bool Button::init(int x,int y,int width,int height ,string image,string text,string function,SDL_Renderer *renderer, TTF_Font *font, bool visible,int tab)
{
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
	this->texture=NULL;
	this->texture=new Texture();
	if(!this->texture->loadTexture(image,renderer,COLOR_KEY))
	{
		return false;
	}
	if(!this->initTextTexture(font,renderer))
	{
		return false;
	}
	this->texture->setAlpha(150);
	return true;
}

bool Button::initTextTexture(TTF_Font *font, SDL_Renderer *renderer)
{
	this->textTexture=new Texture();
	if(!this->textTexture->loadTextTexture(font,text,renderer))
	{
		return false;
	}
	return true;
}

Button::~Button()
{
	this->texture->free();
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
	texture->render(x,y,renderer,clip);
	textTexture->render(x,y,renderer,clip);
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