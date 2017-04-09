#include <iostream>
#include <graphics.h>
#include <winbgi.cpp>
#include <Windows.h>
#include <thread>
#include <ctime>
#include <string>
#include <stdio.h>
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
enum DIRECTION{UP, RIGHT, DOWN, LEFT, PAUSE, SELECT};
bool isRunning = true;
POINT click;
struct pass {
	bool isPressed;
	bool isHeld;
	DIRECTION move;
}playerInput1, playerInput2, mouseInput;
int GrDriver, GrMode, ErrorCode;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void PLAYER1();
void PLAYER2();
void MOUSE();
void TRON();
void BACKGROUND();
void Effects();
void getCursor(POINT &p);
bool mousePress(int);
bool KEYBOARD(int);
void playSound(string, int);
void main() {

	thread tron(TRON);
	thread p1(PLAYER1);
	thread p2(PLAYER2);
	thread mouse(MOUSE);
	thread audio(BACKGROUND);
	thread sfx(Effects);

	audio.join();
	tron.join();
	mouse.join();
	p1.join();
	p2.join();
	sfx.join();

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
void PLAYER1();
void PLAYER2();
void MOUSE();
void TRON();
void BACKGROUND();
void Effects();
void getCursor(POINT &p) {
	while (true) {
		if (GetCursorPos(&p)) {
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0) {
				do {} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
				return;
			}
		}
	}
}
bool mousePress(int);
bool KEYBOARD(int);
void playSound(string, int);
