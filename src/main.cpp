#include "Game.h"

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
	writeFile();
	TTF_CloseFont(font32);
	TTF_CloseFont(font24);
	TTF_CloseFont(yafont32);
	TTF_CloseFont(yafont24);
	TTF_CloseFont(pixelfont24);
	SDL_Quit();
	TTF_Quit();
	return 0;
}