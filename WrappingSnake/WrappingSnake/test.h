
#include "draw.h"


void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode) {
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
	else {
		maxX = getmaxx();
		maxY = getmaxy();
		GRIDH = maxY / UNIT;
		GRIDW = maxX / UNIT;
		XBoardOffset = (maxX - (GRIDW * UNIT)) / 2;
		YBoardOffset = (maxY - (GRIDH * UNIT)) / 2;
	} 
}
void Listener() {
	gr_Start(GrDriver, GrMode, ErrorCode);
	srand((unsigned int)time(NULL));
	POS data;
	while (true) {
		setcolor(BLUE);
		bar(XBoardOffset, YBoardOffset, XBoardOffset + (GRIDW * UNIT), YBoardOffset + (GRIDH * UNIT));
		data.set(GRIDW / 2, GRIDH / 2);
		data.drawSnake();
		while (!GLOBAL.isPressed) {
			GLOBAL.listen();
		}
		Slither.create(GRIDW / 2, GRIDW / 2);
		while (Slither.isAlive) {
			GLOBAL.listen();
			Slither.update();
			Sleep(32);
		}
		cleardevice();
		setcolor(BLUE);
		bar(15, 20, getmaxx() - 15, getmaxy() - 20);
		bar(0, 1070, 1920, 0);
		setbkcolor(BLACK);
		drawAPPLECENTER();
		getch();
		for (int i = 0; i < 99; i++) {
			drawAPPLECENTER(rand() % (maxX - apple_Width), rand() % (maxY - apple_Height));
		}
		getch();
		setcolor(GREEN);
		for (int i = 0; i < 99; i++) {
			int left = rand() % (maxX - 50);
			int top = rand() % (maxY - 50);
			bar(left, top, left + 50, top + 50);
		}
		getch();
		getch();
		getch();
		cleardevice();

	}
}
void test() {
	
	/// GAME
	Listener();

	
}