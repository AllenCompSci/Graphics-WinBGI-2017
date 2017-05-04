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
#include "VirtualKeyList.h"
#include "Pixel.h"
using namespace std;

enum CARTYPE { BLUECAR, REDCAR, GREENCAR, REDTRUCK, BLUETRUCK, MASTERCAR, BLACKCAR, YELLOWTRUCK };
///           Safe   Car1    Car2   Car3    Safe   Log1   Log2     Log3   Log4   LilyPad
enum Column { First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eight, Ninth, Tenth, Intro };
#pragma region PROTOTYPE
void Listener();
bool ActionListener(int);
string StringBuilder();
void gr_Start(int&, int&, int&);
void game();
void introColorSet(char);
void INTRO();
void QuestLog();
void RIVER();
void SIDEWALK();
void SAFEZONE();
void LILYPAD();
void CREATE(string);
int adjust(CARTYPE, int);
void drawTruck(int, int, CARTYPE);
void drawCar(int, int, CARTYPE);
void drawMasterCar(int, int);
void drawRevCar(int, int, CARTYPE);
void draw(int, int, CARTYPE);
void establishGAME();
void updateCREATE(int);
#pragma endregion

int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
bool isRunning = true;
bool isName = false;
int UNIT = 40;
bool firstRun = true;
bool swap1 = true;
bool swap2 = true;
bool debug = true;
bool lillied = true;
const int NUMVECTOR = 6; /// HERE 
int w, h;
int CREATION[] = { 0, 0, 0, 0, 0, 0, 0 };
const int RANDOMIZER[] = { 3, 4, 3, 4, 3, 3, 4 };
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

