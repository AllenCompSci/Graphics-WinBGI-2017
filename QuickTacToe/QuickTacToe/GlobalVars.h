#include "Libs.h"
const int SQR_GRID = 3;
const int UNIT = 100;
const int BKGRNCOLOR = BLACK;
const int XColor = RED;
const int OColor = BLUE;

enum tictactoe { X, O, Empty };
enum GameState {Start_Menu, Game, GameOver};

tictactoe TURN = X;
GameState GAME = Game;
bool isRunning = true;
bool debug = false;
int GrDriver, GrMode, ErrorCode;
int maxX, maxY;

void gr_Start(int&, int&, int&);
bool ActionPress(int);
void TicTacToe();
void Listener();

