#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Entity.h"
#include "Cards.h"

const int Height = 700;
const int Width = 1245;

class RenderWindow 
{
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Renderer* renderer;
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(Entity& p_entity);
	void render(int x, int y, SDL_Texture* p_tex);
	void renderBG(int x, int y, SDL_Texture* p_tex);
	void render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
	void renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor);
	void display();
	void renderText(int p_x, int p_y, std::string text, TTF_Font* font, SDL_Color textColor);
	void renderCard(Card& p_card);
	void renderCardStat(Card& p_card, TTF_Font* font);
private:
	SDL_Window* window;
};