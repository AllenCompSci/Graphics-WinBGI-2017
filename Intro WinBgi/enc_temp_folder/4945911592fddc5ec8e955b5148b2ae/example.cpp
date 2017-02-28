#include <iostream>
#include <winbgi.cpp>
#include <graphics.h>
#include <string>
#include <Windows.h> // Sleep()
#include <ctime>
using namespace std;
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode);
int setMyColor(int color, int rectColor);
int GrDriver, GrMode, ErrorCode;
int maxX, maxY, penColor; // max X and Y as well as penColor
struct Rect {
	int x, y;
	int length;
	int width;
	int color;
	bool fill;
	void init() {
		color = 0;
		generateRandomRect();
		draw();
	}
	void generateRandomRect() {
		Sleep(10);
		fill = false;
		x = (rand() % (maxX - 50)) + (rand() % 20) + 5;
		y = (rand() % (maxY - 50)) + (rand() % 20) + 5;
		width = (rand() % (maxX - 25 - x)) + (rand() % 20) + 5;
		length = (rand() % (maxY - 25 - y)) + (rand() % 20) + 5;
		color = setMyColor(0, 0);
		cout << "new Rect \n";
	}
	void draw() {
		setcolor(color);
		cout << "Bar : " << getcolor() << endl;
		bar(x, y, x + width, y + length);
		fill = true;
	}
	void outline() {
		setcolor(penColor);
		rectangle(x, y, x + width, y + length);
	}
	void setColor(int c) {
		color = c;
	}
	int getColor() {
		return color;
	}
}myRectangle;
void main() {
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);
	maxX = getmaxx(); // From Graphics
	maxY = getmaxy(); // From Graphics
	myRectangle.init();
	penColor = setMyColor(15, myRectangle.getColor());
	while (true) {
		if (penColor == 0) {
			cleardevice();
			myRectangle.generateRandomRect();
			myRectangle.draw();
			penColor = setMyColor(penColor, myRectangle.getColor());
		}
		myRectangle.outline();
		penColor = setMyColor(penColor, myRectangle.getColor());
		Sleep(32);
	}
}
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode) {
	//set the graphics driver
	GrDriver = VGA; //800x600 
	GrMode = VGAMAX;//set graphics mode
	initgraph(&GrDriver, &GrMode, "Title");//start graphics
	//check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
}
int setMyColor(int color, int rectColor) {
	int val;
	do { val = rand() % 16; } 
	while (val == color || val == rectColor);
	return val;
}
// Less than 90 lines of code.