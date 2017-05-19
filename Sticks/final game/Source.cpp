//jaboi sexy beast in tha club
#include <iostream>
#include<cmath>
#include <string>
#include <ctime>
#include<thread> // THREADS
#include <graphics.h>
#include <winbgi.cpp>
#include<Windows.h>  // CURSOR
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include<mmsystem.h>  // SOUND
#include <mciapi.h> // SOUND
#include "VirtualKeys.h" // CUSTOM
#include <sstream>
#pragma comment(lib, "winmm.lib") // SOUND
using namespace std;
int i = 0;
int GrDriver, GrMode, ErrorCode;
bool cursorVisible;
bool pturn = false;
bool cturn = false;
bool brk = false;
bool trubrk = false;
bool out = false;
bool re = false;
bool pturnA = false;
bool change = false;
int centerX, centerY;
int cmode = 0;//computer difficulty mode 1 = easy     2 = hard    3 = p v p
//type of move for the combined move function (1 = move, 2 = split) now in global struct
int dec = 0;
BOOL curVis;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
HCURSOR visible, hCur1, hCur3;
void gr_Start(int&, int&, int&);
void getCursor(POINT &p);
bool MOUSE(int);
bool KEYBOARD(int);
void KEY_LISTENER();
void playSound(string, int);
void gameThread();
INPUT_RECORD irInBuf;

struct PASS{

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
	int sel;
	int at;
	int type;
}global;
int col, deca, x, y;
enum pt{p1, p2};
pt playerturn;
void gr_Start(int &, int &, int &);
char r;
struct hand{
	int fing;
	bool out;
	int pos;//change
}h1, h2, h3, h4;
/*

 h1         h2


 h3         h4

 or

 1          2


 3          4

 */
