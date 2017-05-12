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
enum AIMode{Easy, Medium, Hard};
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
bool WIN(Player, vector<vector<Player>>);
void printBoard(vector<vector<Player>>);
#pragma endregion
#pragma region GLOBAL_VARS
const int NUMVECTOR = 8; /// LoadBMP VECTORSIZE
const int UNIT = 100; /// Might be able to set as const should never change
const int SPEED = 16;
const int NUMCOL = 8;
const int NUMROW = 6;
const int BOARDCOLOR = YELLOW;
const int BACKGROUND = WHITE;
const int OUTLINE = BLACK;
bool isRunning = true;
bool isName = false;
Player Turn = Red;
Player WINNER = NA;
AIMode SETTING = Hard;
/// Global basic ints
int piecesOnBoard = 0;
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
struct AI {
	vector<vector<Player>> BOARD;
	vector<int>PARENTMOVE;
	vector<int>deepDive;
	vector<bool>WORTH;
	vector<vector<vector<Player>>> ABOMINATION;
	Player whoseTurn;
	int APROX[NUMCOL];
	int VALUE;
	Column init(vector<vector<Player>>FROM) {
		reset();
		cpy(BOARD, FROM);
		depthSearch();
		cout << (int)WORTH.size() << " : Data Points 5 itterations Deep Dive \n";
		cout << "BOARD Post depthSearch() : \n";
		cout << "**********************\n";
		printBoard();
		cout << "**********************\n";
		return (Column) getVALUE();
	}
	void reset() {
		for (int i = 0; i < NUMCOL; i++)
			APROX[i] = 0;
		VALUE = -1;
		BOARD.clear();
		ABOMINATION.clear();
		WORTH.clear();
		deepDive.clear();
		PARENTMOVE.clear();
		whoseTurn = Turn;
	}
	void cpy(vector<vector<Player>> &To, vector <vector<Player>>FROM) {
		To.clear();
		for (int i = 0; i < NUMCOL; i++)
		{
			vector<Player> Generic;
			Generic.clear();
			for (int j = 0; j < (int)FROM[i].size(); j++) {
				Generic.push_back(FROM[i][j]);
			}
			To.push_back(Generic);
		}
	}
	void depthSearch() {
		if (piecesOnBoard / 2 < 2) {
			VALUE = -1;
			return;
		}
		int DEPTH = 1;
		bool terminate = false;
		switch (SETTING) {
		case Easy :
			DEPTH = 1;
			break;
		case Medium:
			DEPTH = 3;
			break;
		case Hard:
			DEPTH = 5;
			break;
		}
		if ((piecesOnBoard / 2) < 6)
			DEPTH = 1;
		else if (DEPTH > 3 && (piecesOnBoard / 2) < 12)
			DEPTH = 3;
		int ABOMINATIONPREVIOUSSIZE = (int)ABOMINATION.size();
		vector<vector<Player>> Previous;
		cpy(Previous, BOARD);
		ABOMINATION.push_back(Previous);
		WORTH.push_back(false);
		PARENTMOVE.push_back(-1);
		deepDive.push_back(-1);
		int ABOMINATIONSTARTSIZE = (int)ABOMINATION.size();

		for (int i = 0; i <= DEPTH && (piecesOnBoard + i) < (NUMCOL * NUMROW); i++) {
			ABOMINATIONSTARTSIZE = (int)ABOMINATION.size();
			for (int j = ABOMINATIONPREVIOUSSIZE; j < ABOMINATIONSTARTSIZE; j++) {
				cpy(Previous, ABOMINATION[j]);
				vector<vector<Player>>Heuristic;
				if(!WORTH[j])
				for (int k = 0; k < NUMCOL; k++) {
					cpy(Heuristic, Previous);
					pushROW(Heuristic, (Column)k, whoseTurn);
					if ((int)Heuristic.size() != 0) {
						ABOMINATION.push_back(Heuristic);
						bool isWin = WIN(whoseTurn, Heuristic);
						if (isWin && i < 2) {
							if (i == 0) {
								VALUE = k;
								return;
							}
							if (i == 1) {
								if (PARENTMOVE[j] == k) {
									while (BOARD[k].size() < 6) {
										pushROW(BOARD, (Column)k, whoseTurn);
									}
								}
								else {
									VALUE = k;
									terminate = true;
								}
							}
						}
						WORTH.push_back(isWin);
						deepDive.push_back(i);
						if (PARENTMOVE[j] == -1) {
							PARENTMOVE.push_back(k);
						}
						else
						PARENTMOVE.push_back(PARENTMOVE[j]);
					}
					if (terminate) {
						return;
					}
				}
			}
			if (whoseTurn == Red)
			{
				whoseTurn = Black;
			}
			else {
				whoseTurn = Red;
			}
			ABOMINATIONPREVIOUSSIZE = ABOMINATIONSTARTSIZE;
		}
		for (int i = 1; i < (int)WORTH.size(); i++) {
			if (WORTH[i]) {
				// even depth are my moves 
				// odd depths are opponent moves
				if (deepDive[i] % 2 == 0) {
					if((int)BOARD[PARENTMOVE[i]].size() < 6)
					APROX[PARENTMOVE[i]] += 10000000 / (int)(pow(10, deepDive[i]));
				}
				else {					
					if ((int)BOARD[PARENTMOVE[i]].size() < 6)
					APROX[PARENTMOVE[i]] -= 1000000 / (int)(pow(10, deepDive[i]));
				}
			}
		}
		int max = 0;
		int min = 0;
		bool equal = false;
		for (int i = 1; i < NUMCOL; i++) {
			if (APROX[i] > APROX[max]) {
				max = i;
			}
			if (APROX[i] < APROX[min]) {
				min = i;
			}
			if (APROX[i] == APROX[max] && APROX[i] == APROX[min]) {
				equal = true;
			}
		}
		system("cls");
		
		if (equal) {
			if (APROX[max] == APROX[0] && APROX[min] == APROX[0]) {
				VALUE = -1;
				return;
			}
			else {
				equal = false;
			}
		}
		if (abs(APROX[min]) > APROX[max]) {
			VALUE = min;
			cout << VALUE << " : Column Picked \n";
			return;
			 }
		VALUE = max;
		
			
		
	}
	void pushROW(vector<vector<Player>> &grid, Column selected, Player piece) {
		if ((int)grid[(int)selected].size() < 6) {
			grid[(int)selected].push_back(piece);
		}
		else {
			grid.clear();
		}
	}
	int getVALUE() {
		if (VALUE == -1) {
			
			VALUE = rand() % 8;
		}
		while ((int)BOARD[VALUE].size() == 6) {
			VALUE = rand() % 8;
		}
		cout << VALUE << " : Column Picked \n";
		pushROW(BOARD, (Column)VALUE, Turn);
		printBoard();
		cout << "**********************\n";
		return VALUE;
	}
	void printBoard() {
		cout << "**********\n";
		for (int j = NUMROW - 1; j >= 0; j--) {
			cout << "*";
			for (int i = 0; i < NUMCOL; i++) {
				if ((int)BOARD[i].size() > j) {
					if (BOARD[i][j] == Red) {
						cout << "R";
					}
					else {
						cout << "B";
					}
				}
				else {
					cout << ".";
				}
			}
			cout << "*\n";
		}
		cout << "**********\n";
	}
}enemy;
struct CONNECT4BOARD {
	vector<vector<Player>> BOARD;
	vector <Player> Generic;
	void gamesetup() {
		Generic.clear();
		BOARD.clear();
		piecesOnBoard = 0;
		// Force the board to be empty
		for (int i = 0; i < NUMCOL; i++){
			BOARD.push_back(Generic);
		}

		drawBoard();
	}
	void drawBoard() {
		int COLOR = BACKGROUND;
		int y = UNIT;
		setcolor(BOARDCOLOR);
		bar(0, 0, maxX, maxY);
		setcolor(OUTLINE);
		int x = (maxX - 800) / 2;
		for (int i = 0; i < NUMCOL; i++)
			for (int j = 1; j <= NUMROW; j++) {
				rectangle(x + i*UNIT, y + j*UNIT, x + (i + 1)*UNIT, y + (j + 1)*UNIT);
				COLOR = BACKGROUND;
				if (NUMROW - j < (int)BOARD[i].size()) {
					switch (BOARD[i][NUMROW - j]) {
					case Red:
						COLOR = RED;
						break;
					case Black:
						COLOR = BLACK;
						break;
					}
				}
				draw(x + 50 + i*UNIT, y + 50 + j*UNIT, 45, COLOR);
			}

	}
	void drop(Column selected, Player piece) {
		if (ROW(selected) < 6) {
			drawDrop(selected, piece);
			pushROW(selected, piece);
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
		//cout << "limit : " << limit << " y : " << y + 100 << endl;
		draw(x, limit, radius, PIECE(piece));
		
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
		piecesOnBoard++;
		BOARD[(int)selected].push_back(piece);
	}
	/*
	bool WIN(Player piece, vector<vector<Player>>BOARD) {
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
	*/
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