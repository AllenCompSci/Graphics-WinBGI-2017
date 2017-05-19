#include <stdio.h>  /* defines FILENAME_MAX */
#include <iostream>
#include <fstream> // ifstream ofstream
#include <string> // string
#include <ctime> // rand()
#include <algorithm> // find()
#include <thread> // THREADS 
#include "Virtuso.h"
#include<mmsystem.h>  // SOUND
#include <mciapi.h> // SOUND
#include <vector> 
#include <direct.h>// INCLUDES NEEDED FOR VIRTUSO
#pragma comment(lib, "winmm.lib") // SOUND
#define GetCurrentDir _getcwd  // GET PATH DEFINITION
// USED http://www.youtube-mp3.org/ to rip songs
// USED http://mp3cut.foxcom.su/en/ to cut music

using namespace std;
//Problem with Rap
#pragma region Categories
struct Category {
	// EACH SONG HAS INFORMATION ABOUT IT 
	int numSongs;
	int Height;
	int Width;
	int ** frame;
	
	vector<string> SongTitle;
	vector<int> NumberOfCuts;
	//vector<vector <int>> dur;
	string file;
	vector<int> pickRandom(){
		vector<int> pickAny4;
		if (numSongs > 4){
			while ((int)pickAny4.size() < 4){
				int guess = rand() % numSongs;
				bool notFound = true;
				for (int i = 0; notFound && i < (int)pickAny4.size(); i++)
					if (guess == pickAny4.at(i)){
						notFound = false;
					}
				if (notFound)
					pickAny4.push_back(guess);
			}
		}
		else{
			for (int i = 0; i < numSongs; i++)
				pickAny4.push_back(i);
		}
		for (int i = 0; i < (rand() % 4) + 3; i++)
			random_shuffle(pickAny4.begin(), pickAny4.end());
		return pickAny4;
	}
	int correctTitle(vector <int> pickAny4){
		int pick = rand() % 4;
		return pickAny4.at(pick);
	}
	string getTitle(int pick){
		return SongTitle.at(pick);
	}
	void draw(int x, int y){
		for (int i = 0; i < Height; i++)
			for (int j = 0; j < Width; j++)
				if (frame[i][j] != 99)
					putpixel(x + j, y + i, frame[i][j]);
	}
} Rock, Pop, Jazz, Edm, Rap;

#pragma endregion STRUCT_CATEGORIES

#pragma region VARIABLES
vector <Category> virt;
int barX1, barX2, barY1, barY2, barY3, barY4, barY5, barY6, barY7, barY8;
int mouseCalibrationX, mouseCalibrationY;
int CDX, CDY, CircleX, CircleY;
int chosecatx1, chosecaty1, centerX, centerY;
int catx1, caty1, catx2, caty2, catx3, caty3, catx4, caty4;
int GrDriver;
int GrMode;
int ErrorCode;
bool stop;
string path;
#pragma endregion VIRTUOSO_VARIABLES

#pragma region Functions
void gr_start(int&, int&, int&);
Category Virtuso();
bool GAME(Category choice);
string convert_(string);
bool KEYBOARD(int VirtualKey);
void initgraph(int* device, int* mode, char const* /*pathtodriver*/, string name, HICON t);
void gr_start(int&GrDriver, int&GrMode, int&ErrorCode);
void getCursor(POINT &p);
void shuffle(vector<Category> &t);
void mouseCalibration();
void playSound(string Work, int duration);
int getDuration(string file);
void propogate();
void TITLESCREEN();
void wait();
void SelectDuringSong(POINT & p);
void getPath();
void ENDSCREEN();
#pragma endregion PROTOTYPES

