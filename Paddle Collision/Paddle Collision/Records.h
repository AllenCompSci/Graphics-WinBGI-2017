#include "LoadBMP.h"
#include "Pixel.h"
#include <ctime>

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
	void init(BlockType create, int x, int y, int VALUE) {
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
			rectangle(left, top, left + w, top + h);
		}
		switch (TYPE) {
		case Pitted:
			pitBrick(left, top, left + w, top + h);
			break;
		case Cracked:
			crackBrick(left, top, left + w, top + h);
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
			if (collisionTIME > 300 && TYPE == Indestructable) {
				TYPE = Healthy;
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
				break;
			case Pitted:
				TYPE = Cracked;
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
