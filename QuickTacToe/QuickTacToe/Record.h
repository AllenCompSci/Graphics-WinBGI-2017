#include "GlobalVars.h"

struct pass {
	int VirtualKey;
	bool isPressed;
	POINT click;
	void reset() {
		isPressed = false;
		VirtualKey = NULL;
		click.x = NULL;
		click.y = NULL;
	}
	void getClick() {
		GetCursorPos(&click);
	}
}Input;
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
	BUTTON thisButton;
	int color;
	int value;
	void create(int x1, int y1) {
		color = 0;
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
		thisButton.init(left, top, right, bottom);
		draw();
	}
	void assignVAL(int VAL) {
		value = VAL;
	}
	void draw() {
		switch (currValue) {
		case O:
			drawO();
			return;
		case X:
			drawX();
			return;
		case Empty:
			setcolor(BKGRNCOLOR);
			bar(left + 2, top + 2, right - 2, bottom - 2);
			if (value != -1) {
				setcolor(WHITE);
				settextstyle(4, 0, 7);
				string temp = to_string(value);
				if (value == 99) {
					temp = "&";
				}
				if (value == 100) {
					temp = "S";
				}
				int x = left + (right - left - textwidth(temp.c_str())) / 2;
				int y = top + (bottom - top - textheight(temp.c_str())) / 2;
				outtextxy(x, y, temp.c_str());
				putpixel(0, 0, 0);
			}

			return;
		}
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
		setcolor(OColor);
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
		setcolor(XColor);
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
		floodfill(collection[x].x, collection[x].y, XColor);
	}
	bool isPressed(int VAL) {
		return currValue == Empty && VAL == value;
	}
	bool isClicked(POINT t) {
		return currValue == Empty && thisButton.isPressed(t);
	}
	void SWITCH() {
		if (value != -1) {
			value = -1;
			draw();
			currValue = TURN;
			if (TURN == X) {
				color = XColor;
			}
			else {
				color = OColor;
			}
		}
		draw();
	}
}test;
struct Grid {
	SPACE board[SQR_GRID][SQR_GRID];
	struct GRIDLINE {
		int top, left, right, bottom;
		void init(int l, int t, bool UP, int GRIDLENGTH, int PADDING) {
			left = l;
			top = t;
			if (UP) {
				bottom = top + GRIDLENGTH;
				right = left + PADDING;
			}
			else {
				bottom = top + PADDING;
				right = left + GRIDLENGTH;
			}
			draw();
		}
		void draw() {
			setcolor(WHITE);
			bar(left, top, right, bottom);
		}
	}gridlines[4];
	int screenX;
	int screenY;
	int GRIDLENGTH;
	int PADDING;
	void init() {
		screenX = getmaxx();
		screenY = getmaxy();
		PADDING = 10;
		GRIDLENGTH = (3 * (3 * UNIT) + 2 * PADDING);
		int topGrid = (screenY - GRIDLENGTH) / 2;
		int leftGrid = (screenX - GRIDLENGTH) / 2;
		gridlines[0].init(leftGrid, topGrid + ((3 * UNIT)), false, GRIDLENGTH, PADDING);
		gridlines[1].init(leftGrid + ((3 * UNIT)), topGrid , true, GRIDLENGTH, PADDING);
		gridlines[2].init(leftGrid + ((6 * UNIT) + PADDING), topGrid, true, GRIDLENGTH, PADDING);
		gridlines[3].init(leftGrid, topGrid + ((6 * UNIT) + PADDING), false, GRIDLENGTH, PADDING);
		int VALUE = 1;
		for(int i = 0; i < SQR_GRID; i++)
			for (int j = 0; j < SQR_GRID; j++)
			{
				board[i][j].assignVAL(VALUE++);
				board[i][j].create(leftGrid + (j * ((3 * UNIT) + PADDING)), topGrid + (i * ((3 * UNIT) + PADDING)));
			}

	}
	bool check() {
		bool DIAGONAL = board[1][1].currValue == TURN;
		if (DIAGONAL) {
			if (board[0][0].currValue == board[2][2].currValue && board[0][0].currValue == TURN) {
				return true;												 
			}																 
			if (board[2][0].currValue == board[0][2].currValue && board[2][0].currValue == TURN) {
				return true;
			}
		}
		bool HORIZONTAL = true;
		bool VERTICAL = true;
		for (int i = 0; i < SQR_GRID; i++) {
			HORIZONTAL = board[0][i].currValue == TURN;
			VERTICAL = board[i][i].currValue == TURN;
			for (int j = 1; HORIZONTAL && j < SQR_GRID; j++) {
				if (board[i][j].currValue != TURN)
					HORIZONTAL = false;
			}
			for (int j = 0; VERTICAL && j < SQR_GRID; j++) {
				if (board[j][i].currValue != TURN)
					VERTICAL = false;
			}
			if (HORIZONTAL || VERTICAL)
				return true;
		}
		return false;
	}
	void ENDTurn() {
		if (TURN == X) {
			TURN = O;
		}
		else {
			TURN = X;
		}
	}
}grid;