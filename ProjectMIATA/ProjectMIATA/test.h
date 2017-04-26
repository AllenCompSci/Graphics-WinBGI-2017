#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winbgi.cpp>
#include <graphics.h>
#include "draw.h"
#include <Windows.h>
#include <thread>
#include <vector>
#include <string>

using namespace std;
void KEY_LISTENER();
void getCursor(POINT &p, int VirtualKey);
bool mousePress(int VirtualKey);
bool KEYBOARD(int VirtualKey);
enum MIATA {
	BottomTone, TopTone, Interior, Outline, Wheels, Windows, TireWall, TailLights, STOCK, SLAMMED
};
struct pass {
	int VirtualKey;
	bool isPressed;
	bool isHeld;
	void reset() {
		isPressed = false;
		isHeld = false;
	}
}keyInput, mouseInput;
struct Button {
	int Top, Left, Right, Bottom, Color;
	bool isLEFT;
	void init(int left, int top, int right, int bottom, int color) {
		Top = top;
		Left = left;
		Right = right;
		Bottom = bottom;
		Color = color;
		draw();
	}

	void draw() {
		setcolor(Color);
		bar(Left, Top, Right, Bottom);
		if (Color != WHITE) {
			setcolor(WHITE);
		}
		else {
			setcolor(BLACK);
		}
		rectangle(Left + 1, Top + 1, Right - 1, Bottom - 1);
	}
	bool isPressed(POINT p) {
		
		return ((Left <= p.x) && (p.x <= Right)) && ((Top <= p.y) && (p.y <= Bottom));
	}
	void ARROW() {
		setcolor(WHITE);
		bar(Left, Top, Right, Bottom);
		setcolor(BLACK);
		rectangle(Left, Top, Right, Bottom);
		if (isLEFT) {
			// LEFT BUTTON
			line(((maxX / 8) * 2) - 30, (maxY / 3) * 2, ((maxX / 8) * 2) + 30, ((maxY / 3) * 2) + 30);
			line(((maxX / 8) * 2) - 30, (maxY / 3) * 2, ((maxX / 8) * 2) + 30, ((maxY / 3) * 2) - 30);
			line(((maxX / 8) * 2) + 30, (maxY / 3) * 2 + 30, ((maxX / 8) * 2) + 30, ((maxY / 3) * 2) - 30);
			
		}
		else {
			// RIGHT BUTTON
			line(((maxX / 8) * 6) - 30, ((maxY / 3) * 2) - 30, ((maxX / 8) * 6) + 30, (maxY / 3) * 2);
			line(((maxX / 8) * 6) - 30, ((maxY / 3) * 2) + 30, ((maxX / 8) * 6) + 30, (maxY / 3) * 2);
			line(((maxX / 8) * 6) - 30, (maxY / 3) * 2 + 30, ((maxX / 8) * 6) - 30, ((maxY / 3) * 2) - 30);
		}
		floodfill(Left + (Right - Left) / 2, Top + (Bottom - Top) / 2, BLACK);
	}

}COLOR[16], LEFT, RIGHT;

int GrDriver, GrMode, ErrorCode;
POINT click;
vector <MIATA> BODYKITS;
bool isRunning = true;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode) {
	//set the graphics driver
	GrDriver = VGA; //800x600 by monitor
	GrMode = VGAMAX;//set graphics mode
	initgraph(&GrDriver, &GrMode, "");//start graphics
									  //check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
	else {
		maxX = getmaxx();
		maxY = getmaxy();
	}
}

