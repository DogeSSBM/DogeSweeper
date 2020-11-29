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
		events(frameStart + TPF);
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

int main(int argc, char const *argv[])
{
	const Length window = {800, 600};
	init(window);
	Board board = {
		.dimension = (Length){0, 0};
		.numBombs = 0;
		.tile = NULL;
	};

	while(1){
		Ticks frameStart = getTicks();
		clear();
		if(winCondition(board)){
			freeBoard(board);
			board = newBoard();
		}


		draw();
		events(frameStart + TPF);
	}
	return 0;
}
