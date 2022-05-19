#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<iostream>
#include<vector>
#include<fstream>

#include "RenderWindow.h"
RenderWindow window("CardGame", Width, Height);

#include "Cards.h"
#include "BaseCardType.h"
#include "Button.h"
#include "Slot.h"
#include "User.h"
User Player;
User Bot;

// Get mouse position
int Mouse_x, Mouse_y;

// Use sprites
Card destroyedCard;
bool isDestroyed = false;
int frameCnt = 0;

bool summon(Slot& sSlot, Card& sCard)
{
	int nonEmptySlot = 0;
	for(int m=0;m<S.size();m++)
	{
		if(!S[m].isEmpty) nonEmptySlot++;
	}
	if(nonEmptySlot < sCard.Cost) return false;
	else
	{
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

//Button
Button next(129, 161, 236, 90);
Button nextWin(289, 473, 214, 83);
Button retry(289, 473, 214, 83);
Button returnMenu(691, 473, 214, 83);
Button playNow(543, 388, 159, 54);
Button loadLevelButton(543, 458, 159, 54);
Button rules(543, 528, 159, 54);
Button quitMenu(543, 598, 159, 54);
Button Music(61, 163, 42, 42);
Button Speaker(61, 207, 42, 42);
Button menuBackInstruction(4, 69, 149, 57);
Button levelButton[10];
void initButton()
{
	for(int i=0;i<5;i++)
	{
		levelButton[i].pos.x = 30 + i*250;
		levelButton[i].pos.y = 277;
		levelButton[i].w = 200;
		levelButton[i].h = 200;
	}

	for(int i=5;i<10;i++)
	{
		levelButton[i].pos.x = 30 + i*250;
		levelButton[i].pos.y = 484;
		levelButton[i].w = 200;
		levelButton[i].h = 200;
	}
}
bool levelLock[10];
void initLevelLock()
{
	for(int i=0;i<10;i++)
	{
		levelLock[i] = false;
	}
}

// Texture
SDL_Texture* IGmap = window.loadTexture("res/gfx/IGmap.jpg");
SDL_Texture* IGtitleScreen = window.loadTexture("res/gfx/IGtitleScreen.png");
SDL_Texture* IGpointer = window.loadTexture("res/gfx/IGpointer.png");
SDL_Texture* levelWin = window.loadTexture("res/gfx/levelWin.jpg");
SDL_Texture* levelLose = window.loadTexture("res/gfx/levelLose.jpg");
SDL_Texture* IGmenu = window.loadTexture("res/gfx/IGmenu.jpg");
SDL_Texture* noteON = window.loadTexture("res/gfx/noteON.png");
SDL_Texture* speakerON = window.loadTexture("res/gfx/speakerON.png");
SDL_Texture* IGinstruction = window.loadTexture("res/gfx/IGinstruction.jpg");
SDL_Texture* IGlevelSelect = window.loadTexture("res/gfx/IGlevelSelect.jpg");

// Sound
Mix_Chunk* buttonClick = Mix_LoadWAV("res/sfx/buttonClick.wav");
Mix_Chunk* click = Mix_LoadWAV("res/sfx/click.mp3");
Mix_Chunk* ErrorSFX = Mix_LoadWAV("res/sfx/Error.mp3");
Mix_Chunk* switchSFX = Mix_LoadWAV("res/sfx/switch.mp3");
Mix_Chunk* battleSFX = Mix_LoadWAV("res/sfx/battle.wav");
Mix_Chunk* cardRip = Mix_LoadWAV("res/sfx/cardRip.wav");
Mix_Chunk* summonSFX = Mix_LoadWAV("res/sfx/summonSFX.mp3");
Mix_Chunk* winSFX = Mix_LoadWAV("res/sfx/winSFX.wav");
Mix_Chunk* loseSFX = Mix_LoadWAV("res/sfx/loseSFX.wav");
bool soundFX = true;

// Music
Mix_Music* BGmusic = Mix_LoadMUS("res/sfx/BGmusic.mp3");
bool musicFX = true;

// Color
SDL_Color white = { 255, 255, 255 };
SDL_Color black = { 0, 0, 0 };

// Font
TTF_Font* font32 = TTF_OpenFont("res/font/font.ttf", 32);
TTF_Font* font24 = TTF_OpenFont("res/font/font.ttf", 24);
TTF_Font* yafont32 = TTF_OpenFont("res/font/yafont.ttf", 32);
TTF_Font* yafont24 = TTF_OpenFont("res/font/yafont.ttf", 24);
TTF_Font* pixelfont24 = TTF_OpenFont("res/font/pixelfont.ttf", 24);

// Game running condition
bool gameRunning = true;

// Title screen
bool title = false;
bool advanceTitle = false;

SDL_Event event;

int state = 0; //0 = title screen, 1 = menu, 2 = game, 3 = levelWin, 4 = levelLose, 5 = instruction, 6 = level select

// Delta Time
Uint64 currentTick = SDL_GetPerformanceCounter();
Uint64 lastTick = 0;
double deltaTime = 0;
void deltaTimeCal()
{
	lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );
}

// level setting
int level = 0;
bool level_over = false;
int turn = 0;
bool turn_over = false;
int dmgGap = 0;
bool setWin = true;

// battle setting
bool S_battle_over[4] = {bool(false), bool(false), bool(false), bool(false)};
bool OS_battle_over[4] = {bool(false), bool(false), bool(false), bool(false)};

// pointer
Pointer p_pointer(237 , 261, IGpointer);

// penalty for summon
int sumPen = 0;

// big screen card
Card bigCard;

void graphic()
{
	window.clear();
	window.renderBG(0, 0, IGmap);
	window.renderSoundIcon(noteON, speakerON);
	window.renderMuteNote(musicFX);
	window.renderMuteSpeaker(soundFX);

	window.renderDmgTaken(Player.DmgTaken, Bot.DmgTaken, pixelfont24);
	window.renderDeckSize(Deck, pixelfont24);
	window.renderSummonPenalty(sumPen, pixelfont24);
	window.renderBigCard(bigCard);

	// hand card
	for(int i=0;i<Hand.size();i++)
	{
		window.renderCard(Hand[i]);
	}
	
	// destroy card sprite
	if(isDestroyed == true)
	{
		if(soundFX) Mix_PlayChannel(-1, cardRip, 0);
		if(frameCnt<=12)
		{
			window.renderCardRip(destroyedCard, frameCnt);
			frameCnt++;
		}
		else
		{
			frameCnt = 0;
			isDestroyed = false;
		}
	}

	// my card
	for(int j=0;j<S.size();j++)
	{
		if(!S[j].isEmpty)
		{
			window.renderCard(S[j].sCard);
			window.renderCardStat(S[j].sCard, pixelfont24);
		}
	}

	// opponent card
	for(int i=0;i<OSlot.size();i++)
	{
		if(!OSlot[i].isEmpty)
		{
			window.renderCard(OSlot[i].sCard);
			window.renderCardStat(OSlot[i].sCard, pixelfont24);
		}
	}

	// opponent wait card
	for(int i=0;i<OSlot.size();i++)
	{
		if(!OwaitSlot[i].isEmpty)
		{
			window.renderWaitCard(OwaitSlot[i].sCard);
		}
	}

	// pointer
	window.renderPointer(p_pointer);

	window.display();
}
void titleScreen()
{
	if(SDL_GetTicks64() < 4000)
	{
		if(!title){
			Mix_PlayMusic(BGmusic, -1);
			title = true;
		}
		//Get our controls and events
		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			}
		}
		window.clear();
		window.renderCenter(0, 0, "NguyenTuanDuc", yafont32, white);
		window.display();
	}
	else
	{
		if(!advanceTitle)
		{
			Mix_PlayChannel(-1, buttonClick, 0);
			advanceTitle = true;
		}

		while (SDL_PollEvent(&event))
		{
			switch(event.type)
			{
			case SDL_QUIT:
				gameRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				Mix_PlayChannel(-1, buttonClick, 0);
				state = 1;
				break;
			case SDL_KEYDOWN:
				Mix_PlayChannel(-1, buttonClick, 0);
				state = 1;
				break;	
			}
		}

		window.clear();
		window.renderBG(0, 0, IGtitleScreen);
		window.display();
	}
}

