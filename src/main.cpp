#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include<iostream>
#include<vector>

#include "RenderWindow.h"
RenderWindow window("CardGame", Width, Height);

#include "Cards.h"
#include "BaseCardType.h"
#include "Button.h"
#include "Slot.h"
std::vector<Slot> S = {
    Slot(400, 328),
    Slot(515, 328),
    Slot(630, 328),
    Slot(744, 328)
};

int Mouse_x, Mouse_y;

bool summon(Slot& sSlot, Card& sCard, SDL_Event* e)
{
	int nonEmptySlot = 0;
	for(int m=0;m<S.size();m++)
	{
		if(!S[m].isEmpty) nonEmptySlot++;
	}
	if(nonEmptySlot < sCard.Cost) return false;
	else
	{
		if(sSlot.isEmpty == false) window.renderCardRip(sSlot.sCard);
		sSlot.sCard = sCard;
		sSlot.sCard.target.x = sSlot.pos.x;
		sSlot.sCard.target.y = sSlot.pos.y;;		
		return true;
	}
}

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "HEY.. SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_init has failed. Error: " << SDL_GetError() << std::endl;
	if (!(TTF_Init()))
		std::cout << "TTF_init has failed. Error: " << SDL_GetError() << std::endl;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	return true;
}

bool SDLinit = init();

SDL_Texture* IGmap = window.loadTexture("res/gfx/IGmap_Resized.png");
SDL_Texture* IGtitleScreen = window.loadTexture("res/gfx/IGtitleScreen.png");

Mix_Chunk* advanceTitleScreen = Mix_LoadWAV("res/sfx/titlescreenplay.wav");
Mix_Chunk* click = Mix_LoadWAV("res/sfx/mouseclick.mp3");
Mix_Chunk* titleScreenMusic = Mix_LoadWAV("res/sfx/titleScreenMusic.mp3");
Mix_Chunk* ErrorSFX = Mix_LoadWAV("res/sfx/Error.mp3");

SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };

TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font24 = TTF_OpenFont("res/font/font.ttf", 24);
TTF_Font* yafont32 = TTF_OpenFont("res/font/yafont.ttf", 32);
TTF_Font* yafont24 = TTF_OpenFont("res/font/yafont.ttf", 24);
TTF_Font* pixelfont24 = TTF_OpenFont("res/font/pixelfont.ttf", 24);


bool gameRunning = true;

bool title = false;
bool advanceTitle = false;

SDL_Event event;
SDL_Event mouseEvent;

int state = 1; //0 = title screen, 1 = game, 2 = end screen

Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;

void graphic(){}
void titleScreen()
{
	// if(SDL_GetTicks64() < 5000)
	// {
	// 	if(!title){
	// 		Mix_PlayChannel(-1, titleScreenMusic, 5);
	// 		title = true;
	// 	}
	// 	//Get our controls and events
	// 	while (SDL_PollEvent(&event))
	// 	{
	// 		switch(event.type)
	// 		{
	// 		case SDL_QUIT:
	// 			gameRunning = false;
	// 			break;
	// 		}
	// 	}
	// 	window.clear();
	// 	window.renderCenter(0, 0, "NguyenTuanDuc Project", yafont32, white);
	// 	window.display();
	// }
	// else
	// {
	// 	if(!advanceTitle)
	// 	{
	// 		Mix_PlayChannel(-1, advanceTitleScreen, 0);
	// 		advanceTitle = true;
	// 	}

	// 	while (SDL_PollEvent(&event))
	// 	{
	// 		switch(event.type)
	// 		{
	// 		case SDL_QUIT:
	// 			gameRunning = false;
	// 			break;
	// 		case SDL_MOUSEBUTTONDOWN:
	// 			Mix_PlayChannel(-1, advanceTitleScreen, 0);
	// 			state = 1;
	// 			break;
	// 		case SDL_KEYDOWN:
	// 			Mix_PlayChannel(-1, advanceTitleScreen, 0);
	// 			state = 1;
	// 			break;	
	// 		}
	// 	}

	// 	window.clear();
	// 	window.renderBG(0, 0, IGtitleScreen);
	// 	window.display();
	// }
}

void update()
{
	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );
	
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			SDL_GetMouseState(&Mouse_x, &Mouse_y);
			for(int i=0;i<Hand.size();i++)
			{
				if(Hand[i].isUp)
				{
					for(int j=0;j<S.size();j++)
					if(Mouse_x >= S[j].pos.x && Mouse_x <= S[j].pos.x + S[j].w && Mouse_y >= S[j].pos.y && Mouse_y <= S[j].pos.y + S[j].h)
					{
						if(summon(S[j], Hand[i], &mouseEvent))
						{
							S[j].isEmpty = false;
							Hand.erase(Hand.begin()+i);
						}
						else Mix_PlayChannel(-1, ErrorSFX, 0);
					}	
				}
				Hand[i].handleEvent(&event);
			}
			if(Mouse_x >= next.pos.x && Mouse_x <= next.pos.x + next.w && Mouse_y >= next.pos.y && Mouse_y <= next.pos.y + next.h)
			{
				Mix_PlayChannel(-1, advanceTitleScreen, 0);
				drawCard();
			}
		}
	}

	window.clear();
	window.renderBG(0, 0, IGmap);

	updateHand();

	for(int i=0;i<Hand.size();i++)
	{
		Hand[i].update(deltaTime);
		window.renderCard(Hand[i]);
	}
	
	for(int j=0;j<S.size();j++)
	{
		if(!S[j].isEmpty)
		{
			S[j].sCard.update(deltaTime);
			window.renderCard(S[j].sCard);
			window.renderCardStat(S[j].sCard, pixelfont24);
		}
	}

	window.display();
}

void game(){
	if(state == 0){
		titleScreen();
	}
	else{
		update();
		graphic();
	}
}


int main( int argc, char* args[] )
{
	shuffleDeck();
	getCard();
	while(gameRunning){
		game();
	}
	//Free resources and close SDL
	window.cleanUp();
	TTF_CloseFont(font32);
	TTF_CloseFont(font24);
	TTF_CloseFont(yafont32);
	TTF_CloseFont(yafont24);
	TTF_CloseFont(pixelfont24);
	SDL_Quit();
	TTF_Quit();
	return 0;
}