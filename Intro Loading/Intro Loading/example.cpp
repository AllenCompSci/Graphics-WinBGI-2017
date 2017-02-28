#include<iostream>
#include<cmath>
#include<string>
#include<ctime>
#include<thread> // THREADS
#include<graphics.h>
#include<winbgi.cpp>
#include<Windows.h>  // CURSOR
#include <stdio.h> 
#include<mmsystem.h>  // SOUND
#include <mciapi.h> // SOUND
#include "VirtualKeys.h" // CUSTOM Virtual key list
#pragma comment(lib, "winmm.lib") // SOUND
using namespace std;
#pragma region GLOBALVARs
int GrDriver, GrMode, ErrorCode;
bool cursorVisible;
int centerX, centerY;
BOOL curVis;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
HCURSOR visible, hCur1, hCur3;
#pragma region Function_Prototypes
void gr_Start(int&, int&, int&); // Starts the graphics window
void getCursor(POINT &p); // uses struct POINT
bool MOUSE(int); // if mouse pressed
bool KEYBOARD(int); // if keyboard hit
void KEY_LISTENER(); // mouse/keyboard listner
void playSound(string, int);  // sound effect
void gameThread();
#pragma endregion
INPUT_RECORD irInBuf;
struct PASS {

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
}global;
#pragma endregion
void main()
{

	srand(time(NULL));


	thread g(gameThread); // Thread definiton
	thread t(KEY_LISTENER);

	global.press = global.hold = false;
	t.join(); // thread t start
	g.join();
	return;
}
#pragma region Functions
void gr_Start(int&GrDriver, int&GrMode, int&ErrorCode)
{
	//set the graphics driver
	GrDriver = VGA; //800x600
					//set graphics mode
	GrMode = VGAMAX;
	//start graphics
	initgraph(&GrDriver, &GrMode, "");
	//check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
		//getch();
		//exit(1);
	}
}
void getCursor(POINT & p) {
	while (true)
		if (GetCursorPos(&p)) {
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				do {} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
				return;
			}

			Sleep(45);
		}
}
bool MOUSE(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf.EventType = MOUSE_EVENT;
		GetCursorPos(&global.p);
		global.press = true;
		int count = 0;
		do {
			if (count++ > 1400) {
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}
bool KEYBOARD(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf.EventType = KEY_EVENT;
		global.press = true;
		global.VirtualKey = VirtualKey;
		int count = 0;
		do {
			if (count++ > 200) {
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}
void KEY_LISTENER() {

	gr_Start(GrDriver, GrMode, ErrorCode); // initi of graphics window 
	centerX = getmaxx() / 2;
	centerY = getmaxy() / 2;
	hCur1 = LoadCursorFromFile("aero_link.cur");
	hCur3 = LoadCursorFromFile("Jolt-Arrow-Normal-Select.cur");
	visible = LoadCursorFromFile("Chill Working.ani");
	SetCursor(hCur1);

	cursorVisible = true;

	while (true) {
		if (MOUSE(VK_LBUTTON)) {
			cout << "Left Button\n";
			SetCursor(visible);
			POINT v;
			GetCursorPos(&v);
			cout << v.x << ", " << v.y << endl;
		}
		if (MOUSE(VK_RBUTTON)) {
			cout << "Right Button\n";
			SetCursor(visible);
		}
		if (MOUSE(VK_MBUTTON))
		{
			playSound("starwars.wav", 2100);
			cout << "Mouse Wheel\n";
			SetCursor(visible);
		}
		if (MOUSE(VK_SCROLL)) {
			cout << "SCROLL \n";
		}
		if (KEYBOARD(VK_LEFT)) {
			cout << "Left Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_RIGHT)) {
			cout << "Right Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_UP)) {
			cout << "Up Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_DOWN)) {
			cout << "Down Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_LSHIFT)) {
			cout << "Left Shift\n";
		}
		if (KEYBOARD(VK_RSHIFT))
		{
			cout << "Right Shift\n";
		}
		if (KEYBOARD(VK_ESCAPE)) {
			cout << "Escape Pressed\n";
			Sleep(1000);

			Sleep(500);
			return;
		}
		if (KEYBOARD(VK_RETURN))
		{
			cout << "Return Pressed\n";
		}
		if (KEYBOARD(VK_F1))
		{
			cout << "F1 Pressed \n";
			SetCursor(hCur1);

		}
		if (KEYBOARD(VK_SPACE)) {
			cout << "Spacebar Pressed \n";
		}
		if (KEYBOARD(VK_A)) {
			cout << "A Key Pressed \n";
		}
		if (KEYBOARD(VK_W)) {
			cout << "W Key Pressed \n";
		}
		if (KEYBOARD(VK_S)) {
			cout << "S Key Pressed \n";
		}
		if (KEYBOARD(VK_D)) {
			cout << "D Key Pressed \n";
		}

		// Room For More Ifs specified keys act in a specific way.
		// look through the VirtualKeys.h list to have other specific keys
	}

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
void gameThread() {

	Sleep(300);

	settextstyle(3, 0, 72);
	setcolor(3);
	moveto(300, 500);
	outtext("We Begin");
	while (!global.press);

	global.press = global.hold = false;
	cleardevice();
	setcolor(4);

	while (true) {

		if (global.press) {
			cleardevice();
			if (irInBuf.EventType == MOUSE_EVENT) {
				fillellipse(centerX, centerY, 100, 100);
			}
			if (irInBuf.EventType == KEY_EVENT) {
				bar(centerX - 200, centerY - 200, centerX + 200, centerY + 200);
			}
			global.press = false;
		}
		if (global.hold) {
			setcolor(rand() % 15 + 1);
			if (irInBuf.EventType == MOUSE_EVENT) {
				fillellipse(centerX, centerY, 100, 100);
			}
			if (irInBuf.EventType == KEY_EVENT) {
				bar(centerX - 200, centerY - 200, centerX + 200, centerY + 200);
			}
			if (global.VirtualKey == VK_ESCAPE) {
				return;
			}
			global.hold = false;
		}

	}

	closegraph();


}
#pragma endregion
// 273 Lines of code total. 