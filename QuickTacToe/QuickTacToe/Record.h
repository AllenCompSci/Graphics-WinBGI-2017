#include "GlobalVars.h"

struct pass {
	int VirtualKey;
	bool isPressed;
	bool isHeld;
	void reset() {
		isPressed = false;
		isHeld = false;
	}
}playerInput1, mouseInput;
struct BUTTON {
	int Left, Top, Right, Bottom;
	void init(int x1, int y1, int x2, int y2) {
		if (x1 < x2) {
			Left = x1;
			Right = x2;
		}
		else {
			Left = x2;
			Right = x1;
		}
		if (y1 < y2) {
			Top = y1;
			Bottom = y2;
		}
		else {
			Top = y2;
			Bottom = y1;
		}
	}
	void init(BUTTON t) {
		Left = t.Left;
		Right = t.Right;
		Top = t.Top;
		Bottom = t.Bottom;
	}
	bool isPressed(POINT p) {
		if (debug) {
			cout << "Point.X : " << p.x << " Point.Y : " << p.y << endl;
			cout << "LEFT  : (" << Left << ")" << (Left <= p.x) << endl;
			cout << "RIGHT : (" << Right << ")" << (p.x <= Right) << endl;
			cout << "TOP   : (" << Top << ")" << (Top <= p.y) << endl;
			cout << "BOTTOM: (" << Bottom << ")" << (p.y <= Bottom) << endl;
		}
		return ((Left <= p.x) && (p.x <= Right)) && ((Top <= p.y) && (p.y <= Bottom));
	}
	void render(int color) {
		setcolor(color);
		bar(Left, Top, Right, Bottom);
	}
	bool isEqual(BUTTON t) {
		return t.Left == Left && t.Right == Right && t.Bottom == Bottom && t.Top == Top;
	}
};
struct SPACE {
	tictactoe currValue;
	int left, top, right, bottom;
	POINT collection[13];
	int color;
	void create(int x1, int y1) {
		currValue = Empty;
		left = x1;
		top = y1;
		right = x1 + UNIT * 3;
		bottom = y1 + UNIT * 3;
#pragma region X
		collection[0].x = left;
		collection[0].y = top + UNIT / 2;
		collection[1].x = left + UNIT / 2;
		collection[1].y = top;
		collection[2].x = right;
		collection[2].y = bottom - UNIT / 2;
		collection[3].x = right - UNIT / 2;
		collection[3].y = bottom;

		collection[4].x = left;
		collection[4].y = bottom - UNIT / 2;
		collection[5].x = left + UNIT / 2;
		collection[5].y = bottom;
		collection[6].x = right;
		collection[6].y = top + UNIT / 2;
		collection[7].x = right - UNIT / 2;
		collection[7].y = top;

		collection[8].x = left + UNIT / 2;
		collection[8].y = top + UNIT / 2;

		collection[9].x = right - UNIT / 2;
		collection[9].y = top + UNIT / 2;

		collection[10].x = left + (3 * UNIT) / 2;
		collection[10].y = top + (3 * UNIT) / 2;

		collection[11].x = left + UNIT / 2;
		collection[11].y = bottom - UNIT / 2;

		collection[12].x = right - UNIT / 2;
		collection[12].y = bottom - UNIT / 2;
#pragma endregion

	}
	void draw(int c) {
		color = c;
		switch (currValue) {
		case O:
			drawO();
			return;
		case X:
			drawX();
			return;
		case Empty:
			setcolor(BKGRNCOLOR);
			bar(left+2, top+2, right-2, bottom-2);
			return;
		}
	}
	void drawO() {
		setcolor(color);
		fillellipse(collection[10].x, collection[10].y, (UNIT * 3) / 2 - 2, (UNIT * 3) / 2 - 2);
		setcolor(BKGRNCOLOR);
		fillellipse(collection[10].x, collection[10].y, ((UNIT * 3) / 2 - 2)/2, ((UNIT * 3) / 2 - 2) / 2);
	}
	void drawLINE(int x, int x0) {
		drawLINE(collection[x], collection[x0]);
	}
	void drawLINE(POINT t, POINT t0) {
		line(t.x, t.y, t0.x, t0.y);
	}
	void drawX() {
		setcolor(color);
		for (int i = 0; i < 8; i += 4) {
			drawLINE(0 + i, 1 + i);
			drawLINE(1 + i, 2 + i);
			drawLINE(2 + i, 3 + i);
			drawLINE(3 + i, 0 + i);
		}
		ff(8);
		ff(9);
		ff(11);
		ff(12);
		ff(10);
	}
	void ff(int x) {
		floodfill(collection[x].x, collection[x].y, color);
	}
}test;
struct Grid {
	SPACE board[SQR_GRID][SQR_GRID];
	int screenX;

}grid;