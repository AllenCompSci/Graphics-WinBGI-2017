#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <string>
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <fstream>
#include <vector>
#include "Header.h"
//#include <thread>
#include <stdio.h> 
#include <mmsystem.h>
//#include <mciapi.h>
//#include "VirtualKeys.h"
#pragma comment(lib, "winmm.lib")
using namespace std;
int GrDriver,GrMode,ErrorCode;
int *col;
int *x;
int *y;
int *ang;
bool *dead;
int *maxpower;
int *power;
int terrain;
string player;

int calcPrab(int power, int j, int x){
	cout << "Power : " << power << endl;
	cout << "   j  : " << j << endl;
	cout << "   x  : " << x << endl;
	cout << "  eq  : " << (double)  (1.0/(double)(2 * power))  *  ( (double)(j-(2 * (double)power)-x)  *(j-(2*(double)power)-x))  + (2.5 * (double)power) << endl;
	return (int) (double)  (1.0/(double)(2 * power))  *  ( (double)(j-(2 * (double)power)-x)  *(j-(2*(double)power)-x))  + (2.5 * (double)power);

}
struct ground{
	double x;
	double y;
	int bkcolor;
	int whichcolor;
	int color[3];
	double a;
	double b;
	double h;
	double k;
}g;
int playnum;
string _toString(int x){
	if(x == 1) 
		return "1";
	if(x == 2)
		return "2";
	if(x == 3)
		return "3";
	if(x == 4)
		return "4";
	if(x == 5)
		return "5";
	else
		return "" + (char)(x+48);

}
void gr_start(int&GrDriver,int&GrMode,int&ErrorCode){
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver,&GrMode,"");
	ErrorCode = graphresult();
	if(ErrorCode != grOk){
		cout << "Error: " << ErrorCode;
		getch();
		exit(1);
	}
}


