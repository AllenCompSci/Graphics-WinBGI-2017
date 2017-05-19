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
#pragma region ENUMs
enum CARTYPE { BLUECAR, REDCAR, GREENCAR, REDTRUCK, BLUETRUCK, MASTERCAR, BLACKCAR, YELLOWTRUCK };
///           Safe   Car1    Car2   Car3    Safe   Log1   Log2     Log3   Log4   LilyPad
enum Column { First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eight, Ninth, Tenth, Intro };
#pragma endregion
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
bool isSafe(Column, int top, int h);
#pragma endregion
#pragma region GLOBAL_VARS
const int NUMVECTOR = 17; /// LoadBMP VECTORSIZE
const int RANDOMIZER[] = { 3, 4, 3, 4, 3, 3, 4 }; // GAME Frequency 
const int UNIT = 40; /// Might be able to set as const should never change
#pragma region Height_Width
const int frog_Height = 80;
const int frog_Width = 69;
const int MAINSCREEN_Height = 154;
const int MAINSCREEN_Width = 109;
const int Title_Height = 12;
const int Title_Width = 9;
const int Smashed_Height = 16;
const int Smashed_Width = 8;
const int CAR_Height = 132;
const int CAR_Width = 110;
const int TRUCK_Height = 255;
const int TRUCK_Width = 122;
const int LOG_Height = 10;
const int LOG_Width = 42;
const int lily_Height = 80;
const int lily_Width = 80;
#pragma endregion
/// DRAWING Helper
const int black = 16;
const int background = 99;
const int secondaryColor = 19;
const int primaryColor = 17;
const int cabColor = 20;
const int trailer = 23;
const int LIGHTGREENE = 98;
const int GRAYGREEN = 97;
const int GRAYBLUE = 96;
/// BOOL TOGGLE VARIABLES
bool firstRun = true;
bool swap1 = true;
bool swap2 = true;
bool debug = true;
bool lillied = true;
bool isRunning = true;
bool isName = false;
/// Global basic ints 
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
int w, h; /// BMP using for passing through functions 
int CREATION[] = { 0, 0, 0, 0, 0, 0, 0 }; // Updating Values storing since created
#pragma endregion;
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
struct NotFrogger {
	int top;
	int left;
	int right;
	int bottom;
	int MOVEMENT;
	int XOffset;
	bool alive;
	int deathAnimation;
	Column currColumn;
	void create(int x, int y) {
		currColumn = Intro;
		XOffset = (80 - (frog_Width + 1)) / 2;
		MOVEMENT = 20;
		left = x;
		right = x + 80;
		top = y;
		bottom = y + 80;
		alive = true;
		deathAnimation = 0;
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
		if (alive) {
			for (int i = 0; i < frog_Width; i++)
				for (int j = 0; j < frog_Height; j++)
					if (frog_ARRY[j][i] != 99)
						if (currColumn == Tenth && frog_ARRY[j][i] == GREEN) {
							putpixel(left + XOffset + i, top + j, WHITE);
						}
						else
							putpixel(left + XOffset + i, top + j, frog_ARRY[j][i]);
		}
		else {

			deathAnimation++;
			if ((currColumn == Second || currColumn == Third || currColumn == Fourth) && deathAnimation < 10) {
				int deathUNIT = 4;
				setcolor(YELLOW);
				right = 80 + left;
				for (int i = 0; i < Smashed_Height; i++) {
					for (int j = 0; j < Smashed_Width; j++) {
						if (Smashed[i][j] != 'D') {
							bar(left + (j + 1) * deathUNIT, top + (i + 2) * deathUNIT, left + (j + 2) * deathUNIT, top + (3 + i) * deathUNIT);
							bar(left + ((Smashed_Height - j - 1) * deathUNIT), top + (i + 2) * deathUNIT, left + ((Smashed_Height - j)* deathUNIT), top + (3 + i) * deathUNIT);
						}
					}
				}
			}
			/// WATER DEATH NEED ROAD DEATH
			if ((currColumn == Sixth || currColumn == Seventh || currColumn == Eight || currColumn == Ninth || currColumn == Tenth) && deathAnimation < 10) {
				/// AQUA
				remove();
				int Cx = left + 35;
				int Cy = top + 40;
				int radius1 = UNIT - 5 - (deathAnimation * 2);
				int radius2 = UNIT - 5 - (deathAnimation * 3);
				setcolor(CYAN);
				circle(Cx, Cy, radius1);
				circle(Cx, Cy, radius2);
				setcolor(WHITE);
				switch (rand() % 3) {
				case 0:
					Cx -= 2;
					break;
				case 1:
					Cy += 4;
					break;
				case 2:
					Cx += 3;
					Cy -= 5;
				}
				if (deathAnimation < 6)
					fillellipse(Cx, Cy, 3, 3);
			}
			if (deathAnimation == 15) {
				remove();
				lillied = true;
			}
		}
	}
	void kill() {
		remove();
		deathAnimation = 0;
		alive = false;
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
		if (alive && !isSafe(currColumn, top, 80)) {
			kill();
		}
			if (GLOBAL.isPressed && alive) {
				if (currColumn != Sixth && currColumn != Seventh && currColumn != Eight && currColumn != Ninth) {
					remove();
				}
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
			case 'F':
				kill();
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
			return BLUE;
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
		setcolor(DARKGRAY);

		bar(left + 6, top - dy, right - 6, bottom - dy);
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
		setcolor(DARKGRAY);

		bar(left + 6, top, right - 6, bottom - dy);
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
		bar(firstStripeX+2, top, firstStripeX + UNIT, bottom);
		bar(secondStripeX, top, secondStripeX + UNIT, bottom);
		setcolor(DARKGRAY);
		for (int i = 0; i < (maxY / UNIT) / 4; i++) {
			bar(firstStripeX + 2, UNIT + (i * itteration), firstStripeX + UNIT, UNIT + (i * itteration) + space);
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
	int xOFFSET, yOFFSET;
	int logUNIT;
	bool up, invert;
	void create(int x, int y) {
		xOFFSET = 15;
		yOFFSET = 20;
		logUNIT = 5; // 5x5 each Pixel
		left = x * UNIT;
		right = x + 80;
		top = y * UNIT;
		bottom = y + 250;
		dy = -6;
		invert = false;
		up = true;
		draw();
	}
	void create(Column pos) {
		xOFFSET = 15;
		yOFFSET = 20;
		logUNIT = 5; // 5x5 each Pixel
		up = false;
		dy = -15;
		switch (pos) {
		case Sixth:
			left = (int)(25 * UNIT);
			top = maxY - 45;
			up = true;
			break;
		case Seventh:
			left = (int)(28 * UNIT);
			dy = abs(dy);
			top = 0 - 250 + 45;
			break;
		case Eight:
			left = (int)(31 * UNIT);
			dy = abs(dy);
			top = 0 - 250 + 45;
			break;
		case Ninth:
			up = true;
			left = (int)(34 * UNIT);
			top = maxY - 45;
			break;
		}
		invert = rand() % 3 == 0;
		right = left + 80;
		bottom = top + 250;
		
		draw();
	}
	void draw() {
		remove();
		for (int i = 0; i < LOG_Width; i++) {
			for (int j = 0; j < LOG_Height; j++) {
				if (LOG_ARRY[j][i] != 99) {
					setcolor(LOG_ARRY[j][i]);
					if (up) {
						if(!invert)
							bar(left + xOFFSET + (j) * logUNIT, yOFFSET + top + (i) * logUNIT, xOFFSET + left + (j + 1) * logUNIT, yOFFSET + top + (1 + i) * logUNIT);
						else
							bar(right - xOFFSET - (j+1)* logUNIT, yOFFSET + top + (i)* logUNIT, right - xOFFSET - (j) * logUNIT, yOFFSET + top + (1 + i) * logUNIT);
					}
					else {
						if (!invert)
							bar(left + xOFFSET + (j)* logUNIT, bottom - yOFFSET - (i+1)* logUNIT, xOFFSET + left + (j + 1) * logUNIT, bottom - yOFFSET - (i) * logUNIT);
						else
							bar(right - xOFFSET - (j + 1)* logUNIT, bottom - yOFFSET - (i+1)* logUNIT, right - xOFFSET - (j)* logUNIT, bottom - yOFFSET - (i) * logUNIT);

					}
				}
			}
		}
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
	bool LGREEN = true;
	bool GGREEN = false;
	bool GYBLUE = true;
	void draw() {
		for(int i = 0; i < lily_Height; i++)
			for (int j = 0; j < lily_Width; j++) {
				if (lily_ARRY[i][j] != 99)
					putpixel(left + j, top + i, returnColor(lily_ARRY[i][j]));
			}
		/* SQUARE - No Pic
		setcolor(GREEN);
		bar(left, top, right, bottom);
		setcolor(CYAN);
		for (int i = 0; i < 4; i++) {
			rectangle(left + i, top + i, right - i, bottom - i);
		}
		*/
	}
	int returnColor(int value) {
		switch (value) {
		case LIGHTGREENE:
			LGREEN = !LGREEN;
			if (LGREEN)
				return WHITE;
			return GREEN;
		case GRAYGREEN:
			GGREEN = !GGREEN;
			if (GGREEN)
				return DARKGRAY;
			return GREEN;
		case GRAYBLUE:
			GYBLUE = !GYBLUE;
			if (GYBLUE)
				return LIGHTGRAY;
			return BLUE;
		}
		return value;
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
#pragma endregion
#pragma region RECORD_VECTOR(CONTAINERS)
vector <RGB> colors(NUMVECTOR);
vector <NotFrogger> GOLDENFROGS;
vector <Car> CARCOL2; // Column 2
vector <Car> CARCOL3; // Column 3
vector <Car> CARCOL4; // Column 4
vector <Log> LOGCOL6; // Column 6
vector <Log> LOGCOL7; // Column 7
vector <Log> LOGCOL8; // Column 8
vector <Log> LOGCOL9; // Column 9
#pragma endregion
/// LINE 742