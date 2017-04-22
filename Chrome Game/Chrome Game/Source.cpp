/*Game Project
Eric Blake
April 19th, 2017*/
#include <iostream>
#include <winbgi.cpp>
#include <graphics.h>
#include <string>
#include <Windows.h> // Sleep()
#include <ctime>
#include <thread>
#include "VirtualKeys.h"

using namespace std;
enum DIRECTION { UP, RIGHT, DOWN, LEFT, FIRE, LEFTCLICK, RIGHTCLICK, NONE };
bool art = false;
class Character {

public:
	int top, left, right, bottom;
	int dy, ndy;
	bool ML, MR, J;
	int row, row2, col, col2;
	int **grid;
	void create(int t, int l, int r, int b) {
		col = b - t;
		ndy = 0;
		col2 = (int)((col+1) / 2);
		row = r - l;
		row2 = (int)((row + 1) / 2);
		grid = new int*[row];
		for (int i = 0; i < row; i++) {
			grid[i] = new int[col];
		}
		top = t;
		left = l;
		right = r;
		bottom = b;
		dy = -9;
		ML = false;
		MR = false;
		J = false;
		draw();
	}
	void jump();
	void fall();
	bool onGround();
	void remove();
	void draw();
	void moveLeft();
	void moveRight();

};
Character mini;
int GrDriver, GrMode, ErrorCode;
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode);
bool KEYBOARD(int VirtualKey);
void game();
void Key();
int main() {
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);

	thread k(Key);
	thread g(game);

	k.join();
	g.join();
	exit(0);
}
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode) {
	//set the graphics driver
	GrDriver = VGA; //800x600 by monitor
	GrMode = VGAMAX;//set graphics mode
	initgraph(&GrDriver, &GrMode, "");//start graphics
									  //check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
}
void Character::draw() {
	if (ndy > 0) {
		jump();
	}
	if(art)
	for (int i = 0; i <= row2; i++)
	{
		for (int j = 0; j <= col2; j++) {
			grid[i][j] = getpixel(left + j, top + i);
			grid[i][col - j - 1] = getpixel(left + col - j - 1, top + 1);
			grid[row - i - 1][j] = getpixel(left + j, top + row - 1 + i);
			grid[row - i - 1][col - j - 1] = getpixel(left + col - j - 1, top + row - 1 + i);
		}
	}
	setcolor(RED);
	bar(left, top, right, bottom);
}
void Character::remove() {
	if(art)
	for (int i = 0; i <= row2; i++)
	{
		for (int j = 0; j <= col2; j++) {
			putpixel(left + j, top + i, grid[i][j]);
			putpixel(left + col - j - 1, top + i, grid[i][col - j - 1]);
			putpixel(left + j, top + row - i - 1, grid[row - i - 1][j]);
			putpixel(left + col - j - 1, top + row - i - 1, grid[row - i - 1][col - j - 1]);
		}
	}
	else {
		setcolor(BLACK);
		bar(left, top, right, bottom);
	}
}
void Character::jump() {
	ndy--;
	Character::top += (Character::dy - 8);
	Character::bottom += (Character::dy - 8);
}
void Character::fall() {
	if (!Character::onGround() && ndy == 0) {
		Character::top -= Character::dy;
		Character::bottom -= Character::dy;
	}
}
void Character::moveLeft() {
	Character::left -= 8;
	Character::right -= 8;
}
void Character::moveRight() {
	Character::left += 8;
	Character::right += 8;
}
bool Character::onGround() {
	/// Check if they are on a platform at all.
	return bottom >= 1000;
}
void Key() {
	while (true) {
		if (KEYBOARD(VK_A)) {
			mini.ML = true;
		}
		if (KEYBOARD(VK_W)) {
			if (mini.onGround())
				mini.J = true;
		}
		if (KEYBOARD(VK_D)) {
			mini.MR = true;
		}
		if (KEYBOARD(VK_SPACE)) {
			if(mini.ndy < 12)
				mini.ndy += 4;
		}

		Sleep(15);
	}
}
void game() {
	Sleep(100);
	mini.create(40, 720, 760, 80);
	while (true) {
		mini.remove();
		mini.fall();
		if (mini.ML) {
			mini.moveLeft();
			mini.ML = false;
		}
		if (mini.MR) {
			mini.moveRight();

			mini.MR = false;
		}
		if (mini.J) {
			mini.ndy = 15;
			mini.J = false;
		}
		mini.draw();
		Sleep(62);
	}
}
bool KEYBOARD(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		int count = 0;
		do {
			if (count++ > 200) {
				return true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);

		return true;
	}
	return false;

}