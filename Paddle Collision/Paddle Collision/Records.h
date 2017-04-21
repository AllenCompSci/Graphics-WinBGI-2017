#include "LoadBMP.h"
enum GAMESTATE { START_MENU, PLAYER_1, GAME, PAUSE, END, ESCAPE };
enum DIRECTION { UP, RIGHT, DOWN, LEFT, FIRE, LEFTCLICK, RIGHTCLICK, NONE };
enum WIN { CONT, LOSE, TIE };
enum COLLISION { Left_Side, Far_Left, Mid_Left, Mid, Mid_Right, Far_Right, Right_Side, Block_Top, Block_Bottom };
enum BlockType { Healthy, Pitted, Cracked, Destroyed, Indestructable, PADDLE };
enum POWERUP { Regular, Multiball, ShortPaddle, LongPaddle, Small, Large };
const int BLOCKUNIT = 40;
const int RegularBallRadius = BLOCKUNIT / 2;
const int SmallBallRadius = BLOCKUNIT / 3;
const int LargeBallRadius = (int)(BLOCKUNIT / 1.25);
const int PaddleSpeed = 8;
bool debug = false;
HWND stealth; //creating stealth (window is not visible)
//HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
struct pass {
	int VirtualKey;
	bool isPressed;
	bool isHeld;
	DIRECTION move;
	void reset() {
		isPressed = false;
		isHeld = false;
	}
}playerInput1, mouseInput;
struct Ball {
	int x, y, radius, color;
	int dx, dy;
	POWERUP grabbed;
	void init(int left, int top, int w) { // Take Left and Top from Block
		color = LIGHTGRAY;
		radius = RegularBallRadius;
		x = left + w / 2;
		y = top - radius - 1;
		dx = 0;
		dy = -10;
	}
	void init(Ball center, bool Left) {
		color = LIGHTGRAY;
		radius = center.radius;
		if (Left)
			x = (int)(center.x - (radius * 2.5));
		else
			x = (int)(center.x + (radius * 2.5));
		y = center.y;
		dx = center.dx;
		dy = center.dy;
	}
	void update() {
		x += dx;
		y += dy;
	}
	void remove() {
		setcolor(BLACK);
		fillellipse(x, y, radius, radius);
	}
	void draw() {
		setcolor(color);
		fillellipse(x, y, radius, radius);
	}
	void tick() {
		remove();
		update();
	}
}temp;
struct BLOCK {
	BlockType TYPE;
	int top, left;
	int h, w;
	int color;
	bool damaged;
	int collisionTIME;
	int RandomX, RandomY;
	void init(BlockType create, int x, int y, int VALUE) {
		RandomX = -1;
		RandomY = -1;
		collisionTIME = 0;
		top = y;
		left = x;
		TYPE = create;
		h = BLOCKUNIT;
		w = h * 3;
		color = VALUE;
		if (TYPE == PADDLE) {
			w = h * 5;
			h /= 2;
		}

	}
	void draw() {
		setcolor(color);
		bar(left, top, left + w, top + h);
		if (TYPE == Indestructable) {
			setcolor(DARKGRAY);
			bar(left, top, left + w, top + h);
			setcolor(WHITE);
			int i = 1;
			for(; (top +i + 10) < (top + h -i); i += 3)
				rectangle(left+i, top+i, left + w-i, top + h-i);
			i -= 3;
			int rds = h - (2 *i);
			rds /= 2;
			int hp = (int)(5 * (collisionTIME / 300.0));
			int adjust = 5;
			for (int j = left + i + (rds * 2); j < (left + w - i - (rds * 2)) ; j += (rds * 2)) {
				if (adjust-- == hp) {
					setcolor(BLACK);
				}
				fillellipse(j + rds, top + i + rds, rds, rds);
			}
		
		}
		switch (TYPE) {
		case Pitted:
			if (RandomX == -1) {
				POINT temp = pitBrick(left, top, left + w, top + h);
				RandomX = temp.x;
				RandomY = temp.y;
			}
			else {
				pitBrick(left, top, left + w, top + h, RandomX, RandomY);
			}
			break;
		case Cracked:
			if (RandomX == -1) {
				POINT temp = crackBrick(left, top, left + w, top + h);
				RandomX = temp.x;
				RandomY = temp.y;
			}
			else {
				crackBrick(left, top, left + w, top + h, RandomX, RandomY);
			}
			break;
		case Indestructable:

			break;
		case Destroyed:
			setcolor(BLACK);
			bar(left, top, left + w, top + h);
		}
	}
	bool hit(Ball curr) {
		int r = curr.radius;
		int L = left - r;
		int R = left + w + r;
		int T = top - r;
		int B = top + r + h;
		int Xc = curr.x;
		int Yc = curr.y;
		if (L <= Xc && Xc <= R) {
			if (T <= Yc && Yc <= B) {
			
				damaged = true;
				collisionTIME++;
				if (debug) {
					cout << "Ball X : " << Xc << "\nBall Y : " << Yc << "\nBall R : " << r << endl;
					cout << "Block Top : " << top << "\nBlock Left : " << left << "\nBlock h : " << h << "\nBlock w : " << w << endl;
					cout << "Block  T  : " << T << "\nBlock  L   : " << L << "\nBlock R : " << R << "\nBlock B : " << B << endl;
					//system("pause");
				}
			}
		}

		if ((TYPE == PADDLE || TYPE == Indestructable) && damaged) {
			damaged = false;
			if (collisionTIME % 30 == 0)
				draw();
			if (collisionTIME > 300 && TYPE == Indestructable) {
				TYPE = Healthy;
				collisionTIME = 0;
			}
			if(debug)
			cout << "PADDLE HIT" << endl;
			return true;
		}
		return damaged;
	}
	void update() {
		if (damaged) {
			switch (TYPE) {
			case Healthy:
				TYPE = Pitted;
				collisionTIME = 0;
				break;
			case Pitted:
				TYPE = Cracked;
				RandomX = -1;
				RandomY = -1;
				collisionTIME = 0;
				break;
			case Cracked:
				TYPE = Destroyed;
				break;
			}
			draw();
			damaged = false;
		}


	}
	void MOVEPADDLE(int XPadding) {
		int BLACKLEFT, BLACKRIGHT;
		bool render = false;
		if (playerInput1.isPressed || playerInput1.isHeld) {
			int multiplier = 1;
			if (playerInput1.isHeld) {
				multiplier *= 2;
				playerInput1.isHeld = false;
			}
			if (playerInput1.move == LEFT) {
				BLACKRIGHT = left + w; 
				left -= PaddleSpeed * multiplier;
				if (left < XPadding) {
					left = XPadding;
				}
				BLACKLEFT = left + w;
				render = true;
			}
			else if (playerInput1.move == RIGHT) {
				BLACKLEFT = left;
				left += PaddleSpeed * multiplier;
				if (left + w > getmaxx() - XPadding) {
					left = getmaxx() - XPadding - w;
				}
				BLACKRIGHT = left;
				render = true;
			}
			if (render) {
				draw();
				setcolor(BLACK);
				bar(BLACKLEFT, top, BLACKRIGHT, top + h);
				playerInput1.move = NONE;
				playerInput1.isPressed = false;
			}
			
		}
	}
}PONG, general;
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
}Playerselect[6];
struct PLAYERPOS {
	int maxR;
	int maxC;
	int color;
	int row;
	int col;
	pass *listener;
	int unit;
	char C;
	void init(int startR, int startC, int unitSize, pass *input, int COLOR, int row1, int col1) {
		color = COLOR;
		row = startR;
		col = startC;
		unit = unitSize;
		listener = input;
		maxR = row1;
		maxC = col1;
		switch (color)
		{
		case BLUE:
			C = 'B';
			break;
		case RED:
			C = 'R';
			break;
		case GREEN:
			C = 'G';
			break;
		case YELLOW:
			C = 'Y';
			break;
		case MAGENTA:
			C = 'M';
			break;
		}
		render();
	}
	void tick() {
		switch (listener->move) {
		case UP:
			row--;
			return;
		case DOWN:
			row++;
			return;
		case LEFT:
			col--;
			return;
		case RIGHT:
			col++;
			return;
		}
	}
	void render() {
		setcolor(color);
		bar(col*(unit + 1), row*(unit + 1), (col + 1)*(unit + 1), (row + 1)*(unit + 1));
	}
	WIN collide(PLAYERPOS opponent) {
		if (maxR < row || row < 0 || maxC < col || col < 0) {
			return LOSE;
		}
		if (opponent.row == row && opponent.col == col) {
			return TIE;
		}
		if (pix(opponent)) {
			return LOSE;
		}
		return CONT;
	}
	bool pix(PLAYERPOS opponent) {
		int X = (col) * (unit + 1) + (unit + 1) / 2;
		int Y = (row) * (unit + 1) + (unit + 1) / 2;
		return getpixel(X, Y) == color || getpixel(X, Y) == opponent.color;
	}
	void state() {
		cout << "row : " << row << " col : " << col << endl;
	}
}readyPlayer1, readyPlayer2;
struct ButtonHighlight {
	int Left, Top, Right, Bottom;
	int padding;
	BUTTON inner;
	void init(int x) {
		padding = x / 2;
	}
	void pick(BUTTON t) {
		inner.init(t);
		Left = t.Left - padding;
		Right = t.Right + padding;
		Top = t.Top - padding;
		Bottom = t.Bottom + padding;
	}
	void draw() {
		render(WHITE);
	}
	void remove() {
		render(BLACK);
	}
	void render(int color) {
		setcolor(color);
		rectangle(Left, Top, Right, Bottom);
		rectangle(inner.Left, inner.Top, inner.Right, inner.Bottom);
		floodfill(Left + 1, Top + 1, color);

	}
}selected;