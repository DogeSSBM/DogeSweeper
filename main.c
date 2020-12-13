#include "Includes.h"

typedef enum{
	M_FLAG= -2,
	M_QUES,
	M_NONE=  0,
	M_NUM1,
	M_NUM2,
	M_NUM3,
	M_NUM4,
	M_NUM5,
	M_NUM6,
	M_NUM7,
	M_NUM8,
	M_BOMB
}TileMark;

typedef struct{
	bool isBomb;
	bool isClicked;
	TileMark mark;
}Tile;

typedef struct{
	Length len;
	uint scale;
	uint numMines;
	uint numTile;
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

void plantMines(Board *board, const Coord safepos)
{
	for(uint i = 0; i < board->len.x; i++)
		memset(board->tile[i], 0, sizeof(Tile)*board->len.y);

	board->numTile = (board->len.x * board->len.y) - board->numMines;
	for(uint i = 0; i < board->numMines;){
		const Coord tpos = {
			randRange(0,board->len.x),
			randRange(0,board->len.x)
		};
		if(!board->tile[tpos.x][tpos.y].isBomb){
			board->tile[tpos.x][tpos.y].isBomb = true;
			i++;
		}
	}
}

void printBoardBombs(const Board board)
{
	printf("Dimensions (%3u, %3u)\n", board.len.x, board.len.y);
	printf("Bombs %4u\n", board.numMines);
	for(uint y = 0; y < board.len.y; y++){
		for(uint x = 0; x < board.len.x; x++){
			if(board.tile[x][y].isClicked)
				putchar(board.tile[x][y].mark>0?'0'+board.tile[x][y].mark:'-');
			else
				putchar(board.tile[x][y].isBomb?'B':'#');
			putchar(' ');
		}
		putchar('\n');
	}
	putchar('\n');
}

void drawTile(const Board board, const Coord tpos)
{
	const uint outline = board.scale/10;
	const Coord wpos = {tpos.x*board.scale, tpos.y*board.scale};
	const Coord mpos = coordAdd(wpos, board.scale);
	char text[2] = " ";
	setFontSize(board.scale/2);

	setRGB(0xFF, 0xFF, 0xFF);
	fillSquareCoord(wpos, board.scale);
	setRGB(0x7B, 0x7B, 0x7B);
	fillSquareCoord(coordAdd(wpos, outline),board.scale-outline);
	setRGB(0xBD, 0xBD, 0xBD);
	if(!board.tile[tpos.x][tpos.y].isClicked){
		fillSquareCoord(coordAdd(wpos, outline),board.scale-2*outline);
		if(board.tile[tpos.x][tpos.y].mark > 0){
			setFontColor(PINK);
			text[0] = board.tile[tpos.x][tpos.y].mark+'0';
		}
	}else{
		switch(board.tile[tpos.x][tpos.y].mark){
		case M_QUES:
			setFontColor(BLACK);
			text[0] = '?';
			break;
		case M_FLAG:
			setFontColor(RED);
			text[0] = 'F';
			break;
		default:
			break;
		}
	}
	drawTextCoord(mpos, text);
}

void drawBoard(const Board board)
{
	for(uint y = 0; y < board.len.y; y++){
		for(uint x = 0; x < board.len.x; x++){
			drawTile(board, (Coord){x,y});
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

uint calcNum(const Board board, const Coord pos)
{
	uint count = 0;
	for(uint i = 0; i < 8; i++){
		const Coord n = coordShift(
			i<4 ? pos : coordShift(pos, dirROR(i%4), 1),
			i,
			1
		);
		if(n.x >= 0 && n.y >= 0 &&
		n.x < board.len.x && n.y < board.len.y){
			count += board.tile[n.x][n.y].isBomb;
		}
	}
	return count;
}

// args
// xlen ylen nummines scale
int main(int argc, char const *argv[])
{
	Board board = {
		.len = (Length){30, 16},
		.scale = 40,
		.numMines = 99,
		.numTile = 0,
		.tile = NULL
	};

	switch (argc) {
		case 5:
			board.scale = clamp(strToInt(argv[4]),1,200);
		case 4:
			board.numMines = clamp(strToInt(argv[3]),1,200);
		case 3:
			board.len.x = clamp(strToInt(argv[2]),1,200);
			board.len.y = clamp(strToInt(argv[1]),1,200);
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
				if(board.tile[tpos.x][tpos.y].isBomb||board.numTile==0)
					plantMines(&board, tpos);
				board.tile[tpos.x][tpos.y].isClicked = true;
				board.tile[tpos.x][tpos.y].mark = calcNum(board, tpos);
				printBoardBombs(board);
			}
		}

		draw();
		events(frameStart + TPF);
	}
	return 0;
}
