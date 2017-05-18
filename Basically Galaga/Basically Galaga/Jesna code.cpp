/*Rasna Baweja, Jessica Mingee, Lindsay Rabalais
Game Project: Bascially Galaga
*/


#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <iomanip>
#include <stdio.h>
#include <winbgi.cpp>
#include <string>
#include <thread>
#include <graphics.h>
#include "images.h"
#include "frontANI.h"
#include <string>
#include <vector>


using namespace std;
int GrDriver, GrMode, ErrorCode;
void gr_start(int&, int&, int&);

void initgraph(int* device, int* mode, char const* /*pathtodriver*/, string name){
	int index;
	static WNDCLASS wcApp;

	gdi_error_code = grOk;

	if (wcApp.lpszClassName == NULL) {
		wcApp.lpszClassName = "BGIlibrary";
		wcApp.hInstance = 0;
		wcApp.lpfnWndProc = WndProc;
		wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcApp.hIcon = (HICON)LoadImage(NULL, "squirrel_ASL_icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);;
		wcApp.lpszMenuName = 0;
		wcApp.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		wcApp.style = CS_SAVEBITS;
		wcApp.cbClsExtra = 0;
		wcApp.cbWndExtra = 0;

		if (!RegisterClass(&wcApp)) {
			gdi_error_code = GetLastError();
			return;
		}

		pPalette = (NPLOGPALETTE)LocalAlloc(LMEM_FIXED,
			sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*PALETTE_SIZE);

		pPalette->palVersion = 0x300;
		pPalette->palNumEntries = PALETTE_SIZE;
		memset(pPalette->palPalEntry, 0, sizeof(PALETTEENTRY)*PALETTE_SIZE);
		for (index = 0; index < BG; index++) {
			pPalette->palPalEntry[index].peFlags = PC_EXPLICIT;
			pPalette->palPalEntry[index].peRed = index;
			pPalette->palPalEntry[PALETTE_SIZE - BG + index].peFlags = PC_EXPLICIT;
			pPalette->palPalEntry[PALETTE_SIZE - BG + index].peRed =
				PALETTE_SIZE - BG + index;
		}
		hBackgroundBrush = CreateSolidBrush(PALETTEINDEX(BG));
		hBrush[EMPTY_FILL] = (HBRUSH)GetStockObject(NULL_BRUSH);
		hBrush[SOLID_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SolidBrushBitmap));
		hBrush[LINE_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LineBrushBitmap));
		hBrush[LTSLASH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LtslashBrushBitmap));
		hBrush[SLASH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SlashBrushBitmap));
		hBrush[BKSLASH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, BkslashBrushBitmap));
		hBrush[LTBKSLASH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LtbkslashBrushBitmap));
		hBrush[HATCH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, HatchBrushBitmap));
		hBrush[XHATCH_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, XhatchBrushBitmap));
		hBrush[INTERLEAVE_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, InterleaveBrushBitmap));
		hBrush[WIDE_DOT_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, WidedotBrushBitmap));
		hBrush[CLOSE_DOT_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, ClosedotBrushBitmap));
		hBrush[USER_FILL] =
			CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SolidBrushBitmap));
	}
	memcpy(BGIpalette, BGIcolor, sizeof BGIpalette);
	current_palette.size = MAXCOLORS + 1;
	for (index = 10; index <= MAXCOLORS; index++) {
		pPalette->palPalEntry[index] = BGIcolor[0];
	}
	for (index = 0; index <= MAXCOLORS; index++) {
		current_palette.colors[index] = index;
		pPalette->palPalEntry[index + BG] = BGIcolor[index];
	}
	hPalette = CreatePalette(pPalette);
	detect_mode(device, mode);
	set_defaults();

	hWnd = CreateWindow("BGIlibrary", name.c_str(),
		WS_OVERLAPPEDWINDOW,
		0, 0, window_width + BORDER_WIDTH,
		window_height + BORDER_HEIGHT,
		(HWND)NULL, (HMENU)NULL,
		0, NULL);
	if (hWnd == NULL) {
		gdi_error_code = GetLastError();
		return;
	}
	ShowWindow(hWnd, *mode == VGAMAX ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
	UpdateWindow(hWnd);
}