//hand layout
void startsound(){
	mciSendString("open \"panda.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
	mciSendString("play mp3 repeat", NULL, 0, NULL);
}
void startscreen(){
	
	setcolor(BLACK);
	floodfill(0, 0, YELLOW);
	setbkcolor(BLACK);
	int colorsss = rand() % 14 + 2;
	setcolor(WHITE);
	settextstyle(3, 0, 20);
	string sticks = "STICKS";
	string anykey = "Press any key to continue";
	setcolor(colorsss);
	outtextxy(500, 500, sticks.c_str());
	setcolor(colorsss);
	settextstyle(10, 0, 5);
	outtextxy(700, 700, anykey.c_str());
	setcolor(BLACK);
	
	getch();


}
void screentwo(int &i, int &cmode){
	char input1 = 0;
	char input2 = 0;
	cleardevice();
	setcolor(BLACK);
	string warning = "Hit number according to which ever you want!";
	string pvp = "1. Player vs. Player";
	string pvp2 = "Player vs. Player";
	string pvai = "2. Player vs. Computer EASY";
	string pvai2 = "Player vs. Computer EASY";
	string pvai3 = "3. Player vs. Computer HARD";
	string pvai4 = "Player vs. Computer HARD";
	string time1 = "1. 5 Second";
	string time2 = "2. 10 Second";
	string time3 = "3. 15 Second";
	string time4 = "4. Unlimited";
	setcolor(BLUE);
	outtextxy(10, 0, warning.c_str());
	outtextxy(300, 308, pvp.c_str());
	outtextxy(300, 508, pvai.c_str());
	outtextxy(300, 708, pvai3.c_str());
	setbkcolor(BLACK);
	do{
		input1 = getch();
		switch (input1)
		{
		case '1':
			cmode = 3;
			cleardevice();
			setbkcolor(BLACK);
			outtextxy(0, 0, pvp2.c_str());
			setcolor(BLUE);
			outtextxy(0, 900, warning.c_str());
			setcolor(GREEN);
			outtextxy(10, 50, time1.c_str());
			setcolor(CYAN);
			outtextxy(10, 100, time2.c_str());
			setcolor(LIGHTBLUE);
			outtextxy(10, 150, time3.c_str());
			setcolor(LIGHTMAGENTA);
			outtextxy(10, 200, time4.c_str());
			do{
				input2 = getch();
				switch (input2){
				case '1':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time1.c_str());
					i = 5;
					break;
				case '2':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time2.c_str());
					i = 10;
					break;
				case '3':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time3.c_str());
					i = 15;
					break;
				case '4':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time4.c_str());
					i = 999999999;
					break;
				}
				break;
			} while (1>0);
			
			break;

		case '3':
			cmode = 2;
			cleardevice();
			setbkcolor(BLACK);
			outtextxy(0, 0, pvai4.c_str());
			setcolor(BLUE);
			outtextxy(0, 900, warning.c_str());
			setcolor(GREEN);
			outtextxy(10, 50, time1.c_str());
			setcolor(CYAN);
			outtextxy(10, 100, time2.c_str());
			setcolor(LIGHTBLUE);
			outtextxy(10, 150, time3.c_str());
			setcolor(LIGHTMAGENTA);
			outtextxy(10, 200, time4.c_str());
			do{
				input2 = getch();
				switch (input2){
				case '1':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time1.c_str());
					i = 5;
					break;
				case '2':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time2.c_str());
					i = 10;
					break;
				case '3':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time3.c_str());
					i = 15;
					break;
				case '4':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time4.c_str());
					i = 999999999;
					break;
				}
				break;
			} while (1>0);

			break;

		case '2':
			cmode = 1;
			cleardevice();
			setbkcolor(BLACK);
			outtextxy(0, 0, pvai2.c_str());
			setcolor(BLUE);
			outtextxy(0, 900, warning.c_str());
			setcolor(GREEN);
			outtextxy(10, 50, time1.c_str());
			setcolor(CYAN);
			outtextxy(10, 100, time2.c_str());
			setcolor(LIGHTBLUE);
			outtextxy(10, 150, time3.c_str());
			setcolor(LIGHTMAGENTA);
			outtextxy(10, 200, time4.c_str());
			do{
				input2 = getch();
				switch (input2){
				case '1':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time1.c_str());
					i = 5;
					break;
				case '2':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time2.c_str());
					i = 10;
					break;
				case '3':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time3.c_str());
					i = 15;
					break;
				case '4':
					cleardevice();
					setbkcolor(BLACK);
					setcolor(RED);
					outtextxy(0, 0, time4.c_str());
					i = 999999999;
					break;
				}
				break;
			} while (1 > 0);
		}
		//playSound("thrill.mp3", 210000);
		break;
	} while (1>0);


	/*string time1 = "1. 5 Second";
	string time2 = "2. 10 Second";
	string time3 = "3. 15 Second";
	string time4 = "4. Unlimited";
	*/







}
void playscreen(hand h1, hand h2, hand h3, hand h4){
	cleardevice();
	if (h1.out == false){
		setcolor(WHITE);
		fillellipse(600, 200, 100, 100);
		setcolor(WHITE);
		bar(640, 200, 670, 400);//index
		bar(600, 220, 630, 420);//middle
		bar(560, 212, 590, 412);//ring	
		bar(520, 150, 550, 350);//pinky
	}
	if (h2.out == false){
		setcolor(WHITE);
		fillellipse(1320, 200, 100, 100);
		setcolor(WHITE);
		bar(1250, 200, 1279, 400);//index
		bar(1290, 220, 1319, 420);//middle
		bar(1330, 212, 1359, 412);//ring
		bar(1370, 150, 1399, 350);//pinky
	}
	if (h3.out == false){
		//hand 3
		setcolor(WHITE);
		fillellipse(600, 820, 100, 100);
		setcolor(WHITE);
		bar(640, 620, 670, 800);//index
		bar(600, 600, 630, 780);//middle
		bar(560, 608, 590, 788);//ring	
		bar(520, 670, 550, 850);//pinky
	}
	if (h4.out == false){
		//hand 4
		setcolor(WHITE);
		fillellipse(1320, 820, 100, 100);
		bar(1250, 619, 1280, 800);//index
		bar(1290, 599, 1320, 780);//middle
		bar(1330, 607, 1360, 788);//ring	
		bar(1370, 669, 1400, 850);//pinky
	}
	getch();
}
void winscreen1(){
	int randcolor;
	string onewon = "Player 1 WON";

	randcolor = rand() % 14 + 2;
	cleardevice();
	setcolor(randcolor);
	settextstyle(10, 0, 5);
	outtextxy(1900 / 2, 1017 / 2, onewon.c_str());

}
void winscreen2(){
	int randcolor;

	string compwon = "Computer WON";
	randcolor = rand() % 14 + 2;
	cleardevice();
	setcolor(randcolor);
	settextstyle(10, 0, 5);
	outtextxy(1900 / 2, 1017 / 2, compwon.c_str());

}
void winscreen3(){
	int randcolor;

	string compwon = "Player 2 WON";
	randcolor = rand() % 14 + 2;
	cleardevice();
	setcolor(randcolor);
	settextstyle(10, 0, 5);
	outtextxy(1900 / 2, 1017 / 2, compwon.c_str());

}
void movea(PASS &global, hand &sel, hand &at){
	if (global.type == 1){
		at.fing = sel.fing + at.fing;
		if (at.fing == 5){
			at.out = true;
		}
		else if (at.fing > 5){
			at.fing -= 5;
		}
	}
	else if (global.type == 2){
		if (sel.fing == 2 && at.out == true){
			sel.fing = 1;
			at.out = false;
			at.fing = 1;
		}
		else if (sel.fing == 4 && at.out == true){
			sel.fing = 2;
			at.out = false;
			at.fing = 2;
		}
		else{
			cout << "Error: Must have even number of fingers and a dead hand to split";
		}
	}

}
void split(hand &handsplit, hand &handzero){
	if (handsplit.fing == 2 && handzero.out == true){
		handsplit.fing = 1;
		handzero.out = false;
		handzero.fing = 1;
	}
	else if (handsplit.fing == 4 && handzero.out == true){
		handsplit.fing = 2;
		handzero.out = false;
		handzero.fing = 2;
	}
	else{
		cout << "Error: Must have even number of fingers and a dead hand to split";
	}
}
void playgen(hand h1, hand h2, hand h3, hand h4){
	if (h1.out == false){
		switch (h1.fing){
		case 4:
			setcolor(WHITE);
			bar(520, 150, 550, 350);//pinky

		case 3:
			setcolor(WHITE);
			bar(560, 212, 590, 412);//ring	

		case 2:
			setcolor(WHITE);
			bar(600, 220, 630, 420);//middle
		case 1:
			setcolor(WHITE);
			//hand 1
			fillellipse(600, 200, 100, 100);
			setcolor(WHITE);
			bar(640, 200, 670, 400);//index
		}
	}
	if (h2.out == false){
		switch (h2.fing){
		case 4:
			setcolor(WHITE);
			bar(1370, 150, 1399, 350);//pinky
		case 3:
			setcolor(WHITE);
			bar(1330, 212, 1359, 412);//ring
		case 2:
			setcolor(WHITE);
			bar(1290, 220, 1319, 420);//middle
		case 1:
			setcolor(WHITE);
			//hand 3
			fillellipse(1320, 200, 100, 100);
			setcolor(WHITE);
			bar(1250, 200, 1279, 400);//index
		}
	}
	if (h3.out == false){
		switch (h3.fing){
		case 4:
			setcolor(WHITE);
			bar(520, 670, 550, 850);//pinky
		case 3:
			setcolor(WHITE);
			bar(560, 608, 590, 788);//ring
		case 2:
			setcolor(WHITE);
			bar(600, 600, 630, 780);//middle
		case 1:
			//hand 3
			setcolor(WHITE);
			fillellipse(600, 820, 100, 100);
			setcolor(WHITE);
			bar(640, 620, 670, 800);//index
		}
	}
	if (h4.out == false){
		switch (h4.fing){
		case 4:
			setcolor(WHITE);
			bar(1370, 669, 1400, 850);//pinky
		case 3:
			setcolor(WHITE);
			bar(1330, 607, 1360, 788);//ring
		case 2:
			setcolor(WHITE);
			bar(1290, 599, 1320, 780);//middle
		case 1:
			setcolor(WHITE);
			//hand 4
			fillellipse(1320, 820, 100, 100);
			bar(1250, 619, 1280, 800);//index
		}
		if (pturn == true){
			outtextxy(1500, 0, "player turn");
		}
		else if (cturn == true){
			outtextxy(1500, 0, "computer turn");
		}
		else if (playerturn == p1){
			outtextxy(1500, 0, "player 1 turn");
		}
		else if (playerturn == p2){
			outtextxy(1500, 0, "player 2 turn");
		}
	}
}// I need to integrate a thing for the hand gen for if they are out it does a blank one
void gen(int x, int y, hand hand){
	switch (hand.pos){
	case 1:
		switch (hand.fing){
		case 4:
			setcolor(WHITE);
			bar(x - 80, y - 50, x - 50, y + 150);//pinky
		case 3:
			setcolor(WHITE);
			bar(x - 40, y + 12, x - 10, y + 212);//ring	
		case 2:
			setcolor(WHITE);
			bar(x, y + 20, x + 30, y + 220);//middle
		case 1:
			setcolor(WHITE);
			bar(x + 40, y, x + 70, y + 200);//index
			setcolor(BLUE);
			//circle(600, 200, 100);//hand 1
			circle(x, y, 101);//hand 1
			circle(x, y, 102);
			circle(x, y, 103);
			setcolor(WHITE);
			fillellipse(x, y, 100, 100);
			Sleep(50);
			setcolor(BLACK);
			bar(x + 40, y, x + 70, y + 200);//index
			bar(x, y + 20, x + 30, y + 220);//middle
			bar(x - 40, y + 12, x - 10, y + 212);//ring	
			bar(x - 80, y - 50, x - 50, y + 150);//pinky
			setcolor(BLACK);
			//circle(600, 200, 100);//hand 1
			fillellipse(x, y, 103, 103);
		}
		break;
	case 2:
		switch (hand.fing){
		case 4:
			setcolor(WHITE);
			bar(x + 50, y - 50, x + 80, y + 150);//pinky
			
		case 3:
			setcolor(WHITE);
			bar(x + 10, y + 12, x + 40, y + 212);//ring
			
		case 2:
			setcolor(WHITE);
			bar(x - 30, y + 20, x, y + 220);//middle
		case 1:
			setcolor(WHITE);
			bar(x - 70, y, x - 40, y + 200);//index
			setcolor(BLUE);
			//circle(1320, 200, 100);//hand 2
			circle(x, y, 101);//hand 2
			circle(x, y, 102);
			circle(x, y, 103);
			setcolor(WHITE);
			fillellipse(x, y, 100, 100);
			Sleep(50);
			setcolor(BLACK);
			bar(x - 70, y, x - 40, y + 200);//index
			bar(x - 30, y + 20, x, y + 220);//middle
			bar(x + 10, y + 12, x + 40, y + 212);//ring
			bar(x + 50, y - 50, x + 80, y + 150);//pinky	
			setcolor(BLACK);
			//circle(1320, 200, 100);//hand 2
			fillellipse(x, y, 103, 103);
			
		}
		break;
	case 3:
		switch (hand.fing){
		case 4:
			setcolor(WHITE);
			bar(x - 80, y - 130, x - 50, y + 30);//pinky
		case 3:
			setcolor(WHITE);
			bar(x - 40, y - 192, x - 10, y - 12);//ring	
		case 2:
			setcolor(WHITE);
			bar(x, y - 200, x + 30, y - 40);//middle
		case 1:
			setcolor(WHITE);
			bar(x + 40, y - 200, x + 70, y - 20);//index
			setcolor(BLUE);
			circle(x, y, 101);//hand 3
			circle(x, y, 102);
			circle(x, y, 103);
			setcolor(WHITE);
			fillellipse(x, y, 100, 100);
			Sleep(50);
			setcolor(BLACK);
			bar(x + 40, y - 200, x + 70, y - 20);//index
			bar(x, y - 200, x + 30, y - 40);//middle
			bar(x - 40, y - 192, x - 10, y - 12);//ring	
			bar(x - 80, y - 130, x - 50, y + 30);//pinky
			setcolor(BLACK);
			fillellipse(x, y, 103, 103);
		}
		break;
	case 4:
		switch (hand.fing){
		case 4:
			setcolor(WHITE);
			bar(x + 50, y - 130, x + 80, y + 30);//pinky
		case 3:
			setcolor(WHITE);
			bar(x + 10, y - 212, x + 40, y - 32);//ring	
		case 2:
			setcolor(WHITE);
			bar(x - 30, y - 220, x, y - 40);//middle
		case 1:
			setcolor(WHITE);
			bar(x - 70, y - 200, x - 40, y + 20);//index
			setcolor(BLUE);
			circle(x, y, 101);//hand 3
			circle(x, y, 102);
			circle(x, y, 103);
			setcolor(WHITE);
			fillellipse(x, y, 100, 100);
			Sleep(50);
			setcolor(BLACK);
			bar(x - 70, y - 200, x - 40, y + 20);//index
			bar(x - 30, y - 220, x, y - 40);//middle
			bar(x + 10, y - 212, x + 40, y - 32);//ring	
			bar(x + 50, y - 130, x + 80, y + 30);//pinky
			setcolor(BLACK);
			fillellipse(x, y, 103, 103);
		}
		break;
	}
}//change
/*
void h1gen(int x, int y){

	
	setcolor(WHITE);
	bar(x + 40, y, x + 70, y + 200);//index
	bar(x, y + 20, x + 30, y + 220);//middle
	bar(x - 40, y + 12, x - 10, y + 212);//ring	
	bar(x - 80, y - 50, x - 50, y + 150);//pinky
	setcolor(BLUE);
	//circle(600, 200, 100);//hand 1
	circle(x, y, 100);//hand 1
	setcolor(WHITE);
	floodfill(x, y, BLUE);
	Sleep(500);
	setcolor(BLACK);
	bar(x + 40, y, x + 70, y + 200);//index
	bar(x, y + 20, x + 30, y + 220);//middle
	bar(x - 40, y + 12, x - 10, y + 212);//ring	
	bar(x - 80, y - 50, x - 50, y + 150);//pinky
	setcolor(BLACK);
	//circle(600, 200, 100);//hand 1
	circle(x, y, 100);//hand 1
	setcolor(BLACK);
	floodfill(x, y, BLACK);
}//change : black over all
void h2gen(int x, int y){
	
	setcolor(WHITE);
	bar(x - 70, y, x - 40, y + 200);//index
	bar(x - 30, y + 20, x, y + 220);//middle
	bar(x + 10, y + 12, x + 40, y + 212);//ring
	bar(x + 50, y - 50, x + 80, y + 150);//pinky	
	setcolor(BLUE);
	//circle(1320, 200, 100);//hand 2
	circle(x, y, 100);//hand 2
	setcolor(WHITE);
	floodfill(x, y, BLUE);
	Sleep(500);
	setcolor(BLACK);
	bar(x - 70, y, x - 40, y + 200);//index
	bar(x - 30, y + 20, x, y + 220);//middle
	bar(x + 10, y + 12, x + 40, y + 212);//ring
	bar(x + 50, y - 50, x + 80, y + 150);//pinky	
	setcolor(BLACK);
	//circle(1320, 200, 100);//hand 2
	circle(x, y, 100);//hand 2
	setcolor(BLACK);
	floodfill(x, y, BLACK);
	
}
void h3gen(int x, int y){
	//circle(600, 820, 100);//hand 3
	
	setcolor(WHITE);
	bar(x + 40, y - 200, x + 70, y - 20);//index
	bar(x, y - 200, x + 30, y - 40);//middle
	bar(x - 40, y - 192, x - 10, y - 12);//ring	
	bar(x - 80, y - 130, x - 50, y + 30);//pinky
	setcolor(BLUE);
	circle(x, y, 100);//hand 3
	setcolor(WHITE);
	floodfill(x, y, BLUE);
	Sleep(500);
	setcolor(BLACK);
	bar(x + 40, y - 200, x + 70, y - 20);//index
	bar(x, y - 200, x + 30, y - 40);//middle
	bar(x - 40, y - 192, x - 10, y - 12);//ring	
	bar(x - 80, y - 130, x - 50, y + 30);//pinky
	setcolor(BLACK);
	circle(x, y, 100);//hand 3
	setcolor(BLACK);
	floodfill(x, y, BLACK);
}
void h4gen(int x, int y){
	//circle(1320, 820, 100);//hand 4
	
	setcolor(WHITE);
	bar(x - 70, y - 200, x - 40, y + 20);//index
	bar(x - 30, y - 220, x, y - 40);//middle
	bar(x + 10, y - 212, x + 40, y - 32);//ring	
	bar(x + 50, y - 130, x + 80, y + 30);//pinky
	setcolor(BLUE);
	circle(x, y, 100);//hand 3
	setcolor(WHITE);
	floodfill(x, y, BLUE);
	Sleep(500);
	setcolor(BLACK);
	bar(x - 70, y - 200, x - 40, y + 20);//index
	bar(x - 30, y - 220, x, y - 40);//middle
	bar(x + 10, y - 212, x + 40, y - 32);//ring	
	bar(x + 50, y - 130, x + 80, y + 30);//pinky
	setcolor(BLACK);
	circle(x, y, 100);//hand 3
	setcolor(BLACK);
	floodfill(x, y, BLACK);
}//change?
*/
//old gen
void handm(PASS &global){
	switch (global.sel){
	case 1:
		switch (global.at){
		case 2:
			movea(global, h1, h2);
			break;
		case 3:
			movea(global, h1, h3);
			break;
		case 4:
			movea(global, h1, h4);
			break;
		}
		break;
	case 2:
		switch (global.at){
		case 1:
			movea(global, h2, h1);
			break;
		case 3:
			movea(global, h2, h3);
			break;
		case 4:
			movea(global, h2, h4);
			break;
		}
		break;
	case 3:
		switch (global.at){
		case 1:
			movea(global, h3, h1);
			break;
		case 2:
			movea(global, h3, h2);
			break;
		case 4:
			movea(global, h3, h4);
			break;
		}
		break;
	case 4:
		switch (global.at){
		case 1:
			movea(global, h4, h1);
			break;
		case 2:
			movea(global, h4, h2);
			break;
		case 3:
			movea(global, h4, h3);
			break;
		}
		break;
	}
}
void player(PASS &global, bool &pturn){
	switch (global.sel){
	case 3:
		switch (global.at){
		case 1:
			//move(h3, h1);
			global.sel = 3;
			global.at = 1;
			global.type = 1;
			break;
		case 2:
			//move(h3, h2);
			global.sel = 3;
			global.at = 2;
			global.type = 1;
			break;
		}
	case 4:
		switch (global.at){
		case 1:
			//move(h4, h1);
			global.sel = 4;
			global.at = 1;
			global.type = 1;
			break;
		case 2:
			//move(h4, h2);
			global.sel = 4;
			global.at = 2;
			global.type = 1;
			break;
		}
	}
	pturn = false;

}
void playerA(PASS &global, bool &pturn){
	switch (global.sel){
	case 3:
		switch (global.at){
		case 1:
			//move(h3, h1);
			global.sel = 3;
			global.at = 1;
			global.type = 1;
			break;
		case 2:
			//move(h3, h2);
			global.sel = 3;
			global.at = 2;
			global.type = 1;
			break;
		}
	case 4:
		switch (global.at){
		case 1:
			//move(h4, h1);
			global.sel = 4;
			global.at = 1;
			global.type = 1;
			break;
		case 2:
			//move(h4, h2);
			global.sel = 4;
			global.at = 2;
			global.type = 1;
			break;
		}
	}
	pturn = false;

}
void checkie3(POINT &p, int &at, bool &brk){
	
	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435)){
		at = 1;
		brk = true;
	}
	
	else if ((p.x > 481 && p.x < 720) && (p.y > 609 && p.y < 947) && global.sel != 3){
		at = 3;
		brk = true;
	}
	
	else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455)){
		at = 2;
		brk = true;
	}
	
	else if ((p.x > 1188 && p.x < 1431) && (p.y > 614 && p.y < 947) && global.sel != 4){
		at = 4;
		brk = true;
	}
}
void checkie2(POINT &p, PASS &global, bool &trubrk){

	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435)){
		if (h1.out == true){
			cout << "hand is out already" << endl;
			trubrk = true;
		}
		else{
			getCursor(global.p);
			checkie3(global.p, global.at, brk);
		}
	}
	else if ((p.x > 481 && p.x < 720) && (p.y > 609 && p.y < 947) && global.sel != 3){
		if (h3.out == true){
			global.type = 2;
			getCursor(global.p);
			checkie3(global.p, global.at, brk);
		}
		else{
			cout << "can't split" << endl;
			trubrk = true;
		}
	}
	else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455)){
		if (h2.out == true){
			cout << "hand is out already" << endl;
			trubrk = true;
		}
		else{
			getCursor(global.p);
			checkie3(global.p, global.at, brk);
		}
	}
	else if ((p.x > 1188 && p.x < 1431) && (p.y > 614 && p.y < 947) && global.sel != 4){
		if (h4.out == true){
			global.type = 2;
			getCursor(global.p);
			checkie3(global.p, global.at, brk);
		}
		else{
			cout << "can't split" << endl;
			trubrk = true;
		}

	}
}
void checkie(POINT &p, int &sel, bool &trubrk, bool &brk){
/*
	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435)){
		sel = 1;
		while (true){
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h1);
			checkie2(p);
		}
	}*/
	if ((p.x > 481 && p.x < 720) && (p.y > 609 && p.y < 947)){
		sel = 3;
		while (true){
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h3);
			playgen(h1, h2, h3, h4);
			checkie2(global.p, global, trubrk);
			if (brk){
				brk = false;
				player(global, pturn);
				handm(global);
				break;
			}
			else if (trubrk){
				trubrk = false;
				break;
			}
		}
	}
	/*else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455)){
		sel = 2;
		while (true){
			playgen(h1, h2, h3, h4);
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h3);
			checkie2(p);
		}
	}*/
	else if ((p.x > 1188 && p.x < 1431) && (p.y > 614 && p.y < 947)){
		sel = 4;
		while (true){
			playgen(h1, h2, h3, h4);
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h4);
			playgen(h1, h2, h3, h4);
			checkie2(global.p, global, trubrk);
			if (brk){
				brk = false;
				player(global, pturn);
				handm(global);
				break;
			}
			else if (trubrk){
				trubrk = false;
				break;
			}
		}
	}
}
void checkieC(POINT &p, int &at, bool &brk){

	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435)){
		at = 1;
		brk = true;
	}

	else if ((p.x > 481 && p.x < 720) && (p.y > 609 && p.y < 947) && global.sel != 3){
		at = 3;
		brk = true;
	}

	else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455)){
		at = 2;
		brk = true;
	}

	else if ((p.x > 1188 && p.x < 1431) && (p.y > 614 && p.y < 947) && global.sel != 4){
		at = 4;
		brk = true;
	}
}
void checkieB(POINT &p, PASS &global, bool &trubrk){

	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435) && global.sel != 1){
		if (h1.out == true){
			global.type = 2;
			getCursor(global.p);
			checkieC(global.p, global.at, brk);
		}
		else{
			cout << "can't split" << endl;
			trubrk = true;
		}
	}
	else if ((p.x > 481 && p.x < 720) && (p.y > 609 && p.y < 947)){
		if (h3.out == true){
			cout << "hand is out already" << endl;
			trubrk = true;
		}
		else{
			getCursor(global.p);
			checkieC(global.p, global.at, brk);
		}
	}
	else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455) && global.sel != 2){
		if (h2.out == true){
			global.type = 2;
			getCursor(global.p);
			checkieC(global.p, global.at, brk);
		}
		else{
			cout << "can't split" << endl;
			trubrk = true;
		}
	}
	else if ((p.x > 1188 && p.x < 1431) && (p.y > 614 && p.y < 947) && global.sel != 4){
		if (h4.out == true){
			cout << "hand is out already" << endl;
			trubrk = true;
		}
		else{
			getCursor(global.p);
			checkieC(global.p, global.at, brk);
		}
	}
}
void checkieA(POINT &p, int &sel, bool &trubrk, bool &brk){
	
	if ((p.x > 477 && p.x < 725) && (p.y > 104 && p.y < 435)){
		sel = 1;
		while (true){
			playgen(h1, h2, h3, h4);
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h1);
			playgen(h1, h2, h3, h4);
			checkieB(global.p, global, trubrk);
			if (brk){
			brk = false;
			playerA(global, pturnA);
			handm(global);
			break;
			}
			else if (trubrk){
			trubrk = false;
			break;
			}
		}
	}
	else if ((p.x > 1205 && p.x < 1438) && (p.y > 106 && p.y < 455)){
		sel = 2;
		while (true){
			playgen(h1, h2, h3, h4);
			GetCursorPos(&global.p);
			gen(global.p.x, global.p.y, h2);
			playgen(h1, h2, h3, h4);
			checkieB(global.p, global, trubrk);
			if (brk){
			brk = false;
			playerA(global, pturnA);
			handm(global);
			break;
			}
			else if (trubrk){
			trubrk = false;
			break;
			}
		}
	}
	
}
void compturn(){

}
void winstrategies(int &sel, int &at, int &mode);

