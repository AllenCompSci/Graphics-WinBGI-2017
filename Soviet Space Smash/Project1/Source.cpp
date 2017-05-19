#pragma region game1

#include <string>

#include <locale>

#include <sstream>

#include <iostream>

#include <fstream>

#include <ctime>

#include <Windows.h>

#include <iomanip>

#include <thread>

#include <stdio.h>

#include <winbgi.cpp>

#include <graphics.h>

#include <mmsystem.h>

#include <mciapi.h>

#include <cstdlib>

#include "VirtualKeys.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

int GrDriver, GrMode, ErrorCode;

bool redraw;

bool gameOver = true;

bool gamerun = true;

void gr_Start(int&, int&, int&);
 
INPUT_RECORD irInBuf;

struct PASS{

	int VirtualKey;

	POINT p;

	bool press;

	bool hold;

}global;

void gr_start(int&GrDriver, int&GrMode, int&ErrorCode){
	GrDriver = VGA;

	GrMode = VGAMAX;

	initgraph(&GrDriver, &GrMode, "");

	ErrorCode = graphresult();

	if (ErrorCode != grOk){

		cout << "Error: " << ErrorCode;

		exit(1);

	}
}

bool KEYBOARD(int VirtualKey){

	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0){

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

struct position{
	double Left;
	double Top;


	double Right;


	double Bot;


	void init(){


		Left = getmaxx() / 2 - 15;


		Top = getmaxy() - 150;


		Right = Left + 20;


		Bot = Top + 20;


	}
};

struct rect{


	position soviet;


};

bool stopSound;

rect make_bar(colors t){

	rect rect1;

	rect1.soviet.init();

	setcolor(t);

	rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);

	return rect1;

}


time_t start;


time_t endtime;


void playSound(string fileName, int duration);


void SOUND();


void KEY_LISTENER(){
	while (gameOver){
		if (KEYBOARD(VK_A)){
			cout << "A Key Pressed \n";
		}
		if (KEYBOARD(VK_W)){
			cout << "W Key Pressed \n";	
		}
		if (KEYBOARD(VK_S)){
			cout << "S Key Pressed \n";	
		}
		if (KEYBOARD(VK_D)){
			cout << "D Key Pressed \n";
		}
		if (KEYBOARD(VK_ESCAPE)){
			gameOver = false; 
			gamerun = false;

		}

	}

	Sleep(500);
	gameOver = false;
}




void explosion(int t, int q, int er, int re, int yt, int uy){
	gameOver = false;
	gamerun = false;
	stopSound = true;
	int x0 = t + 10; int x1 = t + 10; int x2 = t + 10; int x3 = t + 10; int x4 = t + 10; int x5 = t + 10;
	int x6 = t + 10; int x7 = t + 10; int x8 = t + 10;	int x9 = t + 10;	int x10 = t + 10;	int x11 = t + 10;
	int x12 = t + 10;	int x13 = t + 10;	int x14 = t + 10;	int x15 = t + 10;	int y0 = q + 10;	int y1 = q + 10;
	int y2 = q + 10;	int y4 = q + 10;	int y5 = q + 10; int y6 = q + 10;	int y7 = q + 10;
	int y8 = q + 10;	int y9 = q + 10;	int y10 = q + 10;	int y11 = q + 10;
	int y12 = q + 10;	int y13 = q + 10;	int y14 = q + 10;	int y15 = q + 10;
	int radi = 0;	int color = 4; int y3 = q + 10;
	while (radi < 150){
		setcolor(BLACK);		rectangle(er,re,yt,uy);		Sleep(1);	
		x0 -= 1;		y0 -= 2;
		putpixel(x0, y0, 4);	setcolor(4);    radi += 2;		y1 -= 3;
		putpixel(x1, y1, 4);		x2 += 1;		y2 -= 2;
		putpixel(x2, y2, 4);		x3 += 2;		y3 -= 2;
		putpixel(x3, y3, 4);		x4 += 2;		y4 -= 1;
		putpixel(x4, y4, 4);		x5 += 3;
		putpixel(x5, y5, 4);		x6 += 2;		y6 += 1;
		putpixel(x6, y6, 4);		x7 += 2;		y7 += 2;
		putpixel(x7, y7, 4);		x8 += 1;		y8 += 2;
		putpixel(x8, y8, 4);		y9 += 3;
		putpixel(x9, y9, 4);		x10 -= 1;		y10 += 2;
		putpixel(x10, y10, 4);		x11 -= 2;		y11 += 2;
		putpixel(x11, y11, 4);		x12 -= 2;		y12 += 1;
		putpixel(x12, y12, 4);		x13 -= 3;
		putpixel(x13, y13, 4);		x14 -= 2;		y14 -= 1;
		putpixel(x14, y14, 4);		x15 -= 2;		y15 -= 2;
		putpixel(x15, y15, 4);
		if (radi % 10 == 0){
			circle(t+10, q+10, radi);
			setcolor(color);		}	radi += 1;		}
gameOver = false;
gamerun = false;
	}


