#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <Windows.h> // KEY PRESS MOUSE LISTENER NECESSITY
#include <winbgi.cpp>
#include <graphics.h>
#include <thread>
#include "Pixel.h"
#include "VirtualKeyList.h"
#include "Minion.h"
#include "Token.h"
#include "Spell.h"
using namespace std;


#pragma region ENUMS
enum Player {Black, Red, NA};
enum Column {One, Two, Three, Four, Five, Six, Seven, Eight};
#pragma endregion
#pragma region PROTOTYPE
bool ActionListener(int);
void gr_Start(int&, int&, int&); 
void game(); // Thread 1
void Listener(); // Thread 2
void INTRO();
float distance(int, int, int, int);
void draw(int, int, int, int);
void boundedCircle(int, int, int, int, int, int, int, int);
void topCircle(int, int, int, int, int, int, int, int);
string toUpper(string);
unsigned char* ReadBMP(char*);
void CREATE(string);
void cpyCard(const int[][IMGWidth]);
void drawCard(int, int);
void drawMinion(Minion,int, int);
void drawSpell(Spell, int, int);
void drawToken(Token, int, int);
#pragma endregion
#pragma region GLOBAL_VARS
const int NUMVECTOR = 8; /// LoadBMP VECTORSIZE
const int UNIT = 100; /// Might be able to set as const should never change
const int SPEED = 16;
const int NUMCOL = 8;
const int NUMROW = 8;
const int BOARDCOLOR = YELLOW;
const int BACKGROUND = WHITE;
const int OUTLINE = BLACK;
bool isRunning = true;
bool isName = false;
Player Turn = Red;
Player WINNER = NA;
/// Global basic ints 
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
int BLANKCARD[IMGHeight][IMGWidth];
int w, h; /// BMP using for passing through functions 
#pragma endregion
#pragma region RECORDS
struct KeyState {
	int VirtualKey;
	bool isAlpha;
	bool isNumeric;
	bool isMove;
	bool isClicked;
	string Significance;
	bool isPressed;
	POINT Cursor;
	void reset() {
		Significance = "";
		isPressed = false;
		isClicked = false;
		isAlpha = false;
		isNumeric = false;
		VirtualKey = 0;
		Cursor = POINT();
	}
	void resetKey() {
		Significance = "";
		isPressed = false;
		isAlpha = false;
		isNumeric = false;
		VirtualKey = 0;
	}
}GLOBAL;
struct RGB {
	int r;
	int g;
	int b;
	int colorVal;
	void init(int R, int G, int B) {
		r = R;
		g = G;
		b = B;
	}
	void init(int R, int G, int B, int c) {
		r = R;
		g = G;
		b = B;
		colorVal = c;
	}
	bool isEqual(RGB t) {
		return r == t.r && g == t.g && b == t.b;
	}
};
struct CONNECT4BOARD {
	vector<vector<Player>> BOARD;
	vector <Player> Generic;
	void gamesetup() {
		Generic.clear();
		BOARD.clear();
		// Force the board to be empty
		for (int i = 0; i < NUMCOL; i++){
			BOARD.push_back(Generic);
		}

		clearBoard();
	}
	void clearBoard() {
		
		int y = UNIT;
		setcolor(BOARDCOLOR);
		bar(0, 0, maxX, maxY);
		setcolor(OUTLINE);
		int x = (maxX - 800) / 2;
		for (int i = 0; i < 8; i++)
			for (int j = 1; j < 7; j++) {
				rectangle(x + i*UNIT, y + j*UNIT, x + (i + 1)*UNIT, y + (j + 1)*UNIT);
				draw(x + 50 + i*UNIT, y + 50 + j*UNIT, 45, BACKGROUND);
			}

	}
	void drop(Column selected, Player piece) {
		if (ROW(selected) < 6) {
			drawDrop(selected, piece);
		}

	}
	void drawDrop(Column selected, Player piece) {
		int y = UNIT + 50;
		int x = (maxX - 800) / 2;
		x += 50 + (int)(selected) * UNIT;
		for (int i = y; i < 240; i += SPEED) {
			topCircle(x, i, 45, PIECE(piece), x, y, x, y + 100);
			Sleep(50);
			//setcolor(BACKGROUND);
			topCircle(x, i, 45, BOARDCOLOR, x, y, x, y + 100);
			draw(x, y + 100, 45, BACKGROUND);
		}
		y += 100;
		int limit = (7 - ROW(selected))*UNIT + 50;
		int radius = 45;
		for (int i = 240; i < limit; i += SPEED) {
			boundedCircle(x, i, radius, PIECE(piece), x, y, x, y + 100);
			Sleep(50);
			//setcolor(BLACK);
			boundedCircle(x, i, radius, BACKGROUND, x, y, x, y + 100);
			/*
			draw(x, y, 45, BLACK);
			draw(x, y+100, 45, BLACK);
			*/
			if (i > y + radius * 2 && y <= limit - 200)
				y += 100;
		}
		cout << "limit : " << limit << " y : " << y + 100 << endl;
		draw(x, limit, radius, PIECE(piece));
		pushROW(selected, piece);
	}
	int PIECE(Player piece) {
		if (piece == Red){
			return RED;
		}
		return BLACK;
	}
	int ROW(Column selected) {
		return (int)BOARD[(int)selected].size();
	}
	void pushROW(Column selected, Player piece) {
		BOARD[(int)selected].push_back(piece);
	}
	bool win(Player piece) {
		bool win = false;
		for (int i = 0; i < 8; i++) {
			if ((int)BOARD[i].size() > 0 && BOARD[i].at((int)BOARD[i].size() - 1) == piece){
				int index = (int)BOARD[i].size() - 1;
				// RIGHT 
				if ((i + 3) < 8) {
					win = true;
					for (int j = 1; j < 4 && win; j++) {
						win = false;
						if ((int)BOARD[i + j].size() > index) {
							if (BOARD[i + j].at(index) == piece) {
								win = true;
							}
						}
					}
					if (win) {
						return win;
					}
				}
				// DOWN
				if (index > 2) {
					win = true;
					for (int j = 1; j < 4 && win; j++) {
						win = false;
						if (BOARD[i].at(index-j) == piece) {
								win = true;
						}
					}
					if (win) {
						return win;
					}
					// DOWN RIGHT
					if ((i + 3) < 8) {
						win = true;
						for (int j = 1; j < 4 && win; j++) {
							win = false;
							if ((int)BOARD[i + j].size() > (index - j) && BOARD[i + j].at(index - j) == piece) {
								win = true;
							}
						}
						if (win) {
							return win;
						}
					}
				}
				// UP RIGHT
				if ((i + 3) < 8 && index < 3) {
					win = true;
					for (int j = 1; j < 4 && win; j++) {
						win = false;
						if ((int)BOARD[i + j].size() > (index + j) && BOARD[i+j].at(index + j) == piece) {
							win = true;
						}
					}
					if (win) {
						return win;
					}
				}
				
			}
		}
		return win;
	}
}GAME;
/*
0  BLACK
1  BLUE
2  GREEN
3  CYAN
4  RED
5  MAGENTA
6  BROWN
7  LIGHTGRAY
8  DARKGRAY
9  LIGHTBLUE
10 LIGHTGREEN
11 LIGHTCYAN
12 LIGHTRED
13 LIGHTMAGENTA
14 YELLOW
15 WHITE
*/
/// Color Helper
#pragma endregion
#pragma region RECORD_VECTOR(CONTAINERS)
vector <RGB> colors(NUMVECTOR);
#pragma endregion