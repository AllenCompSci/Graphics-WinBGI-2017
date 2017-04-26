/*
Nathan Varghese
Brian Nguyen
Final Project
Start : 04/06/17
Due : 05/31/17

In this project, we will be creating a chess game.

*/

// Libraries
#include <iostream>
#include <ctime>
#include <winbgi.cpp>
#include <Windows.h>
#include <graphics.h>
#include <vector>

using namespace std;

// Global Variables
int maxX, maxY;
int GrDriver, GrMode, ErrorCode;

// Pre-Defined Functions
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void board();
int GRID_side = 100;
int XOffsetBOARD;
int YOffsetBOARD;
void MOVINGBOARD();
enum PIECE{
	King, Queen, Bishop, Knight, Rook, Pawn, Empty
};
enum PIECECOLOR {
	Black, White, NA
};
enum PAWNSpecial {
	UP, DOWN, NotPawn
};
PIECECOLOR colorBoard[8][8];

struct boardPiece {
	// Structures
	PIECE currPiece;
	PIECECOLOR currColor;
	PAWNSpecial pawnMove;
	int index;
	int color;
	int ROW;
	int COL;
	bool hasMoved;
	vector<int> MOVEROW;
	vector<int> MOVECOL;

	void init(int i, PIECE chess, PIECECOLOR color1, int I, int J) {
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
		index = i;
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
					if (colorBoard[ROW - 1][COL] != currColor) {
						add(-1 * i, 0);
					}
				}
			}
			piece = true;
			for (int i = 1; COL - i >= 0 && piece; i++) { // For positions that slide Example moving Left
				if (colorBoard[ROW][COL - 1] != NA) {
					piece = false;
				}
				if (piece)
					add(0, -1 * i);
				else {
					if (colorBoard[ROW][COL - 1] != currColor) {
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
					if (colorBoard[ROW + 1][COL] != currColor) {
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
					if (colorBoard[ROW - 1][COL - i] != currColor) {
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
					if (colorBoard[ROW + 1][COL - i] != currColor) {
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
					if (colorBoard[ROW + 1][COL + i] != currColor) {
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

				if (ROW > 0 && colorBoard[ROW - 1][COL - 2] != currColor)
					add(-1, -2);					   
				if (ROW < 7 && colorBoard[ROW + 1][COL - 2] != currColor)
					add(1, -2);
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
					if (colorBoard[ROW][COL - 1] == NA && colorBoard[ROW][COL - 2] == NA) {
						add(0, -2);
					}
				}
				if (colorBoard[ROW][COL - 1] == NA)
				{
					add(0, -1);
				}
				if (ROW < 7 && colorBoard[ROW + 1][COL - 1] != currColor && colorBoard[ROW + 1][COL - 1] != NA) {
					add(1, -1);
				}
				if (ROW > 0 && colorBoard[ROW - 1][COL - 1] != currColor && colorBoard[ROW - 1][COL - 1] != NA) {
					add(-1, -1);
				}
			}
			else {
				if (!hasMoved) {
					if (colorBoard[ROW][COL + 1] == NA && colorBoard[ROW][COL + 2] == NA) {
						add(0, 2);
					}
				}
				if (colorBoard[ROW][COL + 1] == NA)
				{
					add(0, 1);
				}
				if (ROW < 7 && colorBoard[ROW + 1][COL + 1] != currColor && colorBoard[ROW + 1][COL + 1] != NA) {
					add(1, 1);
				}
				if (ROW > 0 && colorBoard[ROW - 1][COL + 1] != currColor && colorBoard[ROW - 1][COL + 1] != NA) {
					add(-1, 1);
				}
			}
#pragma endregion
			break;
}
}
	void remove() {
		currPiece = Empty;
		currColor = NA;

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

}myboard;
//Main
boardPiece CHESS[8][8];
bool Design[8][8];
void setUpBoard();
void updateChessCurrColor();
void move(int, int, int, int);
void every(int ROW, int COL);
void other(int ROW, int COL);
void main() {

	gr_Start(GrDriver, GrMode, ErrorCode);

	maxX = getmaxx(); // From Graphics
	maxY = getmaxy(); // From Graphics

	//board();
	MOVINGBOARD();
	setUpBoard();
	updateChessCurrColor(); // Call this after every move, to make sure the colors and positions are kept 

	cout << "WELCOME TO CHESS" << endl;
	
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

	/* An example of a possible move
	CHESS[0][0].init(CHESS[0][1].index, CHESS[0][1].currPiece, CHESS[0][1].currColor, 0, 0);
	CHESS[0][1].remove();
	updateChessCurrColor();
	*/
	
	
}
void MOVINGBOARD() {
	XOffsetBOARD = (maxX - GRID_side * 8) / 2;
	YOffsetBOARD = (maxY - GRID_side * 8) / 2;
	bool toggle = true;
	
	
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 8; i++) {
			if (toggle) {
				every(j, i);
			}
			else {
				other(j, i);
			}
			Design[i][j] = toggle;
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
	CHESS[toRow][toCol].init(CHESS[fromRow][fromCol].index, CHESS[fromRow][fromCol].currPiece, CHESS[fromRow][fromCol].currColor, toRow, toCol);
	CHESS[toRow][toCol].hasMoved = true;
	CHESS[fromRow][fromCol].remove();
	updateChessCurrColor();
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
	setcolor(WHITE);
	bar(XOffsetBOARD + COL * GRID_side, YOffsetBOARD + ROW * GRID_side, XOffsetBOARD + COL * GRID_side + GRID_side, YOffsetBOARD + ROW * GRID_side + GRID_side);
}
void other(int ROW, int COL) {
	setcolor(WHITE);
	bar(XOffsetBOARD + COL * GRID_side, YOffsetBOARD + ROW * GRID_side, XOffsetBOARD + COL * GRID_side + GRID_side, YOffsetBOARD + ROW * GRID_side + GRID_side);
	bool swatch = true;
	for (int ii = 0; ii <=GRID_side; ii++) {
		for (int jj = 0; jj <= GRID_side; jj++) {
			if (swatch)
				putpixel(XOffsetBOARD + COL * GRID_side + ii, YOffsetBOARD + ROW * GRID_side + jj, DARKGRAY);
			swatch = !swatch;
		}
	}
}