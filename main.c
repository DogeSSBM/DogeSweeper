#include "Includes.h"

typedef enum{M_NONE, M_FLAG, M_QUES}TileMark;

typedef struct{
	bool isBomb;
	bool isClicked;
	TileMark mark;
}Tile;

typedef struct{
	Length len;
	uint scale;
	uint numMines;
	uint clearedTiles;
	uint flaggedMines;
	Tile **tile;
}Board;

void fillBoard(Board *board)
{
	board->tile = malloc(sizeof(Tile*) * board->len.x);
	for(uint x = 0; x < board->len.x; x++){
		board->tile[x] = malloc(sizeof(Tile) * board->len.y);
		memset(board->tile[x], 0, sizeof(Tile) * board->len.y);
	}
}

void printBoardBombs(Board board)
{
	printf("Dimensions (%3u, %3u)\n", board.len.x, board.len.y);
	printf("Bombs %4u\n", board.numMines);
	for(uint y = 0; y < board.len.y; y++){
		for(uint x = 0; x < board.len.x; x++){
			putchar(board.tile[x][y].isBomb?'B':'-');
		}
		putchar('\n');
	}
	putchar('\n');
}

void drawBoard(Board board)
{
	const uint outline = board.scale/10;

	for(uint y = 0; y < board.len.y; y++){
		for(uint x = 0; x < board.len.x; x++){
			const Coord pos = {x*board.scale, y*board.scale};
			setRGB(0xFF, 0xFF, 0xFF);
			fillSquareCoord(pos, board.scale);
			setRGB(0x7B, 0x7B, 0x7B);
			fillSquareCoord(coordAdd(pos, outline),board.scale-outline);
			setRGB(0xBD, 0xBD, 0xBD);
			fillSquareCoord(coordAdd(pos, outline),board.scale-2*outline);
		}
	}

}

// window position coordinate to tile position coordinate
Coord tposAt(const Coord wpos, const Board board)
{
	return coordClamp(
		coordDiv(wpos, board.scale),
		(Range){0, board.len.x},
		(Range){0, board.len.y}
	);
}

// tile position coordinate to window position coordinate
Coord wposAt(const Coord tpos, const Board board)
{
	return coordClamp(
		coordMul(tpos, board.scale),
		(Range){0, board.len.x*board.scale},
		(Range){0, board.len.y*board.scale}
	);
}

// args
// xlen ylen nummines scale
int main(int argc, char const *argv[])
{
	Board board = {
		.len = (Length){30, 16},
		.scale = 40,
		.numMines = 99,
		.flaggedMines = 0,
		.tile = NULL
	};

	switch (argc) {
		case 5:
			board.scale = wrap(strToInt(argv[4]),1,200);
		case 4:
			board.numMines = wrap(strToInt(argv[3]),1,200);
		case 3:
			board.len.x = wrap(strToInt(argv[2]),1,200);
			board.len.y = wrap(strToInt(argv[1]),1,200);
	}

	Length window = coordMul(board.len, board.scale);
	fillBoard(&board);
	init(window);

	while(1){
		Ticks frameStart = getTicks();
		clear();

		const Coord tpos = tposAt(mouse.pos, board);
		const Coord wtpos = coordOffset(mouse.pos,coordInv(coordMod(mouse.pos, board.scale)));

		const Tile tile = board.tile[tpos.x][tpos.y];

		drawBoard(board);
		if(!tile.isClicked){
			setColor(RED);
			fillBorderCoords(wtpos, (Length){board.scale, board.scale}, board.scale/10);
			if(mousePressed(MOUSE_L)){

			}
		}

		draw();
		events(frameStart + TPF);
	}
	return 0;
}
