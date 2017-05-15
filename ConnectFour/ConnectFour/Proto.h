#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
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

bool ActionListener(int); /// Key Listener Attribute
void gr_Start(int&, int&, int&);  /// Creates Graphics Window
void game(); // Thread 1
void Listener(); // Thread 2
void INTRO(); /// Draws an Intro for Connect 4
float distance(int, int, int, int); /// Application of Distance Forumula
void draw(int, int, int, int); /// Draws a Circle using putpixel
void boundedCircle(int, int, int, int, int, int, int, int); // Allows the Circle to be drawn in only visible areas of two other circles maintaint its shape
void topCircle(int, int, int, int, int, int, int, int); // As bounded, except the top Circle is a Rectangle increaseing the area of its range from being inside two circles to a rectangle and then a circle offset from the radius of the rectangle (VISUAL)
bool WIN(Player, vector<vector<Player>>);  // Checks Left to Right
void printBoard(vector<vector<Player>>); // Displays to console the game. 
string increaseSTR(string);
void GAMELog();
/// LOAD BMP SETTINGS
string toUpper(string);
unsigned char* ReadBMP(char*);
void CREATE(string);
/// CARD FUNCTIONS
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
/// BOARD SIZE
const int NUMCOL = 8;
const int NUMROW = 6;
/// COLOR SCHEMA
const int BOARDCOLOR = YELLOW;
const int BACKGROUND = WHITE;
const int OUTLINE = BLACK;
/// GLOBAL LIFETIME
bool isRunning = true;
/// GLOBAL ASSET for KEYLISTENER
bool isName = false;
bool isRefresh = false;
/// GAME ENUMS TURN, WHO WINS, AISETTING
Player Turn = Red;
Player WINNER = NA;
AIMode SETTING = Hard;
/// Global basic ints
int piecesOnBoard = 0; // Count of Pieces Played
int GrDriver, GrMode, ErrorCode;
int maxX, maxY; // Size of canvas
int BLANKCARD[IMGHeight][IMGWidth]; /// CARD SPECIFIC FUNCTION.
int w, h; /// BMP using for passing through functions 
vector <int> XColBounds;
ofstream outfile;
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
	vector<vector<Player>> currGAME;
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
		cpy(currGAME, FROM);
		depthSearch();
		outfile << (int)WORTH.size() << " : Data Points 5 itterations Deep Dive \n";
		outfile << "BOARD Post depthSearch() : \n";
		outfile << "**********************\n";
		printBoard();
		outfile << "**********************\n";
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
		currGAME.clear();
		PARENTMOVE.clear();
		whoseTurn = Turn;
	}
	void cpy(vector<vector<Player>> &To, vector <vector<Player>>FROM) {
		To.clear();
		for (int i = 0; i < NUMCOL; i++)
		{
			vector<Player> Generic(FROM[i]);
			//Generic.clear();	
		/*	
		if (!FROM[i].empty())
				copy(FROM[i].begin(), FROM[i].end(), Generic.begin());
		
			for (int j = 0; j < (int)FROM[i].size(); j++) {
				Generic.push_back(FROM[i][j]);
			}
		*/	
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
								return; // prioritize win
							}
							if (i == 1) {
								if (PARENTMOVE[j] == k) {
									while (BOARD[k].size() < 6) {
										pushROW(BOARD, (Column)k, whoseTurn); // TRY TO BLOCK LOOSE PIECE FROM GOING IN k COLUMN
									}
								}
								else {
									VALUE = k;
									terminate = true; // Terminates based on the fact that this isn't an end move. 
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
			if (whoseTurn == Red) /// Changes Whose Turn is simualted 
			{
				whoseTurn = Black;
			}
			else {
				whoseTurn = Red;
			}
			ABOMINATIONPREVIOUSSIZE = ABOMINATIONSTARTSIZE;
		}
		for (int i = 1; i < (int)WORTH.size(); i++) { // Evaluates ABOMINATION a vector of all possible moves
			if (WORTH[i]) {
				// even depth are my moves 
				// odd depths are opponent moves
				if (deepDive[i] % 2 == 0) {
					if((int)BOARD[PARENTMOVE[i]].size() < 6) /// If this is a possible move
					APROX[PARENTMOVE[i]] += 10000000 / (int)(pow(10, deepDive[i]));
				}
				else {					
					if ((int)BOARD[PARENTMOVE[i]].size() < 6) // Same
					APROX[PARENTMOVE[i]] -= 1000000 / (int)(pow(10, deepDive[i]));
				}
			}
		}
		int max = 0;
		int min = 0;
		bool equal = false;
		for (int i = 1; i < NUMCOL; i++) { // Judge Evaluation
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
		
		if (equal) {
			if (APROX[max] == APROX[0] && APROX[min] == APROX[0]) {
				VALUE = -1;
				return;
			}
			else {
				equal = false;
			}
		}
		if (abs(APROX[min]) > APROX[max]) { // Set
			VALUE = min;
			return;
			 }
		VALUE = max;
		
			
		
	}
	void pushROW(vector<vector<Player>> &grid, Column selected, Player piece) {
		if ((int)grid[(int)selected].size() < 6) {
			grid[(int)selected].push_back(piece);
		}
		else {
			grid.clear(); /// Clears solution 
		}
	}
	int getVALUE() {
		if (VALUE == -1) {
			VALUE = rand() % 8; /// Random 
		}
		bool SpecialCase = true; /// When maybe you block the only possible move
		for (int i = 0; i < NUMCOL && SpecialCase; i++) { /// Checks that special case
			if (BOARD[i].size() != NUMCOL) {
				SpecialCase = false;
			}
		}
		if (SpecialCase) {
			cpy(BOARD, currGAME);
			outfile << "**********************\n";
			printBoard();
			outfile << "**********************\n";
		}
		while ((int)BOARD[VALUE].size() == NUMCOL) {
			VALUE = rand() % NUMROW;
		}
		outfile << VALUE << " : Column Picked \n";
		pushROW(BOARD, (Column)VALUE, Turn);
		printBoard();
		outfile << "**********************\n";
		return VALUE; // Returns a column selected and evaluated 
	}
	void printBoard() {
		outfile << "**********\n";
		for (int j = NUMROW - 1; j >= 0; j--) {
			outfile << "*";
			for (int i = 0; i < NUMCOL; i++) {
				if ((int)BOARD[i].size() > j) {
					if (BOARD[i][j] == Red) {
						outfile << "R";
					}
					else {
						outfile << "B";
					}
				}
				else {
					outfile << ".";
				}
			}
			outfile << "*\n";
		}
		outfile << "**********\n";
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
		GAMELog();
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
}GAME;
struct PLAYERCLICK {
	void setup() {
		int x = (maxX - 800) / 2;
		for (int i = 0; i <= NUMCOL; i++)
			XColBounds.push_back(x + i*UNIT); /// DEFINES THE BOUNDS.
	}
	Column mouseCHECK() {
		bool PLAYERTURN = true;
		while (PLAYERTURN) {
			for(int i = 0; i < NUMCOL; i++)
				if (GLOBAL.isClicked && GLOBAL.Cursor.x >= XColBounds[i] && GLOBAL.Cursor.x < XColBounds[i + 1]) {
					GLOBAL.reset();
					if (GAME.ROW((Column)i) < 6) {
						return (Column)i;
					}
				}

			Sleep(10);
		}
		return rand() % 2 == 0 ? Three : Four;
	}
}Human;
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
// 495