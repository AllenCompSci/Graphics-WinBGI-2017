#include <iostream>
#include <graphics.h>
#include <winbgi.cpp>
#include <time.h>
#include <Windows.h>
#include <string>
#include "shapes.h"
#include "VirtualKeys.h"

using namespace std;

INPUT_RECORD irInBuf;



void gr_start(int&GrDriver, int&GrMode, int&ErrorCode) {
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk) {
		cout << "Error: " << ErrorCode;
		getch();
		exit(1);
	}
}

void displayv(){
	string vertype = "kirb quest alpha v.0.35";
	outtextxy(5, 5, vertype.c_str());
}

struct PASS{

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
}global;

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

struct circprops {

	int color;
	int posx;
	int posy;
	int speedx;
	int speedy;
	int radius;
	int rx;
	int ry;
	int height;
	bool movingup;
	

}player;

struct platprops {

	int color;
	int top;
	int bottom;
	int left;
	int right;
	int speed;
}plat[5];

void genprops() { //generate points
	player.radius = 50;
	player.rx = player.radius;
	player.ry = player.radius;
	player.posx = getmaxx() / 2;
	player.posy = getmaxy() / 2;
	player.color = 5;
	player.speedx = 7;
	player.height = 0;
	player.movingup = false;
	

	plat[0].top = getmaxy() - 100;
	plat[0].bottom = getmaxy() - 75;
	plat[0].left = 0; //getmaxx() / 4;
	plat[0].right = getmaxx();// plat[0].left + 225;
	plat[0].color = 7;


}

void leftright(){ //left/right player movement
	
	

	global.press = global.hold = false;
	if (global.press){
		if (KEYBOARD(VK_A))
			player.posx -= player.speedx;
		if (KEYBOARD(VK_D))
			player.posx += player.speedx;
		global.press = false;
	}
	if (global.hold){
		if (KEYBOARD(VK_A))
			player.posx -= player.speedx;
		if (KEYBOARD(VK_D))
			player.posx += player.speedx;
		global.hold = false;
	}

	
}

void squish(){ //squishing animation
	for (int i = 0; i < 10; i++){
		player.rx++;
		player.ry--;
		kaybe();
		setcolor(player.color);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
		delay(1);
		setcolor(0);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
	}
	for (int i = 0; i < 10; i++){
		player.rx--;
		player.ry++;
		kaybe();
		setcolor(player.color);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
		delay(1);
		setcolor(0);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
	}
	
}

void movement(){ //circle movement
	while (true) {
		
		if (player.posy + player.radius >= plat[0].top){ //collision
			player.movingup = true;
			player.height = 0;
			squish();
		}

		if (player.height >= 0 && player.height <= 250){//collision with air
			if (player.movingup)
				player.speedy = -5;
			else
				player.speedy = 1;
		}
		else if (player.height > 250 && player.height <= 425){
			if (player.movingup)
				player.speedy = -4;
			else
				player.speedy = 2;
		}
		else if (player.height > 425 && player.height <= 463){
			if (player.movingup)
				player.speedy = -3;
			else
				player.speedy = 3;
		}
		else if (player.height > 463 && player.height <= 479){
			if (player.movingup)
				player.speedy = -2;
			else
				player.speedy = 4;
		}
		else if (player.height > 479 && player.height <= 488){
			if (player.movingup)
				player.speedy = -1;
			else
				player.speedy = 5;
		}
		else if (player.height > 288){
			if (player.movingup){
				player.movingup = false;
				player.height = 0;
			}

		}








		if (player.movingup){//moving up
			player.speedy = -5;
			player.height -= player.speedy;
		}
		else {//moving down
			player.speedy = 5;
			player.height += player.speedy;
		}

		player.posy += player.speedy;

		

		setcolor(player.color);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
		delay(1);
		setcolor(0);
		fillellipse(player.posx, player.posy, player.rx, player.ry);





	}//while
}

void plats(){
	setcolor(plat[0].color);
	bar(plat[0].left, plat[0].top, plat[0].right, plat[0].bottom);

}



