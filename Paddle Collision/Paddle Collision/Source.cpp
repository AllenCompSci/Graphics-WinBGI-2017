#include "Records.h"
#include "VirtualKeys.h"
vector <int> PlayerColorChoices;
bool isRunning = true;
bool selectColor, finalizeColor;
POINT click;
GAMESTATE Paddle = START_MENU;
vector <Ball> Entities(1);
vector <BLOCK> levelManager;
int GrDriver, GrMode, ErrorCode;
INPUT_RECORD irInBuf1, irInBuf2;
BlockType randomType();
void main() {
	bool GmNotArt = (true);
	if (GmNotArt) {
/*
The following segment of three lines of close uses stealth and h to hide the console
*/
#pragma region HIDECONSOLE
		AllocConsole();
		stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, 0);
#pragma endregion
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
		if (KEYBOARD1(VK_P)) {
			/// PAUSE
		}
		if (KEYBOARD1(VK_ESCAPE)) {
			/// EXIT GAME
			isRunning = false;
			exit(0);
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
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);
	int maxX = getmaxx();
	int maxY = getmaxy();
	int unitSize = 9;
	int redraw = 0;
	int row = maxY / (unitSize + 1);
	int col = maxX / (unitSize + 1);
	int GMOverCount;
	int level = 0;
	bool changeStates = true;
	BUTTON start, end;
	WIN P1, P2;
	cout << boolalpha; /// prints out true and false as bool vars
	///{START_MENU, PLAYER1, PLAYER2, GAME, END};
	int XPadding;
	while (isRunning) {
		switch (Paddle) {
		case START_MENU:
			Paddle = GAME;
			break;
		case GAME:
			if (changeStates) {
				cleardevice();
				PONG.init(PADDLE, (int)((getmaxx() - (BLOCKUNIT * 5)) / 2), (int)((getmaxy() - BLOCKUNIT * 1.5)), WHITE);
				Entities.erase(Entities.begin() + 1, Entities.end());
				Entities[0].init(PONG.left, PONG.top, PONG.w);
				PONG.draw();
				level++;
				switch (level) {
				case 1:
					XPadding = level1();
					break;
				case 2:
					XPadding = level2();
					break;
				case 3:
					XPadding = level3();
					break;
				case 4:
					XPadding = level4();
					break;
				case 5:
				case 6:
				case 7:
				default :
					Paddle = END;
					break;
				}
				redraw = 0;
				
				for (int i = 0; i < (int)levelManager.size(); i++) {
					levelManager[i].draw();
				}
				changeStates = false;
			}
			PONG.MOVEPADDLE(XPadding);
			changeStates = true;
			for (int i = 0; i < (int)Entities.size(); i++) {
				Entities[i].tick();
				for (int j = 0; j < (int)levelManager.size(); j++) {
					if (levelManager.at(j).hit(Entities[i])) {
						levelManager[j].update();
						if (Entities[i].dy > 0) { // from top
							Entities[i].y = levelManager[j].top + Entities[i].radius;
						}
						else if (Entities[i].dy < 0) { // from bottom
							Entities[i].y = levelManager[j].top + levelManager[j].h + Entities[i].radius;
						}
						else { // side
							if (Entities[i].dx > 0) { // from Left
								Entities[i].x = levelManager[j].left + Entities[i].radius;
							}
							else if (Entities[i].dx < 0) { // from Right
								Entities[i].x = levelManager[j].left + levelManager[j].w + Entities[i].radius;
							}
						}
						Entities[i].dy *= -1;
					}
					if (levelManager[j].TYPE == Healthy || levelManager[j].TYPE == Pitted || levelManager[j].TYPE == Cracked) {
						changeStates = false;
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
			if (redraw++ > 3000) {
				for (int i = 0; i < (int)levelManager.size(); i++) {
					if(levelManager[i].collisionTIME != 0)
					levelManager[i].draw();
				}
				for (int i = 0; i < (int)Entities.size(); i++) {
					Entities[i].draw();
				}
				PONG.draw();
				redraw = -1;
			}
			Sleep(15);
			break;
		case END:
			Sleep(2000);
			isRunning = false;
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
	int NUMRowBlocks = (int)( screenWidth / (BLOCKUNIT * 3 * 1.05));
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
int level2() {
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
	int initHeight = BLOCKUNIT * 2;
	int NUMRowBlocks = (int)(screenWidth / (BLOCKUNIT * 3 * 1.05));
	int XPadding = screenWidth - (int)(((NUMRowBlocks - 1) * BLOCKUNIT * 3 * 1.05) + (BLOCKUNIT * 3));
	XPadding /= 2;
	setcolor(LIGHTGRAY);
	bar(0, 0, XPadding, screenHeight);
	bar(screenWidth - XPadding, 0, screenWidth, screenHeight);
	bar(0, 0, screenWidth, XPadding);
	bool checker = true;
	for (int i = 0; i < 5; i++) {
		int value; // color
		switch (i) {
		case 0:
			value = WHITE;
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
		case 4:
			value = CYAN;
			break;
		}

		for (int j = 0; j < NUMRowBlocks; j++) {
			if (checker) {
				general.init(randomType(), (int)(j*BLOCKUNIT * 3 * 1.05) + XPadding, (int)(i*BLOCKUNIT * 1.05 + initHeight), value);
				levelManager.push_back(general);
				checker = false;
			}
			else {
				checker = true;
			}
		}
	}
	return XPadding;
}
int level3() {
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
	int initHeight = BLOCKUNIT * 2;
	int NUMRowBlocks = (int)(screenWidth / (BLOCKUNIT * 3 * 1.05));
	int XPadding = screenWidth - (int)(((NUMRowBlocks - 1) * BLOCKUNIT * 3 * 1.05) + (BLOCKUNIT * 3));
	XPadding /= 2;
	setcolor(LIGHTGRAY);
	bar(0, 0, XPadding, screenHeight);
	bar(screenWidth - XPadding, 0, screenWidth, screenHeight);
	bar(0, 0, screenWidth, XPadding);
	int checker = NUMRowBlocks / 3;
	for (int i = 0; i < 5; i++) {
		int value; // color
		switch (i) {
		case 0:
			value = WHITE;
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
		case 4:
			value = CYAN;
			break;
		}

		for (int j = 0; j < NUMRowBlocks; j++) {
			if ((int)(j/checker)!=1 || i== 0 || i == 4) {
				general.init(randomType(), (int)(j*BLOCKUNIT * 3 * 1.05) + XPadding, (int)(i*BLOCKUNIT * 1.05 + initHeight), value);
				levelManager.push_back(general);
			}
		}
	}
	return XPadding;
}
int level4() {
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
	int initHeight = BLOCKUNIT * 2;
	int NUMRowBlocks = (int)(screenWidth / (BLOCKUNIT * 3 * 1.05));
	int XPadding = screenWidth - (int)(((NUMRowBlocks - 1) * BLOCKUNIT * 3 * 1.05) + (BLOCKUNIT * 3));
	XPadding /= 2;
	setcolor(LIGHTGRAY);
	bar(0, 0, XPadding, screenHeight);
	bar(screenWidth - XPadding, 0, screenWidth, screenHeight);
	bar(0, 0, screenWidth, XPadding);
	bool checker = true;
	for (int i = 0; i < 6; i++) {
		int value; // color
		switch (i) {
		case 0:
			value = WHITE;
			break;
		case 1:
			value = BLUE;
			break;
		case 2:
			value = GREEN;
			break;
		case 3:
			value = YELLOW;
			break;
		case 4:
			value = CYAN;
			break;
		case 5:
			value = RED;
			break;
		}

		for (int j = 0; j < NUMRowBlocks; j++) {
			if (checker || i == 0 || i == 5) {
				general.init(randomType(), (int)(j*BLOCKUNIT * 3 * 1.05) + XPadding, (int)(i*BLOCKUNIT * 1.05 + initHeight), value);
				levelManager.push_back(general);
				if (i != 0 || i != 5) {
					checker = false;
				}
			}
			else {
				checker = true;
			}
		}

		}
	
	return XPadding;
}
BlockType randomType() {
	int value = rand() % 5;
	if (value == 3) {
		value = rand() % 4;
	}
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