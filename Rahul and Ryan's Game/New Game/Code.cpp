#include <iostream>
#include <iomanip>
#include <ctime>
#include <string>
#include <fstream>
#include <conio.h>
#include <vector>
#include <graphics.h>
#include <winbgi.cpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<cmath>
#include <sstream>
#include<thread> // THREADS
#include<Windows.h>  // CURSOR
#include<mmsystem.h>  // SOUND
#include <mciapi.h> // SOUND
#include "VirtualKeys.h" // CUSTOM
#define _WIN32_WINNT 0x0500
#pragma comment (lib, "winmm.lib") //sound


using namespace std;
int carDelay;
int GrDriver, GrMode, ErrorCode;
bool gameACTIVE;
bool cursorVisible;
int centerX, centerY;
BOOL curVis;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
HCURSOR visible, hCur1, hCur3;
void gr_Start(int&, int&, int&);
void getCursor(POINT &p);
bool MOUSE(int);
bool KEYBOARD(int);
void KEY_LISTENER();
void music();
void playSound(string, int);
void Collision();
void enemies();
INPUT_RECORD irInBuf;
int yoy, ycoord1, ycoord2, ycoord3, ycoord4, ycoord5, ycoord6, xcoord1, xcoord2, xcoord3, xcoord4, xcoord5, xcoord6, xcoord7, xcoord8, xcoord9, xcoord10, xcoord11, xcoord12;
int y7, y2, p1, p2, p3, p4, p5, p6;
int fad;
int lim;
int a, q;

time_t then;


void barlane1(int w, int x, int y, int z){//generate enemy rectangles in lane 1
	setcolor(0);
	bar(w, x, y, z);
}

void barlane2(int q, int e, int r, int t){//generate enemy rectangles in lane 2
	setcolor(0);
	bar(q, e, r, t);
}

void barlane3(int u, int i, int o, int p){//generate enemy rectangles in lane 3
	setcolor(0);
	bar(u, i, o, p);
}

void medians(){// generate the dashed lines in each lane
	yoy = 100;
	setcolor(14);
	bar(310, yoy, 330, yoy + 100);
	bar(310, yoy + 200, 330, yoy + 300);
	bar(310, yoy + 400, 330, yoy + 500);
	bar(310, yoy + 600, 330, yoy + 700);
	bar(310, yoy + 800, 330, yoy + 900);

	bar(950, yoy, 970, yoy + 100);
	bar(950, yoy + 200, 970, yoy + 300);
	bar(950, yoy + 400, 970, yoy + 500);
	bar(950, yoy + 600, 970, yoy + 700);
	bar(950, yoy + 800, 970, yoy + 900);

	bar(1590, yoy, 1610, yoy + 100);
	bar(1590, yoy + 200, 1610, yoy + 300);
	bar(1590, yoy + 400, 1610, yoy + 500);
	bar(1590, yoy + 600, 1610, yoy + 700);
	bar(1590, yoy + 800, 1610, yoy + 900);


}

void lines(){//generate the two lane lines
	setcolor(14);
	line(639, 0, 639, 1017);
	line(1278, 0, 1278, 1017);
}

string ToString(int asdf)//To String
{
	stringstream ss;
	ss << asdf;
	string str = ss.str();
	return str;
}

void epicfailure(){//display "FAIL", code at the end will run this in a crash
	gameACTIVE = false;
	Sleep(100);
	cleardevice();
	while (1 > 0){
		setcolor(4);//These rectangles make up the giant letters 'FAIL'
		bar(100, 100, 200, 900);
		bar(150, 100, 400, 200);
		bar(150, 300, 400, 400);
		bar(450, 100, 550, 900);
		bar(450, 100, 900, 150);
		bar(450, 300, 900, 350);
		bar(800, 100, 900, 900);
		bar(1100, 100, 1250, 900);
		bar(1400, 100, 1550, 900);
		bar(1400, 800, 1800, 900);
		fad = clock() - then;
		playSound("carskid.wav", 0);//car skid
		playSound("explosion.wav", 0);//car crash noise
		Sleep(30);
		settextstyle(3, HORIZ_DIR, 3);
		setcolor(4);
		outtextxy(100, 950, "your score: ");
		settextstyle(3, HORIZ_DIR, 3);
		string fail = ToString(fad);
		setcolor(4);
		moveto(275, 950);
		outtext(TEXT(fail.c_str()));
		_getch();
	}
}

