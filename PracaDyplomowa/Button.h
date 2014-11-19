#ifndef BUTTON_H
#define BUTTON_H

#include "Libs.h"
#include "Texture.h"

class Button
{
private:
	int ages;
	int currentAge;
	Texture **texture;
	Texture **textTexture;
	int x,y;
	int defWidth,defHeight;
	int width,height;
	string text;
	string function;
	int tab;
	bool clicked;
	bool selected;
	bool visible;
	bool initTextTexture(TTF_Font *font,SDL_Renderer *renderer, int index);
public:
	Button();
	~Button();
	void changeAge(int age);
	void render(SDL_Renderer *renderer);
	void collision(SDL_Event &e, const Uint8 *state);
	bool getClicked();
	void setClicked();
	bool getSelected();
	bool isVisible();
	void setVisible(bool visible);
	string getText();
	string getFunction();
	void tabChange(int tab);
	bool init(int x,int y,int width,int height ,string *image,string text,string function,SDL_Renderer *renderer, string *fonts, bool visible, int tab, int ages);
};
#endif