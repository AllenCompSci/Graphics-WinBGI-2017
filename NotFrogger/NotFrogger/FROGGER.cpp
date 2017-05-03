/*
Frogger Games Clean
*/

#include "LoadBMP.h"
#include <winbgi.cpp>
#include <graphics.h>
#include <thread>
#include "Pixel.h"
#include "VirtualKeyList.h"

void Listener();
bool ActionListener(int);
string StringBuilder();
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode);
void game();
void introColorSet(char value);
void INTRO();
void QuestLog();
void RIVER();
void SIDEWALK();
void SAFEZONE();
void LILYPAD();
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
bool isRunning = true;
bool isName = false;
int UNIT = 40;
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
///          Safe   Car1    Car2   Car3    Safe   Log1   Log2     Log3   Log4   LilyPad
enum Column {First, Second, Third, Fourth, Fifth, Sixth, Seventh, Eight, Ninth, Tenth, Intro};
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
		MOVEMENT = 5;
		left = x;
		right = x+80;
		top = y;
		bottom = y+80;
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
			draw();
			do { Sleep(14); } while (ActionListener(GLOBAL.VirtualKey));
			GLOBAL.resetKey();
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
		setcolor(LIGHTGRAY);
		bar(left, top, right, bottom);
		drawLines();
	}
	void drawLines() {
		setcolor(YELLOW);
		bar(firstStripeX, top, firstStripeX + UNIT, bottom);
		bar(secondStripeX, top, secondStripeX + UNIT, bottom);
		setcolor(LIGHTGRAY);
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

};
struct Lilypad {
	int top;
	int left;
	int right;
	int bottom;
	bool container;
	void create(int x, int y) {
		left = x*UNIT;
		right = left+80;
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
			rectangle(left+i, top+i, right-i, bottom-i);
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
					if(yellowgreen)
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
int main() {
	//CREATE("frog.bmp"); // To Create Froger PixelMap
	//CREATE("MAINSCREEN.bmp"); // To Create WiseFrog PixelMap
	thread FROGGER(game);
	thread LISTENER(Listener);
	FROGGER.join();
	LISTENER.join();

	system("pause");


}
void game() {

	gr_Start(GrDriver, GrMode, ErrorCode);
	bool debug = true;
	maxX = getmaxx();
	maxY = getmaxy();
	if (!debug) {
		INTRO();
		QuestLog();
		string name = StringBuilder();
		cleardevice();
		settextstyle(0, 3, 72);
		/// Draws CHAR NAME ON SIDE GETS RID OF UNUSED SPACE
		outtextxy(43 * UNIT, 5 * UNIT + textwidth(name.c_str()), name.c_str());
	}
	
	/// Setup Side Menu
	notfrogger.create( (int)(1.7 * UNIT), 3 * UNIT);
	
	/// GAME SETUP
	RIVER(); 
	SIDEWALK();
	SAFEZONE();
	LILYPAD();
	ROAD.init(); /// Possibly need to redraw Lines when Frogger Jumps

	/// WHOLE CREATION OF FROG
	frog.create(220, 600);
	frog.currColumn = First;
	while (isRunning) {

		frog.tick();
		Sleep(15);

	}
	getch();
	


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
	/*
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
	*/
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
	int left, top, right, bottom;
	left = top = right = bottom = 0;
	int OFFSET = UNIT * 2;
	settextstyle(0, 0, 72);
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
			setcolor(BLACK);
			bar(left, top, right, bottom);
			setcolor(WHITE);
			left = (maxX - textwidth(NAME.c_str())) / 2;
			right = left + textwidth(NAME.c_str());
			top = (maxY - OFFSET - textheight(NAME.c_str())) / 2;
			bottom = top + textheight(NAME.c_str());
			outtextxy(left, top, NAME.c_str());
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
void RIVER() {
	setcolor(BLUE);
	bar(24 * UNIT, 0, 42 * UNIT, maxY);
}
void SIDEWALK() {
	setcolor(DARKGRAY);
	bar(20 * UNIT, 0, 24 * UNIT, maxY);
}
void SAFEZONE() {
	setcolor(DARKGRAY);
	bar(5 * UNIT, 0, 8 * UNIT, maxY);
}
void LILYPAD() {
	pads[0].create(37, 7);
	pads[1].create(37, 11);
	pads[2].create(37, 15);
	pads[3].create(37, 19);
}
void introColorSet(char value) {
	switch (value) {
	case 'B':
		setcolor(BLACK);
		break;
	case 'W':
		setcolor(WHITE);
		break;
	case 'G':
		setcolor(GREEN);
		break;
	case 'Y':
		setcolor(YELLOW);
		break;
	}
}
void INTRO(){
	setcolor(RED);
	bar(0, 0, maxX, maxY);
	int Sx = (maxX - (17 * UNIT * 2)) / 2;
	int Sy = (maxY - (12 * UNIT * 2)) / 2;
	bool yg = false;
	for (int i = 0; i < Title_Height; i++) {
		for (int j = 0; j < Title_Width; j++) {
			if (Title[i][j] != 'D') {
				if (Title[i][j] != 'Y') {
					introColorSet(Title[i][j]);
					bar(Sx + (j * 2 * UNIT), Sy + (i * 2 * UNIT), Sx + (j * 2 * UNIT) + (2 * UNIT), Sy + (i * 2 * UNIT) + (2 * UNIT));
					bar(maxX - (Sx + (j * 2 * UNIT) + (2 * UNIT)), Sy + (i * 2 * UNIT), maxX - (Sx + (j * 2 * UNIT)), Sy + (i * 2 * UNIT) + (2 * UNIT));
				}
				else {
					for (int k = Sx + (j * 2 * UNIT); k <= Sx + (j * 2 * UNIT) + (2 * UNIT); k++) {
						for (int l = Sy + (i * 2 * UNIT); l <= Sy + (i * 2 * UNIT) + (2 * UNIT); l++) {
							if (yg)
							{
								putpixel(k, l, YELLOW);
								putpixel(maxX - (k), (l), GREEN);
							}
							else {
								putpixel(k, l, GREEN);
								putpixel(maxX - (k), (l), YELLOW);
							}
							yg = !yg;
								
						}
					}
				}
				Sleep(10);
			}
		}
	}
	Sleep(200);
	GLOBAL.reset();
	do {
		Sleep(10);
	} while (GLOBAL.isClicked == false);
	GLOBAL.reset();
	cleardevice();
}
void QuestLog() {
	string Dialogue[] = {
		"RIBIT! Watch it!",
		"Oh, you there. . .",
		"Where are you headed?",
		" . . . ",
		"You don't say!?",
		"Once I was your age",
		"I jumped everywhere",
		"Many, many places",
		"Jumped over things",
		"Jumped around them",
		"Jumped over them",
		"Jumped under . . .",
		"but today. . .",
		"Well old frogs",
		"we could never make it.",
		"The world,",
		"it's a changing!",
		"Cars! Trucks!",
		"Why some of my best",
		"friends never made it",
		"to the Lilly Pads",
		"YOU go out there and", 
		"Show the world what",
		"Frogs are really made of",
		"Try not to get smashed,",
		"and make a name for yourself",
		"Say. . . ",
		"What was your name?",
		" "
	};
	notfrogger.create(maxX / 2, maxY / 2 + 35);
	int x = 0;
	int y = notfrogger.bottom - 80;
	int limitX = notfrogger.left - 130;
	int sleepTimer = 350;
	frog.create(x, y);
	Sleep(sleepTimer);
	int addition = limitX / 5;
	for (int i = 0; i < 5; i++) {
		frog.remove();
		if (i % 2 == 0) {
			frog.repos((i+1)*addition, y - 40);
		}
		else {
			frog.repos((i+1)*addition, y);
		}
		Sleep(sleepTimer);
	}
	frog.remove();
	frog.repos(limitX, y);
	string NAME;
	int left, top, right, bottom;
	left = top = right = bottom = 0;
	int OFFSET = UNIT * 2;
	settextstyle(0, 0, 72);
	for (int i = 0; i < sizeof(Dialogue)/sizeof(string); i++) {
		NAME = "";
		for (int j = 0; j < (int) Dialogue[i].length(); j++) {
			NAME += Dialogue[i][j];
			setcolor(BLACK);
			bar(left, top, right, bottom);
			setcolor(WHITE);
			left = (maxX - textwidth(NAME.c_str())) / 2;
			right = left + textwidth(NAME.c_str());
			top = (maxY - OFFSET - textheight(NAME.c_str())) / 2;
			bottom = top + textheight(NAME.c_str());
			outtextxy(left, top, NAME.c_str());
			Sleep(sleepTimer);
		}
		Sleep(1000);
	}
	Sleep(1000);
}