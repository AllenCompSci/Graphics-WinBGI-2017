
#define _CRT_SECURE_NO_WARNINGS
#include <winbgi.cpp>
#include <graphics.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>
#include <ctime>

#define VK_A 0x41// ('A')	A
#define VK_D 0x44// ('D')	D
#define VK_S 0x53// ('S')	S
#define VK_W 0x57// ('W')	W

using namespace std;

const int NUMVECTOR = 18; /// HERE 


int w, h;
bool debug = true;
int GrDriver, GrMode, ErrorCode;
int XBoardOffset;
int YBoardOffset;
int GRIDH;
int GRIDW;
int maxX, maxY;
const int UNIT = 50;
const int BKGRND = BLUE;

enum DIRECTION { Up, Down, Left, Right, Null };
const int apple_Height = 49;
const int apple_Width = 40;
#pragma region APPLE_DRAW 
const int apple_ARRY[49][40] = {

	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 99, 99, 99, 99, 99, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 99, 99, 99, 99, 99, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 99, 99, 99, 99, 99, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 99, 99, 99, 99, 99, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 6, 6, 6, 6, 2, 2, 2, 2, 2, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 4, 4, 4, 4, 4, 13, 15, 15, 15, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 15, 15, 15, 15, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
	{ 99, 99, 99, 99, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 4, 4, 4, 4, 4, 4, 4, 4, 99, 99, 99, 99, 99, 99, 99, 99, 99 },
};

#pragma endregion 


void drawAPPLECENTER() {
	int h = apple_Height;
	int w = apple_Width;
	int Xs = (maxX - w) / 2;
	int Ys = (maxY - h) / 2;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if(apple_ARRY[i][j] != 99)
				putpixel((Xs + j), (Ys + i), apple_ARRY[i][j]);
		}
	}
}
void drawAPPLECENTER(int Xs, int Ys) {
	int h = apple_Height;
	int w = apple_Width;	
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (apple_ARRY[i][j] != 99)
				putpixel((Xs + j), (Ys + i), apple_ARRY[i][j]);
		}
	}
}

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
struct BASICKEY {
	bool isPressed;
	int VirtualKey;
	void reset() {
		VirtualKey = -1;
		isPressed = false;
	}
	bool PRESS(int VK) {
		return (GetAsyncKeyState(VK) & 0x8000) != 0;
	}
	void listen() {
		if (PRESS(VK_LEFT) || PRESS(VK_A)) {
			VirtualKey = VK_LEFT;
		}
		else if (PRESS(VK_RIGHT) || PRESS(VK_D)) {
			VirtualKey = VK_RIGHT;
		}
		else if (PRESS(VK_UP) || PRESS(VK_W)) {
			VirtualKey = VK_UP;
		}
		else if (PRESS(VK_DOWN) || PRESS(VK_S)) {
			VirtualKey = VK_DOWN;
		}

		if (VirtualKey != -1)
			isPressed = true;
	}
}GLOBAL;
struct POS {
	int gridX, gridY;
	void set(int x, int y) {
		gridX = x;
		gridY = y;
	}
	int getX() {
		return UNIT * gridX + XBoardOffset;
	}
	int getY() {
		return UNIT * gridY + YBoardOffset;
	}
	void remove() {
		setcolor(BKGRND);
		bar(getX(), getY(), getX() + 50, getY() + 50);
	}
	void drawSnake() {
		setcolor(GREEN);
		bar(getX(), getY(), getX() + 50, getY() + 50);
	}
	void drawAPPLE() {
		drawAPPLECENTER(getX() + 5, getY());
	}
	bool onBoard() {
		return gridX >= 0 && gridY >= 0 && gridX < GRIDW && gridY < GRIDH;
	}
	bool collide(POS Head) {
		return Head.gridX == gridX && Head.gridY == gridY;
	}
}data;
struct SNAKE {
	vector<POS> timeNull;
	DIRECTION dir;
	POS APPLE;
	bool isAlive;
	void create(int x, int y) {
		isAlive = true;
		POS head;
		head.set(x, y);
		if (debug) {
			int UNFURL = (int)timeNull.size() - 1;
			timeNull.clear();
			for (int i = 0; i < UNFURL; i++)
				timeNull.push_back(head);
		}
		else {
			timeNull.clear();
		}
		timeNull.push_back(head);
		GoldenApple();
		dir = Null;
		sidewinder();
		draw();
	}
	void update() {
		if (GLOBAL.isPressed)
			sidewinder();
		HEADOUT();

	}
	void sidewinder() {
		if (GLOBAL.VirtualKey == VK_LEFT && dir != Right) {
			dir = Left;
		}
		else if (GLOBAL.VirtualKey == VK_RIGHT && dir != Left) {
			dir = Right;
		}
		else if (GLOBAL.VirtualKey == VK_UP && dir != Down) {
			dir = Up;
		}
		else if (GLOBAL.VirtualKey == VK_DOWN && dir != Up) {
			dir = Down;
		}
		GLOBAL.reset();
	}
	POS getHEAD() {
		return timeNull[(int)timeNull.size() - 1];
	}
	void HEADOUT() {
		POS head;
		head.set(getHEAD().gridX, getHEAD().gridY);

		switch (dir) {
		case Up:
			head.gridY -= 1;
			break;
		case Down:
			head.gridY += 1;
			break;
		case Left:
			head.gridX -= 1;
			break;
		case Right:
			head.gridX += 1;
		}
		
		if (!head.onBoard()) {
			if (head.gridX < 0) {
				head.gridX = GRIDW - 1;
			}
			else if (head.gridY < 0) {
				head.gridY = GRIDH - 1;
			}
			else if (head.gridX >= GRIDW) {
				head.gridX = 0;
			}
			else {
				head.gridY = 0;
			}
		}
		removeTail();
		if (Oroboros(head)) {
			timeNull.push_back(head);
			draw();
			if (APPLE.collide(head)) {
				timeNull.push_back(head);
				GoldenApple();
			}
		}
	}
	void GoldenApple() {
		bool isValid = true;
		do {
			isValid = true;
			APPLE.set(rand() % GRIDW, rand() % GRIDH);
			for (int i = 0; isValid && i < (int)timeNull.size(); i++)
				isValid = !timeNull[i].collide(APPLE);
		} while (!isValid);
		APPLE.drawAPPLE();
	}
	void removeTail() {
		timeNull[0].remove();
		timeNull.erase(timeNull.begin() + 0);
	}
	bool Oroboros(POS head) {
		for (int i = 0; isAlive && i < (int)timeNull.size(); i++)
			isAlive = !timeNull[i].collide(head);
		return isAlive;
	}
	void draw() {
		for (int i = 0; i < (int)timeNull.size(); i++)
			timeNull[i].drawSnake();
	}
}Slither;