// MAIN FUNCTION DRIVES THE CODE
void main(){
	
#pragma region START GAME
	// INITIALIZE THE RANDOM
	srand((unsigned int)time(NULL));
	// START THE GRAPHICS WINDOW WINBGI
	gr_start(GrDriver, GrMode, ErrorCode);
	// LOAD CURSOR
	HCURSOR hCur1 = LoadCursorFromFile((path + "RESOURCE/stripes_1.cur").c_str());
	// SET CURSOR
	SetCursor(hCur1);
	// INITIALIZE ALL VALUES PULL ALL TITLES FROM TXT FILES INCLUDED
	propogate(); // PROPOGATE CONATINS MOUSE CALIBRATION
	// START TITLE SCREEN	
	TITLESCREEN();
#pragma endregion

#pragma region GAME
	cleardevice();
	bool didIWIN;
	int count = 0;
	do{
		didIWIN = GAME(Virtuso());
		cleardevice();
		cout << boolalpha;
		cout << didIWIN << endl;
		count++;
	} while (didIWIN);

#pragma endregion 

#pragma region GAME OVER
// CLOSING 
	ENDSCREEN();
	wait();
	closegraph();
	exit(1);
#pragma endregion


}

#pragma region GAME

	void propogate(){
		mouseCalibration();
		stop = false;
		HWND stealth; /*creating stealth (window is not visible)*/
		AllocConsole();
		stealth = FindWindowA("ConsoleWindowClass", NULL);
		ShowWindow(stealth, 0);

		int maxX = getmaxx();
		int maxY = getmaxy();
		centerX = maxX / 2;
		centerY = maxY / 2;
		barX1 = 368;
		barX2 = 1551;
		barY1 = 575;
		barY2 = 625;
		barY3 = 675;
		barY4 = 725;
		barY5 = 775;
		barY6 = 825;
		barY7 = 875;
		barY8 = 925;
		CDX, CDY, CircleX, CircleY;
		CDX = centerX - 225; // ADD 226 for CDX2
		CDY = centerY - 300; // ADD 450 for CDY2
		CircleX = centerX;
		CircleY = CDY + 175;
		barX1 = centerX - 591;
		barX2 = barX1 + 1183;
		barY1 = CDY + 351 + 20;
		barY2 = barY1 + 50;
		barY3 = barY2 + 10;
		barY4 = barY3 + 50;
		barY5 = barY4 + 10;
		barY6 = barY5 + 50;
		barY7 = barY6 + 10;
		barY8 = barY7 + 50;
		chosecatx1 = centerX - 262;
		chosecaty1 = centerY - 315;
		catx1 = catx2 = chosecatx1;
		caty1 = chosecaty1 + 100 + 5;
		caty2 = caty1 + 15 + 255;
		catx3 = catx4 = catx1 + 255 + 15;
		caty3 = caty1;
		caty4 = caty2;
		Rock.file = path + "RESOURCE/ROCK/";
		Pop.file = path + "RESOURCE/POP/";
		Jazz.file = path + "RESOURCE/JAZZ/";
		Edm.file = path + "RESOURCE/EDM/";
		Rap.file = path + "RESOURCE/RAP/";
		string line;
		int cuts;
		ifstream read;
		read.open((path + "RESOURCE/ROCK.txt").c_str());
		Rock.Height = rockH;
		Rock.Width = rockW;
		Rock.frame = new int*[Rock.Height];
		for (int i = 0; i < Rock.Height; i++)
		{
			Rock.frame[i] = new int[Rock.Width];
			for (int j = 0; j < Rock.Width; j++){
				Rock.frame[i][j] = rock[i][j];
			}
		}
		while (!read.eof()){
			getline(read, line);
			Rock.SongTitle.push_back(line);
			read >> cuts;
			Rock.NumberOfCuts.push_back(cuts);
			getline(read, line);
		}
		Rock.numSongs = Rock.SongTitle.size();
		read.close();

		read.open((path + "RESOURCE/POP.txt").c_str());
		Pop.Height = popH;
		Pop.Width = popW;
		Pop.frame = new int*[Pop.Height];
		for (int i = 0; i < Pop.Height; i++)
		{
			Pop.frame[i] = new int[Pop.Width];
			for (int j = 0; j < Pop.Width; j++){
				Pop.frame[i][j] = pop[i][j];
			}
		}


		while (!read.eof()){
			getline(read, line);
			Pop.SongTitle.push_back(line);
			read >> cuts;
			Pop.NumberOfCuts.push_back(cuts);
			getline(read, line);
		}
		Pop.numSongs = Pop.SongTitle.size();
		read.close();

		read.open((path + "RESOURCE/JAZZ.txt").c_str());
		Jazz.Height = jazzH;
		Jazz.Width = jazzW;
		Jazz.frame = new int*[Jazz.Height];
		for (int i = 0; i < Jazz.Height; i++)
		{
			Jazz.frame[i] = new int[Jazz.Width];
			for (int j = 0; j < Jazz.Width; j++){
				Jazz.frame[i][j] = jazz[i][j];
			}
		}

		while (!read.eof()){
			getline(read, line);
			Jazz.SongTitle.push_back(line);
			read >> cuts;
			Jazz.NumberOfCuts.push_back(cuts);
			getline(read, line);
		}
		Jazz.numSongs = Jazz.SongTitle.size();
		read.close();

		read.open((path + "RESOURCE/EDM.txt").c_str());
		Edm.Height = edmHeight;
		Edm.Width = edmWidth;
		Edm.frame = new int*[Edm.Height];
		for (int i = 0; i < Edm.Height; i++)
		{
			Edm.frame[i] = new int[Edm.Width];
			for (int j = 0; j < Edm.Width; j++){
				Edm.frame[i][j] = edm[i][j];
			}
		}
		while (!read.eof()){
			getline(read, line);
			Edm.SongTitle.push_back(line);
			read >> cuts;
			Edm.NumberOfCuts.push_back(cuts);
			getline(read, line);
		}
		Edm.numSongs = Edm.SongTitle.size();
		read.close();

		read.open((path + "RESOURCE/RAP.txt").c_str());
		Rap.Height = rapH;
		Rap.Width = rapW;
		Rap.frame = new int*[Rap.Height];
		for (int i = 0; i < Rap.Height; i++)
		{
			Rap.frame[i] = new int[Rap.Width];
			for (int j = 0; j < Rap.Width; j++){
				Rap.frame[i][j] = rap[i][j];
			}
		}
		while (!read.eof()){
			getline(read, line);
			Rap.SongTitle.push_back(line);
			read >> cuts;
			Rap.NumberOfCuts.push_back(cuts);
			getline(read, line);
		}
		Rap.numSongs = Rap.SongTitle.size();
		read.close();



		virt.push_back(Rock); // 0
		virt.push_back(Pop); // 1
		virt.push_back(Jazz); // 2
		virt.push_back(Edm); //3
		virt.push_back(Rap); //4
		//Rock, Pop, Jazz, Edm, Rap;
	}

	void TITLESCREEN(){

		HCURSOR hCur1 = LoadCursorFromFile("RESOURCE\\stripes_1.cur");
		SetCursor(hCur1);
		/*
		drawPOP(5, 5);
		drawEDM(5, 300);
		drawJAZZ(300, 5);
		drawRAP(300, 300);
		drawROCK(600, 100);
		*/
#pragma region TitleScreen 
		setcolor(15);

		string title;
		title = "Virtuoso";
		settextstyle(4, 0, 8);
		outtextxy(750, 508, title.c_str());
		wait();
		setcolor(15);
		string title1;
		title1 = "The Musical Genius";
		settextstyle(4, 0, 8);
		outtextxy(400, 600, title1.c_str());
		wait();
		if (KEYBOARD(VK_ESCAPE)){ exit(1); };
		cleardevice();

#pragma endregion TitleScreen


	}

	bool GAME(Category choice){
		HCURSOR hCur1 = LoadCursorFromFile((path + "RESOURCE/stripes_1.cur").c_str());
		SetCursor(hCur1);
		cout << choice.file << endl;
		vector<int> songs = choice.pickRandom();
		int correct = choice.correctTitle(songs);
		// check and make sure correct is 0 - 3 or 1 - 4 or what.... make sure that corresponds to vector songs if not change correct to random() % 4... 
	    POINT p;
		p.x = 0; 
		
		int Picx1 = 734;
		int Picy1 = 100;
		int Picx2 = 1184;
		int Picy2 = 450;

		string SongPop1 = choice.SongTitle.at(songs.at(0));
		string SongPop2 = choice.SongTitle.at(songs.at(1));
		string SongPop3 = choice.SongTitle.at(songs.at(2));
		string SongPop4 = choice.SongTitle.at(songs.at(3));
			
		// USE THIS ONE
		string use = convert_(choice.SongTitle.at(correct));
		int cutNum = rand() % choice.NumberOfCuts.at(correct) + 1;
		string mp3 = choice.file + use + to_string(cutNum) + ".mp3";
		int duration = getDuration(mp3);
		


		setbkcolor(0);
		cleardevice();// Clicking on the Pop Category
		bar(CDX, CDY, CDX + 450, CDY + 351);
		//bar(Picx1, Picy1, Picx2, Picy2);//CD
		setcolor(9);
		fillellipse(CircleX, CircleY, 175, 175);
		setcolor(1);
		fillellipse(CircleX, CircleY, 25, 25);
		setcolor(7);

		bar(barX1, barY1, barX2, barY2);
		bar(barX1, barY3, barX2, barY4);
		bar(barX1, barY5, barX2, barY6);
		bar(barX1, barY7, barX2, barY8);
		//bar(368, 575, 1551, 625);//Answer Choice 1
		//bar(368, 675, 1551, 725);//2
		//bar(368, 775, 1551, 825);//3
		//bar(368, 875, 1551, 925);//4
		
		settextstyle(4, 0, 4);
		
		// GETS RID OF BOX CREATED BY TEXT 
		setbkcolor(7);
		// SETS TEXT COLOR 
		setcolor(4);
		outtextxy(barX1 + 32, barY1, SongPop1.c_str());
		outtextxy(barX1 + 32, barY3, SongPop2.c_str());
		outtextxy(barX1 + 32, barY5, SongPop3.c_str());
		outtextxy(barX1 + 32, barY7, SongPop4.c_str());
		/*
		
		
		
		int cutNum = rand() % choice.NumberOfCuts.at(correct) + 1;
		string mp3 = choice.file + choice.SongTitle.at(correct) + to_string(cutNum)  + ".mp3";
		int duration = choice.dur.at(correct).at(cutNum - 1);
		playSound(mp3, duration);
		
		
		OR ****** without storing duration 
*/
		setcolor(4);
		fillellipse(CircleX, CircleY, 25, 25);
		setcolor(4); 
		fillellipse(CircleX, CircleY, 25, 25);
		//playSound(mp3, duration);
		thread t(playSound, mp3, duration);
		thread h(SelectDuringSong, ref(p));
		 t.join();
		 h.join();
		setcolor(1);
		fillellipse(CircleX, CircleY, 25, 25);

		setbkcolor(0);

	
		while(true){
			
			if (p.x >= barX1 && p.x <= barX2){
				cout << correct << endl;
				if (p.y >= barY1 && p.y <= barY2){ return correct == songs.at(0); }
				else if (p.y >= barY3 && p.y <= barY4){ return correct == songs.at(1); }
				else if (p.y >= barY5 && p.y <= barY6){ return correct == songs.at(2); }
				else if (p.y >= barY7 && p.y <= barY8){ return correct == songs.at(3); }
				else if (p.y >= (CircleY - 25) && p.y <= (CircleY + 25)) {
					cutNum = rand() % choice.NumberOfCuts.at(correct) + 1;
					mp3 = choice.file + use + to_string(cutNum) + ".mp3";
					duration = getDuration(mp3);
					setcolor(4);
					fillellipse(CircleX, CircleY, 25, 25);
					//playSound(mp3, duration);
					thread t(playSound, mp3, duration);
					thread h(SelectDuringSong, ref(p));
					t.join();
					h.join();
					setcolor(1);
					fillellipse(CircleX, CircleY, 25, 25);
				}
				else {
					p.x = 0;
				}
			}
			getCursor(p);
		}
		
		return false;

	}

	Category Virtuso(){
		HCURSOR hCur1 = LoadCursorFromFile((path + "RESOURCE/stripes_1.cur").c_str());
		SetCursor(hCur1);
		POINT p;
		
		string category;	
		setbkcolor(0);
		cleardevice();
		setcolor(12);
		category = "Choose a Category";
		
		

		settextstyle(4, 0, 5);
		moveto(chosecatx1, chosecaty1);
		outtext(category.c_str());

		shuffle(virt);
		/*
		int catX[4][2], catY[4][2];
		// topLeft Corner
		catX[0][0] = 250;
		catY[0][0] = 347;
		catX[1][0] = 250;
		catY[1][0] = 347+300;
		catX[2][0] = 650;
		catY[2][0] = 382;
		catX[3][0] = 650;
		catY[3][0] = 382+300;
		// bottomRight 
		catX[0][1] = 250 + virt.at(0).Height;
		catY[0][1] = 347 + virt.at(0).Width;
		catX[1][1] = 240 + virt.at(1).Height;
		catY[1][1] = 795 + virt.at(1).Width;
		catX[2][1] = 1590 + virt.at(3).Height;
		catY[2][1] = 382 + virt.at(3).Width;
		catX[3][1] = 1590 + virt.at(4).Height;
		catY[3][1] = 830 + virt.at(4).Width;
		virt.at(0).draw(catX[0][0], catY[0][0]);
		virt.at(1).draw(catX[1][0], catY[1][0]);
		virt.at(2).draw(catX[2][0], catY[2][0]);
		virt.at(3).draw(catX[3][0], catY[3][0]);

		*/
		virt.at(0).draw(catx1, caty1);
		virt.at(1).draw(catx2, caty2);
		virt.at(2).draw(catx3, caty3);
		virt.at(3).draw(catx4, caty4);
	
		bool repeat = false;
		Category gt;
		do{
			repeat = false;
			getCursor(p);
			/*
			do{

			} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) == 0);
		
			if (p.x >= catX[0][0] && p.x <= catX[0][1] && p.y >= catY[0][0] && p.y <= catY[0][1]){ gt = virt.at(0); }
			else if (p.x >= catX[1][0] && p.x <= catX[1][1] && p.y >= catY[1][0] && p.y <= catY[1][1]){ gt = virt.at(1); }
			else if (p.x >= catX[2][0] && p.x <= catX[2][1] && p.y >= catY[2][0] && p.y <= catY[2][1]){ gt = virt.at(2); }
			else if (p.x >= catX[3][0] && p.x <= catX[3][1] && p.y >= catY[3][0] && p.y <= catY[3][1]){ gt = virt.at(3); }
			else{
				repeat = true;
			}

			*/
			cout << p.x << ", " << p.y << endl;
			cout << "CATX1 : " << catx1 << ", CATY1 : " << caty1 << endl;
			if (p.x >= catx1 && p.x <= (catx3 + 255)){
				if (p.x >= catx1 && p.x <= (catx1 + virt.at(0).Width) && p.y >= caty1 && p.y <= (caty1 + virt.at(0).Height)) { gt = virt.at(0); }
				else if (p.x >= catx2 && p.x <= (catx2 + virt.at(1).Width) && p.y >= caty2 && p.y <= (caty2 + virt.at(1).Height)) { gt = virt.at(1); }
				else if (p.x >= catx3 && p.x <= (catx3 + virt.at(2).Width) && p.y >= caty3 && p.y <= (caty3 + virt.at(2).Height)) { gt = virt.at(2); }
				else if (p.x >= catx4 && p.x <= (catx4 + virt.at(3).Width) && p.y >= caty4 && p.y <= (caty4 + virt.at(3).Height)) { gt = virt.at(3); }
				else {
					repeat = true;
				}
			}
			else {
				repeat = true;
			}
		} while (repeat);
		return gt;
	}
	
	void playSound(string Work, int duration){
		stop = true;
		string concat = "open " + Work + " type mpegvideo alias MP3_Device";
		LPCSTR a = concat.c_str();
		mciSendString(a, NULL, 0, 0);

		mciSendString("play MP3_Device", NULL, 0, 0);
		DWORD current = GetTickCount();
		DWORD interval = current + (DWORD)duration;
		while (current < interval && stop){
			current = GetTickCount();
		}
		stop = false;
		//Sleep(duration);
		
		mciSendString("stop MP3_Device", NULL, 0, 0);
		mciSendString("close MP3_Device", NULL, 0, 0);

	}

	int getDuration(string file){


		string cmd = "open " + file + " alias voice1";
		string str = "";
		LPSTR mssg = const_cast<char *>(str.c_str());
		int h = mciSendString(cmd.c_str(), NULL, 0, 0);
		int i = mciSendString("set voice1 time format millisecond", NULL, 0, 0);
		int j = mciSendString("status voice1 length", mssg, 128, 0);
		string::size_type sz;
	return stoi(mssg, &sz);
	}

	void mouseCalibration(){
		HCURSOR hCur1 = LoadCursorFromFile((path + "RESOURCE/stripes_1.cur").c_str());
		SetCursor(hCur1);
		int dx = 0;
		int dy = 0;
		int centerX = getmaxx() / 2;
		int centerY = getmaxy() / 2;
		mouseCalibrationX = 0;
		mouseCalibrationY = 0;
		int RAD = 10;
		bool change;
		do{
			cleardevice();
			setcolor(9);
			settextstyle(4, 0, 4);
			moveto(centerX - 375, centerY - 110);
			outtext("CLICK IN THE CENTER OF THE CIRCLE  ");
			cout << "CENTER : " << centerX << ", " << centerY << endl;
			cout << "CLICK  : ";
			fillellipse(centerX, centerY, RAD, RAD);
			fillellipse(centerX, centerY, RAD, RAD);

			POINT p;
			while (true){
				if (GetCursorPos(&p)){
					if (KEYBOARD(VK_ESCAPE)){ exit(1); };
					if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0){
						do{} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
						if (abs(p.x + mouseCalibrationX - centerX) <= 5)
							p.x += mouseCalibrationX;
						if (abs(p.y + mouseCalibrationY - centerY) <= 5)
							p.y += mouseCalibrationY;
						break;
					}
				}
			}



			cout << p.x << ", " << p.y << endl;
			change = false;
			if (abs(p.x - centerX) > 5)
			{
				mouseCalibrationX = centerX - p.x;
				change = true;
			}
			if (abs(p.y - centerY) > 5)
			{
				mouseCalibrationY = centerY - p.y;
				change = true;
			}
			RAD--;
		} while (RAD > 3 && change);
		cleardevice();
		return;


	}
	
	void ENDSCREEN(){
		setcolor(4); 
		drawGAMEOVER(centerX - 280, centerY - 50);
	}