///  LEFT    200    9*40	13*40  17*40    800   25*40  28*40    31*40  34*40	37*40
///  TOP	   0							  0                                  7*40
///  WIDTH	  80	 80      80    100      160      80                            80
///  HEIGHT 1080	120     150    300     1080                                    80
///  SPEED	   0	 -9      +6     -6        0
///  COLOR     8    4/3       1     15       11                                 GREEN
struct NotFrogger {
	int top;
	int left;
	int right;
	int bottom;
	int MOVEMENT;
	int XOffset;
	Column currColumn;
	void create(int x, int y) {
		currColumn = Intro;
		XOffset = (80 - (frog_Width + 1)) / 2;
		MOVEMENT = 15;
		left = x;
		right = x + 80;
		top = y;
		bottom = y + 80;
		draw();
	}
	void repos(int x, int y) {
		left = x;
		right = x + 80;
		top = y;
		bottom = y + 80;
		draw();
	}
	void draw() {
		for (int i = 0; i < frog_Width; i++)
			for (int j = 0; j < frog_Height; j++)
				if (frog_ARRY[j][i] != 99)
					if (currColumn == Tenth && frog_ARRY[j][i] == GREEN) {
						putpixel(left + XOffset + i, top + j, WHITE);
					}
					else
						putpixel(left + XOffset + i, top + j, frog_ARRY[j][i]);
	}
	void remove() {
		setcolor(getColumn());
		bar(left, top, right, bottom);
	}
	void Moveleft() { // Back
		if (currColumn != First && currColumn != Tenth) {
			currColumn = (Column)((int)currColumn - 1);
		}
		columnSetX();
	}
	void up() { // Up
		if (currColumn != Tenth) {
			top -= MOVEMENT;
			bottom -= MOVEMENT;
		}
	}
	void down() { // Down
		if (currColumn != Tenth) {
			top += MOVEMENT;
			bottom += MOVEMENT;
		}
	}
	void Moveright() { // Forward
		if (currColumn != Tenth) {
			currColumn = (Column)((int)currColumn + 1);
		}
		columnSetX();
	}
	void tick() {
		if (GLOBAL.isPressed) {
			remove();
			adjust();
			/*
			draw();
			do { Sleep(14); } while (ActionListener(GLOBAL.VirtualKey));
			GLOBAL.resetKey();
			*/
		}

	}
	void adjust() {
		if (GLOBAL.isPressed) {
			switch (GLOBAL.Significance[0]) {
			case 'L':
				Moveleft();
				break;
			case 'R':
				Moveright();
				break;
			case 'U':
				up();
				break;
			case 'D':
				down();
				break;
			}
			GLOBAL.resetKey();
		}
	}
	void intro() {


	}
	void columnSetX() {
		cout << "currColumn : " << currColumn << "\n";
		switch (currColumn) {
		case First:
			left = (int)(5.5 * UNIT);
			break;
		case Second:
			left = (int)(8.1 * UNIT);
			break;
		case Third:
			left = (int)(12 * (UNIT));
			break;
		case Fourth:
			left = (int)(17 * (UNIT));
			break;
		case Fifth:
			left = (int)(21.25 * UNIT);
			break;
		case Sixth:
			left = (int)(25 * UNIT);
			break;
		case Seventh:
			left = (int)(28 * UNIT);
			break;
		case Eight:
			left = (int)(31 * UNIT);
			break;
		case Ninth:
			left = (int)(34 * UNIT);
			break;
		case Tenth:
			left = (int)(37 * UNIT);
			lillied = true;
			break;
		default:
			break;
		}

		right = left + 80;
	}
	void boundingBox() {
		setcolor(GREEN);
		rectangle(left, top, right, bottom);
	}
	int getColumn() {
		switch (currColumn) {
		case First:
			return LIGHTGRAY;
		case Second:
		case Third:
		case Fourth:
			return DARKGRAY;
		case Fifth:
			return LIGHTGRAY;
		case Sixth:
		case Seventh:
		case Eight:
		case Ninth:
			return BROWN;
		case Tenth:
			return GREEN;
		default:
			return BLACK;
		}
	}
}frog;
struct Car {
	int top;
	int left;
	int right;
	int bottom;
	int dy;
	int color;
	Column position;
	CARTYPE type;
	void create(int x, int y) {
		left = x;
		right = x + 80;
		top = y;
		bottom = y + 120;
		dy = -9;
		_draw();
	}
	void create(Column pos) {
		position = pos;
		switch (position) {
		case Second:
			left = (int)(8.1 * UNIT - 6);
			right = left + CAR_Width;
			top = (maxY - (CAR_Height / 4));
			bottom = top + CAR_Height;
			dy = -15;
			break;
		case Third:
			left = 12 * UNIT - 6;
			right = left + CAR_Width;
			bottom = CAR_Height/4;
			top = bottom - CAR_Height;
			dy = 16;
			break;
		case Fourth:
			left = 17 * UNIT - 6;
			right = left + TRUCK_Width;
			top = (maxY - (TRUCK_Height / 4));
			bottom = top + TRUCK_Height;
			dy = -12;
			break;
		}
		type = genType();
	}
	CARTYPE genType() {
		switch (position) {
		case Second:
		case Third:
			switch (rand() % 5) {
			case 0:
				return BLUECAR;
			case 1:
				return REDCAR;
			case 2:
				return GREENCAR;
			case 3:
				return MASTERCAR;
			case 4:
				return BLACKCAR;
			}
			break;
		case Fourth:
			switch (rand() % 3) {
			case 0:
				return REDTRUCK;
			case 1:
				return BLUETRUCK;
			case 2:
				return YELLOWTRUCK;
			}
			break;
		}
		return MASTERCAR;
	}
	void render() {
		if (position == Third) {
			drawRevCar(left, top, type);
			return;
		}
		draw(left, top, type);
	}
	void _draw() {
		setcolor(color);
		bar(left, top, right, bottom);
	}
	void move() {
		top += dy;
		bottom += dy;
	}
	void remove() {
		setcolor(LIGHTGRAY);
		if (position != Second)
			bar(left, bottom, right, bottom - dy);
	}
	void tick() {
		//remove();
		move();
		render();
		//draw();
	}

}Type1;
struct Road {
	int top, bottom, left, right;
	int firstStripeX, secondStripeX, space, itteration;
	void init() {
		top = 0;
		bottom = maxY;
		left = (int)(8.1 * UNIT);
		right = (int)(19.9 * UNIT);
		firstStripeX = (int)(10.5 * UNIT);
		secondStripeX = (int)(14.5 * UNIT);
		space = 2 * UNIT;
		itteration = 5 * UNIT;
		draw();
	}
	void draw() {
		setcolor(DARKGRAY);
		bar(left, top, right, bottom);
		drawLines();
	}
	void render(Column t) {
		setcolor(DARKGRAY);
		switch (t) {
		case Second :
			bar(left, top, firstStripeX + UNIT, bottom);
			break;
		case Third :
			bar(firstStripeX, top, secondStripeX + UNIT, bottom);
			break;
		case Fourth :
			bar(secondStripeX, top, right, bottom);
			break;
		}
		drawLines();
	}
	void drawLines() {
		setcolor(YELLOW);
		bar(firstStripeX, top, firstStripeX + UNIT, bottom);
		bar(secondStripeX, top, secondStripeX + UNIT, bottom);
		setcolor(DARKGRAY);
		for (int i = 0; i < (maxY / UNIT) / 4; i++) {
			bar(firstStripeX, UNIT + (i * itteration), firstStripeX + UNIT, UNIT + (i * itteration) + space);
			bar(secondStripeX, UNIT + (i * itteration), secondStripeX + UNIT, UNIT + (i * itteration) + space);
		}
	}
}ROAD;
struct Log {
	int top;
	int left;
	int right;
	int bottom;
	int dy;
	void create(int x, int y) {
		left = x * UNIT;
		right = x + 80;
		top = y * UNIT;
		bottom = y + 250;
		dy = -6;
		draw();
	}