void gr_start(int&GrDirer, int&GrMode, int&ErrorCode){
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "", "Squirrel Galaga");
	ErrorCode = graphresult();
	if (ErrorCode != grOk){
		cout << "Error: " << ErrorCode;
		getch();
		exit(1);
	}
}
//NEEEEEEEEEEEEEWWWWWWWWWWWWWW STUUUUUUUUUUFFFFFFFFFFFFFFFFF
struct RGB {
	int r;
	int g;
	int b;
	int colorVal;
	vector<RGB> collection;
	void init(int R, int G, int B){
		r = R;
		g = G;
		b = B;
	}

	bool isEqual(RGB t){
		return r == t.r && g == t.g && b == t.b;
	}

};
vector <POINT> shots;
vector <RGB> colors;
RGB REDPIXEL, PURPLEPIXEL, BLACKPIXEL, WHITEPIXEL, NULLPIXEL, LIGHTGREY2PIXEL, LIGHTGREYPIXEL, LIGHTREDPIXEL, BROWNPIXEL, BROWN2PIXEL, DARKGREYPIXEL, YELLOWPIXEL, WHITE2PIXEL;
unsigned char* ReadBMP(const char*, string num);
unsigned char* ReadBMP(char*);
void drawSir(int, int);
void drawSquirrel(int, int);
void gr_Start(int&, int&, int&);
void eraseSquirrel(int, int);
void drawSquare(int, int);
void drawRight(int, int);
void drawEagle(int, int);
void drawAni(int, int);
void drawANIF(int, int);
void drawFrameAni(const int frame[aniHeigh][aniWidth], int x, int y);
void backgroundSave(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y);
void redrawBackGround(int background[aniHeigh][aniWidth], int x, int y);
void drawFrameMulti(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration);
void drawFrameAniR(const int frame[aniHeigh][aniWidth], int x, int y);
void backgroundSaveR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y);
void drawFrameMultiR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration);
string convert(int);
POINT outP(int x, int y){
	POINT t;
	t.x = x;
	t.y = y;
	return t;
}
void drawSquirrel(int, int);
void drawSquirrel(int x, int y){
	for (int i = 0; i < squirrel_height; i++)
		for (int j = 0; j < squirrel_width; j++)
		{
			if (squirrel[i][j] != 99)
				putpixel(x + j, y + i, squirrel[i][j]);
		}
}
void drawSir(int x, int y){
	for (int i = 0; i < sirW; i++){
		for (int j = 0; j < sirW; j++)
			if (sirQ[i][j] != 99)
				putpixel(x + j, y + i, sirQ[i][j]);
	}
}
void eraseSquirrel(int x, int y){
	for (int i = 0; i < squirrel_height; i++)
		for (int j = 0; j < squirrel_width; j++)
		{
			if (squirrel[i][j] != 99)
				putpixel(x + j, y + i, 0);
		}
}
void background(){
	setbkcolor(0);
	putpixel(100, 200, 15);
	putpixel(143, 897, 15);
	putpixel(300, 365, 15);
	putpixel(550, 789, 15);
	putpixel(890, 654, 15);
	putpixel(65, 456, 15);
	putpixel(27, 999, 15);
	putpixel(54, 1000, 15);
	putpixel(720, 645, 15);
	putpixel(390, 606, 15);
	putpixel(836, 367, 15);
	putpixel(357, 890, 15);
	putpixel(498, 836, 15);
	putpixel(524, 720, 15);
	putpixel(606, 524, 15);
	putpixel(746, 143, 15);
	putpixel(985, 54, 15);
	putpixel(1000, 153, 15);
	putpixel(999, 3, 15);
	putpixel(456, 201, 15);
	putpixel(123, 478, 15);
	putpixel(789, 325, 15);
	putpixel(365, 165, 15);
	putpixel(897, 982, 15);
	putpixel(300, 800, 15);

	putpixel(1000, 200, 15);
	putpixel(1430, 897, 15);
	putpixel(1000, 365, 15);
	putpixel(1500, 789, 15);
	putpixel(1900, 654, 15);
	putpixel(150, 456, 15);
	putpixel(270, 999, 15);
	putpixel(540, 1000, 15);
	putpixel(1200, 645, 15);
	putpixel(1900, 606, 15);
	putpixel(1360, 367, 15);
	putpixel(1570, 890, 15);
	putpixel(1980, 836, 15);
	putpixel(1240, 720, 15);
	putpixel(1060, 524, 15);
	putpixel(1460, 143, 15);
	putpixel(1850, 54, 15);
	putpixel(1000, 153, 15);
	putpixel(1909, 3, 15);
	putpixel(1506, 201, 15);
	putpixel(1203, 478, 15);
	putpixel(1809, 325, 15);
	putpixel(1605, 165, 15);
	putpixel(1097, 982, 15);
	putpixel(1000, 800, 15);

}
bool chance(){
	int t = rand() % 323;
	if (t % 156 == 0){
		return true;
	}
	return false;
}
bool hit;
bool squirrelSHOT(POINT topLeft, POINT shot){
	return shot.x < (topLeft.x + squirrel_width) && shot.x>topLeft.x && shot.y>(topLeft.y + squirrel_height);
}
void main(){	
	gr_start(GrDriver, GrMode, ErrorCode);
	srand((unsigned int)time(NULL));
#pragma region HIDE CONSOLE
	HWND stealth; /*creating stealth (window is not visible)*/
	AllocConsole();
	stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(stealth, 0);
#pragma endregion
#pragma region VARIABLES;
	hit = false;
	int x = getmaxx() / 2;
	int y = getmaxy() / 2 + 450;
	int j = getmaxx() / 2;
	int k = getmaxy() / 2 + 450;
	int r = 50;
	int style = rand();
	color = rand() % 15 + 1;
	setcolor(15);
	int jvel = 20;
	int kvel = 0;
	int xvel = 0;

	int maxX = getmaxx();
	int maxY = getmaxy();
	int score = 0;
	int yvel = 0;

#pragma endregion VARIABLES;

#pragma region spaceBkgd;

	background();

#pragma endregion spaceBkgd;
	bool ENEM[11];
	POINT sq[11];
	sq[0] = outP(getmaxx() / 2 - 700, 200); // 0 
	sq[1] = outP(getmaxx() / 2 - 450, 200); // 1
	sq[2] = outP(getmaxx() / 2 - 150, 200); // 2
	sq[3] = outP(getmaxx() / 2 + 150, 200); // 3
	sq[4] = outP(getmaxx() / 2 + 450, 200); // 4
	sq[5] = outP(getmaxx() / 2 + 700, 200); // 5
	sq[6] = outP(getmaxx() / 2 - 600, 400); // 6
	sq[7] = outP(getmaxx() / 2 - 300, 400); // 7
	sq[8] = outP(getmaxx() / 2, 400); // 8
	sq[9] = outP(getmaxx() / 2 + 300, 400); // 9
	sq[10] = outP(getmaxx() / 2 + 600, 400); // 10

	int b = rand() / getmaxx();
	int g = rand() / getmaxy();
	bool uGotShot = false;
#pragma region TITLE; 
	//rasna
	do{
		uGotShot = false;
		DWORD timeStart = GetTickCount();
		DWORD ENDTIME = timeStart + (DWORD)(120000); // For Jessica
		DWORD JESSICAGAME = ENDTIME;
		score = 0;
		for (int i = 0; i < 11; i++) ENEM[i] = true;
		shots.clear();
		int key = 99;
		cleardevice();
		background();
		settextstyle(1, 0, 2000);
		setcolor(3);
		moveto(300, 500);
		outtext("Basically Galaga");
		settextstyle(0, 0, 5);
		moveto(300, 700);
		outtext("Press Spacebar to begin. DON'T DIE!");

		drawSir(500, 200);
		drawSir(750, 200);
		drawSir(250, 200);
		while (key != 32){
			


			if (kbhit()){
				int key = getch();
				if (key == 32){
					break;
				}
			}
		}

#pragma endregion TITLE; 

		cleardevice();
		//fillellipse(x,y,r,r);
#pragma region eneMies;
		//jess
		setcolor(4);
		drawSquirrel(getmaxx() / 2, 400);
		drawSquirrel(getmaxx() / 2 + 300, 400);
		drawSquirrel(getmaxx() / 2 + 600, 400);
		drawSquirrel(getmaxx() / 2 - 300, 400);
		drawSquirrel(getmaxx() / 2 - 600, 400);

		drawSquirrel(getmaxx() / 2 + 700, 200);
		drawSquirrel(getmaxx() / 2 + 150, 200);
		drawSquirrel(getmaxx() / 2 + 450, 200);
		drawSquirrel(getmaxx() / 2 - 150, 200);
		drawSquirrel(getmaxx() / 2 - 450, 200);
		drawSquirrel(getmaxx() / 2 - 700, 200);
		bool gameStart = true;
		while (gameStart){
			setcolor(3);
			background();
			fillellipse(j, k, 50, 50);
			fillellipse(x, y, 5, 5);




			POINT check;
			check.x = x;
			check.y = y;
#pragma region topEnemies;
			//squirrelSHOT(sq[0], check); BOOKMARK GO BACK AND CheCK this deliniation versus current for all 11 squirrels. should work with the && ENEM[0] 
			if (y < 260 && x < 370 && x>235 && y>160 && ENEM[0]){
				//cout << "0" << endl; system("pause");
				ENEM[0] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 - 700, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 260 && x < 580 && x>495 && y>160 && ENEM[1]){
				//cout << "1" << endl; system("pause");
				ENEM[1] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 - 450, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 260 && x < 900 && x>805 && y>160 && ENEM[2]){
				//cout << "2" << endl; system("pause");
				ENEM[2] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 - 150, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 260 && x < 1180 && x>1080 && y>160 && ENEM[3]){
				//cout << "3" << endl; system("pause");
				ENEM[3] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 + 150, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 260 && x < 1500 && x>1395 && y>160 && ENEM[4]){
				//cout << "4" << endl; system("pause");
				ENEM[4] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 + 450, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 260 && x < 1750 && x>1645 && y>160 && ENEM[5]){
				//cout << "5" << endl; system("pause");
				ENEM[5] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 + 700, 200);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}
#pragma endregion topEnemies;

#pragma region botEnemies;
			if (y < 460 && x < 455 && x>355 && y>360 && ENEM[6]){
				//cout << "6" << endl; system("pause");
				ENEM[6] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 - 600, 400);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 460 && x < 730 && x>635 && y>360 && ENEM[7]){
				//cout << "7" << endl; system("pause");
				ENEM[7] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 - 300, 400);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 460 && x < 1040 && x>945 && y>360 && ENEM[8]){
				//cout << "8" << endl; system("pause");
				ENEM[8] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2, 400);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 460 && x < 1355 && x>1255 && y>360 && ENEM[9]){
				//cout << "9" << endl; system("pause");
				ENEM[9] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 + 300, 400);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (y < 460 && x < 1650 && x>1555 && y>360 && ENEM[10]){
				//cout << "10" << endl; system("pause");
				ENEM[10] = false;
				hit = true;
				setcolor(0);
				eraseSquirrel(getmaxx() / 2 + 600, 400);
				score = score + 100;
				//cout << score <<endl;
				Sleep(20);
			}

			if (hit){
				hit = false;
				setcolor(0);
				fillellipse(x, y, 5, 5);
				yvel = 0;
				y = getmaxy() / 2 + 450;
			}



