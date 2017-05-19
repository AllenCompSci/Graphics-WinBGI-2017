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
#include "VirtualKeys.h" // CUSTOM
#pragma comment(lib, "winmm.lib") // SOUND

using namespace std;
bool valid = false;
int GrDriver, GrMode, ErrorCode;
bool cursorVisible;
int centerX, centerY;
bool playAgain;
bool gamePLAY;
bool noplay;
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
void border();
bool pizza;
void holes();
void colide();
int holex[15];
int holey[15];
int ax = 940;
int ay = 520;
int bx = 980;
int by = 560;
int j;
time_t current;
time_t start;
//int ax, ay, bx, by;
INPUT_RECORD irInBuf;
struct PASS{

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
}global;

void main()
{

	srand(time(NULL));

	playAgain = true;
		//thread p(playSound);
		thread g(gameThread); // Thread definiton
		thread t(KEY_LISTENER);
		thread c(colide);
	//	thread h(holes);
		global.press = global.hold = false;
		gamePLAY = true;
		t.join(); // thread t start
		g.join();
	//	p.join();
		//h.join();
		c.join();
	return;
}

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

void border(){
	//gr_Start(GrDriver, GrMode, ErrorCode);
	if (gamePLAY==true){
		if (valid == true){
			int colorSet = rand() % 15 + 1;
			
			for (int i = 0; i < 20; i++){
				setcolor(colorSet);
				bar(100 * i, 75, 100 * (i + 1), 100);
				bar(100 * i, 960, 100 * (i + 1), 985);
				//Sleep(17);
				colorSet = rand() % 15 + 1;
			}
		}
	} 
return;

}
void colide(){
	gr_Start(GrDriver, GrMode, ErrorCode);
	do{
		if (valid == true){
			for (int i = 0; i<10; i++){
				if ((ax>= holex[i] -50 && ax<= holex[i]&& ay>= holey[i]-50&& ay<= holey[i]) ||(bx>= holex[i] -50 && bx<= holex[i]&& by>= holey[i]-50&& by<= holey[i]) ){
					valid = false;
				}
			}
		}
	} while (gamePLAY == true);
	return;
}
void holes(){
	
if (gamePLAY==true){
	if (valid == true){
			
			for (int i = 0; i < 10; i++){
				//	for (int k = 0; k < 10; k++){
				setcolor(15);
				setfillstyle(10, 15);
				bar(holex[i] - 50, holey[i] - 50, holex[i], holey[i]);
				//Sleep(17);
				//}
			}
			
			for (int i = 0; i < 10; i++){
				holex[i] = rand() % 1919;
				holey[i] = rand() % (960-150) + 150;
				if (holex[i]<(bx + 75 + 50) && holex[i]>(ax + 75) && holey[i] > (by - 75) && holey[i] < (ay + 75 + 50)){
					holex[i] = rand() % 1919;
					holey[i] = rand() % (960 - 150) + 150;
				}

			}
			int colorset = rand() % 15 + 1;
			for (int i = 0; i < 10; i++){
				colorset = rand() % 15 + 1;
				setfillstyle(1, colorset);
				if (colorset == 3){
					colorset = rand() % 15 + 1;
					setfillstyle(1, colorset);
				}
				bar(holex[i] - 50, holey[i] - 50, holex[i], holey[i]);
				//Sleep(17);
			}

			Sleep(500);
			
	}
} 
	return;
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
			if (count++ > 500){
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}

void KEY_LISTENER(){

//	gr_Start(GrDriver, GrMode, ErrorCode); // initi of graphics window 
	centerX = getmaxx() / 2;
	centerY = getmaxy() / 2;
	//hCur1 = LoadCursorFromFile("aero_link.cur");
	hCur3 = LoadCursorFromFile("Jolt-Arrow-Normal-Select.cur");
	//visible = LoadCursorFromFile("Chill Working.ani");
	SetCursor(hCur3);
	cursorVisible = true;
	
	while (gamePLAY){
		//cout << "Keyboardpizza";
		

		if (MOUSE(VK_LBUTTON)){
			//cout << "Left Button\n";
			//SetCursor(visible);
			//POINT v;
			//GetCursorPos(&v);
			//cout << v.x << ", " << v.y << endl;
		}
		if (MOUSE(VK_RBUTTON)){
			//cout << "Right Button\n";
			//SetCursor(visible);
		}
		if (KEYBOARD(VK_LBUTTON)){
			//cout << "Left Arrow\n";

		}
		if (KEYBOARD(VK_RIGHT)){
			//	cout << "Right Arrow\n";

		}
		if (KEYBOARD(VK_UP)){
			//	cout << "Up Arrow\n";

		}
		if (KEYBOARD(VK_DOWN)){
		}
		if (KEYBOARD(VK_LEFT)){
		}
		if (KEYBOARD(VK_ESCAPE)){
			return;
		}
		if (KEYBOARD(VK_SPACE)){
		}
	}
	return;
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
	
	//mciSendString("stop MP3_Device", NULL, 0, 0);
	//mciSendString("close MP3_Device", NULL, 0, 0);
}

void gameThread() {
	int borderI = 0;
		int k = 0;
		Sleep(300);
		int centerx = (getmaxx() / 2);
		int centery = (getmaxy() / 2);
		settextstyle(2, 0, 72);
		setcolor(7);
		moveto(centerx - 342, centery - 20);
#pragma region Intro Text
		outtext("Give this game a 100 please");
		Sleep(150);
		setcolor(0);
		moveto(centerx - 342, centery - 20);
		outtext("Give this game a 100 please");


		setcolor(15);
		moveto(centerx - textwidth("You Like This Game") / 2, centery - 20);
		outtext("You Like This Game");
		Sleep(150);
		setcolor(0);
		moveto(centerx - textwidth("You Like This Game") / 2, centery - 20);
		outtext("You Like This Game");


		setcolor(15);
		moveto(centerx - 238, centery - 20);
		outtext("TRAVIS HOFFMAN");
		Sleep(750);
		setcolor(0);
		moveto(centerx - 238, centery - 20);
		outtext("TRAVIS HOFFMAN");
	

#pragma endregion
#pragma region Start Screen
		settextstyle(4, 0,7);
		setcolor(15);
		moveto(centerx - textwidth("//TODO: Change This Title Later") / 2, centery - 300);
		outtext("//TODO: Change This Title Later");

		settextstyle(2, 0, 72);

		moveto(centerx - textwidth("Travis Hoffman") / 2, centery + 100);
		outtext("Travis Hoffman");

		moveto(centerx - 69, centery - 20);
		outtext("PLAY");
		rectangle(centerx - 99, centery - 40, centerx + 99, centery + 40);
		bool play = false;
		global.press = false;
		do {
			POINT v;
			GetCursorPos(&v);

			if (v.x >= centerx - 109 && v.y >= centery - 60 && v.x <= centerx + 109 && v.y <= centery + 60) {

				setcolor(0);
				rectangle(centerx - 99, centery - 40, centerx + 99, centery + 40);
				setcolor(15);
				rectangle(centerx - 109, centery - 50, centerx + 109, centery + 50);

			}
			if (global.press == true) {
				if (v.x >= centerx - 109 && v.y >= centery - 50 && v.x <= centerx + 109 && v.y <= centery + 50) {
					play = true;
				}
			}
			if (v.x < centerx - 109 || v.y < centery - 60 || v.x > centerx + 109 || v.y > centery + 60) {
				setcolor(0);
				rectangle(centerx - 109, centery - 50, centerx + 109, centery + 50);
				setcolor(15);
				rectangle(centerx - 99, centery - 40, centerx + 99, centery + 40);
			}
			border();
			global.press = false;
		} while (play == false);


		setcolor(0);
		moveto(centerx - 69, centery - 20);
		outtext("PLAY");
		rectangle(centerx - 109, centery - 50, centerx + 109, centery + 50);

#pragma endregion
#pragma region Clearing Screen Sequence
		int g = 0;
		do{
			
			cleardevice();
			k = 0;
			int ex = 50;
			for (int i = 0; i < 50; i++) {
				setfillstyle(10, 15);
				bar(0 + (ex*i), 87 + k, 50 + (ex*i), 150 + k);
				if (50 + (ex*i) >= getmaxx() - 1){
					i = -1;
					k += 50;
					Sleep(100);
				}
				if (150 + k >= 968){
					g = 77;
					i = 51;
				}
			}
	//	while (g == 0);

			setcolor(15);
			moveto(centerx - textwidth("You Like This Game") / 2, centery - 20);
			outtext("You Like This Game");
			Sleep(75);
			setcolor(0);
			moveto(centerx - textwidth("You Like This Game") / 2, centery - 20);
			outtext("You Like This Game");
#pragma endregion 
		cleardevice();
		setcolor(15);
		setfillstyle(10,15);
		bar(0, 100, getmaxx(), 960);
		setfillstyle(1, 3);
		setcolor(3);

		 ax = 940;
		 ay = 520;
		bx = 980;
		by = 560;
		bar(ax, ay, bx, by);
		time_t currrent;
		global.press = false;
		global.hold = false;

	
		valid = true;
		for (int i = 0; i < 10; i++){
			holex[i] = 0;
			holey[i] = 0;
		}
			playSound("gamesound.mp3", 500000);
		
		start = clock();
		do{
	
			border();
			if (global.press == true||global.hold ==true){
				if (global.VirtualKey == VK_LEFT){
					for (int i = 0; i < 200;){
						setfillstyle(10, 15);
						setcolor(15);
						bar(ax, ay, bx, by);
						ax -= 20;
						bx -= 20;
						setfillstyle(1, 3);
						setcolor(3);
						bar(ax, ay, bx, by);
						border();
						Sleep(12);
						if (global.VirtualKey == VK_RIGHT || global.VirtualKey == VK_UP || global.VirtualKey == VK_DOWN || ax <= 0|| valid==false){
							i = 201;
						}
					}
				}
				if (global.VirtualKey == VK_RIGHT){
					for (int i = 0; i < 200;){
						setfillstyle(10, 15);
						bar(ax, ay, bx, by);
						setfillstyle(1, 3);
						setcolor(3);
						ax += 20;
						bx += 20;
						bar(ax, ay, bx, by);
						border();
						Sleep(12);
						if (global.VirtualKey == VK_LEFT || global.VirtualKey == VK_UP || global.VirtualKey == VK_DOWN || bx >= getmaxx() || valid == false){
							i = 201;
						}
					}
				}
				if (global.VirtualKey == VK_UP){
					for (int i = 0; i < 200;){
						setfillstyle(10, 15);
						bar(ax, ay, bx, by);
						setfillstyle(1, 3);
						setcolor(3);
						ay -= 20;
						by -= 20;
						bar(ax, ay, bx, by);
						border();
						Sleep(12);
						if (global.VirtualKey == VK_RIGHT || global.VirtualKey == VK_LEFT || global.VirtualKey == VK_DOWN|| ay<=100||valid==false){
							i = 201;
						}
					}
				}
					if (global.VirtualKey == VK_DOWN){
						for (int i = 0; i < 200;){
							setfillstyle(10, 15);
							bar(ax, ay, bx, by);
							setfillstyle(1, 3);
							setcolor(3);
							ay += 20;
							by += 20;
							bar(ax, ay, bx, by);
							border();
							Sleep(12);
							if (global.VirtualKey == VK_RIGHT || global.VirtualKey == VK_UP || global.VirtualKey == VK_LEFT|| by>=960||valid==false){
								i = 201;
							}
						}
					}
				if (ax <= 0 || ay <= 100|| bx >= getmaxx() || by >= 960) {
					valid = false;
				}

				holes();

				current = clock();
				j = (current - start);
				Sleep(25);
				global.press = false;

			}
		} while (j <= 120000 && valid == true);
		setcolor(4);
		if (valid == false){
			mciSendString("stop MP3_Device", NULL, 0, 0);
			mciSendString("close MP3_Device", NULL, 0, 0);
		}

		playSound("0477.mp3", 1000);
		bar(ax, ay, bx, by);

		Sleep(2000);
		mciSendString("stop MP3_Device", NULL, 0, 0);
		mciSendString("close MP3_Device", NULL, 0, 0);
		 k = 0;
		 setcolor(0);
		for (int i = 0; i < 50; i++) {
			bar(0 + (ex*i), 1050 -k, 50 + (ex*i), 1100-k);
			if (50 + (ex*i) >= getmaxx()){
				i = -1;
				k += 50;
				Sleep(100);
			}
			if (1000 - k <= 0){
				i = 51;
			}
		}

		

		cleardevice();
		setcolor(15);
		moveto(centerx-175, centery-200);
		outtext("Score: ");
		outtext (to_string(j).c_str());
		moveto(centerx - 175, centery - 20);
		outtext("PLAY AGAIN?");
		rectangle(centerx - 180, centery - 40, centerx + 180, centery + 40);
		play = false;
		global.press = false;
		do {
			POINT v;
			GetCursorPos(&v);

			if (v.x >= centerx - 190 && v.y >= centery - 50 && v.x <= centerx + 190&& v.y <= centery + 60) {

				setcolor(0);
				rectangle(centerx - 180, centery - 40, centerx + 180, centery + 40);
				setcolor(15);
				rectangle(centerx - 190, centery - 50, centerx + 190, centery + 50);

			}
			if (global.press == true) {
				if (v.x >= centerx - 190 && v.y >= centery - 50 && v.x <= centerx + 190 && v.y <= centery + 50) {
					play = true;

				}
			}
			if (v.x < centerx - 190|| v.y < centery - 60 || v.x > centerx + 190|| v.y > centery + 60) {
				setcolor(0);
				rectangle(centerx - 190, centery - 50, centerx + 190, centery + 50);
				setcolor(15);
				rectangle(centerx - 180, centery - 40, centerx + 180, centery + 40);
			}
			global.press = false;
		} while (play == false);
		if (play == false){
			playAgain = true;
			
		}

	}
	while (playAgain == true);

}

