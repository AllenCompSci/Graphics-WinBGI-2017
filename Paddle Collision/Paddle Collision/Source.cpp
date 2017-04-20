#include "Records.h"
#include "VirtualKeys.h"
#include <thread>
#include <mmsystem.h> /// WAV
#include <mciapi.h> /// MP3
#pragma comment(lib, "winmm.lib") /// WAV also maybe MP3

vector <int> PlayerColorChoices;
bool isRunning = true;
bool selectColor, finalizeColor;
char ** grid;
POINT click;
GAMESTATE Paddle = START_MENU;
struct BUTTON {
	int Left, Top, Right, Bottom;
	void init(int x1, int y1, int x2, int y2) {
		if (x1 < x2) {
			Left = x1;
			Right = x2;
		}
		else {
			Left = x2;
			Right = x1;
		}
		if (y1 < y2) {
			Top = y1;
			Bottom = y2;
		}
		else {
			Top = y2;
			Bottom = y1;
		}
	}
	void init(BUTTON t) {
		Left = t.Left;
		Right = t.Right;
		Top = t.Top;
		Bottom = t.Bottom;
	}
	bool isPressed(POINT p) {
		if (debug) {
			cout << "Point.X : " << p.x << " Point.Y : " << p.y << endl;
			cout << "LEFT  : (" << Left << ")" << (Left <= p.x) << endl;
			cout << "RIGHT : (" << Right << ")" << (p.x <= Right) << endl;
			cout << "TOP   : (" << Top << ")" << (Top <= p.y) << endl;
			cout << "BOTTOM: (" << Bottom << ")" << (p.y <= Bottom) << endl;
		}
		return ((Left <= p.x) && (p.x <= Right)) && ((Top <= p.y) && (p.y <= Bottom));
	}
	void render(int color) {
		setcolor(color);
		bar(Left, Top, Right, Bottom);
	}
	bool isEqual(BUTTON t) {
		return t.Left == Left && t.Right == Right && t.Bottom == Bottom && t.Top == Top;
	}
}Playerselect[6];
struct PLAYERPOS {
	int maxR;
	int maxC;
	int color;
	int row;
	int col;
	pass *listener;
	int unit;
	char C;
	void init(int startR, int startC, int unitSize, pass *input, int COLOR, int row1, int col1) {
		color = COLOR;
		row = startR;
		col = startC;
		unit = unitSize;
		listener = input;
		maxR = row1;
		maxC = col1;
		switch (color)
		{
		case BLUE:
			C = 'B';
			break;
		case RED:
			C = 'R';
			break;
		case GREEN:
			C = 'G';
			break;
		case YELLOW:
			C = 'Y';
			break;
		case MAGENTA:
			C = 'M';
			break;
		}
		render();
	}
	void tick() {
		switch (listener->move) {
		case UP:
			row--;
			return;
		case DOWN:
			row++;
			return;
		case LEFT:
			col--;
			return;
		case RIGHT:
			col++;
			return;
		}
	}
	void render() {
		setcolor(color);
		bar(col*(unit + 1), row*(unit + 1), (col + 1)*(unit + 1), (row + 1)*(unit + 1));
	}
	WIN collide(PLAYERPOS opponent) {
		if (maxR < row || row < 0 || maxC < col || col < 0) {
			return LOSE;
		}
		if (opponent.row == row && opponent.col == col) {
			return TIE;
		}
		if (pix(opponent)) {
			return LOSE;
		}
		return CONT;
	}
	bool pix(PLAYERPOS opponent) {
		int X = (col) * (unit + 1) + (unit + 1) / 2;
		int Y = (row) * (unit + 1) + (unit + 1) / 2;
		return getpixel(X, Y) == color || getpixel(X, Y) == opponent.color;
	}
	void state() {
		cout << "row : " << row << " col : " << col << endl;
	}
}readyPlayer1, readyPlayer2;
struct ButtonHighlight {
	int Left, Top, Right, Bottom;
	int padding;
	BUTTON inner;
	void init(int x) {
		padding = x / 2;
	}
	void pick(BUTTON t) {
		inner.init(t);
		Left = t.Left - padding;
		Right = t.Right + padding;
		Top = t.Top - padding;
		Bottom = t.Bottom + padding;
	}
	void draw() {
		render(WHITE);
	}
	void remove() {
		render(BLACK);
	}
	void render(int color) {
		setcolor(color);
		rectangle(Left, Top, Right, Bottom);
		rectangle(inner.Left, inner.Top, inner.Right, inner.Bottom);
		floodfill(Left + 1, Top + 1, color);

	}
}selected;