#pragma endregion botEnemies;

#pragma endregion eneMies;


#pragma region AGRRESSION
			//jess
			/*
			MAKE THEM B Aggressive B B Agressive
			*/

			for (int i = 0; i < 11; i++)
				if (ENEM[i]){
					if (chance()){
						POINT newShot = sq[i];
						//cout << i << endl;
						//getch();
						newShot.x += 105 / 2;
						newShot.y += 69;
						shots.push_back(newShot);
					}
				}
			//	cout << "Shots : " << shots.size();
			for (int i = 0; i < (int)shots.size(); i++)
			{

				int Sx = shots.at(i).x;
				int Sy = shots.at(i).y;
				setcolor(0);
				fillellipse(Sx, Sy, 5, 5);
				Sy += 30;
				setcolor(14); // enjoy diff from 4th period ;P
				fillellipse(Sx, Sy, 5, 5);
				shots.at(i).x = Sx;
				shots.at(i).y = Sy;
				int dist = (int)sqrt(pow((double)(Sy - k), 2.0) + pow((double)Sx - j, 2.0));
				if (dist <= 55){
					uGotShot = true;
				}
				if (Sy + 5 <  maxY)
					fillellipse(x, y, 5, 5);
				else
				{
					shots.erase(shots.begin() + i);
					i--;

				}

			}



