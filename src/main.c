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

Board* newBoard(const Length len, const uint numBombs)
{
	
}

int main(int argc, char const *argv[])
{
	const Length window = {800, 600};
	init(window);

	while(1){
		Ticks frameStart = getTicks();
		clear();



		draw();
		events(frameStart + TPF);
	}
	return 0;
}