void fakeclock(){
	for (int g = i; g > -1; g--){
		stringstream ss;
		ss << g;
	
		string str = ss.str();
		
		setcolor(WHITE);
	
		settextstyle(4, 0, 4);
	
		if (g > 9)
			outtextxy(0, 0, str.c_str());
		else
		{
			str = "0" + str;
			outtextxy(0, 0, str.c_str());
		}
		
		Sleep(1000);
		
	}
}

void winchk(hand h1, hand h2, hand h3, hand h4, bool &trubrk){
	if (h1.out == true && h2.out == true){
		winscreen1();
		Sleep(1000);
		trubrk = true;
	}
	else if (h3.out == true && h4.out == true){
		winscreen2();
		Sleep(1000);
		trubrk = true;
	}
}
void winchkA(hand h1, hand h2, hand h3, hand h4, bool &trubrk){
	if (h1.out == true && h2.out == true){
		winscreen3();
		Sleep(1000);
		trubrk = true;
	}
	else if (h3.out == true && h4.out == true){
		winscreen1();
		Sleep(1000);
		trubrk = true;
	}
}
void regen(){
	if (cmode == 2){
		while (true){//this while loop is for player vs computer hard mode. We will need 2 more while loops in a switch case based on selection of mode
			cturn = true;
			while (true){
				if (re){
					return;
				}
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				Sleep(1000);
				if (re){
					return;
				}
				winstrategies(global.sel, global.at, global.type);
				handm(global);
				if (re){
					return;
				}
				playgen(h1, h2, h3, h4);
				Sleep(1000);
				cturn = false;
				
				cleardevice();
				break;
			}
			winchk(h1, h2, h3, h4, trubrk);
			if (trubrk){
				trubrk = false;
				break;
			}
			
			pturn = true;
			while (true){
				if (re){
					return;
				}
				/*playgen(h1, h2, h3, h4);
				GetCursorPos(&global.p);
				cout << "x = " << global.p.x << endl;
				cout << "y = " << global.p.y << endl;
				gen(global.p.x, global.p.y, h2);*/
				//getCursor(global.p);
				//cout << "x = " << global.p.x << endl;
				//cout << "y = " << global.p.y << endl;
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				getCursor(global.p);
				if (re){
					return;
				}
				checkie(global.p, global.sel, trubrk, brk);
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				Sleep(1000);
				
				cleardevice();
				if (pturn == false){
					break;
				}
			}
			winchk(h1, h2, h3, h4, trubrk);
			if (trubrk){
				trubrk = false;
				break;
			}
			
		}
	}
	if (cmode == 1){
		while (true){//this while loop is for player vs computer easy mode.
			pturn = true;
			while (true){
				playgen(h1, h2, h3, h4);
				getCursor(global.p);
				checkie(global.p, global.sel, trubrk, brk);
				player(global, pturn);
				if (pturn == false){
					break;
				}
			}
			cturn = true;
			while (true){
				playgen(h1, h2, h3, h4);
				Sleep(1000);



			}

		}
	}
	if (cmode == 3){
		pturnA = true;
		playerturn = p1;
		while (true){//this while loop is for player vs player.
			pturnA = true;
			while (true){
				if (re){
					return;
				}
				playgen(h1, h2, h3, h4);
				getCursor(global.p);
				if (re){
					return;
				}
				checkieA(global.p, global.sel, trubrk, brk);
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				Sleep(1000);
				if (re){
					return;
				}
				if (pturnA == false){
					playerturn = p2;
					break;
				}
				cleardevice();
			}
			winchkA(h1, h2, h3, h4, trubrk);
			if (trubrk){
				trubrk = false;
				break;
			}
			
			pturnA = true;
			while (true){
				
				if (re){
					return;
				}
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				getCursor(global.p);
				checkie(global.p, global.sel, trubrk, brk);
				if (re){
					return;
				}
				playgen(h1, h2, h3, h4);
				if (re){
					return;
				}
				Sleep(1000);
				if (re){
					return;
				}
				if (pturnA == false){
					playerturn = p1;
					break;
				}
				cleardevice();
			}
			winchkA(h1, h2, h3, h4, trubrk);
			if (trubrk){
				trubrk = false;
				break;
			}
			
		}
	}
}
/*
1 : (477, 104) ; (725, 435)


3 : (481, 609) ; (720, 947)


2 : (1205, 106) ; (1438, 455)


4 : (1188, 614) ; (1431, 947)
*/
void newgame(){
	h1.fing = 1;
	h1.out = false;
	h1.pos = 1;
	h2.fing = 1;
	h2.out = false;
	h2.pos = 2;
	h3.fing = 1;
	h3.out = false;
	h3.pos = 3;
	h4.fing = 1;
	h4.out = false;
	h4.pos = 4;
}//change
int randcol(){
	col = rand() % 15;//+ 1;
	return col;
}
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
void rainbow(){
	x = (int)fRand(0, 1919);
	y = (int)fRand(0, 1017);
	putpixel(x, y, randcol());
}
void easymove(int &sel, int &at, int &mode){
	int whi1 = rand() % 2 + 1;
	int whi2 = rand() % 2 + 1;
	switch (whi1){
	case 1:
		if (whi2 == 1){
			//move(h1, h3);
			sel = 1;
			at = 3;
		}
		else if (whi2 == 2){
			//move(h1, h4);
			sel = 1;
			at = 4;
		}
		break;
	case 2:
		if (whi2 == 1){
			//move(h2, h3);
			sel = 2;
			at = 3;
		}
		else if (whi2 == 2){
			//move(h2, h4);
			sel = 2;
			at = 4;
		}
		break;
	}
	mode = 1;
}
/*
void mvmnt(int x, int y, int &dec){
	switch (dec){
	case 0:
		while (true){
			h1gen(x, y);
		}

		break;
	case 1:
		while (true){
			h2gen(x, y);
		}
		break;
	case 2:
		while (true){
			h3gen(x, y);
		}
		break;
	case 3:
		while (true){
			h4gen(x, y);
		}
		break;
	}
}*/
void winstrategies(int &sel, int &at, int &mode){
	if ((h1.fing == 2) && h2.fing == 0){
		//split(h1, h2);
		sel = 1;
		at = 2;
		mode = 2;
		//new argument for which type of move? merging split and move?
	}
	else if ((h1.fing == 0) && h2.fing == 2){
		//split(h2, h1);
		sel = 2;
		at = 1;
		mode = 2;
	}
	else if ((h1.fing == 4) && h2.fing == 0){
		//split(h1, h2);
		sel = 1;
		at = 2;
		mode = 2;
	}
	else if ((h1.fing == 0) && h2.fing == 4){
		//split(h2, h1);
		sel = 2;
		at = 1;
		mode = 2;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 3) && ((h3.fing == 2) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4)){
		//move(h2, h3);
		sel = 2;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 3) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h3.fing == 2) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4))){
		//move(h2, h3);
		sel = 2;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 3) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 2)){
		//move(h2, h4);
		sel = 2;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 3) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h4.fing == 2) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4))){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 2) && ((h3.fing == 3) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4)){
		//move(h2, h3);
		sel = 2;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 2) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h3.fing == 3) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4))){
		//move(h1, h3);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 2) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 3)){
		//move(h2, h4);
		sel = 2;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 2) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h4.fing == 3) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4))){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 4) && ((h3.fing == 1) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4)){
		//move(h2, h3);
		sel = 2;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 1) && h2.fing == 1) && ((h3.fing == 4) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4)){
		//move(h2, h3);
		sel = 2;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 4) && h2.fing == 4) && ((h4.fing == 1) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4)){
		//move(h2, h4);
		sel = 2;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 1) && h2.fing == 1) && ((h4.fing == 4) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4)){
		//move(h2, h4);
		sel = 2;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 4) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h3.fing == 1) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4))){
		//move(h1, h3);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if (((h1.fing == 1) && (h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && ((h3.fing == 4) && h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4))){
		//move(h1, h3);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if (((h2.fing == 1 || h2.fing == 2 || h2.fing == 3 || h2.fing == 4) && h1.fing == 4) && ((h4.fing == 1) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4)){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if (((h1.fing == 1 || h1.fing == 2 || h1.fing == 3 || h1.fing == 1) && h2.fing == 1) && ((h4.fing == 4) && h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4)){
		//move(h2, h4);
		sel = 2;
		at = 4;
		mode = 1;
	}
	else if ((h1.fing == 3 && h2.fing == 3) && ((h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4) && h3.fing == 2)){
		//move(h1, h3);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if ((h1.fing == 3 && h2.fing == 3) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 2)){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if ((h1.fing == 2 && h2.fing == 2) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 3)){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if ((h1.fing == 2 && h2.fing == 2) && ((h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4) && h3.fing == 3)){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if ((h1.fing == 4 && h2.fing == 4) && ((h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4) && h3.fing == 1)){
		//move(h1, h3);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if ((h1.fing == 4 && h2.fing == 4) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 1)){
		//move(h1, h4);
		sel = 1;
		at = 4;
		mode = 1;
	}
	else if ((h1.fing == 1 && h2.fing == 1) && ((h3.fing == 1 || h3.fing == 2 || h3.fing == 3 || h3.fing == 4) && h4.fing == 4)){
		//move(h1, h4);
		sel = 1;
		at = 3;
		mode = 1;
	}
	else if ((h1.fing == 1 && h2.fing == 1) && ((h4.fing == 1 || h4.fing == 2 || h4.fing == 3 || h4.fing == 4) && h3.fing == 4)){
		//move(h1, h3);
	}
	else{
		easymove(sel, at, mode);
	}
}
void main(){
	gr_Start(GrDriver, GrMode, ErrorCode);
	srand(time(NULL));
	startsound();
	newgame();
	startscreen();
	screentwo(i, cmode);
	cleardevice();
	playgen(h1, h2, h3, h4);
	thread f(regen);
	thread s(fakeclock);
	f.join();
	s.join();
	settextstyle(3, 0, 5);
	h2.fing = 2;
	for (int i = 0; i < 100000; i++){
		GetCursorPos(&global.p);
		gen(global.p.x, global.p.y, h2);
	}
	//string swag = "I love PAD";
	//outtextxy(500, 500, swag.c_str());
	/*for (int i = 0; i < 100000; i++){
		rainbow();
	}*/
	getch(); 
	closegraph();
}
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode){
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error: " << ErrorCode;
		cin >> r;
		exit(1);
	}
}
void getCursor(POINT & p){
	while (true)
		if (GetCursorPos(&p)){
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				do{} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
				return;
			}

			Sleep(45);
		}
}
bool MOUSE(int VirtualKey){
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf.EventType = MOUSE_EVENT;
		GetCursorPos(&global.p);
		global.press = true;
		int count = 0;
		do{
			if (count++ > 1400){
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}
bool KEYBOARD(int VirtualKey){
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
void KEY_LISTENER(){

	gr_Start(GrDriver, GrMode, ErrorCode); // initi of graphics window 
	centerX = getmaxx() / 2;
	centerY = getmaxy() / 2;
	hCur1 = LoadCursorFromFile("aero_link.cur");
	hCur3 = LoadCursorFromFile("Jolt-Arrow-Normal-Select.cur");
	visible = LoadCursorFromFile("Chill Working.ani");
	SetCursor(hCur1);

	cursorVisible = true;

	while (true){
		if (MOUSE(VK_LBUTTON)){
			cout << "Left Button\n";
			SetCursor(visible);
			POINT v;
			GetCursorPos(&v);
			cout << v.x << ", " << v.y << endl;
		}
		if (MOUSE(VK_RBUTTON)){
			cout << "Right Button\n";
			SetCursor(visible);
		}
		if (MOUSE(VK_MBUTTON))
		{
			playSound("starwars.wav", 2100);
			cout << "Mouse Wheel\n";
			SetCursor(visible);
		}
		if (MOUSE(VK_SCROLL)){
			cout << "SCROLL \n";
		}
		if (KEYBOARD(VK_LEFT)){
			cout << "Left Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_RIGHT)){
			cout << "Right Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_UP)){
			cout << "Up Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_DOWN)){
			cout << "Down Arrow\n";
			SetCursor(hCur3);
		}
		if (KEYBOARD(VK_LSHIFT)){
			cout << "Left Shift\n";
		}
		if (KEYBOARD(VK_RSHIFT))
		{
			cout << "Right Shift\n";
		}
		if (KEYBOARD(VK_ESCAPE)){
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
		if (KEYBOARD(VK_SPACE)){
			cout << "Spacebar Pressed \n";
		}
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

		// Room For More Ifs specified keys act in a specific way.
		// look through the VirtualKeys.h list to have other specific keys
	}

}
void playSound(string fileName, int duration){
	if (fileName.substr(fileName.length() - 2) == "v"){
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
void gameThread(){

	Sleep(300);

	settextstyle(3, 0, 72);
	setcolor(3);
	moveto(300, 500);
	outtext("We Begin");
	while (!global.press);

	global.press = global.hold = false;
	cleardevice();
	setcolor(4);

	while (true){

		if (global.press){
			cleardevice();
			if (irInBuf.EventType == MOUSE_EVENT){
				fillellipse(centerX, centerY, 100, 100);
			}
			if (irInBuf.EventType == KEY_EVENT){
				bar(centerX - 200, centerY - 200, centerX + 200, centerY + 200);
			}
			global.press = false;
		}
		if (global.hold){
			setcolor(rand() % 15 + 1);
			if (irInBuf.EventType == MOUSE_EVENT){
				fillellipse(centerX, centerY, 100, 100);
			}
			if (irInBuf.EventType == KEY_EVENT){
				bar(centerX - 200, centerY - 200, centerX + 200, centerY + 200);
			}
			if (global.VirtualKey == VK_ESCAPE){
				return;
			}
			global.hold = false;
		}

	}

	closegraph();


}