#pragma endregion

#pragma region movSide;
			//ras
			int xvel = 20;
			setcolor(0);
			fillellipse(x, y, 5, 5);
			y -= yvel;
			setcolor(5);
			fillellipse(x, y, 5, 5);
			if (kbhit()){
				int key = getch();
				if (key == 96) score = 1100;
				if (key == '5') {
					cleardevice();
					moveto(getmaxx()/2, getmaxy()/2);
					outtext("CHEATER... WE SEE YOU");
				}
				if (key == '6' && (x + 50) < maxX && (x - 50)){
					setcolor(0);
					fillellipse(j, k, 50, 50);
					fillellipse(x, y, 5, 5);
					j += jvel;
					k += kvel;
					x += xvel;

					Sleep(0);
				}


				if (key == '4' && x > 50){
					setcolor(0);
					fillellipse(j, k, 50, 50);
					fillellipse(x, y, 5, 5);
					j -= jvel;
					k -= kvel;
					x -= xvel;

					Sleep(0);

				}


				if (key == '8'){
					setcolor(0);
					//fillellipse(j, k, 50, 50);
					fillellipse(x, y, 5, 5);
					yvel = 100;
					y -= yvel;
					Sleep(3);

					if (y + 5 < 0){
						yvel = 0;
						y = getmaxy() / 2 + 450;
						setcolor(3);
						fillellipse(x, y, 5, 5);

					}
				}


			}
			else
				Sleep(45);
