#include "DogeLib/Includes.h"

typedef struct{
	bool isBomb;
	bool hasFlag;
	bool isCleared;
	int num;
}Tile;

typedef struct{
	Length dimension;
	uint numBombs;
	Tile **tile;
}Board;

Board newBoard(void)
{
	int value[3] = {0};
	uint entry = 0;
	while(entry < 3){
		Ticks frameStart = getTicks();
		clear();



		draw();
		//events(frameStart + TPF);
	}
}

void freeBoard(Board board)
{
	if(board.tile != NULL)
	{
		for(uint x = 0; x < board.dimension.x; x++){
			free(board.tile[x]);
		}
		free(board.tile);
		board.tile = NULL;
	}
	board.dimension = (Length){0,0};
	board.numBombs = 0;
}

bool winCondition(const Board board)
{
	if(board.tile == NULL)
		return true;
	return false;
}

void onKey(Event event)
{
	printf(
		"Key (%s) %s\n",
		SDL_GetKeyName(event.key.keysym.sym),
		event.key.state == SDL_PRESSED?"PRESSED":"RELEASED"
	);
}

void onClick(Event event)
{
	char b = '?';
	switch(event.button.button){
		case SDL_BUTTON_LEFT:
			b = 'L';
			break;
		case SDL_BUTTON_MIDDLE:
			b = 'M';
			break;
		case SDL_BUTTON_RIGHT:
			b = 'R';
			break;
		case SDL_BUTTON_X1:
			b = '1';
			break;
		case SDL_BUTTON_X2:
			b = '2';
			break;
	}
	printf(
		"Mouse (%c) %s at (%3d,%3d)\n",
		b,
		event.button.state == SDL_PRESSED?"PRESSED":"RELEASED",
		event.button.x,
		event.button.y
	);
}

int main(int argc, char const *argv[])
{
	const Length window = {800, 600};
	init(window);
	// Board board = {
	// 	.dimension = (Length){0, 0};
	// 	.numBombs = 0;
	// 	.tile = NULL;
	// };

	EventHandler *handlerList = NULL;
	handlerList = registerEvent(handlerList, SDL_KEYUP, onKey);
	handlerList = registerEvent(handlerList, SDL_KEYDOWN, onKey);
	handlerList = registerEvent(handlerList, SDL_MOUSEBUTTONDOWN, onClick);
	handlerList = registerEvent(handlerList, SDL_MOUSEBUTTONUP, onClick);

	while(1){
		Ticks frameStart = getTicks();
		clear();

		draw();
		events(frameStart + TPF, handlerList);
	}
	return 0;
}
