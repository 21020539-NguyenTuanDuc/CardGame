#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>

#include "Entity.h"
#include "Cards.h"
#include "Pointer.h"
#include "Button.h"


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
	void renderWaitCard(Card& p_card);
	void renderCardStat(Card& p_card, TTF_Font* font);
	void renderCardRip(Card& p_card, int cnt);
	void renderPointer(Pointer& p_pointer);
	void renderDmgTaken(int dmgA, int dmgB, TTF_Font* font);
	void renderDeckSize(std::vector<Card>& Deck, TTF_Font* font);
	void renderSummonPenalty(int &sumPen, TTF_Font* font);
	void renderBigCard(Card& bigCard);
	void renderSoundIcon(SDL_Texture* noteON, SDL_Texture* speakerON);
	void renderMuteNote(bool musicFX);
	void renderMuteSpeaker(bool soundFX);
	void renderButtonLock(Button& tempButton, bool levelLocked, SDL_Texture* Lock);
private:
	SDL_Window* window;
};