void enemies(){

	double move = .07;

	bool keyPress;

	rect rect1;	

	rect1 = make_bar(BLUE);

	bool redrawBLOCK = true;
	
	redraw = true;
	
	while (gameOver == true){
		
		start = clock();
		int rectlr[400]; int recttb[400]; int x[400]; int y[400];
		for (int i = 0; i <= 399; i++){	
		
			
			
			Sleep(10);
			
			if (i == 399){	
			
				i -= 1;	

			}

		
			if (i % 2 == 0){
				x[i] = rand() % 1910 + 10;

				y[i] = rand() % 1015;

			}

			
#pragma endregion
				if (i % 4 == 0){ //Increase by multiples of 4 to decrease density
#pragma region game2
					rectlr[i] = rand() % 1910 + 10;

					recttb[i] = -20;
				}
	
			for (int c = 0; gameOver && c <= i - 1; c +=4){	
			
				setcolor(BLACK);
				
				rectangle(rectlr[c], recttb[c], rectlr[c] + 20, recttb[c] + 20);
				
				putpixel(x[c], y[c], BLACK);

			y[c]+= 1;		
#pragma endregion
			recttb[c] += 8;	 //Higher makes enemies faster 

#pragma region game3

			setcolor(RED);
			
			rectangle(rectlr[c], recttb[c], rectlr[c] + 20, recttb[c] + 20);
			
			putpixel(x[c], y[c], WHITE);
			
			if (recttb[c] > 1040){
			
				rectlr[c] = rand() % 1910 + 10;	recttb[c] = -20;
				
			}

			if (y[c] > 1020){
			
				y[c] = rand() % 1910 + 10;
				
				y[c] = -5;
				
			}
			
			if (c <= i){
			
				if (global.hold == TRUE){
				
					if (global.VirtualKey == VK_A){
					
						if (rect1.soviet.Left >= 5){
						
							setcolor(BLACK);
							
							rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);
							
							rect1.soviet.Left -= move;
							
							rect1.soviet.Right -= move;
							
							setcolor(BLUE);
							
							rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);
							
							global.VirtualKey != VK_A;
							
							global.hold = FALSE;


							
						}
						
					}
					
					if (global.VirtualKey == VK_D){
					
						if (rect1.soviet.Right <= 1910){
						
							setcolor(BLACK);
							
							rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);
							
							rect1.soviet.Left += move;
							
							rect1.soviet.Right += move;
							
							setcolor(BLUE);
							
							rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);
							
							global.VirtualKey != VK_D;
							
							global.hold = FALSE;
							
						}
						

					}
					

				}
#pragma endregion
#pragma region game4
				setcolor(BLUE);

				
				rectangle(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);

				
			}
			
			if ((recttb[c] + 20 >= rect1.soviet.Top) && (recttb[c] <= rect1.soviet.Bot)){

			
				if ((rect1.soviet.Left >= rectlr[c]) && (rect1.soviet.Left <= rectlr[c] + 20)){

				
					explosion(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);	

					
					endtime = clock();

					
					gameOver = false;

					
					gamerun = false;

					
					return;

					
				}
				
				if ((rect1.soviet.Right >= rectlr[c]) && (rect1.soviet.Right <= rectlr[c] + 20)){

				
					explosion(rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Left, rect1.soviet.Top, rect1.soviet.Right, rect1.soviet.Bot);

					
					endtime = clock();

					
					gameOver = false;

					
					gamerun = false;	

					
					return;
#pragma endregion 
					}
				}
			}
		}
	}
}
#pragma region game4
void main(){

	srand((unsigned int)time(NULL));

	gr_start(GrDriver, GrMode, ErrorCode);

	global.press = global.hold = false;



	int a = 1;

	int b = 1;

	int c = 1;

	int d = 1;

	int e = 1;

	int f = 1;

	int q = 1;

	int h = 1;




	
	int scroll = -2;
	
	int py1 = 1700;
	
	settextstyle(10, 0, 3);
	
	/*
	
	do{
	
	setcolor(BLACK);
	
	delay(3);
	
	outtextxy(100, py1 + 100, "You, Commander-in-chief Ronald Reagan, fight for a free world, fight for freedom, fight for a bright future. Are you ready?");
	
	outtextxy(200, py1, "It’s up to you, Command-in-Chief Reagan, to fight back the Soviet bear hordes. You are humanity's last hope.");
	
	outtextxy(30, py1 - 100, "The Red Army Soviet Space Bears Grand Space Fleet is off the bow of your spacecraft and they are sending everything they’ve got at you. ");
	
	outtextxy(250, py1 - 200, "You are Commander-in-Chief Ronald Reagan, the last hope for the free world; Champion of America.");
	
	outtextxy(150, py1 - 300, "The Soviets sought to expand their grand communist regime across all available land, sea, and air, including… Space.");
	
	outtextxy(700, py1 - 400, "Thousands were killed daily by the Red Communist Bear Army; Stalin's personal army of death and stuff. And they were all bears.");
	
	outtextxy(200, py1 - 500, "The people of the Soviet Union were horribly oppressed; so much so that they forgot what freedom was.");
	
	outtextxy(150, py1 - 600, "A long time ago in a Europe far far away there was the kingdom of the Soviet Union, ruled by the evil, cruel Stalin. ");
	
	py1 += scroll;
	
	setcolor(YELLOW);
	
	outtextxy(100, py1 + 100, "You, Commander-in-chief Ronald Reagan, fight for a free world, fight for freedom, fight for a bright future. Are you ready?");
	
	outtextxy(200, py1, "It’s up to you, Command-in-Chief Reagan, to fight back the Soviet bear hordes. You are humanity's last hope.");
	
	outtextxy(30, py1 - 100, "The Red Army Soviet Space Bears Grand Space Fleet is off the bow of your spacecraft and they are sending everything they’ve got at you. ");
	
	outtextxy(250, py1 - 200, "You are Commander-in-Chief Ronald Reagan, the last hope for the free world; Champion of America.");
	
	outtextxy(150, py1 - 300, "The Soviets sought to expand their grand communist regime across all available land, sea, and air, including… Space.");
	
	outtextxy(70, py1 - 400, "Thousands were killed daily by the Red Communist Bear Army; Stalin's personal army of death and stuff. And they were all bears.");
	
	outtextxy(200, py1 - 500, "The people of the Soviet Union were horribly oppressed; so much so that they forgot what freedom was.");
	
	outtextxy(150, py1 - 600, "A long time ago in a Europe far far away there was the kingdom of the Soviet Union, ruled by the evil, cruel Stalin. ");
	
	if (py1 == -130){
	
	b += 1;
	
	}
	
	} while (b == 1);
	
	#pragma endregion
	
	*/
	
	Sleep(1000);


	SetCursor(0);



do{
	int radius = 1;

	int radius2 = 1;
	
	int radius3 = 1;

	int radius4 = 1;
	
	int radius5 = 1;

	int radius6 = 1;
	
	int radius7 = 1;

	int radius8 = 1;
	
	int radius9 = 1;

	int radius10 = 1;
	
	int radius11 = 1;

	int radius12 = 1;
	
	int radius13 = 1;

	int radius14 = 1;
	
	int radius15 = 1;

	int radius16 = 1;


	
	do{

		delay(1);
		
		setcolor(BLACK);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius2);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius3);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius4);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius5);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius6);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius7);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius8);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius9);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius10);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius11);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius12);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius13);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius14);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius15);
		
		circle(getmaxx() / 2, getmaxy() / 2, radius16);
		
		radius += 25;

			radius2 += 24;
		
			radius3 += 23;
			
			radius4 += 22;
			
			radius5 += 21;
			
			radius6 += 20;
			
			radius7 += 19;
			
			radius8 += 18;

			radius9 += 17;
			
			radius10 += 16;
			
			radius11 += 15;
			
			radius12 += 14;
			
			radius13 += 13;
			
			radius14 += 12;
			
			radius15 += 11;
			
			radius16 += 10;
			
			setcolor(WHITE);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius);
		
			circle(getmaxx() / 2, getmaxy() / 2, radius2);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius3);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius4);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius5);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius6);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius7);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius8);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius9);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius10);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius11);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius12);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius13);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius14);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius15);
			
			circle(getmaxx() / 2, getmaxy() / 2, radius16);
			
			if (radius > 2750){
			
				c += 1;
		
			}
	
	} while (c == 1);


	c = 1;	f++;

} while (f < 2);