vector <Ball> Entities(1);
vector <BLOCK> levelManager;
int GrDriver, GrMode, ErrorCode;
INPUT_RECORD irInBuf1, irInBuf2;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void PLAYER1();
void MOUSE();
void PADDLEGAME();
void BACKGROUND();
void getCursor(POINT &p, int VirtualKey);
bool mousePress(int);
bool KEYBOARD1(int);
void playSound(string, int);
void playSoundSFX(string, int);
int level1();
BlockType randomType();
void main() {
	bool GmNotArt = (true);
	if (GmNotArt) {
		thread paddle(PADDLEGAME);
		thread p1(PLAYER1);
		thread mouse(MOUSE);
		thread audio(BACKGROUND);

		audio.join();
		paddle.join();
		mouse.join();
		p1.join();
	}
	else {
		CREATE("cracked2.bmp");
	}
}
void gr_Start(int&GrDriver, int&GrMode, int&ErrorCode) {
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk) {
		cout << "Error : " << ErrorCode << "/n";
	}
}
void PLAYER1() {
	while (isRunning) {
		if (KEYBOARD1(VK_A)) {
			playerInput1.move = LEFT;
		}
		if (KEYBOARD1(VK_D)) {
			playerInput1.move = RIGHT;
		}
		if (KEYBOARD1(VK_W)) {
			playerInput1.move = UP;
		}
		if (KEYBOARD1(VK_S)) {
			playerInput1.move = DOWN;
		}
		if (KEYBOARD1(VK_SPACE)) {
			temp.init(PONG.left, PONG.top, PONG.w);
			Entities.push_back(temp);
		}
		Sleep(15);
	}
}
void MOUSE() {
	while (isRunning) {
		if (mousePress(VK_LBUTTON)) {
			mouseInput.isPressed = true;
		}
		if (mousePress(VK_RBUTTON)) {

		}
		if (mousePress(VK_MBUTTON))
		{

		}
		Sleep(15);
	}
}
void PADDLEGAME() {
	gr_Start(GrDriver, GrMode, ErrorCode);
	int maxX = getmaxx();
	int maxY = getmaxy();
	int unitSize = 9;

	int row = maxY / (unitSize + 1);
	int col = maxX / (unitSize + 1);
	grid = new char *[row];
	for (int i = 0; i < row; i++) {
		grid[i] = new char[col];
		for (int j = 0; j < col; j++) {
			grid[i][j] = '0';
		}
	}
	// grid [row][col]


	bool changeStates = true;
	BUTTON start, end;
	WIN P1, P2;
	cout << boolalpha; /// prints out true and false as bool vars
	PONG.init(PADDLE,(int) ((getmaxx() - (BLOCKUNIT * 5)) / 2),(int)( (getmaxy() - BLOCKUNIT * 1.5)), WHITE);
	///{START_MENU, PLAYER1, PLAYER2, GAME, END};
	Entities[0].init(PONG.left, PONG.top, PONG.w);
	int XPadding;
	while (isRunning) {
		switch (Paddle) {
		case START_MENU:
			Paddle = GAME;
			break;
		case GAME:
			if (changeStates) {
				PONG.draw();
				XPadding = level1();
				
				for (int i = 0; i < (int)levelManager.size(); i++) {
					levelManager[i].draw();
				}
				changeStates = false;
			}
			PONG.MOVEPADDLE(XPadding);
			for (int i = 0; i < (int)Entities.size(); i++) {
				Entities[i].tick();
				int count = 0;
				for (int j = 0; j < (int)levelManager.size(); j++) {
					if (levelManager.at(j).hit(Entities[i])) {
						levelManager[j].update();
						Entities[i].dy *= -1;
					}
					if (levelManager[j].TYPE != Indestructable) {
						count++;
					}
					if (levelManager[j].TYPE == Destroyed) {
						if (debug)
							cout << "REMOVE : " << levelManager.size();
						levelManager.erase(levelManager.begin() + j);
						j--;
						if (debug)
							cout << "POST   : " << levelManager.size();
					}
				}
				if (PONG.hit(Entities[i])) {
					Entities[i].dy *= -1;
					Entities[i].y = PONG.top - Entities[i].radius;
				}
				if (count == 0) {
					Paddle = END;
				}
				if (Entities[i].x - Entities[i].radius <= XPadding) {
					// LEFT BUMPER
					Entities[i].x = XPadding;
					Entities[i].dx *= -1;
				}
				if(Entities[i].x + Entities[i].radius >= maxX - XPadding) {
					// RIGHT BUMPER
					Entities[i].x = maxX - XPadding;
					Entities[i].dx *= -1;
				}
				if ((Entities[i].y - Entities[i].radius) <= XPadding) {
					// TOP BUMPER
					Entities[i].y = XPadding + Entities[i].radius;
					Entities[i].dy *= -1;
				}
				if ((Entities[i].y) >= maxY) {
					// GAME OVER DUDE
					Entities.erase(Entities.begin() + i);
					i--;
					if(Entities.size() < 1)
						Paddle = END;
				}
				else {
					Entities[i].draw();
				}
			}
			Sleep(15);
			break;
		case END:
			Sleep(2000);
			// for now. 
			closegraph();
			break;
		}
	}
}
void BACKGROUND() {
	string bkgMusic = "ThemeTron.mp3";
	string sfxCrash = "sfxCrash.mp3";
	bool game = false;
	while (isRunning) {
		while (Paddle != END) {
			playSoundSFX(bkgMusic, 8200);
			game = true;
			Sleep(2);
		}
		if (Paddle == END && game) {
			playSoundSFX(sfxCrash, 987);
			game = false;
		}
		Sleep(15);
	}
}
void getCursor(POINT &p, int VirtualKey) {
	while (true) {
		if (GetCursorPos(&p)) {
			if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0) {
				do {} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
				return;
			}
		}
	}
}
bool mousePress(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0) {
		GetCursorPos(&click);
		mouseInput.VirtualKey = VirtualKey;
		int count = 0;
		do {
			if (count++ > 1400) {
				mouseInput.isHeld = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}
bool KEYBOARD1(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf1.EventType = KEY_EVENT;
		playerInput1.VirtualKey = VirtualKey;
		playerInput1.isPressed = true;
		int count = 0;
		do {
			if (count++ > 200) {
				playerInput1.isHeld = true;
				return true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		
		return true;
	}
	return false;

}
void playSound(string fileName, int duration) {
	if (fileName.substr(fileName.length() - 2) == "v") {
		PlaySound(TEXT(fileName.c_str()), NULL, SND_ASYNC);
		Sleep(duration);
		PlaySound(TEXT(fileName.c_str()), 0, 0);
		return;
	}
	string concat = "open " + fileName + " type mpegvideo alias MP3_Device";
	LPCSTR a = concat.c_str();
	mciSendString(a, NULL, 0, 0);

	mciSendString("play MP3_Device", NULL, 0, 0);
	Sleep(duration);
	mciSendString("stop MP3_Device", NULL, 0, 0);
	mciSendString("close MP3_Device", NULL, 0, 0);
}
void playSoundSFX(string fileName, int duration) {
	int current = 0;
	GAMESTATE stop = (Paddle != END) ? END : START_MENU;
	if (fileName.substr(fileName.length() - 2) == "v") {
		PlaySound(TEXT(fileName.c_str()), NULL, SND_ASYNC);
		while (current < duration && Paddle != stop) {
			current += 15;
			Sleep(15);
		}
		PlaySound(TEXT(fileName.c_str()), 0, 0);
		return;
	}
	string concat = "open " + fileName + " type mpegvideo alias MP3_Device";
	LPCSTR a = concat.c_str();
	mciSendString(a, NULL, 0, 0);

	mciSendString("play MP3_Device", NULL, 0, 0);
	while (current < duration && Paddle != stop) {
		current += 15;
		Sleep(15);
	}
	mciSendString("stop MP3_Device", NULL, 0, 0);
	mciSendString("close MP3_Device", NULL, 0, 0);
}
int level1() {
	int screenWidth = getmaxx();
	int screenHeight = getmaxy();
	/*
	Each Block is Height BLOCKUNIT
	and    with    Width BLOCKUNIT * 3
	level one has 4 rows
	Blue
	Red
	Green 
	Yellow
	*/
	int initHeight = BLOCKUNIT * 3;
	int NUMRowBlocks = screenWidth / (BLOCKUNIT * 3 * 1.05);
	int XPadding = screenWidth - (int)(((NUMRowBlocks-1) * BLOCKUNIT * 3 * 1.05) + (BLOCKUNIT * 3));
	XPadding /= 2;
	setcolor(LIGHTGRAY);
	bar(0, 0, XPadding, screenHeight);
	bar(screenWidth - XPadding, 0, screenWidth, screenHeight);
	bar(0, 0, screenWidth, XPadding);
	for (int i = 0; i < 4; i++) {
		int value; // color
		switch (i) {
		case 0:
			value = BLUE;
			break;
		case 1:
			value = RED;
			break;
		case 2:
			value = GREEN;
			break;
		case 3:
			value = YELLOW;
			break;
		}
			
		for (int j = 0; j < NUMRowBlocks; j++) {
			general.init(randomType(), (int)(j*BLOCKUNIT * 3 * 1.05) + XPadding, (int)(i*BLOCKUNIT * 1.05 + initHeight), value);
			levelManager.push_back(general);
		}
	}

	return XPadding;
}
BlockType randomType() {
	int value = rand() % 5;
	if (value == 3) {
		value = rand() % 4;
	}
	switch (value) {
	case 0:
		return Healthy;
	case 1:
		return Pitted;
	case 2:
		return Cracked;
	case 3:
		return Indestructable;
	}
	return Healthy;
}