#pragma endregion Virtuoso : The Musical Genius

#pragma region Mechanics 
	// CODED KEYBOARD HIT LISTENER
	bool KEYBOARD(int VirtualKey){
		if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
		{
			if (VirtualKey == VK_ESCAPE)
				exit(1);
		}
		return false;
	}
	// Modified for Visual Effect
	void initgraph(int* device, int* mode, char const* /*pathtodriver*/, string name, HICON t){  
#pragma region COPIED FROM WINBGI 
		int index;
		static WNDCLASS wcApp;

		gdi_error_code = grOk;

		if (wcApp.lpszClassName == NULL) {
			wcApp.lpszClassName = "BGIlibrary";
			wcApp.hInstance = 0;
			wcApp.lpfnWndProc = WndProc;
			wcApp.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcApp.hIcon = t;  // Modified wcApp.hIcon to set the Icon to what I wanted it to be. 
			wcApp.lpszMenuName = 0;
			wcApp.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			wcApp.style = CS_SAVEBITS;
			wcApp.cbClsExtra = 0;
			wcApp.cbWndExtra = 0;

			if (!RegisterClass(&wcApp)) {
				gdi_error_code = GetLastError();
				return;
			}

			pPalette = (NPLOGPALETTE)LocalAlloc(LMEM_FIXED,
				sizeof(LOGPALETTE) + sizeof(PALETTEENTRY)*PALETTE_SIZE);

			pPalette->palVersion = 0x300;
			pPalette->palNumEntries = PALETTE_SIZE;
			memset(pPalette->palPalEntry, 0, sizeof(PALETTEENTRY)*PALETTE_SIZE);
			for (index = 0; index < BG; index++) {
				pPalette->palPalEntry[index].peFlags = PC_EXPLICIT;
				pPalette->palPalEntry[index].peRed = index;
				pPalette->palPalEntry[PALETTE_SIZE - BG + index].peFlags = PC_EXPLICIT;
				pPalette->palPalEntry[PALETTE_SIZE - BG + index].peRed =
					PALETTE_SIZE - BG + index;
			}
			hBackgroundBrush = CreateSolidBrush(PALETTEINDEX(BG));
			hBrush[EMPTY_FILL] = (HBRUSH)GetStockObject(NULL_BRUSH);
			hBrush[SOLID_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SolidBrushBitmap));
			hBrush[LINE_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LineBrushBitmap));
			hBrush[LTSLASH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LtslashBrushBitmap));
			hBrush[SLASH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SlashBrushBitmap));
			hBrush[BKSLASH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, BkslashBrushBitmap));
			hBrush[LTBKSLASH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, LtbkslashBrushBitmap));
			hBrush[HATCH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, HatchBrushBitmap));
			hBrush[XHATCH_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, XhatchBrushBitmap));
			hBrush[INTERLEAVE_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, InterleaveBrushBitmap));
			hBrush[WIDE_DOT_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, WidedotBrushBitmap));
			hBrush[CLOSE_DOT_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, ClosedotBrushBitmap));
			hBrush[USER_FILL] =
				CreatePatternBrush(CreateBitmap(8, 8, 1, 1, SolidBrushBitmap));
		}
		memcpy(BGIpalette, BGIcolor, sizeof BGIpalette);
		current_palette.size = MAXCOLORS + 1;
		for (index = 10; index <= MAXCOLORS; index++) {
			pPalette->palPalEntry[index] = BGIcolor[0];
		}
		for (index = 0; index <= MAXCOLORS; index++) {
			current_palette.colors[index] = index;
			pPalette->palPalEntry[index + BG] = BGIcolor[index];
		}
		hPalette = CreatePalette(pPalette);
		detect_mode(device, mode);
		set_defaults();