thread sound(SOUND);

thread u(enemies);

thread ty(KEY_LISTENER);



sound.join();

u.join();

ty.join();

Sleep(2000);


cleardevice(); 

#pragma region hello-lose-over

do{

	setcolor(WHITE); outtextxy(880, 510, "You Lose"); Sleep(500);
	
	outtextxy(1000, 510, "."); Sleep(250); outtextxy(1020, 510, "."); Sleep(250); outtextxy(1040, 510, "."); Sleep(250); outtextxy(1060, 510, "."); Sleep(250);

	setcolor(BLACK); Sleep(1000); outtextxy(880, 510, "You Lose");  outtextxy(1000, 510, "."); outtextxy(1020, 510, "."); outtextxy(1040, 510, "."); outtextxy(1060, 510, ".");
	
	setcolor(WHITE); outtextxy(860, 510, "Game Over"); Sleep(1000);

	outtextxy(1000, 510, "."); Sleep(250); outtextxy(1020, 510, "."); Sleep(250); outtextxy(1040, 510, "."); Sleep(250);  outtextxy(1060, 510, "."); Sleep(250);
	
	setcolor(BLACK); Sleep(1000); outtextxy(860, 510, "Game Over"); outtextxy(1000, 510, "."); outtextxy(1020, 510, "."); outtextxy(1040, 510, "."); outtextxy(1060, 510, ".");

	a += 1;
	
} while (a == 1);

string score = "Score: " + to_string(endtime);

setcolor(WHITE); 

cout << endtime << " " << score;

outtextxy(880, 510,  score.c_str());	

outtextxy(600, 510, "Score:"); 

Sleep(10000);

outtextxy(1000, 510, "");


#pragma endregion





closegraph();}


void SOUND(){

	string fileName = "Soviet_March.mp3";

	stopSound = false;

	int duration = 280000;

	while (gameOver){

		playSound(fileName, duration);


	}

}


void playSound(string fileName, int duration){
	
	string concat = "open " + fileName + " type mpegvideo alias MP3_Device";
	
	LPCSTR a = concat.c_str();
	
	mciSendString(a, NULL, 0, 0);

	
	mciSendString("play MP3_Device", NULL, 0, 0);
	
	DWORD current = GetTickCount();
	
	DWORD endSong = current + (DWORD)duration;
	
	while (!stopSound && current < endSong){
	
		current = GetTickCount();

	}

	
	mciSendString("stop MP3_Device", NULL, 0, 0);
	
	mciSendString("close MP3_Device", NULL, 0, 0);


}



#pragma endregion 