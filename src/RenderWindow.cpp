#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string.h>

#include "RenderWindow.h"
#include "Entity.h"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		std::cout << "Window failed to init. Error: " << SDL_GetError() << std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if (texture == NULL)
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << std::endl;

	return texture;
}

void RenderWindow::cleanUp()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void RenderWindow::clear()
{
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity)
{
	SDL_Rect src; 
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;

	SDL_Rect dst;
	dst.x = p_entity.getPos().x + (p_entity.getCurrentFrame().w - p_entity.getCurrentFrame().w*p_entity.getScale().x)/2;
	dst.y = p_entity.getPos().y + (p_entity.getCurrentFrame().h - p_entity.getCurrentFrame().h*p_entity.getScale().y)/2;
	dst.w = p_entity.getCurrentFrame().w*p_entity.getScale().x;
	dst.h = p_entity.getCurrentFrame().h*p_entity.getScale().y;

	SDL_RenderCopyEx(renderer, p_entity.getTex(), &src, &dst, p_entity.getAngle(), 0, SDL_FLIP_NONE);
}

void RenderWindow::render(int x, int y, SDL_Texture* p_tex)
{
	SDL_Rect src; 
	src.x = 0;
	src. y = 0;
	src.w;
	src.h;

	SDL_QueryTexture(p_tex, NULL, NULL, &src.w, &src.h);

	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;

	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::renderBG(int x, int y, SDL_Texture* p_tex)
{
	SDL_RenderCopy(renderer, p_tex, NULL, NULL);
}

void RenderWindow::render(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = p_x;
		dst.y = p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_FreeSurface(surfaceMessage);
	 	SDL_DestroyTexture(message);
}

void RenderWindow::renderCenter(float p_x, float p_y, const char* p_text, TTF_Font* font, SDL_Color textColor)
{
		SDL_Surface* surfaceMessage = TTF_RenderText_Blended( font, p_text, textColor);
		SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

		SDL_Rect src;
		src.x = 0;
		src.y = 0;
		src.w = surfaceMessage->w;
		src.h = surfaceMessage->h; 

		SDL_Rect dst;
		dst.x = Width/2 - src.w/2 + p_x;
		dst.y = Height/2 - src.h/2 + p_y;
		dst.w = src.w;
		dst.h = src.h;

		SDL_RenderCopy(renderer, message, &src, &dst);
		SDL_FreeSurface(surfaceMessage);
		SDL_DestroyTexture(message);
}

void RenderWindow::display()
{
	SDL_RenderPresent(renderer);
}

void RenderWindow::renderText(int p_x, int p_y, std::string text, TTF_Font* font, SDL_Color textColor)
{
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    SDL_Rect dsRect = {p_x, p_y, textSurface->w, textSurface->h};
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_RenderCopy(renderer, textTexture, nullptr, &dsRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void RenderWindow::renderCard(Card& p_card)
{
	SDL_Rect dst;
	SDL_QueryTexture(p_card.cardTexture, NULL, NULL, &dst.w, &dst.h);
	dst.w /= 1.72;
	dst.h /= 1.632;
	dst.x = p_card.pos.x;
	dst.y = p_card.pos.y;
	SDL_RenderCopy(renderer, p_card.cardTexture, NULL, &dst);
}

void RenderWindow::renderWaitCard(Card& p_card)
{
	SDL_Rect dst;
	SDL_QueryTexture(p_card.cardTexture, NULL, NULL, &dst.w, &dst.h);
	dst.w /= (1.72);
	dst.h /= (1.632*2);
	dst.x = p_card.pos.x;
	dst.y = p_card.pos.y;

	SDL_Rect src;
	src.x = 6;
	src.y = 82;
	src.w = 144 ;
	src.h = 116;

	SDL_RenderCopy(renderer, p_card.cardTexture, &src, &dst);
}

void RenderWindow::renderCardStat(Card& p_card, TTF_Font* font)
{
	SDL_Color white = { 255, 255, 255 };
	renderText(p_card.pos.x+card_w/2-15, p_card.pos.y+card_h, std::to_string(p_card.ATK) + "/" + std::to_string(p_card.HP), font, white); // 15 fix stat to middle card
}

void RenderWindow::renderCardRip(Card& p_card, int cnt)
{
	const int cardRipFrame = 13;
	const int sprite_w = 123;
	const int sprite_h = 182;
	SDL_Rect gSpriteClips[ cardRipFrame ];
	SDL_Texture* spriteSheet = loadTexture("res/gfx/cardripFX.png");
	//Set sprite clips
	gSpriteClips[ 0 ].x =  18;
	gSpriteClips[ 0 ].y =  13;
	gSpriteClips[ 0 ].w = sprite_w;
	gSpriteClips[ 0 ].h = sprite_h;

	gSpriteClips[ 1 ].x =  143;
	gSpriteClips[ 1 ].y =   13;
	gSpriteClips[ 1 ].w = sprite_w;
	gSpriteClips[ 1 ].h = sprite_h;
	
	gSpriteClips[ 2 ].x = 268;
	gSpriteClips[ 2 ].y =  13;
	gSpriteClips[ 2 ].w = sprite_w;
	gSpriteClips[ 2 ].h = sprite_h;

	gSpriteClips[ 3 ].x = 393;
	gSpriteClips[ 3 ].y =  13;
	gSpriteClips[ 3 ].w = sprite_w;;
	gSpriteClips[ 3 ].h = sprite_h;

	gSpriteClips[ 4 ].x = 509;
	gSpriteClips[ 4 ].y =  13;
	gSpriteClips[ 4 ].w = sprite_w;
	gSpriteClips[ 4 ].h = sprite_h;

	gSpriteClips[ 5 ].x = 630;
	gSpriteClips[ 5 ].y =  13;
	gSpriteClips[ 5 ].w = sprite_w;
	gSpriteClips[ 5 ].h = sprite_h;
	
	gSpriteClips[ 6 ].x = 750;
	gSpriteClips[ 6 ].y =  13;
	gSpriteClips[ 6 ].w = sprite_w;
	gSpriteClips[ 6 ].h = sprite_h;

	gSpriteClips[ 7 ].x =  17;
	gSpriteClips[ 7 ].y = 238;
	gSpriteClips[ 7 ].w = sprite_w;
	gSpriteClips[ 7 ].h = sprite_h;
	
	gSpriteClips[ 8 ].x =  134;
	gSpriteClips[ 8 ].y =  238;
	gSpriteClips[ 8 ].w = sprite_w;
	gSpriteClips[ 8 ].h = sprite_h;

	gSpriteClips[ 9 ].x = 266;
	gSpriteClips[ 9 ].y = 238;
	gSpriteClips[ 9 ].w = sprite_w;
	gSpriteClips[ 9 ].h = sprite_h;
	
	gSpriteClips[ 10 ].x = 386;
	gSpriteClips[ 10 ].y = 238;
	gSpriteClips[ 10 ].w = sprite_w;
	gSpriteClips[ 10 ].h = sprite_h;

	gSpriteClips[ 11 ].x = 537;
	gSpriteClips[ 11 ].y = 238;
	gSpriteClips[ 11 ].w = sprite_w;
	gSpriteClips[ 11 ].h = sprite_h;

	gSpriteClips[ 12 ].x = 689;
	gSpriteClips[ 12 ].y = 238;
	gSpriteClips[ 12 ].w = sprite_w;
	gSpriteClips[ 12 ].h = sprite_h;

	SDL_Rect dst;
	dst.w = sprite_w;
	dst.h = sprite_h;
	dst.x = p_card.target.x - 16; // 18 = (sprite_w - card_w) / 2;
	dst.y = p_card.target.y - 28;

	SDL_RenderCopy(renderer, spriteSheet, &gSpriteClips[cnt], &dst);
}

void RenderWindow::renderPointer(Pointer& p_pointer)
{
	SDL_Rect dst;
	dst.w = 19;
	dst.h = 18;
	dst.x = p_pointer.pos.x; // 23 = distance between each
	dst.y = p_pointer.pos.y;

	SDL_RenderCopy(renderer, p_pointer.pointerTex, NULL, &dst);
}

void RenderWindow::renderDmgTaken(int dmgA, int dmgB, TTF_Font* font)
{
	SDL_Color white = { 255, 255, 255 };
	renderText(161, 403, std::to_string(dmgA), font, white);
	renderText(334, 403, std::to_string(dmgB), font, white);
}

void RenderWindow::renderDeckSize(std::vector<Card>& Deck, TTF_Font* font)
{
	SDL_Color white = { 255, 255, 255 };
	renderText(1084, 480, std::to_string(Deck.size()), font ,white);
}