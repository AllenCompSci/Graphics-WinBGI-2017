#include "Libs.h"
const int SQR_GRID = 3;
const int UNIT = 100;
int BKGRNCOLOR = BLACK;
bool isRunning = true;
bool debug = false;

enum tictactoe{ X, O, Empty};
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;
tictactoe TURN = X;
void gr_Start(int&, int&, int&);