	void draw() {
		setcolor(6);
		bar(left, top, right, bottom);
	}
	void move() {
		top += dy;
		bottom += dy;
	}
	void remove() {
		setcolor(BLUE);
		bar(left, top, right, bottom);
	}
	void tick() {
		remove();
		move();
		draw();
	}

}platform;
struct Lilypad {
	int top;
	int left;
	int right;
	int bottom;
	bool container;
	void create(int x, int y) {
		left = x*UNIT;
		right = left + 80;
		top = y*UNIT;
		bottom = top + 80;
		container = false;
		draw();
	}

	void draw() {
		setcolor(GREEN);
		bar(left, top, right, bottom);
		setcolor(CYAN);
		for (int i = 0; i < 4; i++) {
			rectangle(left + i, top + i, right - i, bottom - i);
		}
	}
	bool containsFrog() {
		if (frog.currColumn == Tenth && frog.top < (top + UNIT) && frog.bottom >(bottom + UNIT)) {
			return true;
		}
		return false;
	}
	void tick() {
		if (!container)
		{
			container = containsFrog();
		}
	}

}pads[4];
struct WiseFrog {
	int top;
	int left;
	int right;
	int bottom;
	void create(int x, int y) {
		left = x;
		right = x + MAINSCREEN_Width;
		top = y;
		bottom = y + MAINSCREEN_Height;
		draw();
	}
	void remove() {
		setcolor(BLACK);
		bar(left, top, right, bottom);
	}
	void draw() {
		/* BOUNDING BOX : WHITE
		setcolor(WHITE);
		rectangle(left, top, right, bottom);
		*/
		int bluegreen = 0;
		bool yellowgreen = true;
		for (int i = 0; i < MAINSCREEN_Width; i++)
			for (int j = 0; j < MAINSCREEN_Height; j++)
				switch (MAINSCREEN_ARRY[j][i]) {
				case 99:
					break;
				case 1:
					if (j < MAINSCREEN_Height / 2)
						break;
				case 2:
					if (bluegreen % 3 == 0)
						putpixel(left + i, top + j, 1);
					else
						putpixel(left + i, top + j, 2);
					bluegreen++;
					break;
				case 8:
					putpixel(left + i, top + j, 8);
					break;

				case 14:
					if (yellowgreen)
						putpixel(left + i, top + j, 14);
					else
						putpixel(left + i, top + j, 2);
					yellowgreen = !yellowgreen;
					break;
				default:
					putpixel(left + i, top + j, 0);
					break;
				}
	}
}notfrogger;
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
vector <NotFrogger> GOLDENFROGS;
vector <Car> CARCOL2;
vector <Car> CARCOL3;
vector <Car> CARCOL4;
vector <Log> LOGCOL6;
vector <Log> LOGCOL7;
vector <Log> LOGCOL8;
vector <Log> LOGCOL9;