/*
Frogger Games Clean
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <Windows.h>
#include <winbgi.cpp>
#include <graphics.h>
#include <thread>
#include "VirtualKeyList.h"
using namespace std;
void Listener();
bool ActionListener(int);
string StringBuilder();
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode);
void game();
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
bool isRunning = true;
bool isName = false;


///          Safe   Car1    Car2   Car3    Safe   Log1   Log2     Log3   LilyPad
enum Column {First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eight, Ninth};
///  LEFT    200    9*40	13*40  17*40    800   25*40  28*40    32*40
///  TOP	   0     						  0	 
///  WIDTH	  80	 80      80    100      160      80
///  HEIGHT 1080	120     150    300     1080
///  SPEED	   0	 -9      +6     -6        0
///  COLOR     8    4/3       1     15       11
struct NotFrogger {
	int top;
	int left;
	int right;
	int bottom;
	Column currColumn;
	void create(int x, int y, int x1, int y1) {
		left = x;
		right = x1;
		top = y;
		bottom = y1;
		draw();
	}

	void draw() {
		setcolor(2);
		bar(left, top, right, bottom);
	}
	void remove() {
		setcolor(getColumn());
		bar(left, top, right, bottom);
	}
	void adjust() {

	}
	void tick() {
		if (GLOBAL.isPressed) {
			remove();
			adjust();
		}
		draw();
	}
	int getColumn() {
		switch (currColumn) {
		case First:
			return DARKGRAY;
		case Second:
		case Third:
		case Fourth:
			return LIGHTGRAY;
		case Fifth:
			return DARKGRAY;
		case Sixth:
		case Seventh:
		case Eight:
			return BROWN;
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
	void create(int x, int y) {
		left = x;
		right = x + 80;
		top = y;
		bottom = y + 120;
		dy = -9;
		draw();
	}
	void create(Column position) {


	}

	void draw() {
		setcolor(color);
		bar(left, top, right, bottom);
	}
	void move() {
		top += dy;
		bottom += dy;
	}
	void remove() {
		setcolor(LIGHTGRAY);
		if(position != Second)
			bar(left, bottom, right, bottom - dy);
	}
	void tick() {
		remove();
		move();
		draw();
	}

}EnemyType1;
struct Log {
	int top;
	int left;
	int right;
	int bottom;
	int dy;
	void create(int x, int y) {
		left = x;
		right = x + 80;
		top = y;
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

};
struct Lilypad {
	int top;
	int left;
	int right;
	int bottom;
	void create(int x, int y, int x1, int y1) {
		left = x;
		right = x1;
		top = y;
		bottom = y1;
		draw();
	}

	void draw() {
		setcolor(2);
		bar(left, top, right, bottom);
	}
	void remove() {
		setcolor(BLACK);
		bar(left, top, right, bottom);
	}
	void tick() {
		remove();
		draw();
	}

}pads[3];
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

int main() {

	thread FROGGER(game);
	thread LISTENER(Listener);
	FROGGER.join();
	LISTENER.join();

	system("pause");


}
void game() {

	gr_Start(GrDriver, GrMode, ErrorCode);

	StringBuilder();

	maxX = getmaxx();
	maxY = getmaxy();
	/*River*/
	setcolor(1);
	rectangle(960, 0, 1680, 1080);
	bar(960, 0, 1680, 1080);
	/*Sidewalk*/
	setcolor(11);
	rectangle(800, 0, 960, 1080);
	bar(800, 0, 960, 1080);
	/*Road*/
	setcolor(0);
	rectangle(200, 0, 800, 1080);
	bar(200, 0, 800, 1080);
	/*SafeZone*/
	setcolor(8);
	rectangle(200, 0, 280, 1080);
	bar(200, 0, 280, 1080);
	pads[0].create(1520, 80, 1640, 200);
	pads[1].create(1520, 320, 1640, 440);
	pads[2].create(1520, 560, 1640, 720);
	frog.create(200, 600, 280, 680);
	EnemyType1.create(9 * 40, 1050);
	while (EnemyType1.top > 0) {
		EnemyType1.tick();
		Sleep(32);


	}
	/*
	blue.create(13 * 40, 1050);
	while (blue.bottom < getmaxy()) {
		blue.tick();
		Sleep(32);
	}
	white.create(17 * 40, 1050);
	while (white.top > 0) {
		white.tick();
		Sleep(32);
	}
	cyan.create(9 * 40, 1050);
	while (cyan.top > 0) {
		cyan.tick();
		Sleep(32);
	}
	brown.create(25 * 40, 1050);
	while (brown.top > 0) {
		brown.tick();
		Sleep(32);
	}
	brown2.create(28 * 40, 1050);
	while (brown2.top > 0) {
		brown2.tick();
		Sleep(32);
	}
	brown3.create(32 * 40, 1050);
	while (brown3.top > 0) {
		brown3.tick();
		Sleep(32);
	}
	brown4.create(32 * 40, 1050);
	while (brown4.top > 0) {
		brown4.tick();
		Sleep(32);
	}
	*/

	setcolor(10);
	rectangle(0, 120, 160, 560);
	bar(0, 120, 160, 560);
	setcolor(2);
	/*T*/
	line(0, 600, 160, 600);
	line(80, 600, 80, 720);
	/*I*/
	line(80, 840, 80, 760);
	/*M*/
	line(0, 1000, 0, 920);
	line(40, 1000, 0, 920);
	line(40, 1000, 80, 920);
	line(80, 920, 80, 1000);
	/*E*/
	line(120, 920, 120, 1000);
	line(120, 1000, 160, 1000);
	line(120, 960, 160, 960);
	line(120, 920, 160, 920);

	setcolor(7);
	rectangle(200, 0, 800, 1080);
	bar(200, 0, 800, 1080);
	setcolor(8);
	rectangle(200, 0, 280, 1200);
	bar(200, 0, 280, 1200);
	setcolor(4);
	rectangle(360, 40, 440, 160);
	bar(360, 40, 440, 160);
	setcolor(3);
	rectangle(480, 240, 600, 520);
	bar(480, 240, 600, 520);
	rectangle(360, 800, 440, 320);
	bar(360, 800, 440, 320);
	setcolor(6);
	rectangle(680, 80, 800, 560);
	bar(680, 80, 800, 560);
	/*Sidewalk*/
	setcolor(11);
	rectangle(800, 0, 960, 1080);
	bar(800, 0, 960, 1080);
	/*NotFrogger*/
	setcolor(2);
	line(200, 600, 200, 680);
	line(200, 600, 280, 640);
	line(280, 640, 200, 680);
	/*River*/
	setcolor(1);
	rectangle(960, 0, 1680, 1080);
	bar(960, 0, 1680, 1080);
	/*Logs*/
	setcolor(6);
	rectangle(1000, 200, 1080, 600);
	bar(1000, 200, 1080, 600);
	rectangle(1360, 200, 1440, 360);
	bar(1360, 200, 1440, 360);
	rectangle(1360, 680, 1440, 1120);
	bar(1360, 680, 1440, 1120);
	rectangle(1120, 680, 1280, 1200);
	bar(1120, 680, 1280, 1200);
	/*Lilypad*/
	setcolor(2);
	rectangle(1520, 80, 1640, 200);
	bar(1520, 80, 1640, 200);
	rectangle(1520, 320, 1640, 440);
	bar(1520, 320, 1640, 440);
	rectangle(1520, 560, 1640, 720);
	bar(1520, 560, 1640, 720);
	rectangle(1520, 1120, 1640, 1200);
	/*NotFrogger movements*/
}
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode) {
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
	return;
}
string StringBuilder() {
	string NAME = "";
	isName = true;
	string BufferedReader = "";
	while (BufferedReader != "RETURN") {
		if (GLOBAL.isPressed) {
			BufferedReader = GLOBAL.Significance;
			if (BufferedReader != "RETURN" && BufferedReader != "Left Mouse Click") {
				if (BufferedReader != "BACK") {
					NAME += BufferedReader;
				}
				else {
					NAME = NAME.substr(0, NAME.length() - 1);
				}
			}
			do { Sleep(3); } while (ActionListener(GLOBAL.VirtualKey));
			GLOBAL.resetKey();
			cout << NAME << "\n";
		}
		
		Sleep(15);
	}
	return NAME;
}
void Listener() {
	GLOBAL.reset();
	while (isRunning) {
		if (!GLOBAL.isPressed) {
			/// Mouse Listener
			if (ActionListener(VK_LBUTTON)) {
				// Left Mouse Button
				GetCursorPos(&GLOBAL.Cursor);
				GLOBAL.VirtualKey = VK_LBUTTON;
				GLOBAL.Significance = "Left Mouse Click";
				do { Sleep(100); } while (ActionListener(VK_LBUTTON));
				GLOBAL.isClicked = true;
			}
			/// Irregular KeyListener Build String
			if (isName) {
				if (ActionListener(VK_B)) {
					GLOBAL.VirtualKey = VK_B;
					GLOBAL.Significance = "B";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_C)) {
					GLOBAL.VirtualKey = VK_C;
					GLOBAL.Significance = "C";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_E)) {
					GLOBAL.VirtualKey = VK_E;
					GLOBAL.Significance = "E";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_F)) {
					GLOBAL.VirtualKey = VK_F;
					GLOBAL.Significance = "F";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_G)) {
					GLOBAL.VirtualKey = VK_G;
					GLOBAL.Significance = "G";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_H)) {
					GLOBAL.VirtualKey = VK_H;
					GLOBAL.Significance = "H";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_I)) {
					GLOBAL.VirtualKey = VK_I;
					GLOBAL.Significance = "I";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_J)) {
					GLOBAL.VirtualKey = VK_J;
					GLOBAL.Significance = "J";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_K)) {
					GLOBAL.VirtualKey = VK_K;
					GLOBAL.Significance = "K";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_L)) {
					GLOBAL.VirtualKey = VK_L;
					GLOBAL.Significance = "L";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_M)) {
					GLOBAL.VirtualKey = VK_M;
					GLOBAL.Significance = "M";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_N)) {
					GLOBAL.VirtualKey = VK_N;
					GLOBAL.Significance = "N";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_O)) {
					GLOBAL.VirtualKey = VK_O;
					GLOBAL.Significance = "O";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_P)) {
					GLOBAL.VirtualKey = VK_P;
					GLOBAL.Significance = "P";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Q)) {
					GLOBAL.VirtualKey = VK_Q;
					GLOBAL.Significance = "Q";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_R)) {
					GLOBAL.VirtualKey = VK_R;
					GLOBAL.Significance = "R";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_T)) {
					GLOBAL.VirtualKey = VK_T;
					GLOBAL.Significance = "T";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_U)) {
					GLOBAL.VirtualKey = VK_U;
					GLOBAL.Significance = "U";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_V)) {
					GLOBAL.VirtualKey = VK_V;
					GLOBAL.Significance = "V";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_X)) {
					GLOBAL.VirtualKey = VK_X;
					GLOBAL.Significance = "X";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Y)) {
					GLOBAL.VirtualKey = VK_Y;
					GLOBAL.Significance = "Y";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Z)) {
					GLOBAL.VirtualKey = VK_Z;
					GLOBAL.Significance = "Z";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}

				else if (ActionListener(VK_A)) {
					GLOBAL.VirtualKey = VK_A;
					GLOBAL.Significance = "A";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}

				else if (ActionListener(VK_W)) {
					GLOBAL.VirtualKey = VK_W;
					GLOBAL.Significance = "W";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_S)) {
					GLOBAL.VirtualKey = VK_S;
					GLOBAL.Significance = "S";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_D)) {
					GLOBAL.VirtualKey = VK_D;
					GLOBAL.Significance = "D";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}

				else if (ActionListener(VK_1) || ActionListener(VK_NUMPAD1)) {
					GLOBAL.VirtualKey = VK_1;
					GLOBAL.Significance = "1";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_2) || ActionListener(VK_NUMPAD2)) {
					GLOBAL.VirtualKey = VK_2;
					GLOBAL.Significance = "2";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_3) || ActionListener(VK_NUMPAD3)) {
					GLOBAL.VirtualKey = VK_3;
					GLOBAL.Significance = "3";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_4) || ActionListener(VK_NUMPAD4)) {
					GLOBAL.VirtualKey = VK_4;
					GLOBAL.Significance = "4";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_5) || ActionListener(VK_NUMPAD5)) {
					GLOBAL.VirtualKey = VK_5;
					GLOBAL.Significance = "5";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_6) || ActionListener(VK_NUMPAD6)) {
					GLOBAL.VirtualKey = VK_6;
					GLOBAL.Significance = "6";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_7) || ActionListener(VK_NUMPAD7)) {
					GLOBAL.VirtualKey = VK_7;
					GLOBAL.Significance = "7";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_8) || ActionListener(VK_NUMPAD8)) {
					GLOBAL.VirtualKey = VK_8;
					GLOBAL.Significance = "8";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_9) || ActionListener(VK_NUMPAD9)) {
					GLOBAL.VirtualKey = VK_9;
					GLOBAL.Significance = "9";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_0) || ActionListener(VK_NUMPAD0)) {
					GLOBAL.VirtualKey = VK_0;
					GLOBAL.Significance = "0";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_RETURN)) {
					GLOBAL.VirtualKey = VK_RETURN;
					GLOBAL.Significance = "RETURN";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_SPACE)) {
					GLOBAL.VirtualKey = VK_SPACE;
					GLOBAL.Significance = " ";
					GLOBAL.isPressed = true;
				}
				else if(ActionListener(VK_BACK)) {
					GLOBAL.VirtualKey = VK_BACK;
					GLOBAL.Significance = "BACK";
					GLOBAL.isPressed = true;
				}
			}
			/// Regular Movements for FROGGER
		    else {

			if (ActionListener(VK_LEFT) || ActionListener(VK_A)) {
				GLOBAL.VirtualKey = VK_LEFT;
				GLOBAL.Significance = "LEFT";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_RIGHT) || ActionListener(VK_D)) {
				GLOBAL.VirtualKey = VK_RIGHT;
				GLOBAL.Significance = "RIGHT";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_UP) || ActionListener(VK_W)) {
				GLOBAL.VirtualKey = VK_UP;
				GLOBAL.Significance = "UP";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_DOWN) || ActionListener(VK_S)) {
				GLOBAL.VirtualKey = VK_DOWN;
				GLOBAL.Significance = "DOWN";
				GLOBAL.isPressed = true;
			}
			
			}
		}
		Sleep(15);
	}
}
#pragma endregion
/// AsynKeyState
bool ActionListener(int VirtualKey) {
	return ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
}