void playSound(string fileName, int dur){
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
	return;
}

void gr_start(int&GrDriver, int&GrMode, int&ErrorCode){
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk){
		cout << "Error:" << ErrorCode;
		_getch();
		exit(1);
	}
}

struct PASS{

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
}global;

void main(){
	gr_start(GrDriver, GrMode, ErrorCode);
	carDelay = 50;
	hCur1 = LoadCursorFromFile("invisible2.cur");
	SetCursor(hCur1);
	srand(time(NULL));
	gameACTIVE = true;
	q = 25000;
	settextstyle(3, HORIZ_DIR, 3);
	moveto(680, 100);
	outtext("GLITCHY TAXI FEAT. GHOST RIDER");
	moveto(150, 300);
	outtext("This game will start automatically in about 25 seconds. Please read the directions below and put sound on.");
	moveto(50, 510);
	outtext("You are a car driving on the wrong side of the road. Press J to move left and K to move right in order to avoid oncoming vehicles.");
	moveto(500, 560);
	outtext("Be warned, the oncoming vehicles will get faster over time.");
	moveto(40, 610);
	outtext("Your car and the enemies are invisible and only cast a shadow. Your car will be bright yellow until the first time you move it.");
	moveto(80, 660);
	outtext("Afterword, your car will fade into the darkness, randomly flashing yellow every so often to alert you to its position.");
	Sleep(q);
	cleardevice();
	then = clock();
	setcolor(14);
		///
		thread t(KEY_LISTENER);
		thread c(Collision);
		thread x(enemies);
		thread m(music);
		global.press = global.hold = false;///
		t.join();
		x.join();
		c.join();
		m.join();
		return;///
		srand(time(NULL));

		



		lines();
		medians();

		
		system("cls");
		


		_getch();
		closegraph();
	}