void button() {
	int countUp = 0;
	int screenInto8 = (getmaxx() / 8);
	while (countUp <= 7) {

		COLOR[countUp].init((((countUp + 1)*screenInto8) - screenInto8), (getmaxy()- 100), ((countUp + 1)*screenInto8),(getmaxy()-50), countUp);
		COLOR[countUp+8].init((((countUp + 1)*screenInto8) - screenInto8), (getmaxy() - 50), ((countUp + 1)*screenInto8), getmaxy(), countUp+8);

		countUp++;
	}
	LEFT.init(((maxX / 8) * 2) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 2) + 40, ((maxY / 3) * 2) + 40, WHITE);
	LEFT.isLEFT = true;
	RIGHT.init(((maxX / 8) * 6) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 6) + 40, ((maxY / 3) * 2) + 40, WHITE);
	
	LEFT.ARROW();
	RIGHT.ARROW();
	/*
	setcolor(WHITE);
	bar(((maxX / 8) * 2) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 2) + 40, ((maxY / 3) * 2) + 40);
	bar(((maxX / 8) * 6) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 6) + 40, ((maxY / 3) * 2) + 40);
	setcolor(BLACK);
	rectangle(((maxX / 8) * 2) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 2) + 40, ((maxY / 3) * 2) + 40);
	rectangle(((maxX / 8) * 6) - 40, ((maxY / 3) * 2) - 40, ((maxX / 8) * 6) + 40, ((maxY / 3) * 2) + 40);
	*/
	


	

}
void design();
void KEY_LISTENER();
void test() {
	thread miata(design);
	thread listener(KEY_LISTENER);
	miata.join();
	listener.join();
	
	//BottomTone, TopTone, Interior, Outline, Wheels, Windows, TireWall, TailLights,
	
}
void design(){
	gr_Start(GrDriver, GrMode, ErrorCode);
	char t;
	BODYKITS.push_back(TopTone);
	BODYKITS.push_back(BottomTone);
	BODYKITS.push_back(Interior);
	BODYKITS.push_back(Outline);
	BODYKITS.push_back(Wheels);
	BODYKITS.push_back(Windows);
	BODYKITS.push_back(TireWall);
	BODYKITS.push_back(TailLights);
	string Miata = "1990 Mazda MX-5 Miata";
	MIATA SELECTION = STOCK;
	MIATA Part = TopTone;
	bool ChangeCar = true;
	bool ChangePart = true;
	bool ColorCar = false;
	int COLORSELECT = 0;
	int NUMParts = BODYKITS.size();
	int currKIT = 0;
	setbkcolor(14);
	cleardevice();
	setbkcolor(0);
	background = 14;
	tireWall = 8;
	outline = 0;
	bodyRails = 0;
	lightsAndHandle = 7;
	interior = 6;
	windows = 8;
	wheels = 7;
	bottomTone = 4;
	tailLights = 12;
	topTone = 4;	
	settextstyle(0, 0, 4);
		int textWidth = (textwidth(Miata.c_str())) / 2;
		int center = (getmaxx() / 2) - (textWidth);
		outtextxy(center, 5, Miata.c_str());

		button();
	while (isRunning) {

		if (ChangeCar) {
			setbkcolor(BLACK);
			if (ColorCar) {
				switch (Part) {
					/*
					tireWall = 90;
					bodyRails = 89;
					lightsAndHandle = 56;
					interior = 66;
					windows = 87;
					wheels = 88;
					bottomTone = 45;
					tailLights = 86;
					topTone = 91;
					outline = 0;
					*/
				case TopTone:
					topTone = COLORSELECT;
					break;
				case BottomTone:
					bottomTone = COLORSELECT;
					break;
				case Interior:
					interior = COLORSELECT;
					break;
				case Outline:
					outline = COLORSELECT;
					break;
				case Wheels:
					wheels = COLORSELECT;
					break;
				case Windows:
					windows = COLORSELECT;
					break;
				case TireWall:
					tireWall = COLORSELECT;
					break;
				case TailLights:
					tailLights = COLORSELECT;
					break;
				}
			}
			if (SELECTION == STOCK) {
				
				drawMiata((maxX - miata_Width) / 2, (maxY - miata_Height) / 3);
			}
			else if (SELECTION == SLAMMED) {

			}
			ChangeCar = false;
			ColorCar = false;
		}
		
		if (ChangePart) {
			setcolor(background);
			bar(LEFT.Right + 2 , (2 * maxY / 3) - miata_Height, RIGHT.Left - 2, (2 * maxY / 3) + miata_Height);
			setbkcolor(BLACK);
			switch (Part) {
			case TopTone:
				drawtopTone((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - topTone_Height);
				break;
			case BottomTone:
				drawbottomTone((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - bottomTone_Height);
				break;
			case Interior:
				drawinterior((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - interior_Height);
				break;
			case Outline:
				drawoutline((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - outline_Height);
				break;
			case Wheels:
				drawwheels((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - wheels_Height);
				break;
			case Windows:
				drawwindows((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - windows_Height);
				break;
			case TireWall:
				drawtireWall((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - tireWall_Height);
				break;
			case TailLights:
				drawtailLights((maxX - miata_Width) / 2, ((maxY / 3) + (maxY / 3)) - tailLights_Height);
				break;
			}
			ChangePart = false;
		}

		if (mouseInput.isPressed && LEFT.isPressed(click) || keyInput.isPressed && keyInput.VirtualKey == VK_LEFT) {
			currKIT = (NUMParts + currKIT - 1) % NUMParts;
			Part = BODYKITS.at(currKIT);
			ChangePart = true;
			mouseInput.reset();
			keyInput.reset();
		}
		else if (mouseInput.isPressed && RIGHT.isPressed(click) || keyInput.isPressed && keyInput.VirtualKey == VK_RIGHT) {
			currKIT = (currKIT + 1) % NUMParts;
			Part = BODYKITS.at(currKIT);
			ChangePart = true;
			mouseInput.reset();
			keyInput.reset();
		}
		else if (mouseInput.isPressed) {
			COLORSELECT = 99;
			for (int i = 0; i < 16 && COLORSELECT == 99; i++) {
				if (COLOR[i].isPressed(click)) {
					cout << i << endl;
					COLORSELECT = i;
					ChangeCar = true;
					ChangePart = true;
					ColorCar = true;
				}
			}
			mouseInput.reset();
			keyInput.reset();
		}
		
		Sleep(15);
	}
}
void KEY_LISTENER() {
	while (isRunning) {
		if (mousePress(VK_LBUTTON)) {

		}
		if (KEYBOARD(VK_LEFT)) {

		}
		if (KEYBOARD(VK_RIGHT)) {

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
		mouseInput.isPressed = true;
		return true;
	}
	return false;
}
bool KEYBOARD(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		keyInput.VirtualKey = VirtualKey;
		int count = 0;
		do {
			if (count++ > 200) {
				keyInput.isHeld = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		keyInput.isPressed = true;
		return true;
	}
	return false;

}