void update()
{
	deltaTimeCal();

	if(turn_over)
	{
		sumPen = 0;

		for(int i=0;i<4;i++)
		{
			if(!S_battle_over[i] && isDestroyed == false)
			{
				Slot::battle(S[i], OSlot[i], Player, Bot);
				S[i].battleUP(); // move up
				if(soundFX) Mix_PlayChannel(-1, battleSFX, 0);
				S_battle_over[i] = true;
				if(OSlot[i].sCard.HP <= 0 && OSlot[i].isEmpty == false)
				{
					OSlot[i].isEmpty = true;
					destroyedCard = OSlot[i].sCard;
					isDestroyed = true;
				}
			}
		}
		
		// check win
		if(Bot.DmgTaken - Player.DmgTaken >= 5)
		{
			level_over = true;
			setWin = true;
			if(soundFX) Mix_PlayChannel(-1, winSFX, 0);
		}
		if(Bot.DmgTaken - Player.DmgTaken <= -5)
		{
			level_over = true;
			setWin = false;
			if(soundFX) Mix_PlayChannel(-1, loseSFX, 0);
		}

		// push opponent card up
		if(isDestroyed == false && S[3].sCard.pos.y == S[3].pos.y && S[2].sCard.pos.y == S[2].pos.y && S[1].sCard.pos.y == S[1].pos.y && S[0].sCard.pos.y == S[0].pos.y)
		{
			for(int i=0;i<4;i++)
			{
				if(OwaitSlot[i].isEmpty && ORow[i].size()!=0 && isDestroyed == false)
				{
					OwaitSlot[i].sCard = ORow[i].front();
					ORow[i].erase(ORow[i].begin());
					OwaitSlot[i].sCard.target = OwaitSlot[i].pos;
					OwaitSlot[i].isEmpty = false;
				}
				if(OSlot[i].isEmpty && !OwaitSlot[i].isEmpty && isDestroyed == false)
				{
					OSlot[i].sCard = OwaitSlot[i].sCard;
					OSlot[i].sCard.target = OSlot[i].pos;
					OSlot[i].isEmpty = false;
					OwaitSlot[i].isEmpty = true;
				}
			}
		}

		if(S_battle_over[0] && S_battle_over[1] && S_battle_over[2] && S_battle_over[3])
		{
			for(int i=0;i<4;i++)
			{
				if(!OS_battle_over[i] && isDestroyed == false)
				{
					Slot::battle(OSlot[i], S[i], Bot, Player);
					OSlot[i].battleDOWN(); // move down
					if(soundFX) Mix_PlayChannel(-1, battleSFX, 0);
					OS_battle_over[i] = true;
					if(S[i].sCard.HP <= 0 && S[i].isEmpty == false)
					{
						S[i].isEmpty = true;
						destroyedCard = S[i].sCard;
						isDestroyed = true;
					}
				}
			}
		}

		if(S_battle_over[0] && S_battle_over[1] && S_battle_over[2] && S_battle_over[3] && OS_battle_over[0] && OS_battle_over[1] && OS_battle_over[2] && OS_battle_over[3])
		{
			turn_over = false;
			if(Hand.size()<10) drawCard();
			for(int i=0;i<4;i++)
			{
				S_battle_over[i] = false;
				OS_battle_over[i] = false;
			}
		}
	}

	if(dmgGap > Bot.DmgTaken - Player.DmgTaken)
	{
		dmgGap--;
		if(soundFX) Mix_PlayChannel(-1, switchSFX, 0);
	}
	if(dmgGap < Bot.DmgTaken - Player.DmgTaken)
	{
		dmgGap++;
		if(soundFX) Mix_PlayChannel(-1, switchSFX, 0);
	}
	p_pointer.updateTarget(dmgGap);
	p_pointer.update(deltaTime);
	if(dmgGap >= 5)
	{
		level_over = true;
		setWin = true;
		if(soundFX) Mix_PlayChannel(-1, winSFX, 0);
	}
	if(dmgGap <= -5)
	{
		level_over = true;
		setWin = false;
		if(soundFX) Mix_PlayChannel(-1, loseSFX, 0);
	}

	if(level_over == true && isDestroyed == false)
	{
		if(setWin == true) state = 3;
		else state = 4;
	}

	// push opponent card up
	for(int i=0;i<4;i++)
	{
		if(OwaitSlot[i].isEmpty && ORow[i].size()!=0)
		{
			OwaitSlot[i].sCard = ORow[i].front();
			ORow[i].erase(ORow[i].begin());
			OwaitSlot[i].sCard.target = OwaitSlot[i].pos;
			OwaitSlot[i].isEmpty = false;
		}
		if(OSlot[i].isEmpty && !OwaitSlot[i].isEmpty)
		{
			OSlot[i].sCard = OwaitSlot[i].sCard;
			OSlot[i].sCard.target = OSlot[i].pos;
			OSlot[i].isEmpty = false;
			OwaitSlot[i].isEmpty = true;
		}
	}

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
						if(summon(S[j], Hand[i]))
						{
							if(soundFX) Mix_PlayChannel(-1, summonSFX, 0);
							if(S[j].isEmpty == false) isDestroyed = true;
							S[j].isEmpty = false;
							destroyedCard = S[j].sCard;
							Hand.erase(Hand.begin()+i);
							Player.DmgTaken += S[j].sCard.Cost;
							Player.DmgTaken += sumPen;
							sumPen++;
						}
						else
						{
							if(soundFX) Mix_PlayChannel(-1, ErrorSFX, 0);
						}
					}	
				}
				if(Hand[i].handleEvent(&event)) if(soundFX) Mix_PlayChannel(-1, click, 0);
			}
			for(int i=0;i<Hand.size();i++)
			{
				Hand[i].handleBigScreenEvent(&event, bigCard);
			}
			for(int i=0;i<4;i++)
			{
				S[i].sCard.handleBigScreenEvent(&event, bigCard);
				OSlot[i].sCard.handleBigScreenEvent(&event, bigCard);
			}
			if(next.handleButtonEvent(&event) && turn_over == false)
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				turn_over = true;
				turn++;
			}
			if(Music.handleButtonEvent(&event))
			{
				if(musicFX == true)
				{
					musicFX = false;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
					Mix_PauseMusic();
				}
				else
				{
					musicFX = true;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
					Mix_ResumeMusic();
				}
			}
			if(Speaker.handleButtonEvent(&event))
			{
				if(soundFX == false)
				{					
					soundFX = true;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				}
				else soundFX = false;
			}
			break;
		}
	}

	updateHand();

	for(int i=0;i<Hand.size();i++)
	{
		Hand[i].update(deltaTime);
	}

	for(int j=0;j<S.size();j++)
	{
		if(!S[j].isEmpty)
		{
			S[j].sCard.update(deltaTime);
			S[j].slotUpdate();
		}
	}

	for(int i=0;i<OSlot.size();i++)
	{
		if(!OSlot[i].isEmpty)
		{
			OSlot[i].sCard.update(deltaTime);
			OSlot[i].slotUpdate();
		}
		if(!OwaitSlot[i].isEmpty)
		{
			OwaitSlot[i].sCard.update(deltaTime);
		}
	}

}