#pragma endregion wcApp.hIcon Modification 
		hWnd = CreateWindow("BGIlibrary", name.c_str(), // modified name to change Window Name
			WS_OVERLAPPEDWINDOW,
			0, 0, window_width + BORDER_WIDTH,
			window_height + BORDER_HEIGHT,
			(HWND)NULL, (HMENU)NULL,
			0, NULL);
#pragma region COPIED FROM WINBGI

		if (hWnd == NULL) {
			gdi_error_code = GetLastError();
			return;
		}
		ShowWindow(hWnd, *mode == VGAMAX ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
		UpdateWindow(hWnd);
#pragma endregion
	}
	// Calls Modified initgraph detailed above
	void gr_start(int&GrDriver, int&GrMode, int&ErrorCode)
	{

		GrDriver = VGA;
		GrMode = VGAMAX;
		HICON t = (HICON)LoadImage(NULL, (path + "RESOURCE/NyanCat.ico").c_str(), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		initgraph(&GrDriver, &GrMode, "", "Virtuoso  :  The Musical Genius", t);
		ErrorCode = graphresult();
		if (ErrorCode != grOk)
		{
			cout << "error: " << ErrorCode;
			getch();
			exit(1);
		}
	}
	// Contains mouse calibration
	void getCursor(POINT &p){
		while (true){
			if (GetCursorPos(&p)){
				if (KEYBOARD(VK_ESCAPE)){ exit(1); };
				if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0){
					do{} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
					p.x += mouseCalibrationX;
					p.y += mouseCalibrationY;
					return;
				}
			}
		}
	}
	// Replaces all ' ' with '_'
	string convert_(string randString){
		for (int i = 0; i < (int)randString.length(); i++)
			if (randString[i] == ' '){
				randString[i] = '_';
			}
		return randString;
	}
	// Shuffles all valuse in a vector around 
	void shuffle(vector<Category> &t){
		for (int i = 0; i < rand() % 15; i++)
			random_shuffle(t.begin(), t.end());

	}
	// Listens for either keyboard hit or mouse click
	void wait(){
		while (true){
			if (kbhit()){
				int x = getch();
				if (KEYBOARD(VK_ESCAPE)){ exit(1); };
				return;
			}
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0){
				do{} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
				return;
			}
			if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0){
				do{} while ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0);
				return;
			}
		}
		return;

	}
	// Listens for mouse selection during song
	void SelectDuringSong(POINT &p){
		while (!stop);
		while (stop){
			getCursor(p);
			if (p.x != 0){
				stop = false;
			}
		}
	}
	// GRABS PATH SETS VALUE TO GLOBAL STRING
	void getPath(){
		char cCurrentPath[FILENAME_MAX];
		GetCurrentDir(cCurrentPath, sizeof(cCurrentPath));
		cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
		printf("The current working directory is %s", cCurrentPath);
		string a(cCurrentPath);
		path = a;
	}
#pragma endregion 
