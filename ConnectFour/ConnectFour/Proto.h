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
const int UNIT = 40; /// Might be able to set as const should never change
const int NUMCOL = 8;
const int NUMROW = 8;
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
		// Force the board to be empty
		for (int i = 0; i < NUMCOL; i++){
			BOARD.push_back(Generic);
		}


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