#pragma endregion movSide;

#pragma region ENDSCREEN;
			if (GetTickCount() >= JESSICAGAME || uGotShot){
				gameStart = false;
				cleardevice();
				background();
				setcolor(2);
				settextstyle(0, 0, 4);

				int centerX = getmaxx() / 2;
				int centerY = getmaxy() / 2;
				centerY -= 50;
				centerX -= GameOverW / 2;
				drawGAMEOVER(centerX, centerY);
				centerX += (GameOverW / 2) - sirW / 2;
				centerY -= sirW;
				drawSir(centerX, centerY);
				moveto(centerX - 250, centerY - 50);
				outtext("JESSICA SAYS NO WIN 4 U");//Gift from Tj, Glorious Leader, and Dylan: Defeat"); // JESSICA SAYS NO WIN 4 U
				getch();
			}

			//jess
			if (score == 1100){
				gameStart = false;
				cleardevice();
				background();
				setcolor(2);
				settextstyle(0, 0, 4);
				moveto(getmaxx() / 2 - 150, 1);
				outtext("You WIN!!!!!!!");
				int xvel = 0;
				int yvel = 50;
				int qvel = 0;
				int svel = 50;
				int r = 20;
				int x = (getmaxx() / 2) - 150;
				int y = getmaxy() / 2;
				int q = (getmaxx() / 2) + 150;
				int s = getmaxy() / 2;
				int maxX = getmaxx();
				int maxY = getmaxy();
				bool animate = true;
				do{


					while ((x + xvel + r) < maxX - 90 && (x + xvel - r) > -10 && (y + yvel - r) > 150 && (y + yvel + r) < maxY - 15 && (q + qvel + r) < maxX - 90 && (q + qvel - r) > 0 && (s + svel - r) > 20 && (s + svel + r) < maxY - 15 && animate){
						if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0){
							animate = false;
						}

						drawSquirrel(x, y);
						drawSquirrel(q, s);

						x += xvel;
						y += yvel;
						q += qvel;
						s += svel;
						Sleep(3);
					}

					xvel = (rand() % 6 > 3) ? (rand() % 9) + 5 : -1 * (rand() % 9 + 5);
					yvel = (rand() % 6 > 3) ? (rand() % 9) + 5 : -1 * (rand() % 9);
					while (xvel == 0 && yvel == xvel && animate){
						if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0){
							animate = false;
						}
						xvel = (rand() % 6 > 4) ? (rand() % 9) : -1 * (rand() % 9 + 5);
						yvel = (rand() % 6 > 4) ? (rand() % 9) : -1 * (rand() % 9 + 5);
					}
					qvel = (rand() % 6 > 3) ? (rand() % 9) + 5 : -1 * (rand() % 10 + 5);
					svel = (rand() % 6 > 3) ? (rand() % 9) + 5 : -1 * (rand() % 10 + 5);
					while (qvel == 0 && svel == qvel && animate){
						if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0){
							animate = false;
						}
						qvel = (rand() % 6 > 4) ? (rand() % 9) : -1 * (rand() % 10 + 5);
						svel = (rand() % 6 > 4) ? (rand() % 9) : -1 * (rand() % 10 + 5);
					}




					if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) != 0){
						animate = false;
					}




				} while (animate);




#pragma endregion ENDSCREEN;
				getch();
			}
		}
		Sleep(100);
		getch();
	} while (true);

	getch();
	closegraph();

}

