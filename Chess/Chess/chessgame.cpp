/*
Nathan Varghese
Brian Nguyen
Final Project
Start : 04/06/17
Due : 05/31/17

In this project, we will be creating a chess game.

*/

// Libraries
#define _CRT_SECURE_NO_WARNINGS /// Necessary for fopen()
#include <iostream>
#include <ctime>
#include <winbgi.cpp>
#include <Windows.h>
#include <graphics.h>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include "VirtualKeyList.h"
#include "Pieces.h" // Contains all of the Pieces arrays
using namespace std;

// Enums
enum PIECE {
	King, Queen, Bishop, Knight, Rook, Pawn, Empty
};
enum PIECECOLOR {
	Black, White, NA
};
enum PAWNSpecial {
	UP, DOWN, NotPawn
};

// Global Variables
int maxX, maxY;
int GrDriver, GrMode, ErrorCode;
const int NUMVECTOR = 3;
int w, h;
const int GRID_side = 100;
int XOffsetBOARD;
int YOffsetBOARD;
bool Design[8][8];
int P1TakenROW = 0;
int P1TakenCOL = -2;
int P2TakenCOL = 9;
int P2TakenROW = 0;
bool isRunning = true;
bool NewGame = true;
bool CHECK = false;
bool MATE = false;
bool EndTurn = true; // Sets Turn to White
POINT TopLeftBoard[8][8];
PIECECOLOR colorBoard[8][8];
PIECECOLOR Turn = NA; // Start off anything but Black for White, and White for Black

/// Color of the Pieces
const int COLOR1 = BLACK;
const int COLOR2 = WHITE;
/// Color of the Board
const int BOARD1 = RED;
const int OTHER1 = WHITE;
const int OTHER2 = DARKGRAY;
// Prototyped Threads
void Chess();
void Listener();
// Prototyped Functions
void setUpBoard();
void updateChessCurrColor();
void move(int, int, int, int);
void every(int ROW, int COL);
void other(int ROW, int COL);
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void board();
void LoadBMP();
bool ActionListener(int VirtualKey);
unsigned char* ReadBMP(char* filename);
string toUpper(string s);
void MOVINGBOARD();
void drawBoard();
// Prototyped Drawing Functions for Pieces
void drawRook(int ROW, int COL, PIECECOLOR player);
void drawKnight(int ROW, int COL, PIECECOLOR player);
void drawBishop(int ROW, int COL, PIECECOLOR player);
void drawQueen(int ROW, int COL, PIECECOLOR player);
void drawKing(int ROW, int COL, PIECECOLOR player);
void drawPawn(int ROW, int COL, PIECECOLOR player);


