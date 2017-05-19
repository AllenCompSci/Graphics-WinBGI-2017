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
int GrDriver, GrMode, ErrorCode;
void gr_start(int&, int&, int&);
void getCursor(POINT &p);
bool MOUSE(int);
void KEY_LISTENER();
INPUT_RECORD irInBuf;
POINT p;

int ber[3][3]; // 3x3 
int ber2[12][6]; // 6x12
int adf[4][4];// temporary 4x4
string winsound = "cheer.mp3";
string battle = "bttle.mp3";
string siren = "sirn.mp3";
string plane = "plne.mp3";
string march = "mrch.mp3";
string tiesound = "taps.mp3";
bool cursor = false; // checking if there is a value for p
struct playerprint{

	POINT e;
	int i;
	int j;
	void select(){ // Finding x and y values of square centers (3x3)

		switch (i){

		case(0) :
			e.x = 650;
			break;
		case(1) :
			e.x = 950;
			break;
		case(2) :
			e.x = 1250;
			break;

		};

		switch (j){

		case(0) :
			e.y = 200;
			break;
		case(1) :
			e.y = 500; 
			break;
		case(2) :
			e.y = 800;
			break;

		};

	}
	void select2(){  // Finding x and y values of square centers (6x12)

		switch (i){

		case(0) :
			e.x = 125;
			break;
		case(1) :
			e.x = 275;
			break;
		case(2) :
			e.x = 425;
			break;
		case(3) :
			e.x = 575;
			break;
		case(4) :
			e.x = 725;
			break;
		case(5) :
			e.x = 875;
			break;
		case(6) :
			e.x = 1025;
			break;
		case(7) :
			e.x = 1175;
			break;
		case(8) :
			e.x = 1325;
			break;
		case(9) :
			e.x = 1475;
			break;
		case(10) :
			e.x = 1625;
			break;
		case(11) :
			e.x = 1775;
			break;

		}

		switch (j){

		case(0) :
			e.y = 125;
			break;
		case(1) :
			e.y = 275;
			break;
		case(2) :
			e.y = 425;
			break;
		case(3) :
			e.y = 575;
			break;
		case(4) :
			e.y = 725;
			break;
		case(5) :
			e.y = 875;
			break;

		}
	}
	void inpl(int pl){ // Inputs info for player's selection of placing piece (3x3)
	
			switch (pl){

			case (1) :
				ber[i][j] = 1;
				break;
			case (2) :
				ber[i][j] = 10;
				break;
			case (3) :
				ber[i][j] = 100;
				break;
			};
	}
	void inpl2(int pl){ // Inputs info for player's selection of placing piece (3x3)

		switch (pl){

		case (1) :
			ber2[i][j] = 1;
			break;
		case (2) :
			ber2[i][j] = 10;
			break;
		case (3) :
			ber2[i][j] = 100;
			break;
		};

	}
	void printsign(int player, int size){


		if (player == 1){ // Printing out pieces to game board

			line(e.x - size, e.y + size, e.x + size, e.y - size);
			line(e.x + size, e.y + size, e.x - size, e.y - size);

		}
		else if (player == 2){

			circle(e.x, e.y, size);

		}
		else if (player == 3){

			line(e.x, e.y + size, e.x, e.y - size);
			line(e.x - size, e.y, e.x + size, e.y);

		}

	}

}player[4];
string turns();
string turns2();
bool chkthr();
void threeX3Board();
void sixX12();
void getCursor(POINT& p);
bool entryscreen();
void detectregion(playerprint& i);
void detectregion2(playerprint& i);
void transfer(int, int);
bool chka();
void list();
void playSound(string, int);
void main(){
	srand((unsigned int)time(NULL));
	gr_start(GrDriver, GrMode, ErrorCode);
	
		do{
			cleardevice();
		} while (entryscreen());

		

		//system("pause");
		//cleardevice();
		exit(1);
		//closegraph();
		system("pause");
		

}
string turns(){
	
	string x;
	int count;
	int count2 = 0;
	
	do {
		if (count2 % 2 == 0){

			count = 1;

		}
		else if (count2 % 2 == 1){

			count = 2;

		}
		do{

			detectregion(player[count]);
			player[count].select();

		} while (ber[player[count].i][player[count].j] != 0);
		player[count].inpl(count);
		player[count].printsign(count, 125);
		list();
		count2++;
	} while (!chkthr() && count2 <= 8);
	
	if (!chkthr() && count2 > 8){

		x = "Tie";
	}
	else if (count == 1){
		x = "Player 1 Wins!";
	}

	else if (count == 2){
		x = "Player 2 Wins!";
	}
	else {

		x = " ";

	}

	for (int i = 0; i < 3;){

		for (int j = 0; j < 3;){

			ber[i][j] = 0;
			j++;

		}
		i++;
	}

	return x;
}
string turns2(bool i){
	string x;
	int count;
	int count2 = 0;
	do {

		if (i == true){ // Sets game for 3 player mode

			if (count2 % 3 == 0){

				count = 1;

			}
			else if (count2 % 3 == 1){

				count = 2;

			}
			else if (count2 % 3 == 2){

				count = 3;

			}

		}
		else if (i == false){ // Sets game for 2 player mode

			if (count2 % 2 == 0){

				count = 1;

			}
			else if (count2 % 2 == 1){

				count = 2;

			}

		}
		

		do{

			detectregion2(player[count]);
			player[count].select2();

		} while (ber2[player[count].i][player[count].j] != 0); // Does NOT allow for double piecing
		player[count].inpl2(count); // Transfers info to arrays
		player[count].printsign(count, 50); // Prints pieces
		list();
		count2++;
	} while (!chka() && count2 <= 71);
	if (!chka() && count2 > 71){
		x = "Tie";
	}
	else if (count == 1){
		x = "Player 1 Wins!";
	}
	else if (count == 2){
		x = "Player 2 Wins!";
	}
	else if (count == 3){
		x = "Player 3 Wins!";
	}
	else{
		x = " ";
	}

	for (int i = 0; i < 12;){

		for (int j = 0; j < 6;){

			ber2[i][j] = 0;
			j++;

		}
		i++;
	}

	return x;
}
void getCursor(POINT& p){
	while (true)
		if (GetCursorPos(&p)){
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				do{
					GetCursorPos(&p);
				} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
				return;
			}

			Sleep(45);
		}
}
bool entryscreen(){

	bool f;

	string x = " ";

	string Welcome = "Welcome to Tactic Toe! ";
	setcolor(YELLOW);
	settextstyle(1, 0, 8);

	string Mode = "Select a Mode: ";
	setcolor(YELLOW);
	settextstyle(1, 0, 8);

	outtextxy((getmaxx() / 2) - 700, 100, Welcome.c_str());
	outtextxy((getmaxx() / 2) - 450, 200, Mode.c_str());

	string Normal = "Normal Mode";
	setcolor(BLUE);
	settextstyle(1, 0, 8);
	outtextxy((getmaxx() / 2) - 400, 400, Normal.c_str());


	string Enlarged = "Enlarged Mode";
	setcolor(GREEN);
	settextstyle(1, 0, 8);
	outtextxy((getmaxx() / 2) - 400, 700, Enlarged.c_str());

	setcolor(RED);
	settextstyle(1, 0, 8);
	outtextxy(1650, 900, "Exit");

	setcolor(BLACK);
	rectangle(400, 625, 1600, 900);
	rectangle(400, 300, 1600, 600);
	rectangle(1619, 860, 1918, 1016);
	rectangle(500, 400, 900, 600);
	rectangle(1000, 400, 1400, 600);


	getCursor(p);
	if ((p.x >= 400 && p.x <= 1600) && (p.y >= 300 && p.y <= 600)){ // Checks if player selected normal mode

		cleardevice();
		threeX3Board();
		x = turns();
		cleardevice();
		if (x == "Tie"){

			outtextxy(850, (getmaxy() / 2), x.c_str());
			playSound(tiesound, 47400);
		}
		else{

			outtextxy(600, (getmaxy() / 2), x.c_str());
			playSound(winsound, 6132);

		}
		f = true;

	}
	else if ((p.x >= 400 && p.x <= 1600) && (p.y >= 625 && p.y <= 900)){ // Checks if players picked large mode

		
		// add the screen where it asks how many players.
		cleardevice();
		setcolor(BLUE);
		outtextxy(450, 200, "How many players?");
		outtextxy(675, 450, "2");
		outtextxy(1175, 450, "3");
		getCursor(p);


		if ((p.x >= 500 && p.x <= 900) && (p.y >= 400 && p.y <= 600)){ // Checks if player selected 2 player mode

			cleardevice();
			sixX12();
			x = turns2(false);
			getch();
			cleardevice();
			if (x == "Tie"){

				outtextxy(850, (getmaxy() / 2), x.c_str());
				playSound(tiesound, 47400);
			}
			else{

				outtextxy(600, (getmaxy() / 2), x.c_str());
				playSound(winsound, 6132);

			}
			f = true;

		}
		else if ((p.x >= 1000 && p.x <= 1400) && (p.y >= 400 && p.y <= 600)){ // Checks if player selected 3 player mode

			cleardevice();
			sixX12();
			x = turns2(true);
			getch();
			cleardevice();
			if (x == "Tie"){

				outtextxy(850, (getmaxy() / 2), x.c_str());
				playSound(tiesound, 47400);
			}
			else{

				outtextxy(600, (getmaxy() / 2), x.c_str());
				playSound(winsound, 6132);

			}
			f = true;

		}
		else {

			f = false; // If player clicked somewhere else

		}

	}
	else if ((p.x >= 1619 && p.x <= 1919) && (p.y >= 860 && p.y <= 1017)){ // Checks if player clicked exit
		f = false;
		
		//exit(1);
		

	}
	else{
		
		f = true;

	}
	cursor = false;
	return f;
}
void list(){


	int scount = (rand() % 4) + 1;
	while (scount >= 1 && scount <= 4){

		if (scount == 1){

			playSound(battle, 3849);
			scount = 20;
		}
		if (scount == 2){

			playSound(siren, 5000);
			scount = 20;
		}
		if (scount == 3){

			playSound(plane, 3805);
			scount = 20;
		}
		if (scount == 4){

			playSound(march, 3500);
			scount = 20;
		}
	}




}
void detectregion(playerprint& i){
	
	if (cursor == false)
		getCursor(p);

	if (p.x > 500 && p.x < 800){

		i.i = 0;

		if (p.y > 50 && p.y < 350){

			i.j = 0;
			// 1 is the sign for player 1
			//change to whatever the sign of the player is

		}
		else if (p.y > 350 && p.y < 650){

			i.j = 1;

		}
		else if (p.y > 650 && p.y < 950){

			i.j = 2;

		}


	}
	else if (p.x > 800 && p.x < 1100){

		i.i = 1;
		if (p.y > 50 && p.y < 350){

			i.j = 0;
			// 1 is the sign for player 1
			//change to whatever the sign of the player is

		}
		else if (p.y > 350 && p.y < 650){

			i.j = 1;

		}
		else if (p.y > 650 && p.y < 950){

			i.j = 2;

		}


	}
	else if (p.x > 1100 && p.x < 1400){

		i.i = 2;
		if (p.y > 50 && p.y < 350){

			i.j = 0;
			// 1 is the sign for player 1
			//change to whatever the sign of the player is

		}
		else if (p.y > 350 && p.y < 650){

			i.j = 1;

		}
		else if (p.y > 650 && p.y < 950){

			i.j = 2;

		}

	}
	cursor = false;
}
void detectregion2(playerprint& i){

	if (cursor == false)
		getCursor(p);

	int left, right, up, bottom; // Defines squares for 6x12 to set up detection
	left = 50;
	right = 200;
	up = 50;
	bottom = 200;
	for (int w = 0; w <= 5;){ // Checks rows + defining click

		if (p.y > up && p.y < bottom) 
			i.j = w;

		for (int v = 0; v <= 11;){ // Checks columns + defing click
			
			if (p.x > left && p.x < right)
				i.i = v;

			left += 150; // Moving check area right
			right += 150;
			v++;
		}
		left = 50; // Setting square to original column
		right = 150;

		up += 150;
		bottom += 150;
		w++;
	}
	cursor = false;
}
bool chkthr(){ // Checks for 3x3

	bool r = false;

	for (int i = 0; i < 3;){ // Checks rows for win

		if ((ber[i][0] + ber[i][1] + ber[i][2] == 3 || ber[i][0] + ber[i][1] + ber[i][2] == 30))
			r = true;

		i++;
	}
	for (int i = 0; i < 3;){ // Checks columns for win

		if (ber[0][i] + ber[1][i] + ber[2][i] == 3 || ber[0][i] + ber[1][i] + ber[2][i] == 30)
			r = true;

		i++;
	}
	if ((ber[0][0] + ber[1][1] + ber[2][2] == 3 || ber[0][0] + ber[1][1] + ber[2][2] == 30)) // Checks diagonals for win
		r = true;

	if ((ber[0][2] + ber[1][1] + ber[2][0] == 3 || ber[0][2] + ber[1][1] + ber[2][0] == 30)) // Checks diagonals for win
		r = true;

	return r;

}
void transfer(int q, int r){ // Moves 4x4 Array around to check for 4 in a row (6x12)
	int g = 0;
	int h = 0;
	for (int i = q; i < q + 4; i++){

		for (int j = r; j < r + 4; j++){

			adf[g][h] = ber2[i][j];
			h++;
		}
		h = 0;
		g++;
	}

}
bool chka(){ // Checks 4x4 portions in 6x12
	bool r = false;

		for (int e = 0; e <= 8; e++){ // Moves 4x4 check left and right

			for (int f = 0; f <= 2; f++){ // Moves 4x4 check up and down

				transfer(e, f); // Moves 4x4 around

				for (int i = 0; i < 4;){ // Checks rows for win

					if ((adf[i][0] + adf[i][1] + adf[i][2] + adf[i][3] == 4 || adf[i][0] + adf[i][1] + adf[i][2] + adf[i][3] == 40 || adf[i][0] + adf[i][1] + adf[i][2] + adf[i][3] == 400))
						r = true;
					i++;
				}
				for (int i = 0; i < 4;){ // Checks columns for win

					if (adf[0][i] + adf[1][i] + adf[2][i] + adf[3][i] == 4 || adf[0][i] + adf[1][i] + adf[2][i] + adf[3][i] == 40 || adf[0][i] + adf[1][i] + adf[2][i] + adf[3][i] == 400)
						r = true;
					i++;
				}
				if ((adf[0][0] + adf[1][1] + adf[2][2] + adf[3][3] == 4 || adf[0][0] + adf[1][1] + adf[2][2] + adf[3][3] == 40 || adf[0][0] + adf[1][1] + adf[2][2] + adf[3][3] == 400)) // Checks Diagonals for win
					r = true;

				if ((adf[0][3] + adf[1][2] + adf[2][1] + adf[3][0] == 4 || adf[0][3] + adf[1][2] + adf[2][1] + adf[3][0] == 40 || adf[0][3] + adf[1][2] + adf[2][1] + adf[3][0] == 400)) // Checks Diagonals for win
					r = true;
			}
		}

	return r;
}
void threeX3Board(){
	setcolor(15);
	rectangle(500, 50, 800, 350);
	rectangle(500, 350, 800, 650);
	rectangle(500, 650, 800, 950);
	rectangle(800, 50, 1100, 350);
	rectangle(800, 350, 1100, 650);
	rectangle(800, 650, 1100, 950);
	rectangle(1100, 50, 1400, 350);
	rectangle(1100, 350, 1400, 650);
	rectangle(1100, 650, 1400, 950);
}
void sixX12(){

	int left, right, up, bottom;
	left = 50;
	right = 200;
	up = 50;
	bottom = 200;
	setcolor(15);
	for (int w = 0; w <= 5; w++){
		rectangle(left, up, right, bottom);
		for (int i = 0; i <= 11; i++){


			rectangle(left, up, right, bottom);
			left += 150;
			right += 150;

		}
		left = 50;
		right = 200;

		up += 150;
		bottom += 150;
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
	DWORD current = GetTickCount();
	DWORD interval = current + (DWORD)duration;
	bool t = true;
	while (current < interval && t){
		current = GetTickCount();
		if (kbhit()){
			int r = getch();
			t = false;
		}
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
		{
			do{
				GetCursorPos(&p);
			} while ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0);
			cursor = true;
			t = false;
		}
	}
	//Sleep(duration);
	mciSendString("stop MP3_Device", NULL, 0, 0);
	mciSendString("close MP3_Device", NULL, 0, 0);
}
void gr_start(int &GrDriver, int&GrMode, int&ErrorCode){
	GrDriver = VGA;//800x600
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");

	ErrorCode = graphresult();
	if (ErrorCode != grOk){
		cout << "Error : " << ErrorCode << endl;
		getch();
		exit(1);
	}
}