void homescreen(){
	settextstyle(4, 0, 8);
	outtextxy(50, 50, "TANKS");
	setcolor(7);
	settextstyle(2, 0, 8);
	setcolor(4);
	setfillstyle(1, 4);
	fillellipse(getmaxx()/2, getmaxy()/2, 200, 200);
	setcolor(15);
	settextstyle(2, 0, 15);
	outtextxy((getmaxx()/2)-45, getmaxy()/2-15, "GO!");
	settextstyle(2, 0, 10);
	outtextxy(getmaxx() / 2 - 300, getmaxy() / 2 - 255, "Press any key to continue");
	getch();
	cleardevice();
	settextstyle(4, 0, 8);
	outtextxy(50, 50, "TANKS");
	setfillstyle(1, 7);
	setcolor(7);
	bar(250, 250, 750, 575);
	bar(1000, 250, 1500, 575);
	settextstyle(2, 0, 5);
	outtextxy(1050, 300, "TERRAIN TYPE");
	outtextxy(300, 300, "PLAYERS:");
	outtextxy(1100, 350, "(6) Mountain");
	outtextxy(1100, 400, "(7) Forest");
	outtextxy(1100, 450, "(8) Desert");
	outtextxy(1100, 500, "(9) Random");

	outtextxy(350, 350, "(2) 2 Players");
	outtextxy(350, 400, "(3) 3 Players");
	outtextxy(350, 450, "(4) 4 Players");
	outtextxy(350, 500, "(5) 5 Players");

	setcolor(1);
	circle(300, 350, 15);
	circle(300, 400, 15);
	circle(300, 450, 15);
	circle(300, 500, 15);

	circle(1050, 350, 15);
	circle(1050, 400, 15);
	circle(1050, 450, 15);
	circle(1050, 500, 15);
	setfillstyle(1, 4);
	setcolor(4);
	fillellipse(getmaxx() / 2 - 75, getmaxy() / 2 + 275, 200, 200);
	settextstyle(2, 0, 15);
	setcolor(7);
	outtextxy(getmaxx() / 2 - 200, getmaxy() / 2 + 250, "CONTINUE");
	settextstyle(2, 0, 6);
	outtextxy(getmaxx() / 2 + 150, getmaxy() / 2 + 250, "After selecting your options,");
	outtextxy(getmaxx() / 2 + 150, getmaxy() / 2 + 275, "press spacebar to continue");

	terrain = -1;
	playnum = -1;
	bool choose = true;
	while (choose == true){
		if (kbhit()){
			int choice = getch();
			if (choice == '2'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(300, 350, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(300, 400, 15, 15);
				fillellipse(300, 450, 15, 15);
				fillellipse(300, 500, 15, 15);
				playnum = 2;
			}
			else if (choice == '3'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(300, 400, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(300, 350, 15, 15);
				fillellipse(300, 450, 15, 15);
				fillellipse(300, 500, 15, 15);
				playnum = 3;
			}
			else if (choice == '4'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(300, 450, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(300, 350, 15, 15);
				fillellipse(300, 400, 15, 15);
				fillellipse(300, 500, 15, 15);
				playnum = 4;
			}
			else if (choice == '5'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(300, 500, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(300, 350, 15, 15);
				fillellipse(300, 400, 15, 15);
				fillellipse(300, 450, 15, 15);
				playnum = 5;
			}
			else if (choice == '6'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(1050, 350, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(1050, 400, 15, 15);
				fillellipse(1050, 450, 15, 15);
				fillellipse(1050, 500, 15, 15);
				terrain = 0;	
			}
			else if (choice == '7'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(1050, 400, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(1050, 350, 15, 15);
				fillellipse(1050, 450, 15, 15);
				fillellipse(1050, 500, 15, 15);
				terrain = 1;
			}
			else if (choice == '8'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(1050, 450, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(1050, 350, 15, 15);
				fillellipse(1050, 400, 15, 15);
				fillellipse(1050, 500, 15, 15);
				terrain = 2;
			}
			else if (choice == '9'){
				setcolor(0);
				setfillstyle(1, 0);
				fillellipse(1050, 500, 15, 15);
				setcolor(8);
				setfillstyle(1, 8);
				fillellipse(1050, 350, 15, 15);
				fillellipse(1050, 400, 15, 15);
				fillellipse(1050, 450, 15, 15);
				terrain = rand()%3;
			}
			else if (terrain != -1 && playnum != -1){
			choose = false;
			cleardevice();
			}
		}
	}
	cleardevice();
}
int backcolor[1919][1017];
void preparescreen(){
	g.x = 0;
	g.y = 0;
	g.whichcolor = 0;
	if(terrain == 0){//mountain
		g.color[0] = 7;
		g.color[1] = 9;
		g.color[2] = 15;
		g.a = rand()%50+275;
		g.b = rand()%2*.01+.009;
		g.h = rand()%1000;
		g.k = rand()%50+475;
		drawMount();
	}
	else if(terrain == 1){//forest
		g.color[0] = 0;
		g.color[1] = 2;
		g.color[2] = 6;
		g.a = rand()%10+95;
		g.b = rand()%2*.001+.009;
		g.h = rand()%1000;
		g.k = rand()%50+475;
		drawForest();
	}
	else if(terrain == 2){//desert
		g.color[0] = 7;
		g.color[1] = 12;
		g.color[2] = 14;
		g.a = rand()%10+145;
		g.b = rand()%10*.0005+.0005;
		g.h = rand()%1000;
		g.k = rand()%50+500;
		drawDesert();
	}
	for(g.x = 1; g.x < getmaxx(); g.x++){
		for(g.y = g.a-g.k; g.y < getmaxy(); g.y++){
			if(g.y > g.a*(sin(g.b*g.x+g.h))+g.k){
				int whichcolor = (int)rand()%3;
				putpixel(g.x,g.y,g.color[whichcolor]);
			}
		}
	}

	x = new int[playnum];
	y = new int[playnum];
	col = new int[playnum];
	ang = new int[playnum];
	dead = new bool[playnum];
	maxpower = new int[playnum];
	power = new int[playnum];
	col[1] = 1;
	col[2] = 2;
	if(playnum > 2)
		col[3] = 3;
	if(playnum > 3)
		col[4] = 4;
	if(playnum > 4)
		col[5] = 5;
	for(int i = 0; i < playnum; i++){
		x[i] = rand()%(getmaxx() / playnum - (20 / playnum)) + (getmaxx() / playnum - (20 / playnum)) * i;
		y[i] = g.a*(sin(g.b*x[i]+g.h))+g.k-15;
		ang[i] = rand()%180;
		dead[i] = false;
		power[i] = 100;
		maxpower[i] = 100;

		setcolor(col[i]);
		setfillstyle(1, col[i]);
		bar(x[i] - 20, y[i], x[i] + 20, y[i] + 16);//tank body
		fillellipse(x[i], y[i], 13, 13); //top bubble
		
		fillellipse(x[i] - 20, y[i] + 8, 8, 8); //rounded sides
		fillellipse(x[i] + 20, y[i] + 8, 8, 8);
		
		setcolor(0);
		setfillstyle(1, 0);
		pieslice(x[i], y[i], ang[i] - 1, ang[i] + 1, 35); //cannon
		bar(x[i] - 17, y[i] + 9, x[i] + 17, y[i] + 17); //treads
		fillellipse(x[i] - 17, y[i] + 13, 2, 4);
		fillellipse(x[i] + 17, y[i] + 13, 2, 4);
	}
	bar(0,0,getmaxx(), 100);
}
void fixtanks(){
	for(int i = 0; i < playnum; i++){
		for(int k = (x[i] - 37); k < (x[i] + 37); k++){//redraws background
			for(int j = (y[i] - 37); j < (y[i] + 37); j++){
				putpixel(k,j,backcolor[k][j]);
			}
		}
		for(g.x = (x[i] - 37); g.x < (x[i] + 37); g.x++){//redraws frontground
			for(g.y = (y[i] - 37); g.y < (y[i] + 37); g.y++){
				if(g.y > g.a*(sin(g.b*g.x+g.h))+g.k){
					int whichcolor = (int)rand()%3;
					putpixel(g.x,g.y,g.color[whichcolor]);
				}
			}
		}
	}
	for(int i = 0; i < playnum; i++){
		if(dead[i] == false){
			setcolor(col[i]);//redraws tanks
			setfillstyle(1, col[i]);
			bar(x[i] - 20, y[i], x[i] + 20, y[i] + 16);//tank body
			fillellipse(x[i], y[i], 13, 13); //top bubble
			
			fillellipse(x[i] - 20, y[i] + 8, 8, 8); //rounded sides
			fillellipse(x[i] + 20, y[i] + 8, 8, 8);
			
			setcolor(0);
			setfillstyle(1, 0);
			pieslice(x[i], y[i], ang[i] - 1, ang[i] + 1, 35); //cannon
			bar(x[i] - 17, y[i] + 9, x[i] + 17, y[i] + 17); //treads
			fillellipse(x[i] - 17, y[i] + 13, 2, 4);
			fillellipse(x[i] + 17, y[i] + 13, 2, 4);
		}
	}
}
void turns(){
moreturns:
	bool state;
	int turnend = 0;
	int thegetch;
	for(int i = 0; i < playnum; i++){
		for(int j = 0; j < playnum; j++){
			if(dead[i] == true)
				i++;
		}
		setcolor(col[i]);
		settextstyle(1,0,2);
		player = "Player " + _toString(i + 1);
		outtextxy(25,25,player.c_str());
		state = true;
		while(state){
			if(kbhit()){
				int key = getch();
				if(key == 'a' && ang[i] < 180){//aim left
					ang[i]+=5;
					fixtanks();
				}
				if(key == 'd' && ang[i] > 0){//aim right
					ang[i]-=5;
					fixtanks();
				}
				if(key == 'w' && power[i] < maxpower[i]){//increase power
					power[i]+=5;
					fixtanks();
				}
				if(key == 's' && power[i] < maxpower[i]){//decrease power
					power[i]-=5;
					fixtanks();
				}
				if(key == 'j' && x[i] > 50){//move left
					x[i]-=5;
					y[i] = g.a*(sin(g.b*x[i]+g.h))+g.k-15;
					fixtanks();
				}
				if(key == 'l' && x[i] < (getmaxx() - 50)){//move right
					x[i]+=5;
					y[i] = g.a*(sin(g.b*x[i]+g.h))+g.k-15;
					fixtanks();
				}
				if(key == 32){
					setfillstyle(1,15);
					int k;
					for(int j = x[i]; j < getmaxx(); j++){
						k = calcPrab(power[i], j, x[i]);
						k=(double)(1.0/(double)(2 * power[i]))*((double)(j-(2 * (double)power[i])-x[i])*(j-(2*(double)power[i])-x[i]))+(2.5*(double)power[i]);
						if(k > g.a*(sin(g.b*j+g.h))+g.k)
							j = getmaxx()+100;
						setcolor(15);
						fillellipse(j,k,3,3);
						for(int l = j - 3; l < j + 3; l++){
							for(int m = k - 3; m < k + 3; m++){
								int whichcolor = (int)rand()%3;
								if(m < g.a*(sin(g.b*l+g.h))+g.k)
									putpixel(l,m,backcolor[k][j]);
								else
									putpixel(l,m,g.color[whichcolor]);
			}
		}
		for(g.x = (x[i] - 37); g.x < (x[i] + 37); g.x++){//redraws frontground
			for(g.y = (y[i] - 37); g.y < (y[i] + 37); g.y++){
				if(g.y > g.a*(sin(g.b*g.x+g.h))+g.k){
					int whichcolor = (int)rand()%3;
					putpixel(g.x,g.y,g.color[whichcolor]);
				}
			}
		}
					}
					fixtanks();
					state = false;
				}
			}
		}
		
	}
	for(int i = 0; i < playnum; i++){
		if(dead[i] == true)
			turnend++;
	}
	if(turnend < (playnum - 1))
		goto moreturns;
}
void main(){
	gr_start(GrDriver,GrMode,ErrorCode);
	srand((unsigned int)time(NULL));

	homescreen();
	preparescreen();
	turns();
	

	getch();
	closegraph();
}


