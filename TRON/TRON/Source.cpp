#include "LoadBMP.h"
#include "Pixel.h"
#include "VirtualKeys.h"
#include <thread>
#include <ctime>
#include <mmsystem.h> /// WAV
#include <mciapi.h> /// MP3
#pragma comment(lib, "winmm.lib") /// WAV also maybe MP3
enum GAMESTATE{START_MENU, PLAYER_1, PLAYER_2, GAME, END};
enum DIRECTION{UP, RIGHT, DOWN, LEFT, PAUSE, SELECT, LEFTCLICK, RIGHTCLICK};
enum WIN{CONT, LOSE, TIE};
vector <int> PlayerColorChoices;
bool isRunning = true;
bool debug = false;
bool selectColor, finalizeColor;
char ** grid;
POINT click;
GAMESTATE Tron = START_MENU;
struct pass {
	int VirtualKey;
	bool isPressed;
	bool isHeld;
	DIRECTION move;
	void reset() {
		isPressed = false;
		isHeld = false;
	}
}playerInput1, playerInput2, mouseInput;
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
		padding = x/2;
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
int GrDriver, GrMode, ErrorCode;
INPUT_RECORD irInBuf1, irInBuf2;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void PLAYER1();
void PLAYER2();
void MOUSE();
void TRON();
void BACKGROUND();
void getCursor(POINT &p, int VirtualKey);
bool mousePress(int);
bool KEYBOARD1(int);
bool KEYBOARD2(int);
void playSound(string, int);
void playSoundSFX(string, int);
void resetListeners();
void setColorPallet();
void drawPLAYER(int);
bool checkPLAYER(int);
void main() {
	bool GmNotArt = (true);
	if (GmNotArt) {
		thread tron(TRON);
		thread p1(PLAYER1);
		thread p2(PLAYER2);
		thread mouse(MOUSE);
		thread audio(BACKGROUND);

		audio.join();
		tron.join();
		mouse.join();
		p1.join();
		p2.join();
	}
	else {
		CREATE("player2.bmp");
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
			if(playerInput1.move != RIGHT)
			playerInput1.move = LEFT;
		}
		if (KEYBOARD1(VK_D)) {
			if (playerInput1.move != LEFT)
			playerInput1.move = RIGHT;
		}
		if (KEYBOARD1(VK_W)) {
			if(playerInput1.move != DOWN)
			playerInput1.move = UP;
		}
		if (KEYBOARD1(VK_S)) {
			if(playerInput1.move != UP)
			playerInput1.move = DOWN;
		}

		Sleep(15);
	}
}
void PLAYER2() {
	while (isRunning) {
		if (KEYBOARD2(VK_LEFT)) {
			if (playerInput2.move != RIGHT)
			playerInput2.move = LEFT;
		}
		if (KEYBOARD2(VK_RIGHT)) {
			if (playerInput2.move != LEFT)
			playerInput2.move = RIGHT;
		}
		if (KEYBOARD2(VK_UP)) {
			if (playerInput2.move != DOWN)
			playerInput2.move = UP;
		}
		if (KEYBOARD2(VK_DOWN)) {
			if (playerInput2.move != UP)
			playerInput2.move = DOWN;
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
void TRON() {
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
	resetListeners();
	///{START_MENU, PLAYER1, PLAYER2, GAME, END};
	while (isRunning) {
		switch (Tron) {
		case START_MENU:
			if (changeStates) {
				setColorPallet();
				cleardevice();
				int top, left, bottom, right;
				top = (maxY / 2) / 2;
				bottom = maxY / 2;
				left = ((maxX / 2) / 2) / 2;
				bottom = bottom - top;
				top = ((maxY - title_Height) / 2) + title_Height;
				bottom = top + bottom;
				right = maxX - left;
				start.init(left, top, right, bottom);
				drawTitle((maxX - title_Width) / 2, (maxY - title_Height) / 2);
				start.render(7);
				changeStates = false;
			}
			if (mouseInput.isPressed && start.isPressed(click)) {
				changeStates = true;
				Tron = PLAYER_1;//GAME;///
				//cout << "SWITCH STATE \n";
			}
			resetListeners();
			break;
		case PLAYER_1:
			if (changeStates) {
				selectColor = false;
				finalizeColor = false;
				cleardevice();
				drawPLAYER(1);
			}
			changeStates = checkPLAYER(1);
			resetListeners();
			break;
		case PLAYER_2:
			if (changeStates) {
				selectColor = false;
				finalizeColor = false;
				cleardevice();
				drawPLAYER(2);
			}
			changeStates = checkPLAYER(2);
			resetListeners();
			break;
		case GAME:
			if (changeStates) {
				drawGrid(unitSize, maxX, maxY, CYAN, WHITE);
				playerInput1.move = RIGHT;
				playerInput2.move = LEFT;
				readyPlayer1.init((row / 2), 10, unitSize, &playerInput1, readyPlayer1.color, row, col);
				readyPlayer2.init((row / 2), (col - 10), unitSize, &playerInput2, readyPlayer2.color, row, col);
				changeStates = false;
			}
			readyPlayer1.tick();
			readyPlayer2.tick();
			P1 = readyPlayer1.collide(readyPlayer2);
			P2 = readyPlayer2.collide(readyPlayer1);
			if (P1 != CONT || P2 != CONT) {
				
				if (P1 == P2) {
					P1 = P2 = TIE;
				}
				changeStates = true;
				Tron = END;
			}
			else {
				readyPlayer1.render();
				readyPlayer2.render();
				
			}
			resetListeners();
			break;
		case END:
			if (changeStates) {
				int top, left, bottom, right;
				top = (maxY / 2) / 2;
				bottom = maxY / 2;
				left = ((maxX / 2) / 2) / 2;
				bottom = bottom - top;
				top = ((maxY - title_Height) / 2) + title_Height;
				bottom = top + bottom;
				right = maxX - left;
				end.init(left, top, right, bottom);
				int Xs = (maxX - title_Width) / 2;
				int Ys = (maxY - title_Height) / 2;
				drawTitleE(Xs, Ys);
				start.init(Xs, Ys, Xs + title_Width, Ys + title_Height);
				end.render(4);
				settextstyle(2, 0, 96);
				setbkcolor(CYAN);
				string GAMEOVER = "";
				if (P1 == LOSE) {
					 GAMEOVER = "Player 2 Wins!";
					 Player2((maxX-player_Width)/2,Ys-player_Height,readyPlayer2.color);
				}
				else if (P2 == LOSE) {
					GAMEOVER = "Player 1 Wins!";
					Player1((maxX - player_Width) / 2, Ys - player_Height, readyPlayer1.color);
				}
				else {
					GAMEOVER = "DRAW";
					setcolor(WHITE);
					Ys -= textheight(GAMEOVER.c_str());
					Xs = (maxX - textwidth(GAMEOVER.c_str())) / 2;
					outtextxy(Xs, Ys, GAMEOVER.c_str());
					putpixel(0, 0, CYAN);
				}
				setcolor(BLACK);
				setbkcolor(BLACK);
				cout << GAMEOVER << "\n";
				changeStates = false;
			}
			if (mouseInput.isPressed && start.isPressed(click)) {
				changeStates = true;
				Tron = START_MENU;
			}
			else if (mouseInput.isPressed && end.isPressed(click)) {
				isRunning = false;
			}
			resetListeners();
			
			break;
		}
		Sleep(15);
	}
}
void BACKGROUND() {
	string bkgMusic = "ThemeTron.mp3";
	string sfxCrash = "sfxCrash.mp3";
	bool game = false;
	while (isRunning) {
		while (Tron != END) {
			playSoundSFX(bkgMusic, 8200);
			game = true;
			Sleep(2);
		}
		if (Tron == END && game) {
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
		int count = 0;
		do {
			if (count++ > 200) {
				playerInput1.isHeld = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		playerInput1.isPressed = true;
		return true;
	}
	return false;

}
bool KEYBOARD2(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf2.EventType = KEY_EVENT;
		playerInput2.VirtualKey = VirtualKey;
		int count = 0;
		do {
			if (count++ > 200) {
				playerInput2.isHeld = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		playerInput2.isPressed = true;
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
	GAMESTATE stop = (Tron != END) ? END : START_MENU;
	if (fileName.substr(fileName.length() - 2) == "v") {
		PlaySound(TEXT(fileName.c_str()), NULL, SND_ASYNC);
		while (current < duration && Tron != stop) {
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
	while (current < duration && Tron != stop) {
		current += 15;
		Sleep(15);
	}
	mciSendString("stop MP3_Device", NULL, 0, 0);
	mciSendString("close MP3_Device", NULL, 0, 0);
}
void resetListeners() {
	playerInput1.reset();
	playerInput2.reset();
	mouseInput.reset();
}
void setColorPallet() {
	PlayerColorChoices.clear();
	PlayerColorChoices.push_back(RED); // 0
	PlayerColorChoices.push_back(BLUE); // 1
	PlayerColorChoices.push_back(BROWN); // 2
	PlayerColorChoices.push_back(MAGENTA); // 3
	PlayerColorChoices.push_back(GREEN); // 4
	PlayerColorChoices.push_back(LIGHTMAGENTA); // 5
	PlayerColorChoices.push_back(YELLOW); // 6
}
void drawPLAYER(int playerNum) {
	int Xf, Xs, Xt, Yf, Ys, Yt; // Xfirst, Xsecond, Xthird (three rows)
	int padding = 25; 
	selected.init(padding);
	int h = player_Height;
	Yf = (getmaxy() - ((h*3) + (padding*2))) / 2;
	Ys = Yf + h + padding; 
	Yt = Ys + h + padding;
	Xf = (getmaxx() - 520) / 2;
	padding = (padding * 2);
	Xt = Xs = (getmaxx() - (h * 3 + (padding * 2))) / 2;
	if (playerNum == 1) {
		Player1O(Xf, Yf, CYAN);
	}
	else {
		Player2O(Xf, Yf, CYAN);
	}
	
	int pointX = Xs;
	for (int i = 0; i < 3; i++) {
		Playerselect[i].init(pointX, Ys, pointX + h, Ys + h);
		Playerselect[i + 3].init(pointX, Yt, pointX + h, Yt + h);
		Playerselect[i].render(PlayerColorChoices.at(i));
		Playerselect[i + 3].render(PlayerColorChoices.at(i + 3));
		pointX += h + padding;
	}

}
bool checkPLAYER(int playerNum) {
	
	if (mouseInput.isPressed){
		for (int i = 0; i < 6; i++) {
			if (Playerselect[i].isPressed(click)) {
				if (selectColor) {
					if (selected.inner.isEqual(Playerselect[i])) {
						if (playerNum == 1) {
							readyPlayer1.color = PlayerColorChoices.at(i);
							PlayerColorChoices.erase(PlayerColorChoices.begin() + i);
							Tron = PLAYER_2;
							return true;
						}
						else {
							readyPlayer2.color = PlayerColorChoices.at(i);
							Tron = GAME;
							return true;
						}
					}
					else {
						selected.remove();
						selected.pick(Playerselect[i]);
						selected.draw();
					}
				}
				else {
					selectColor = true;
					selected.pick(Playerselect[i]);
					selected.draw();
				}
			}
		}
	}
	return false;
}