// User-defined Structures
struct RGB {
	int r;
	int g;
	int b;
	int colorVal;
	void init(int R, int G, int B) {
		r = R;
		g = G;
		b = B;
	}
	void init(int R, int G, int B, int c) {
		r = R;
		g = G;
		b = B;
		colorVal = c;
	}
	bool isEqual(RGB t) {
		return r == t.r && g == t.g && b == t.b;
	}
};
struct boardPiece {
	// Structures
	PIECE currPiece;
	PIECECOLOR currColor;
	PAWNSpecial pawnMove;
	vector<int> MOVEROW;
	vector<int> MOVECOL;
	int index;
	int color;
	int ROW;
	int COL;
	bool hasMoved;
	bool haze;
	bool selected;
	void init(int i, PIECE chess, PIECECOLOR color1, int I, int J) {
		selected = false;
		haze = false;
		hasMoved = false;
		currColor = color1;
		currPiece = chess;
		if (currPiece == Pawn) {
			if (ROW == 6)
				pawnMove = UP;
			else
				pawnMove = DOWN;
		}
		else {
			pawnMove = NotPawn;
		}
		index = i; /// Currently unused, I think this was for vector
		if (currColor == White) {
			color = WHITE;
		}
		else if(currColor == Black){
			color = BLACK;
		}
		ROW = I;
		COL = J;
	}
	void calcMove() {
		MOVEROW.clear();
		MOVECOL.clear();
		bool piece = true; /// SLIDE BOOLEAN
		switch (currPiece) {
		case King:
#pragma region KING_MOVE
			if (ROW > 0) {
				if (colorBoard[ROW - 1][COL] != currColor) {
					add(-1, 0);
				}
				if (COL < 7 && colorBoard[ROW - 1][COL + 1] != currColor) {
					add(-1, 1);
				}
				if (COL > 0 && colorBoard[ROW - 1][COL - 1] != currColor) {
					add(-1, -1);
				}

			}
			if (COL < 7) {
				if(colorBoard[ROW][COL+1]!= currColor)
					add(0, 1);
				if(ROW < 7 && colorBoard[ROW+1][COL + 1] != currColor)
					add(1, 1);
			}
			if (ROW < 7 && colorBoard[ROW + 1][COL] != currColor) {
				add(1, 0);
			}
			if (ROW < 7 && COL > 0 && colorBoard[ROW + 1][COL - 1] != currColor) {
				add(1, -1);
			}
			if (COL > 0 && colorBoard[ROW][COL - 1] != currColor) {
				add(0, -1);
			}
#pragma endregion
			break;
		case Queen: 
		/// QUEEN is a combination of both ROOK and BISHOP MOVESETS
		case Rook:
#pragma region Rook_Move
			piece = true;
			for (int i = 1; COL + i < 8 && piece; i++) { // For positions that slide Example moving Right
				if (colorBoard[ROW][COL + i] != NA) {
					piece = false;
				}
				if(piece)
					add(0, i);
				else{
					if (colorBoard[ROW][COL + i] != currColor) {
						add(0, i);
					}
				}
			}
			piece = true;
			for (int i = 1; ROW - i >= 0 && piece; i++) { // For positions that slide Example moving Up
				if (colorBoard[ROW - i][COL] != NA) {
					piece = false;
				}
				if (piece)
					add(-1 * i, 0);
				else {
					if (colorBoard[ROW - i][COL] != currColor) {
						add(-1 * i, 0);
					}
				}
			}
			piece = true;
			for (int i = 1; COL - i >= 0 && piece; i++) { // For positions that slide Example moving Left
				if (colorBoard[ROW][COL - i] != NA) {
					piece = false;
				}
				if (piece)
					add(0, -1 * i);
				else {
					if (colorBoard[ROW][COL - i] != currColor) {
						add(0, -1 * i);
					}
				}

			}	
			piece = true;
			for (int i = 1; ROW + i < 8 && piece; i++) { // For positions that slide Example moving Down
				if (colorBoard[ROW + i][COL] != NA) {
					piece = false;
				}
				if (piece)
					add(i, 0);
				else {
					if (colorBoard[ROW + i][COL] != currColor) {
						add(i, 0);
					}
				}
			}
#pragma endregion
			if(currPiece == Rook)
				break;
		case Bishop:
#pragma region Bishop_Move
			piece = true;
			for (int i = 1; ROW - i >= 0 && COL + i < 8 && piece; i++) { // For positions that slide Example moving  Up Right 
				if (colorBoard[ROW - i][COL + i] != NA) {
					piece = false;
				}
				if (piece)
					add(-1 * i, i);
				else {
					if (colorBoard[ROW - i][COL + i] != currColor) {
						add(-1 * i, i);
					}
				}

			}
			piece = true;
			for (int i = 1; ROW - i >= 0 && COL - i >= 0 && piece; i++) { // For positions that slide Example moving  Up Left 
				if (colorBoard[ROW - i][COL - i] != NA) {
					piece = false;
				}
				if (piece)
					add(-1 * i, -1 * i);
				else {
					if (colorBoard[ROW - i][COL - i] != currColor) {
						add(-1 * i, -1 * i);
					}
				}

			}
			piece = true;
			for (int i = 1; ROW + i < 8 && COL - i >= 0 && piece; i++) { // For positions that slide Example moving  Down Left 
				if (colorBoard[ROW + i][COL - i] != NA) {
					piece = false;
				}
				if (piece)
					add(i, -1 * i);
				else {
					if (colorBoard[ROW + i][COL - i] != currColor) {
						add(i, -1 * i);
					}
				}
			}
			piece = true;
			for (int i = 1; ROW + i < 8 && COL + i < 8 && piece; i++) { // For positions that slide Example moving  Down Right 
				if (colorBoard[ROW + i][COL + i] != NA) {
					piece = false;
				}
				if (piece)
					add(i, i);
				else {
					if (colorBoard[ROW + i][COL + i] != currColor) {
						add(i, i);
					}
				}

			}
#pragma endregion
			break;
		case Knight:
#pragma region Knight_Move
			if (ROW > 1) {
				if (COL > 0 && colorBoard[ROW - 2][COL - 1] != currColor)
					add(-2, -1);
				if (COL < 7 && colorBoard[ROW - 2][COL + 1] != currColor)
					add(-2, 1);
			}
			if (COL < 6) {

				if (ROW > 0 && colorBoard[ROW - 1][COL + 2] != currColor)
					add(-1, 2);					   
				if (ROW < 7 && colorBoard[ROW + 1][COL + 2] != currColor)
					add(1, 2);
			}
			if (ROW < 6) {
				if (COL > 0 && colorBoard[ROW + 2][COL - 1] != currColor)
					add(2, -1);			  
				if (COL < 7 && colorBoard[ROW + 2][COL + 1] != currColor)
					add(2, 1);
			}
			if (COL > 1) {

				if (ROW > 0 && colorBoard[ROW - 1][COL - 2] != currColor)
					add(-1, -2);
				if (ROW < 7 && colorBoard[ROW + 1][COL - 2] != currColor)
					add(1, -2);
			}
#pragma endregion		
		break;

		case Pawn:
#pragma region Pawn_Move
			if (pawnMove == UP) {
				// Defined as the upwards Pawn in the negative Y direction
				// All pawns can always move up, 
				if (!hasMoved) {
					if (colorBoard[ROW - 1][COL] == NA && colorBoard[ROW - 2][COL] == NA) {
						add(-2, 0);
					}
				}
				if (colorBoard[ROW - 1][COL] == NA)
				{
					add(-1, 0);
				}
				if (COL < 7 && colorBoard[ROW - 1][COL + 1] != currColor && colorBoard[ROW - 1][COL + 1] != NA) {
					add(-1, 1);
				}
				if (COL > 0 && colorBoard[ROW - 1][COL - 1] != currColor && colorBoard[ROW - 1][COL - 1] != NA) {
					add(-1, -1);
				}
			}
			else {
				if (!hasMoved) {
					if (colorBoard[ROW + 1][COL] == NA && colorBoard[ROW + 2][COL] == NA) {
						add(2, 0);
					}
				}
				if (colorBoard[ROW + 1][COL] == NA)
				{
					add(1, 0);
				}
				if (COL < 7 && colorBoard[ROW + 1][COL + 1] != currColor && colorBoard[ROW + 1][COL + 1] != NA) {
					add(1, 1);
				}
				if (COL > 0 && colorBoard[ROW + 1][COL - 1] != currColor && colorBoard[ROW + 1][COL - 1] != NA) {
					add(1, -1);
				}
			}
#pragma endregion
			break;
}

}
	void add(int dR, int dC) {
		Radd(dR);
		Cadd(dC);
	}
	void Radd(int deltaRow) {
		MOVEROW.push_back(ROW + deltaRow);
	}
	void Cadd(int deltaCol) {
		MOVECOL.push_back(COL + deltaCol);
	}
	void render() {
		switch (currPiece) {
		case Pawn:
			drawPawn(ROW, COL, currColor);
			break;
		case Knight:
			drawKnight(ROW, COL, currColor);
			break;
		case Rook:
			drawRook(ROW, COL, currColor);
			break;
		case Bishop:
			drawBishop(ROW, COL, currColor);
			break;
		case King:
			drawKing(ROW, COL, currColor);
			break;
		case Queen:
			drawQueen(ROW, COL, currColor);
			break;
		case Empty:
			remove();
			break;
		}
	}
	void remove() {
		currPiece = Empty;
		currColor = NA;
		if (Design[ROW][COL]) {
			every(ROW, COL);
		}
		else {
			other(ROW, COL);
		}
	}
	void offBoard() {
		if (currColor == NA) {
			return;
		}
		int tempROW = ROW;
		int tempCOL = COL;
		if (currColor == Black) {
			ROW = P1TakenROW++;
			COL = P1TakenCOL;
			if (P1TakenROW == 8) {
				P1TakenCOL--;
				P1TakenROW = 0;
			}
			other(ROW, COL); // DRAW A SQUARE UNDER REMOVED
		}
		else {
			ROW = P2TakenROW++;
			COL = P2TakenCOL;
			if (P2TakenROW == 8) {
				P2TakenCOL++;
				P2TakenROW = 0;
			}
			other(ROW, COL); // DRAW A SQUARE UNDER REMOVED
		}
		render();
		ROW = tempROW;
		COL = tempCOL;
		if (currPiece == King) {
			MATE = true;
			NewGame = true;
		}
		remove();
		
		Sleep(100);
	}
	void isClicked() {
		if (currPiece != Empty) {
			selected = !selected;
			if (selected)
				drawSelect();
			else {
				rerender();

			}
		}
	}
	void rerender() {
		if (Design[ROW][COL]) {
			every(ROW, COL);
		}
		else {
			other(ROW, COL);
		}
		render();
	}
	void drawSelect() {
		setcolor(YELLOW);
		for(int i = 0; i < 5; i++)
		rectangle((XOffsetBOARD + COL * GRID_side) + i, (YOffsetBOARD + ROW * GRID_side) + i, (XOffsetBOARD + COL * GRID_side + GRID_side) - i , (YOffsetBOARD + ROW * GRID_side + GRID_side) - i);
	}
	void select() {
		if (!haze) {
			selectMove();
		}
		else {
			deselectMove();
		}
	}
	void selectMove() {
		haze = true;
		drawHaze();
	}
	void deselectMove() {
		haze = false;
		rerender();
	}
	void drawHaze() {
		setbkcolor(BLACK);
		setcolor(YELLOW);
		bar(XOffsetBOARD + COL * GRID_side, YOffsetBOARD + ROW * GRID_side, XOffsetBOARD + COL * GRID_side + GRID_side, YOffsetBOARD + ROW * GRID_side + GRID_side);
		if(currPiece != Empty)
			render();
		/*
		bool swatch = false;
		for (int ii = 0; ii <= GRID_side; ii++) {
			for (int jj = 0; jj <= GRID_side; jj++) {
				if (swatch)
					putpixel(XOffsetBOARD + COL * GRID_side + ii, YOffsetBOARD + ROW * GRID_side + jj, OTHER2);
				swatch = !swatch;
			}
		}
		*/
	}
	void reset() {
		haze = false;
		selected = false;
		if (Design[ROW][COL]) {
			every(ROW, COL);
		}
		else {
			other(ROW, COL);
		}
		render();
	}
}myboard, CHESS[8][8];
struct SQR {
	int ROW;
	int COL;
	bool valid;
	bool isValidRow() {
		return (ROW < 8 && ROW >= 0);
	}
	bool isValidCol() {
		return (COL < 8 && COL >= 0);
	}
	void isValid() {
		 if(isValidRow() && isValidCol()) {
			valid = true;
		}
		else {
			reset();
		}		
	}
	void reset() {
		valid = false;
		ROW = -1;
		COL = -1;
	}
	bool compare(SQR t) {
		return (t.ROW == ROW && t.COL == COL);
	}
	void store(SQR t) {
		ROW = t.ROW;
		COL = t.COL;
		valid = t.valid;
	}
}Action, Hold;
struct KeyState {
	int VirtualKey;
	bool isAlpha;
	bool isNumeric;
	string Significance;
	bool isPressed;
	POINT Cursor;
	void grid() {
		if (VirtualKey == VK_LBUTTON)
		{
			Action.ROW = (Cursor.y - YOffsetBOARD) / GRID_side;
			Action.COL = (Cursor.x - XOffsetBOARD) / GRID_side;
			Significance = "RETURN";
		}
		if (isAlpha) {
			Action.COL = (int)(Significance[0] - 'A');
			if (!Action.isValidCol()) {
				Action.COL = -1;
			}
			else {
				if (Action.isValidRow()) {
					Action.isValid();
				}
			}
		}
		if (isNumeric) {
			Action.ROW = 7 - (int)(Significance[0] - '1');
			if (!Action.isValidRow()) {
				Action.ROW = -1;
			}
			else {
				if (Action.isValidCol()) {
					Action.isValid();
				}
			}
		}
		if (Significance == "RETURN") {
			Action.isValid();
			if (Action.valid) {
				if (Hold.valid) {
					if (Hold.compare(Action)) {
						CHESS[Hold.ROW][Hold.COL].isClicked();
						for (int i = 0; i < (int)CHESS[Hold.ROW][Hold.COL].MOVECOL.size(); i++) {
							CHESS[CHESS[Hold.ROW][Hold.COL].MOVEROW.at(i)][CHESS[Hold.ROW][Hold.COL].MOVECOL.at(i)].select();
						}
						Hold.reset();
						Action.reset();
					}
					else {
						if (CHESS[Action.ROW][Action.COL].haze) {
							move(Action.ROW, Action.COL, Hold.ROW, Hold.COL);
						}
					}
				}
				else {
					if (CHESS[Action.ROW][Action.COL].currColor == Turn) {
						Hold.store(Action);
						Hold.isValid();
						CHESS[Hold.ROW][Hold.COL].isClicked();
						for (int i = 0; i < (int)CHESS[Hold.ROW][Hold.COL].MOVECOL.size(); i++) {
							CHESS[CHESS[Hold.ROW][Hold.COL].MOVEROW.at(i)][CHESS[Hold.ROW][Hold.COL].MOVECOL.at(i)].select();
						}
					}
					else {
						Action.reset();
					}
				}
			}
		}
		reset();
	}
	void reset() {
		Significance = "";
		isPressed = false;
		VirtualKey = 0;
		Cursor = POINT();
	}
}GLOBAL;
// Global Variables of Struct Types
vector <RGB> colors(NUMVECTOR);
//Main
void main() {
	//LoadBMP();
	
	cout << "WELCOME TO CHESS" << endl;

	/// Define Threads
	thread game(Chess);
	thread listener(Listener);
	/// Join Threads
	game.join();
	listener.join();

	/// THIS WAS FOR TESTING ONLY
	/*
	Sleep(4000);
	for (int i = 0; i < 8; i++) {
		CHESS[0][i].offBoard();
		CHESS[1][i].offBoard();
		CHESS[6][i].offBoard();
		CHESS[7][i].offBoard();

	}
	*/
	/// MAKE SURE TO COMMENT OUT JUST VISUAL (454-463)
	
	system("pause");
	return;
}
void setUpBoard() {
	/*
		{myboard.init(0,Rook, White), myboard.wKnight[0], myboard.wBishop[0], myboard.wQueen, myboard.wKing, myboard.wBishop[1], myboard.wKnight[1], myboard.wRook[1]},		// 1
		{myboard.wPawn[0], myboard.wPawn[1], myboard.wPawn[2], myboard.wPawn[3], myboard.wPawn[4], myboard.wPawn[5], myboard.wPawn[6], myboard.wPawn[7]},			// 2
		{myboard.hold,			myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold},	// 3
		{myboard.hold,			myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold},	// 4
		{myboard.hold,			myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold},	// 5
		{myboard.hold,			myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold,			 myboard.hold},	// 6
		{ myboard.bPawn[0], myboard.bPawn[1], myboard.bPawn[2], myboard.bPawn[3], myboard.bPawn[4], myboard.bPawn[5], myboard.bPawn[6], myboard.bPawn[7]},			// 7
		{ myboard.bRook[0], myboard.bKnight[0], myboard.bBishop[0], myboard.bQueen, myboard.bKing, myboard.bBishop[1], myboard.bKnight[1], myboard.bRook[1]}, 
	};		// 8
*/
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			CHESS[i][j].init(0, Empty, NA, i, j);
			if (i == 1 ){		
				CHESS[i][j].init(j, Pawn, White, i , j);
			}
			if (i == 6) {
				CHESS[i][j].init(j, Pawn, Black, i, j);
			}
		}
	}
	CHESS[0][0].init(0, Rook, White, 0,0);
	CHESS[7][0].init(0, Rook, Black, 7,0);
	CHESS[0][7].init(1, Rook, White, 0,7);
	CHESS[7][7].init(1, Rook, Black, 7,7);
	CHESS[0][1].init(0, Knight, White, 0, 1);
	CHESS[7][1].init(0, Knight, Black, 7, 1);
	CHESS[0][6].init(1, Knight, White, 0, 6);
	CHESS[7][6].init(1, Knight, Black, 7, 6);
	CHESS[0][2].init(0, Bishop, White, 0, 2);
	CHESS[7][2].init(0, Bishop, Black, 7, 2);
	CHESS[0][5].init(1, Bishop, White, 0, 5);
	CHESS[7][5].init(1, Bishop, Black, 7, 5);
	CHESS[0][3].init(0, Queen, White, 0, 3);
	CHESS[7][3].init(0, Queen, Black, 7, 3);
	CHESS[0][4].init(0, King, White, 0, 4);
	CHESS[7][4].init(0, King, Black, 7, 4);
	for (int i = 0; i < 8; i++) {
		CHESS[0][i].render();
		CHESS[1][i].render();
		CHESS[6][i].render();
		CHESS[7][i].render();
	}
}
void drawBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
			CHESS[i][j].render();
	}
}
void MOVINGBOARD() {
	XOffsetBOARD = (maxX - GRID_side * 8) / 2;
	YOffsetBOARD = (maxY - GRID_side * 8) / 2;
	bool toggle = true;
	for (int j = 0; j < 8; j++) { // ROW
		for (int i = 0; i < 8; i++) { // COL
			if (toggle) {
				every(j, i);
			}
			else {
				other(j, i);
			}
			Design[j][i] = toggle;
			toggle = !toggle;
		}
		toggle = !toggle;
	}
	for (int i = 0; i < 3; i++) {
		setcolor(WHITE);
		rectangle(XOffsetBOARD - 1 - (i * 2), YOffsetBOARD - 1 - (i * 2), XOffsetBOARD + 8 * GRID_side + 1 + (i * 2), YOffsetBOARD + 8 * GRID_side + 1 + (i * 2));
		setcolor(BLACK);
		rectangle(XOffsetBOARD - (i * 2), YOffsetBOARD - (i * 2), XOffsetBOARD + 8 * GRID_side + (i * 2), YOffsetBOARD + 8 * GRID_side + (i * 2));
	}
}
void move(int toRow, int toCol, int fromRow, int fromCol) { /// EVERY MOVE EXCEPT FOR THE SPECIAL SWAPS like Queen Side Castle etc 
	CHESS[toRow][toCol].offBoard();
	CHESS[toRow][toCol].init(CHESS[fromRow][fromCol].index, CHESS[fromRow][fromCol].currPiece, CHESS[fromRow][fromCol].currColor, toRow, toCol);
	CHESS[toRow][toCol].hasMoved = true;
	if (CHESS[fromRow][fromCol].currPiece == Pawn) {
		CHESS[toRow][toCol].pawnMove = CHESS[fromRow][fromCol].pawnMove;
	}
	CHESS[fromRow][fromCol].remove();
	if ((toRow == 0 || toRow == 7) && CHESS[toRow][toCol].currPiece == Pawn) {
		CHESS[toRow][toCol].currPiece = Queen;
	}
	/// HERE IF THE CHESS[toRow][toCol] contains a Pawn that has reached the end of the board GIVE CHOICE OF NEW PIECE ***** 
	CHESS[toRow][toCol].rerender();
	EndTurn = true;
}
void updateChessCurrColor()
{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			colorBoard[i][j] = CHESS[i][j].currColor;
		}
	}
}
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode) {
	// Set the graphics driver
	GrDriver = VGA; // 800x600 by monitor
	GrMode = VGAMAX; // set graphics mode
	initgraph(&GrDriver, &GrMode, ""); // start graphics
									   // check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
}
///Old Board Design
void board() {
	// ROW 1
	bar(0, 0, 100, 100);
	rectangle(100, 0, 200, 100);
	bar(200, 0, 300, 100);
	rectangle(300, 0, 400, 100);
	bar(400, 0, 500, 100);
	rectangle(500, 0, 600, 100);
	bar(600, 0, 700, 100);
	rectangle(700, 0, 800, 100);

	// ROW 2
	rectangle(0, 100, 100, 200);
	bar(100, 100, 200, 200);
	rectangle(200, 100, 300, 200);
	bar(300, 100, 400, 200);
	rectangle(400, 100, 500, 200);
	bar(500, 100, 600, 200);
	rectangle(600, 100, 700, 200);
	bar(700, 100, 800, 200);

	// ROW 3
	bar(0, 200, 100, 300);
	rectangle(100, 200, 200, 300);
	bar(200, 200, 300, 300);
	rectangle(300, 200, 400, 300);
	bar(400, 200, 500, 300);
	rectangle(500, 200, 600, 300);
	bar(600, 200, 700, 300);
	rectangle(700, 200, 800, 300);

	// ROW 4
	rectangle(0, 300, 100, 400);
	bar(100, 300, 200, 400);
	rectangle(200, 300, 300, 400);
	bar(300, 300, 400, 400);
	rectangle(400, 300, 500, 400);
	bar(500, 300, 600, 400);
	rectangle(600, 300, 700, 400);
	bar(700, 300, 800, 400);

	// ROW 5
	bar(0, 400, 100, 500);
	rectangle(100, 400, 200, 500);
	bar(200, 400, 300, 500);
	rectangle(300, 400, 400, 500);
	bar(400, 400, 500, 500);
	rectangle(500, 400, 600, 500);
	bar(600, 400, 700, 500);
	rectangle(700, 400, 800, 500);

	// ROW 6
	rectangle(0, 500, 100, 600);
	bar(100, 500, 200, 600);
	rectangle(200, 500, 300, 600);
	bar(300, 500, 400, 600);
	rectangle(400, 500, 500, 600);
	bar(500, 500, 600, 600);
	rectangle(600, 500, 700, 600);
	bar(700, 500, 800, 600);

	// ROW 7
	bar(0, 600, 100, 700);
	rectangle(100, 600, 200, 700);
	bar(200, 600, 300, 700);
	rectangle(300, 600, 400, 700);
	bar(400, 600, 500, 700);
	rectangle(500, 600, 600, 700);
	bar(600, 600, 700, 700);
	rectangle(700, 600, 800, 700);

	// ROW 8
	rectangle(0, 700, 100, 800);
	bar(100, 700, 200, 800);
	rectangle(200, 700, 300, 800);
	bar(300, 700, 400, 800);
	rectangle(400, 700, 500, 800);
	bar(500, 700, 600, 800);
	rectangle(600, 700, 700, 800);
	bar(700, 700, 800, 800);

	return;
} 
void every(int ROW, int COL) {
	setcolor(BOARD1);
	bar(XOffsetBOARD + COL * GRID_side, YOffsetBOARD + ROW * GRID_side, XOffsetBOARD + COL * GRID_side + GRID_side, YOffsetBOARD + ROW * GRID_side + GRID_side);
}
void other(int ROW, int COL) {
	setcolor(OTHER1);
	bar(XOffsetBOARD + COL * GRID_side, YOffsetBOARD + ROW * GRID_side, XOffsetBOARD + COL * GRID_side + GRID_side, YOffsetBOARD + ROW * GRID_side + GRID_side);
	bool swatch = true;
	for (int ii = 0; ii <=GRID_side; ii++) {
		for (int jj = 0; jj <= GRID_side; jj++) {
			if (swatch)
				putpixel(XOffsetBOARD + COL * GRID_side + ii, YOffsetBOARD + ROW * GRID_side + jj, OTHER2);
			swatch = !swatch;
		}
	}
}
#pragma region LoadBMP
string toUpper(string s) {
	for (int i = 0; i < (int)s.length(); i++) {
		if (s[i] >= 97 && s[i] <= 122)
			s[i] -= 32;
	}
	return s;
}
unsigned char* ReadBMP(char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (f == NULL)
		throw "Argument Exception";
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
	int width, height;
	// extract image height and width from header
	if (w == 0 || h == 0) {
		width = *(int*)&info[18];
		height = *(int*)&info[22];
	}
	else {
		width = w;
		height = h;
	}
	string n(filename);
	n = n.substr(0, n.length() - 4);
	cout << "FILE : " << n << endl;
	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;
	ofstream files, paint;
	files.open("RGB.txt");
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;
	paint.open("paint.txt");
	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		paint << "{ ";
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			files << "[" << (int)data[j] << ", " << (int)data[j + 1] << "," << (int)data[j + 2] << "] ";
			RGB n;
			n.init(data[j], data[j + 1], data[j + 2]);
			bool exits = false;
			for (int index = 0; !exits && index < (int)colors.size(); index++)
				if (colors.at(index).isEqual(n))
				{
					exits = true;
					paint << colors.at(index).colorVal;
				}
			if (j + 3 >= (width * 3)) {
				paint << "}," << endl;
			}
			else
				paint << ", ";
			if (!exits)
				colors.push_back(n);
		}
		files << '\n';
	}
	files.close();
	paint.close();
	files.open("vectorBuild.txt");
	for (int i = 0; i < (int)colors.size(); i++)
		files << "colors.at(" << i << ").init( " << colors.at(i).r << ", " << colors.at(i).g << ", " << colors.at(i).b << ", ); \n";
	files.close();
	vector <string> hold;
	ifstream reverse;
	reverse.open("paint.txt");
	while (!reverse.eof()) {
		string line;
		getline(reverse, line);
		hold.push_back(line);
	}
	reverse.close();
	paint.open("paint.txt");
	paint << "const int " << n << "_Height = " << height << "; \n";
	paint << "const int " << n << "_Width = " << width << "; \n";
	paint << "#pragma region " << toUpper(n) << "_DRAW \n";
	paint << "const int " << n << "_ARRY [" << height << "][" << width << "] = { \n";
	while (!hold.empty()) {
		paint << hold.back() << "\n";
		hold.pop_back();
	}
	paint << "}; \n \n #pragma endregion \n";
	paint.close();
	fclose(f);
	return data;
}
void LoadBMP() {
	w = 0;
	h = 0;
	colors.at(2).init(255, 255, 255, (int)'W');
	colors.at(1).init(0, 0, 0, (int)'B');
	colors.at(0).init(252, 71, 25, 99);
	string test[6] = {
		"Bishop", "King", "Knight", "Queen", "Rook", "Pawn"
	};
	for (int i = 0; i < 6; i++) {
		string temp = "Black" + test[i] + ".bmp";
		ReadBMP(TEXT((LPTSTR)temp.c_str()));
		system("pause");
		temp = "White" + test[i] + ".bmp";
		ReadBMP(TEXT((LPTSTR)temp.c_str()));
		system("pause");
	}
}
#pragma endregion
#pragma region drawPiece
void drawRook(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhiteRook_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackRook_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
void drawKnight(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhiteKnight_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackKnight_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
void drawBishop(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhiteBishop_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackBishop_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
void drawQueen(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhiteQueen_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackQueen_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
void drawKing(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhiteKing_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackKing_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
void drawPawn(int ROW, int COL, PIECECOLOR player) {
	int x = XOffsetBOARD + COL * GRID_side;
	int y = YOffsetBOARD + ROW * GRID_side + 3;
	if (player == White) {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (WhitePawn_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
		return;
	}
	else {
		for (int i = 0; i < Width; i++) {
			for (int j = 0; j < Height; j++) {
				switch (BlackPawn_ARRY[j][i]) {
				case 99:
					break;
				case 'B':
					putpixel(x + i, y + j, COLOR1);
					break;
				case 'W':
					putpixel(x + i, y + j, COLOR2);
					break;
				}
			}
		}
	}
}
#pragma endregion
#pragma region THREADS
void Chess() {
	// Init Graphics Window
	gr_Start(GrDriver, GrMode, ErrorCode);
	// Grab Values
	maxX = getmaxx(); // From Graphics
	maxY = getmaxy(); // From Graphics
	int ROWselect = -1;
	int COLselect = -1;
	
	while (isRunning) {
		if (NewGame) {
			getch();
			cleardevice();
			/// GameStartUp
			P1TakenROW = 0;
			P1TakenCOL = -2;
			P2TakenCOL = 9;
			P2TakenROW = 0;
			MOVINGBOARD();
			setUpBoard();
			NewGame = false;
			Turn = White;
			EndTurn = true;
			CHECK = false;
			MATE = false;
			//updateChessCurrColor(); // Call this after every move, to make sure the colors and positions are kept 
		}
		if (EndTurn) {
			if (Turn == Black) {
				Turn = White;
			}
			else {
				Turn = Black;
			}
			updateChessCurrColor();
			for (int r = 0; r < 8; r++) {
				for (int c = 0; c < 8; c++) {
					if (CHESS[r][c].haze || CHESS[r][c].selected)
						CHESS[r][c].reset();
					if (colorBoard[r][c] == Turn) {
						CHESS[r][c].calcMove();

					}
					else {
						CHESS[r][c].calcMove();
						for (int i = 0; i < (int)CHESS[r][c].MOVECOL.size(); i++) {
							if (CHESS[CHESS[r][c].MOVEROW[i]][CHESS[r][c].MOVECOL[i]].currPiece == King) {
								CHECK = true;
							}
						}
					}
				}
			}
			GLOBAL.reset();
			Action.reset();
			Hold.reset();
			EndTurn = false;
		}
		switch (Turn) {
		case White:
			if (GLOBAL.isPressed) {
				GLOBAL.grid();
			}
			break;
		case Black:
			if (GLOBAL.isPressed) {
				GLOBAL.grid();
			}
			break;
		}
		Sleep(15);
	}
}
void Listener() {
	GLOBAL.reset();
	while (isRunning) {
		if (!GLOBAL.isPressed) {
 			if (ActionListener(VK_LBUTTON)) {
				// Left Mouse Button
				GetCursorPos(&GLOBAL.Cursor);
				GLOBAL.VirtualKey = VK_LBUTTON;
				GLOBAL.Significance = "Left Mouse Click";
				do { Sleep(100); } while (ActionListener(VK_LBUTTON));
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_A)) {
				GLOBAL.VirtualKey = VK_A;
				GLOBAL.Significance = "A";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_B)) {
				GLOBAL.VirtualKey = VK_B;
				GLOBAL.Significance = "B";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_C)) {
				GLOBAL.VirtualKey = VK_C;
				GLOBAL.Significance = "C";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_D)) {
				GLOBAL.VirtualKey = VK_D;
				GLOBAL.Significance = "D";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_E)) {
				GLOBAL.VirtualKey = VK_E;
				GLOBAL.Significance = "E";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_F)) {
				GLOBAL.VirtualKey = VK_F;
				GLOBAL.Significance = "F";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			if (ActionListener(VK_G)) {
				GLOBAL.VirtualKey = VK_G;
				GLOBAL.Significance = "G";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_H)) {
				GLOBAL.VirtualKey = VK_H;
				GLOBAL.Significance = "H";
				GLOBAL.isAlpha = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_1) || ActionListener(VK_NUMPAD1)) {
				GLOBAL.VirtualKey = VK_1;
				GLOBAL.Significance = "1";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_2) || ActionListener(VK_NUMPAD2)) {
				GLOBAL.VirtualKey = VK_2;
				GLOBAL.Significance = "2";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_3) || ActionListener(VK_NUMPAD3)) {
				GLOBAL.VirtualKey = VK_3;
				GLOBAL.Significance = "3";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_4) || ActionListener(VK_NUMPAD4)) {
				GLOBAL.VirtualKey = VK_4;
				GLOBAL.Significance = "4";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_5) || ActionListener(VK_NUMPAD5)) {
				GLOBAL.VirtualKey = VK_5;
				GLOBAL.Significance = "5";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_6) || ActionListener(VK_NUMPAD6)) {
				GLOBAL.VirtualKey = VK_6;
				GLOBAL.Significance = "6";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_7) || ActionListener(VK_NUMPAD7)) {
				GLOBAL.VirtualKey = VK_7;
				GLOBAL.Significance = "7";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_8) || ActionListener(VK_NUMPAD8)) {
				GLOBAL.VirtualKey = VK_8;
				GLOBAL.Significance = "8";
				GLOBAL.isNumeric = true;
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_LEFT)) {
				GLOBAL.VirtualKey = VK_LEFT;
				GLOBAL.Significance = "LEFT";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_RIGHT)) {
				GLOBAL.VirtualKey = VK_RIGHT;
				GLOBAL.Significance = "RIGHT";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_UP)) {
				GLOBAL.VirtualKey = VK_UP;
				GLOBAL.Significance = "UP";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_DOWN)) {
				GLOBAL.VirtualKey = VK_DOWN;
				GLOBAL.Significance = "DOWN";
				GLOBAL.isPressed = true;
			}
			else if (ActionListener(VK_RETURN)) {
				GLOBAL.VirtualKey = VK_RETURN;
				GLOBAL.Significance = "RETURN";
				GLOBAL.isPressed = true;
			}
		}
		Sleep(15);
	}
}
#pragma endregion
/// AsynKeyState
bool ActionListener(int VirtualKey) {
	return ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
}
