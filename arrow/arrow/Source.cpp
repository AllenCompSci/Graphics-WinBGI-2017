#include <iostream>
#include <winbgi.cpp>
#include <graphics.h>
#include <ctime>
#include <vector>
#include <Windows.h>
using namespace std;
int maxX, maxY, ErrorCode, GrDriver, GrMode;
enum Direction {LEFT, DOWN, RIGHT, UP};
enum COLUMN {RED_COL, YELLOW_COL, GREEN_COL, BLUE_COL};
const int UNIT = 8;
int COLUMNSHIFT = UNIT * 33;
int colcolor = 0;
struct ARROW {
	Direction points;
	int X;
	int Y;
	int dXCoors[7];
	int dYCoors[7];
	int color;
	void init(Direction arrow, COLUMN col) {
		color = 15;
		Y = 0;
		X = 0;
		switch (col) {
		case BLUE_COL:
			X += 8 * UNIT;
		case GREEN_COL:
			X += 8 * UNIT;
		case YELLOW_COL:
			X += 8 * UNIT;
		case RED_COL:
			X += COLUMNSHIFT;
		}
		// creates a list of the change in the X and Y as an array
		for (int i = 0; i < 7; i++) {
			dXCoors[i] = dYCoors[i] = UNIT;
			// BASELINE REFERENCE EVERYTHING IN TERMS OF UNIT
		}
		switch (arrow) {
			/// EVERY dXCoor[0] is one unit shifted so any one unit shifts are unnecessary to express
		case LEFT:
#pragma region LEFT
			// skipped
			dYCoors[0] *= 4; 
			dXCoors[1] *= 3;
			// skipped
			dXCoors[2] *= 3;
			dYCoors[2] *= 3;
			dXCoors[3] *= 7;
			dYCoors[3] *= 3;
			dXCoors[4] *= 7;
			dYCoors[4] *= 5;
			dXCoors[5] *= 3;
			dYCoors[5] *= 5;
			dXCoors[6] *= 3;
			dYCoors[6] *= 7;
#pragma endregion
			break;
		case DOWN:
#pragma region DOWN
			// skipped
			dYCoors[0] *= 5;
			dXCoors[1] *= 3;
			dYCoors[1] *= 5;
			dXCoors[2] *= 3;
			dYCoors[2] *= 2;
			dXCoors[3] *= 5;
			dYCoors[3] *= 2;
			dXCoors[4] *= 5;
			dYCoors[4] *= 5;
			dXCoors[5] *= 7;
			dYCoors[5] *= 5;
			dXCoors[6] *= 4;
			dYCoors[6] *= 7;
#pragma endregion
			break;
		case RIGHT:
#pragma region RIGHT
			// skipped
			dYCoors[0] *= 3;
			dXCoors[1] *= 5;
			dYCoors[1] *= 3;
			dXCoors[2] *= 5;
			// skipped
			dXCoors[3] *= 7;
			dYCoors[3] *= 4;
			dXCoors[4] *= 5;
			dYCoors[4] *= 7;
			dXCoors[5] *= 5;
			dYCoors[5] *= 5;
			// skipped
			dYCoors[6] *= 5;
#pragma endregion
			break;
		case UP:
#pragma region UP
			// skipped
			dYCoors[0] *= 3;
			dXCoors[1] *= 4;
			// skipped
			dXCoors[2] *= 7;
			dYCoors[2] *= 3;
			dXCoors[3] *= 5;
			dYCoors[3] *= 3;
			dXCoors[4] *= 5;
			dYCoors[4] *= 7;
			dXCoors[5] *= 3;
			dYCoors[5] *= 7;
			dXCoors[6] *= 3;
			dYCoors[6] *= 3;
#pragma endregion
			break;
		}
		render();
	}
	void render() {
		setcolor(color);
		for (int i = 0; i < 7; i++) {
			line(X + dXCoors[i], Y + dYCoors[i], X + dXCoors[(i + 1)%7], Y + dYCoors[(i + 1)%7]);
		}
		//putpixel(X + 4*UNIT, Y + 4*UNIT, color);
		floodfill(X + 4*UNIT, Y + 4*UNIT, color);
	}
	void remove() {
		int tmp = color;
		color = colcolor;
		render();
		color = tmp;
	}
	void tick() {
		remove();
		Y += 8;
	}
	bool onScreen() {
		return Y + 4 < maxY;
	}
}curr;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode) {
	GrDriver = VGA; //1920 x 1080
	GrMode = VGAMAX;//set graphics mode
	initgraph(&GrDriver, &GrMode, "");//start graphics
									  //check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
	else {
		maxX = getmaxx();
		maxY = getmaxy();
	}
}
vector<ARROW> visual;
void main() {
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);

	while (true) {
		int percent = rand() % 8934;
		if (percent % 4 == 0) {
			percent = rand() % 4;
			Direction point;
			switch (percent) {
			case 0:
				point = LEFT;
				break;
			case 1:
				point = RIGHT;
				break;
			case 2:
				point = DOWN;
				break;
			case 3:
				point = RIGHT;
				break;
			}
			percent = rand() % 100;
			switch (percent) {
			case 23:
			case 24:
				curr.init(point, RED_COL);
				visual.push_back(curr);
				break;
			case 50:
				curr.init(point, YELLOW_COL);
				visual.push_back(curr);
				break;
			case 74:
			case 75:
				curr.init(point, GREEN_COL);
				visual.push_back(curr);
				break;
			case 96:
			case 99:
				curr.init(point, BLUE_COL);
				visual.push_back(curr);
				break;
			}
		}
		for (int i = 0; i < (int)visual.size(); i++) {
			visual.at(i).tick();
			if (!visual.at(i).onScreen()) {
				visual.erase(visual.begin() + i);
				i--;
			}
			else {
				visual.at(i).render();
			}
		}
		Sleep(23);
	}

}