void levelWon()
{
	window.clear();
	window.renderBG(0, 0, levelWin);
	deltaTimeCal();
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(nextWin.handleButtonEvent(&event)) // need soundFX for button
			{
				for(int i=0;i<4;i++)
				{
					OwaitSlot[i].isEmpty = true;
					OSlot[i].isEmpty = true;
					S[i].isEmpty = true;
				}
				level++;
				loadLevel(level);
				shuffleDeck();
				getCard();
				Player.DmgTaken = 0;
				Bot.DmgTaken = 0;
				sumPen = 0;
				bigCard.cardTexture = NULL;
				turn = 0;
				level_over = false;
				state = 2;
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
			}
			if(returnMenu.handleButtonEvent(&event))
			{
				state = 1;
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
			}
		}
	}
	window.display();
}

void levelLost()
{
	window.clear();
	window.renderBG(0, 0, levelLose);
	deltaTimeCal();
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(nextWin.handleButtonEvent(&event)) // need soundFX for button
			{
				for(int i=0;i<4;i++)
				{
					OwaitSlot[i].isEmpty = true;
					OSlot[i].isEmpty = true;
					S[i].isEmpty = true;
				}
				loadLevel(level);
				shuffleDeck();
				getCard();
				Player.DmgTaken = 0;
				Bot.DmgTaken = 0;
				sumPen = 0;
				bigCard.cardTexture = NULL;
				turn = 0;
				level_over = false;
				state = 2;
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
			}
			if(returnMenu.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				state = 1;
			}
		}
	}
	window.display();
}

