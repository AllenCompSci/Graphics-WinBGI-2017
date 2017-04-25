#include "Libs.h"
const int SQR_GRID = 3;
const int UNIT = 100;
int BKGRNCOLOR = BLACK;
bool isRunning = true;
bool debug = false;

enum tictactoe{ X, O, Empty};
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
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
	}
}