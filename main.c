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
	printf("Tiles %4u\n", board.numTile);
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
	const Coord wpos = coordMul(tpos, board.scale);
	const Coord npos = coordShift(wpos, DIR_R, (outline*5)/2);
	char text[2] = " ";
	setFontSize(board.scale);

	setRGB(0xFF, 0xFF, 0xFF);
	fillSquareCoord(wpos, board.scale);
	setRGB(0x7B, 0x7B, 0x7B);
	fillSquareCoord(coordAdd(wpos, outline),board.scale-outline);
	setRGB(0xBD, 0xBD, 0xBD);
	if(!board.tile[tpos.x][tpos.y].isClicked){
		fillSquareCoord(coordAdd(wpos, outline),board.scale-2*outline);
	}
	if(board.tile[tpos.x][tpos.y].mark > 0){
		setFontColor(PINK);
		intToStr(board.tile[tpos.x][tpos.y].mark, text);
	}else{
		switch(board.tile[tpos.x][tpos.y].mark){
		case M_QUES:
			setFontColor(BLACK);
			strcpy(text,  "?");
			break;
		case M_FLAG:
			setFontColor(RED);
			strcpy(text,  "F");
			break;
		default:
			break;
		}
	}
	drawTextCoord(npos, text);
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
	printf("calcNum at (%3u,%3u)\n", pos.x, pos.y);
	for(uint i = 0; i < 4; i++){
		const Coord d1 = coordShift(pos, i, 1);
		if(coordInRangePair(
			d1,(RangePair){(Range){0,board.len.x},(Range){0,board.len.y}}
		)){count += board.tile[d1.x][d1.y].isBomb;}

		const Coord d2 = coordShift(coordShift(pos, i, 1), dirROR(i), 1);
		if(coordInRangePair(
			d2,(RangePair){(Range){0,board.len.x},(Range){0,board.len.y}}
		)){count += board.tile[d2.x][d2.y].isBomb;}
		printf(
			"\ti=%d\td1=(%3u,%3u)\td2=(%3u,%3u)\n",
			i,
			d1.x,
			d1.y,
			d2.x,
			d2.y
		);
	}
	putchar('\n');
	return count;
}

// args
// xlen ylen nummines scale
int main(int argc, char const *argv[])
{
	char buffer[40] = {0};
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
				board.numTile--;
				board.tile[tpos.x][tpos.y].isClicked = true;
				board.tile[tpos.x][tpos.y].mark = calcNum(board, tpos);
				printBoardBombs(board);
			}
		}

		if(mouseState(MOUSE_R)){

			setFontSize(board.scale);
			setFontColor(BLUE);
			sprintf(buffer, "(%3u,%3u)", mouse.pos.x, mouse.pos.y);
			drawTextCoord(coordShift(mouse.pos,DIR_U,board.scale*2),buffer);
			sprintf(buffer, "(%3u,%3u)", tpos.x, tpos.y);
			drawTextCoord(coordShift(mouse.pos,DIR_U,board.scale),buffer);
		}

		draw();
		events(frameStart + TPF);
	}
	return 0;
}