void menu()
{
	window.clear();
	window.renderBG(0, 0, IGmenu);
	deltaTimeCal();
	window.renderSoundIcon(noteON, speakerON);
	window.renderMuteNote(musicFX);
	window.renderMuteSpeaker(soundFX);

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(playNow.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				level = 0;
				for(int i=0;i<4;i++)
				{
					OwaitSlot[i].isEmpty = true;
					OSlot[i].isEmpty = true;
					S[i].isEmpty = true;
				}
				loadLevel(level);
				shuffleDeck();
				getCard();
				Player.DmgTaken = 0;
				Bot.DmgTaken = 0;
				sumPen = 0;
				bigCard.cardTexture = NULL;
				turn = 0;
				level_over = false;
				state = 2;
			}
			if(loadLevelButton.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				state = 6;
			}
			if(rules.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				state = 5;
			}
			if(quitMenu.handleButtonEvent(&event)) gameRunning = false;
			if(Music.handleButtonEvent(&event))
			{
				if(musicFX == true)
				{
					musicFX = false;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
					Mix_PauseMusic();
				}
				else
				{
					musicFX = true;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
					Mix_ResumeMusic();
				}
			}
			if(Speaker.handleButtonEvent(&event))
			{
				if(soundFX == false)
				{					
					soundFX = true;
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				}
				else soundFX = false;
			}
			break;
		}
	}
	window.display();
}