bool KEYBOARD(int VirtualKey){//from source code
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf.EventType = KEY_EVENT;
		global.press = true;
		global.VirtualKey = VirtualKey;
		int count = 0;
		do{
			if (count++ > 200){
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}

void music(){
	while (gameACTIVE){
		playSound("easy.mp3", 421000);
	}
}

void KEY_LISTENER(){//recognize key events for left(J) and right(K)
	
	
	//gr_start(GrDriver, GrMode, ErrorCode);// initi of graphics window 
	y7 = 800;//player ycoord
	y2 = 1000;//player ycoord
	p3 = 660;//player xcoord
	p4 = 1260;//player xcoord

	while (gameACTIVE){
		if (KEYBOARD(VK_J)){
			cout << "j Key Pressed \n";
			setcolor(0);
			bar(p3, y7, p4, y2);
			setcolor(14);
			p3 -= 650;
			p4 -= 650;
			if (p3 <= 0){
				epicfailure();//kills character if it goes off the screen
			}
		}
		if (KEYBOARD(VK_K)){
			cout << "k Key Pressed \n";
			setcolor(0);
			bar(p3, y7, p4, y2);
			setcolor(14);
			p3 += 650;
			p4 += 650;
			if (p4 >= 1919){
				epicfailure();//kills character if it goes off the screen
			}
		}

	}

}

void enemies(){//draw the character, makes character flash occasionally, create enemies and cause them to move
	while (gameACTIVE){
		srand(time(NULL));
		a = rand() % 3 + 1;
		lim = 1017;
		int b = rand() % 13 + 1;
		setcolor(14);
		bar(p3, y7, p4, y2);//draw character
		if (a == 2 && gameACTIVE){
			setcolor(b);
			ycoord1 = 50;
			ycoord2 = 250;
			xcoord1 = 10;
			xcoord2 = 600;
			xcoord7 = 650;
			xcoord8 = 1250;
			barlane1(xcoord1, ycoord1, xcoord2, ycoord2);
			barlane1(xcoord7, ycoord1, xcoord8, ycoord2);
			
			while (ycoord1 < lim){
				if (!gameACTIVE)
					return;
				
				barlane1(xcoord1, ycoord1, xcoord2, ycoord2);//
				barlane1(xcoord7, ycoord1, xcoord8, ycoord2);//
				//cleardevice();
				ycoord1 += 10;
				ycoord2 += 10;
				lines();
				medians();
				setcolor(b);
				barlane1(xcoord1, ycoord1, xcoord2, ycoord2);
				barlane1(xcoord7, ycoord1, xcoord8, ycoord2);
				Sleep(carDelay);

			}

		}
		if (a == 1 && gameACTIVE){
			setcolor(b);
			ycoord3 = 50;
			ycoord4 = 250;
			xcoord3 = 650;
			xcoord4 = 1250;
			xcoord9 = 1290;
			xcoord10 = 1890;
			barlane2(xcoord3, ycoord3, xcoord4, ycoord4);
			barlane2(xcoord9, ycoord3, xcoord10, ycoord4);
			
			while (ycoord3 < lim){
				if (!gameACTIVE)
					return;
				
				barlane2(xcoord3, ycoord3, xcoord4, ycoord4);
				barlane2(xcoord9, ycoord3, xcoord10, ycoord4);
				lines();
				medians();
				//cleardevice();
				ycoord3 += 10;
				ycoord4 += 10;
				lines();
				medians();
				setcolor(b);
				barlane2(xcoord3, ycoord3, xcoord4, ycoord4);
				barlane2(xcoord9, ycoord3, xcoord10, ycoord4);
				Sleep(carDelay);

			}

		}
		else if(gameACTIVE){
			setcolor(b);
			ycoord5 = 50;
			ycoord6 = 250;
			xcoord5 = 1290;
			xcoord6 = 1890;
			xcoord11 = 10;
			xcoord12 = 600;
			barlane3(xcoord5, ycoord5, xcoord6, ycoord6);
			barlane3(xcoord11, ycoord5, xcoord12, ycoord6);
			
			while (ycoord5 < lim){
				if (!gameACTIVE)
					return;
				
				
				barlane3(xcoord5, ycoord5, xcoord6, ycoord6);
				barlane3(xcoord11, ycoord5, xcoord12, ycoord6);
				lines();
				medians();
				//cleardevice();
				ycoord5 += 10;
				ycoord6 += 10;
				lines();
				medians();
				setcolor(b);
				barlane3(xcoord5, ycoord5, xcoord6, ycoord6);
				barlane3(xcoord11, ycoord5, xcoord12, ycoord6);
				Sleep(carDelay);

			}

		}

			carDelay -= 2;
			if (carDelay < 10){
				carDelay = 10;
			}
		
	}
}

void Collision(){//kills character if it contacts any enemy rectangle
while (1>0 && gameACTIVE){
if (xcoord4 == 1250 && ycoord4 >= 800 && ycoord3 <= 1000 && p3 == 660){
	epicfailure();
}
else if (xcoord2 == 600 && ycoord2 >= 800 && ycoord1 <= 1000 && p3 == 10){
	epicfailure();
}
else if (xcoord6 == 1890 && ycoord6 >= 800 && ycoord5 <= 1000 && p3 == 1310){
	epicfailure();
}
else if (xcoord12 == 600 && ycoord6 >= 800 && ycoord5 <= 1000 && p3 == 10){
	epicfailure();
}
else if (xcoord10 == 1890 && ycoord4 >= 800 && ycoord3 <= 1000 && p3 == 1310){
	epicfailure();
}
else if (xcoord8 == 1250 && ycoord2 >= 800 && ycoord1 <= 1000 && p3 == 660){
	epicfailure();
}
}
}
