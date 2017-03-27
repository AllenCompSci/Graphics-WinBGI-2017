#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winbgi.cpp>
#include <graphics.h>
#include "draw.h"
#include <Windows.h>

using namespace std;

int GrDriver, GrMode, ErrorCode;
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

void test() {
	gr_Start(GrDriver, GrMode, ErrorCode);
	drawCoundDownCenter();
	PageOutline();
	char t = getch();
	setbkcolor(1);
	cleardevice();
	setbkcolor(0);
	PageOutline();
	drawLevel8Center();
	t = getch();
	setbkcolor(1);
	cleardevice();
	setbkcolor(0);
	PageOutline();
	drawLevel7Center();
	t = getch();
	cleardevice();
	setbkcolor(0);
	PageOutline();
	drawLevel6Center();
}