void instruction()
{
	window.clear();
	window.renderBG(0, 0, IGinstruction);

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(menuBackInstruction.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				state = 1;
			}
		}
	}

	window.display();
}

void levelSelect()
{
	window.clear();
	window.renderBG(0, 0, IGlevelSelect);
	deltaTimeCal();

	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
		case SDL_QUIT:
			gameRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(menuBackInstruction.handleButtonEvent(&event))
			{
				if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
				state = 1;
			}
			for(int i=0;i<10;i++)
			{
				if(levelButton[i].handleButtonEvent(&event) && !levelLock[i])
				{
					if(soundFX) Mix_PlayChannel(-1, buttonClick, 0);
					level = i;
					for(int i=0;i<4;i++)
					{
						OwaitSlot[i].isEmpty = true;
						OSlot[i].isEmpty = true;
						S[i].isEmpty = true;
					}
					loadLevel(level);
					shuffleDeck();
					getCard();
					Player.DmgTaken = 0;
					Bot.DmgTaken = 0;
					sumPen = 0;
					bigCard.cardTexture = NULL;
					turn = 0;
					level_over = false;
					state = 2;
				}
			}
		}
	}

	window.display();
}

void game()
{
	if(state == 0){
		titleScreen();
	}
	else if(state == 1)
	{
		menu();
	}
	else if(state == 2)
	{
		update();
		graphic();
	}
	else if(state == 3)
	{
		levelWon();
	}
	else if(state == 4)
	{
		levelLost();
	}
	else if(state == 5)
	{
		instruction();
	}
	else if(state == 6)
	{
		levelSelect();
	}
}


int main( int argc, char* args[] )
{
	initButton();
	initLevelLock();
	shuffleDeck();
	getCard();
	loadLevel(level);
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