#include <iostream>
#include <graphics.h>
#include <winbgi.cpp>
#include <time.h>
#include <Windows.h>
#include <string>
#include <thread>
#include <fstream>
#include "VirtualKeys.h"

#define CENTERX getmaxx() / 2
#define CENTERY getmaxy() / 2

using namespace std;

int GrDriver, GrMode, ErrorCode;

INPUT_RECORD irInBuf;

struct misc {
	int runtime;
	bool squishing;
	bool demoa = false;
}other;

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

void movement();
void reset();
void gamethread();
void shopthread();
void infothread();
void demo();

#pragma region gameplay

struct PASS {

	int VirtualKey;
	POINT p;
	bool press;
	bool hold;
}global;

bool KEYBOARD(int VirtualKey) {
	if ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0)
	{
		irInBuf.EventType = KEY_EVENT;
		global.press = true;
		global.VirtualKey = VirtualKey;
		int count = 0;
		do {
			if (count++ > 200) {
				global.hold = true;
			}
		} while ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
		return true;
	}
	return false;
}

struct levels {
	bool level[10];
	int timercount;
	void level_check() {
		string l;
		game.timercount = clock() - other.runtime;
		if (timercount < 10000) {
			game.level[0] = true;
			l = "Level 1";
			setcolor(GREEN);

		}
		else if (timercount > 10000 && timercount < 25000) {
			game.level[1] = true;
			l = "Level 2";
			setcolor(GREEN);
		}
		else if (timercount > 25000 && timercount < 50000) {
			game.level[2] = true;
			l = "Level 3";
			setcolor(BLUE);

		}
		else if (timercount > 50000 && timercount < 80000) {
			game.level[3] = true;
			l = "Level 4";
			setcolor(BLUE);

		}
		else if (timercount > 80000 && timercount < 120000) {
			game.level[4] = true;
			l = "Level 5";
			setcolor(YELLOW);

		}
		else if (timercount > 120000 && timercount < 156000) {
			game.level[5] = true;
			l = "Level 6";
			setcolor(YELLOW);

		}
		else if (timercount > 156000 && timercount < 192000) {
			game.level[6] = true;
			l = "Level 7";
			setcolor(RED);

		}
		else if (timercount > 192000 && timercount < 228000) {
			game.level[7] = true;
			l = "Level 8";
			setcolor(RED);

		}
		else if (timercount > 228000 && timercount < 264000) {
			game.level[8] = true;
			l = "Level 9";
			setcolor(MAGENTA);

		}
		else if (timercount > 264000 && timercount < 300000) {
			game.level[9] = true;
			l = "Level MAX";
			setcolor(MAGENTA);

		}



		settextstyle(6, 0, 1);
		outtextxy(CENTERX, 5, l.c_str());
	}
}game;

struct points {
	int totalscore;
	int score;
	string dispscore;
	double multiplier;
	int bonus;

	void scorecount() {
		p1.multiplier = .66;
		for (int i = 0; i < 10; i++) {
			if (game.level[i])
				p1.multiplier += .44;

		}

		setcolor(GREEN);
		p1.score = (((clock() - other.runtime) / 10) * p1.multiplier) + p1.bonus;
		p1.dispscore = to_string(p1.score);

		settextstyle(2, 0, 5);
		outtextxy(5, 50, p1.dispscore.c_str());
	}


}p1;

struct statlevels {
	bool sizelevel[4];
	bool speedxlevel[4];
	bool heightxlevel[4];
};

struct circprops {
	string deathtype;

	int color;
	int posx;
	int posy;

	int speedx2;
	int speedx;
	int speedy;

	int radius;
	int rx;
	int ry;

	bool dead;
	bool hit;
	bool movingup;
	int acc[5];
	int y;
	double heightx;
	double heightx2;

	statlevels upgrade;


}player;

struct circfunctions {
	void circ_props() {



		if (player.upgrade.sizelevel[0])
			player.radius = 50;
		if (player.upgrade.sizelevel[1]) {
			player.radius = 45;
			player.upgrade.sizelevel[0] = false;
		}
		if (player.upgrade.sizelevel[2])
			player.radius = 35;
		if (player.upgrade.sizelevel[3])
			player.radius = 25;
		player.rx = player.radius;
		player.ry = player.radius;

		player.posx = CENTERX;
		player.posy = CENTERY;

		if (player.upgrade.speedxlevel[0])
			player.speedx = 3;
		if (player.upgrade.speedxlevel[1]) {
			player.speedx = 5;
			player.upgrade.speedxlevel[0] = false;
		}
		if (player.upgrade.speedxlevel[2])
			player.speedx = 6;
		if (player.upgrade.speedxlevel[3])
			player.speedx = 8;
		player.speedx2 = player.speedx;
		player.speedy = 5;

		player.hit = false;
		player.movingup = false;
		player.dead = false;

		player.y = 4;

		player.acc[0] = { 200 };
		player.acc[1] = { 125 };
		player.acc[2] = { 58 };
		player.acc[3] = { 19 };
		player.acc[4] = { 9 };


		if (player.upgrade.heightxlevel[0])
			player.heightx = 1;
		if (player.upgrade.heightxlevel[1]) {
			player.heightx = 1.25;
			player.upgrade.heightxlevel[0] = false;
		}
		if (player.upgrade.heightxlevel[2])
			player.heightx = 1.5;
		if (player.upgrade.heightxlevel[3])
			player.heightx = 1.75;
		player.heightx2 = player.heightx;
	}
	//no need for move function bc it's just adding one value
	void circ_spawn() {
		setcolor(player.color);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
	}
	void circ_destroy() {
		setcolor(0);
		fillellipse(player.posx, player.posy, player.rx, player.ry);
	}

}circ;

struct platprops {
	int color;
	int top;
	int bottom;
	int left;
	int right;
	int speed;
	int amt;
}plat[12], amount;

struct platfunctions {

	void plat_props() {
		amount.amt = 12;

		plat[0].left = (getmaxx() / 4) * 2.3;
		plat[0].right = plat[0].left + 255;
		plat[0].top = (getmaxy() / 4) * 3;
		plat[0].bottom = plat[0].top + 12;
		plat[0].speed = -3;
		plat[0].color = LIGHTRED;

		plat[1].left = getmaxx();
		plat[1].right = plat[1].left + 255;
		plat[1].top = (getmaxy() / 4) * 3;
		plat[1].bottom = plat[1].top + 12;
		plat[1].speed = -3;
		plat[1].color = LIGHTRED; //blue


		for (int i = 2; i < 12; i++) {
			plat[i].left = getmaxx() + (rand() % getmaxx());
			plat[i].right = plat[i].left + 255;
			plat[i].top = rand() % (CENTERY)+(CENTERY - 25);
			plat[i].bottom = plat[i].top + 12;

			plat[i].speed = -3;

			plat[i].color = LIGHTRED; //yellow
		}
	}
	void plat_move() {
		for (int i = 0; i < amount.amt; i++) {
			plat[i].left += plat[i].speed;
			plat[i].right += plat[i].speed;

			if (plat[i].right < 0) {
				plat[i].left = getmaxx() + (rand() % CENTERX);
				plat[i].right = plat[i].left + 255;
				plat[i].top = rand() % (CENTERY)+(CENTERY - 25);
				plat[i].bottom = plat[i].top + 12;

			}
		}
	}
	void plat_spawn() {
		for (int i = 0; i < amount.amt; i++) {
			setcolor(plat[i].color);
			rectangle(plat[i].left, plat[i].top, plat[i].right, plat[i].bottom);
		}
	}
	void plat_destroy() {
		for (int i = 0; i < amount.amt; i++) {
			setcolor(0);
			rectangle(plat[i].left, plat[i].top, plat[i].right, plat[i].bottom); //platform
		}
	}
	void speed_check() {
		if (game.level[0]) {
			for (int i = 0; i < amount.amt; i++)
				plat[i].speed = -2;
		}
		if (game.level[1]) {
			for (int i = 0; i < amount.amt; i++)
				plat[i].speed = -3;
			amount.amt = 11;
		}
		if (game.level[2]) {
			amount.amt = 10;
		}
		if (game.level[3]) {
			for (int i = 0; i < amount.amt; i++)
				plat[i].speed = -4;
			amount.amt = 9;
		}
		if (game.level[5]) {
			for (int i = 0; i < amount.amt; i++)
				plat[i].speed = -5;
			amount.amt = 8;
		}
		if (game.level[7]) {
			for (int i = 0; i < amount.amt; i++)
				plat[i].speed = -6;
			amount.amt = 7;
		}
		if (game.level[8]) {
			amount.amt = 6;
		}
		if (game.level[9]) {
			amount.amt = 5;
		}
	}

}platform;

struct hazardprops {

	int color;

	int top;
	int left;
	int right;
	int bottom;

	int posx;
	int posy;

	int radius;

	int speed;

	int begintime;
	int atime;

	bool start;
	bool active, warning;
	bool destroyed;

}fireball, toxplat, rock, light, cloud;

struct hazardfunctions {

#pragma region fireball
	int amount;

	void fire_props() {

		fireball.radius = 10;
		fireball.posy = rand() % ((getmaxy() / 4) * 3);
		fireball.posx = getmaxx() + (rand() % getmaxx());
		fireball.right = fireball.posx + fireball.radius;
		fireball.color = 4;
		fireball.speed = -4;

	}
	void fire_move() {
		fireball.posx += fireball.speed;
		fireball.right += fireball.speed;

		if (fireball.posx + fireball.radius < 0) {
			fireball.posy = rand() % ((getmaxy() / 4) * 3);
			fireball.posx = getmaxx() + (rand() % getmaxx());
			fireball.right = fireball.left + fireball.radius;
			fireball.speed = -4;

		}

	}
	void fire_spawn() {
		setcolor(fireball.color);
		fillellipse(fireball.posx, fireball.posy, fireball.radius, fireball.radius);
	}
	void fire_destroy() {
		setcolor(0);
		fillellipse(fireball.posx, fireball.posy, fireball.radius, fireball.radius);
	}
	void kill_check() {
		if (pow(fireball.posx - player.posx, 2) + pow(fireball.posy - player.posy, 2) <= pow(fireball.radius + player.radius, 2)) {
			player.deathtype = "Killed by fireball";
			player.dead = true;
			player.hit = true;
			//exit(0);
		}
	}
#pragma endregion fireball functions

#pragma region toxicplat
	void plat_props() {
		toxplat.left = getmaxx() + (rand() % getmaxx());
		toxplat.right = toxplat.left + 225;
		toxplat.top = rand() % (CENTERY)+CENTERY;
		toxplat.bottom = toxplat.top + 25;

		toxplat.speed = -2;

		toxplat.color = GREEN;

		toxplat.start = false;
	}
	void plat_move() {
		toxplat.left += toxplat.speed;
		toxplat.right += toxplat.speed;

		if (toxplat.right < 0) {
			toxplat.left = getmaxx() * 4 + (rand() % CENTERX);
			toxplat.right = toxplat.left + 225;
			toxplat.top = rand() % (CENTERY)+CENTERY;
			toxplat.bottom = toxplat.top + 25;
		}
	}
	void plat_spawn() {
		setcolor(toxplat.color);
		bar(toxplat.left, toxplat.top, toxplat.right, toxplat.bottom);
	}
	void plat_destroy() {
		setcolor(0);
		bar(toxplat.left, toxplat.top, toxplat.right, toxplat.bottom); //platform
	}
	void toxic_check() {
		//(player.posy + player.radius >= toxplat.top && player.posx > toxplat.left  && player.posx < toxplat.right && player.posy + player.radius <= toxplat.bottom
		// player.hit = true;

		if (player.posy + player.radius >= toxplat.top && player.posx > toxplat.left  && player.posx < toxplat.right && player.posy + player.radius <= toxplat.bottom) {
			player.hit = true;
			if (!toxplat.start) {
				toxplat.begintime = clock();
				toxplat.start = true;
				player.speedx -= player.speedx * 2;
			}
		}

		if (toxplat.start)
			toxplat.atime = clock();
		if (toxplat.atime > toxplat.begintime + 3500) {
			player.speedx = player.speedx2;
			toxplat.start = false;
		}
	}

#pragma endregion toxic platform functions

#pragma region rocks
	void rock_props() {
		rock.color = 15;
		rock.posx = rand() % getmaxx();
		rock.posy = (rand() % getmaxy()) - getmaxy() * 2;
		rock.radius = 20;
		rock.speed = 4;
	}
	void rock_move() {
		rock.posy += rock.speed;

		if (rock.posy - rock.radius > getmaxy()) {
			rock.posx = rand() % getmaxx();
			rock.posy = (rand() % getmaxy()) - getmaxy() * 2;
		}
	}
	void rock_spawn() {
		setcolor(rock.color);
		fillellipse(rock.posx, rock.posy, rock.radius, rock.radius);
	}
	void rock_destroy() {
		setcolor(0);
		fillellipse(rock.posx, rock.posy, rock.radius, rock.radius);
	}
	void rock_kill() {
		if (pow(rock.posx - player.posx, 2) + pow(rock.posy - player.posy, 2) <= pow(rock.radius + player.radius, 2)) {
			player.deathtype = "Killed by falling rock";
			player.dead = true;
			player.hit = true;
			//exit(0);
		}
	}

#pragma endregion rocks functions

#pragma region lightning

	void lightning_props() {
		light.color = 14;

		light.left = rand() % getmaxx();
		light.right = light.left + 15;
		light.top = rand() % getmaxy() - getmaxy() * 6;
		light.bottom = light.top;
		light.speed = 7;

		light.active = false;
		light.active = false;
		light.destroyed = true;

	}
	void lightning_move() {
		light.bottom += light.speed;

		if (light.bottom > -500) {
			light.warning = true;
		}
		if (light.bottom > -1) {
			light.warning = false;
			light.active = true;
			light.speed = 30;
		}
		if (light.bottom > getmaxy()) {

			setcolor(0);
			bar(light.left, light.top, light.right, light.bottom);

			light.left = rand() % getmaxx();
			light.right = light.left + 15;
			light.top = rand() % getmaxy() - getmaxy() * 6;
			light.bottom = light.top;
			light.speed = 7;
			light.active = false;
			light.destroyed = false;
		}

	}
	void lightning_spawn() {
		if (light.warning) {
			setcolor(RED);
			bar(light.left + 6, 5, light.left + 9, 20);
			bar(light.left + 6, 22, light.left + 9, 25);
		}
		if (light.active) {
			setcolor(light.color);
			bar(light.left, light.top, light.right, light.bottom);
		}
	}
	void lightning_check() {
		if (light.active) {
			if (player.posy < light.bottom && player.posx + player.radius > light.left && player.posx - player.radius < light.left) {
				player.deathtype = "Struck by lightning";
				player.dead = true;
				player.hit = true;
			}
		}
	}

#pragma endregion lightning functions

#pragma region raincloud
	void cloud_props() {
		cloud.posy = (rand() % getmaxy()) - getmaxy() * 3;
		cloud.posx = rand() % getmaxx();
		cloud.radius = 25;
		cloud.speed = 1;
	}
	void cloud_move() {
		cloud.posy += cloud.speed;

		if (cloud.posy > getmaxy()) {

			setcolor(0);
			bar(cloud.posx - 23 - cloud.radius, 0, cloud.posx + 23 + cloud.radius, getmaxy());

			cloud.posy = (rand() % getmaxy()) - getmaxy() * 3;
			cloud.posx = rand() % getmaxx();
			cloud.speed = 1;
		}
	}
	void cloud_spawn() {
		if (cloud.posy < 23) {
			setcolor(MAGENTA);
			fillellipse(cloud.posx, cloud.posy, cloud.radius, cloud.radius);
			fillellipse(cloud.posx - 23, cloud.posy - 10, cloud.radius, cloud.radius);
			fillellipse(cloud.posx + 23, cloud.posy + 10, cloud.radius, cloud.radius);
			fillellipse(cloud.posx + 12, cloud.posy - 15, cloud.radius, cloud.radius);
			fillellipse(cloud.posx - 23, cloud.posy + 10, cloud.radius, cloud.radius);
		}
		if (cloud.posy > 23 && cloud.posy < getmaxy()) {
			setcolor(GREEN);
			for (int i = 0; i < 10; i++)
				bar(cloud.posx - 20, 62 + i * 100, cloud.posx - 15, 87 + i * 100);
			for (int i = 0; i < 10; i++)
				bar(cloud.posx + 10, 120 + i * 100, cloud.posx + 15, 145 + i * 100);
			for (int i = 0; i < 10; i++)
				bar(cloud.posx + 30, 153 + i * 100, cloud.posx + 35, 178 + i * 100);
		}
	}
	void cloud_destroy() {
		if (cloud.posy < 22) {
			setcolor(0);
			fillellipse(cloud.posx, cloud.posy, cloud.radius, cloud.radius);
			fillellipse(cloud.posx - 23, cloud.posy - 10, cloud.radius, cloud.radius);
			fillellipse(cloud.posx + 23, cloud.posy + 10, cloud.radius, cloud.radius);
			fillellipse(cloud.posx + 12, cloud.posy - 15, cloud.radius, cloud.radius);
			fillellipse(cloud.posx - 23, cloud.posy + 10, cloud.radius, cloud.radius);
		}
	}
	void cloud_check() {
		if (cloud.posy > 23 && player.posx + player.radius > cloud.posx - 10 && player.posx - player.radius < cloud.posx + 5) {
			player.deathtype = "Disintegrated by acid rain";
			player.dead = true;
			player.hit = true;
		}
	}
#pragma endregion raincloud functions

}fire, toxicplat, rocks, lightning, raincloud;

struct bonusprops {

	int color;

	int posx;
	int posy;

	int radius;

	int speed;

	int begintime;
	int atime;

	bool destroyed;
	bool start;

}coin, flower, star, fly;

struct bonusfunctions {

#pragma region coin
	void coin_props() {

		coin.radius = 6;
		coin.posy = rand() % getmaxy();
		coin.posx = getmaxx() + (rand() % getmaxx());
		coin.color = 14;
		coin.speed = -10;
		coin.destroyed = false;

	}
	void coin_move() {
		coin.posx += coin.speed;

		if (coin.posx + coin.radius < 0) {
			coin.posy = rand() % getmaxy();
			coin.posx = getmaxx() * 3 + (rand() % getmaxx());
			coin.speed = -10;
			coin.destroyed = false;
		}

	}
	void coin_spawn() {
		if (!coin.destroyed) {
			setcolor(coin.color);
			fillellipse(coin.posx, coin.posy, coin.radius, coin.radius);
		}
	}
	void coin_destroy() {
		if (!coin.destroyed) {
			setcolor(0);
			fillellipse(coin.posx, coin.posy, coin.radius, coin.radius);
		}
	}
	void collect_check() {
		if (!coin.destroyed) {
			if (pow(coin.posx - player.posx, 2) + pow(coin.posy - player.posy, 2) <= pow(coin.radius + player.radius, 2)) {
				p1.bonus += 5000;
				setcolor(0);
				fillellipse(coin.posx, coin.posy, coin.radius, coin.radius);
				coin.destroyed = true;
			}

		}
	}
#pragma endregion coin functions 

#pragma region flower
	void flower_props() {
		flower.posx = rand() % getmaxx();
		flower.posy = rand() % (getmaxy() * 1) + (getmaxy() * 4);
		flower.speed = -3;
		flower.radius = 7;
		flower.destroyed = false;
	}
	void flower_move() {
		flower.posy += flower.speed;

		if (flower.posy + flower.radius + 13 < 0) {
			flower.posx = rand() % getmaxx();
			flower.posy = rand() % (getmaxy() * 1) + (getmaxy() * 4);
			flower.speed = -3;
			flower.destroyed = false;
		}
	}
	void flower_spawn() {
		if (!flower.destroyed) {
			setcolor(YELLOW);
			fillellipse(flower.posx, flower.posy, flower.radius, flower.radius);
			setcolor(BLUE);
			fillellipse(flower.posx + 9, flower.posy, 5, 5);
			fillellipse(flower.posx - 9, flower.posy, 5, 5);
			fillellipse(flower.posx, flower.posy + 9, 5, 5);
			fillellipse(flower.posx, flower.posy - 9, 5, 5);
		}
	}
	void flower_destroy() {
		if (!flower.destroyed) {
			setcolor(0);
			fillellipse(flower.posx, flower.posy, flower.radius, flower.radius);
			fillellipse(flower.posx + 9, flower.posy, 5, 5);
			fillellipse(flower.posx - 9, flower.posy, 5, 5);
			fillellipse(flower.posx, flower.posy + 9, 5, 5);
			fillellipse(flower.posx, flower.posy - 9, 5, 5);
		}
	}
	void flower_check() {
		if (!flower.destroyed) {
			if (pow(flower.posx - player.posx, 2) + pow(flower.posy - player.posy, 2) <= pow(flower.radius + player.radius, 2)) {
				flower.destroyed = true;
				if (!flower.start) {
					flower.begintime = clock();
					flower.start = true;
					player.heightx += .75;
				}
			}
		}
		if (flower.start)
			flower.atime = clock();
		if (flower.atime > flower.begintime + 5000) {
			player.heightx = player.heightx2;
			flower.start = false;
		}
	}

#pragma endregion flower functions

#pragma region star
	void star_props() {
		star.posx = rand() % getmaxx();
		star.posy = (rand() % getmaxy()) - getmaxy() * 3;
		star.radius = 7;
		star.speed = 6;
		star.color = 14;
		star.destroyed = false;


	}
	void star_move() {
		star.posy += star.speed;
		star.posx += 3;

		if (star.posy - star.radius > getmaxy()) {
			star.posx = rand() % getmaxx();
			star.posy = (rand() % getmaxy()) - getmaxy() * 3;
			star.speed = 6;
			star.destroyed = false;
		}
	}
	void star_spawn() {
		if (!star.destroyed) {
			setcolor(14);
			fillellipse(star.posx, star.posy, star.radius, star.radius);
			line(star.posx, star.posy, star.posx - 12, star.posy - 12);
			line(star.posx, star.posy, star.posx + 12, star.posy - 12);
			line(star.posx, star.posy, star.posx - 12, star.posy + 12);
			line(star.posx, star.posy, star.posx + 12, star.posy + 12);
		}
	}
	void star_destroy() {
		if (!star.destroyed) {
			setcolor(0);
			fillellipse(star.posx, star.posy, star.radius, star.radius);
			line(star.posx, star.posy, star.posx - 12, star.posy - 12);
			line(star.posx, star.posy, star.posx + 12, star.posy - 12);
			line(star.posx, star.posy, star.posx - 12, star.posy + 12);
			line(star.posx, star.posy, star.posx + 12, star.posy + 12);
		}
	}
	void star_check() {
		if (!star.destroyed) {
			if (pow(star.posx - player.posx, 2) + pow(star.posy - player.posy, 2) <= pow(star.radius + player.radius, 2)) {
				star.destroyed = true;
				if (!star.start) {
					star.begintime = clock();
					star.start = true;
					player.speedx += 5;
				}
			}
		}
		star.atime = clock();
		if (star.atime > star.begintime + 5000) {
			player.speedx = player.speedx2;
			star.start = false;
		}

	}

#pragma endregion star functions

#pragma region void essence
	void essence_props() {
		fly.posx = rand() % getmaxx();
		fly.posy = rand() % getmaxy() + (getmaxy() * 3);
		fly.radius = 10;
		fly.speed = -4;
		fly.destroyed = false;
	}
	void essence_move() {
		fly.posy += fly.speed;
		if (fly.posy < getmaxy())
			fly.posx += fly.speed;

		if (fly.posy + fly.radius < 0) {
			fly.posx = rand() % getmaxx();
			fly.posy = rand() % getmaxy() + (getmaxy() * 3);
			fly.speed = -4;
			fly.destroyed = false;
		}

	}
	void essence_spawn() {
		if (!fly.destroyed) {
			setcolor(MAGENTA);
			fillellipse(fly.posx, fly.posy, fly.radius, fly.radius);
			setcolor(BLACK);
			fillellipse(fly.posx, fly.posy, 5, 5);
			setcolor(BLUE);
			circle(fly.posx, fly.posy, 11);
			arc(fly.posx, fly.posy, 115, 165, 20); //top left
			arc(fly.posx, fly.posy, 15, 435, 20); //top right
			arc(fly.posx, fly.posy, 195, 255, 20); //bottom left
			arc(fly.posx, fly.posy, 285, 345, 20); //bottom right
		}
	}
	void essence_destroy() {
		if (!fly.destroyed) {
			setcolor(0);
			fillellipse(fly.posx, fly.posy, fly.radius, fly.radius);
			fillellipse(fly.posx, fly.posy, 5, 5);
			circle(fly.posx, fly.posy, 11);
			arc(fly.posx, fly.posy, 115, 165, 20); //top left
			arc(fly.posx, fly.posy, 15, 435, 20); //top right
			arc(fly.posx, fly.posy, 195, 255, 20); //bottom left
			arc(fly.posx, fly.posy, 285, 345, 20); //bottom right
		}
	}
	void essence_check() {
		if (!fly.destroyed) {
			if (pow(fly.posx - player.posx, 2) + pow(fly.posy - player.posy, 2) <= pow(fly.radius + player.radius, 2)) {
				fly.destroyed = true;
				if (!fly.start) {
					fly.begintime = clock();
					fly.start = true;
				}
			}
		}
		fly.atime = clock();
		if (fly.atime > fly.begintime + 5000) {

			fly.start = false;
		}

	}
	void fly_move() {
		if (global.press) {
			if (global.VirtualKey == VK_A) {
				player.posx -= player.speedx;
			}
			if (global.VirtualKey == VK_D) {
				player.posx += player.speedx;
			}
			if (global.VirtualKey == VK_S) {
				player.posy += player.speedx;
			}
			if (global.VirtualKey == VK_W) {
				player.posy -= player.speedx;
			}
			if (global.VirtualKey == VK_ESCAPE)
				exit(1);
			global.press = false;
		}
		if (global.hold) {
			if (global.VirtualKey == VK_A) {
				player.posx -= player.speedx;
			}
			if (global.VirtualKey == VK_D) {
				player.posx += player.speedx;
			}
			if (global.VirtualKey == VK_S) {
				player.posy += player.speedx;
			}
			if (global.VirtualKey == VK_W) {
				player.posy -= player.speedx;
			}
			if (global.VirtualKey == VK_ESCAPE)
				exit(1);
			global.hold = false;

		}
	}
	void essence_power() {
		while (fly.start) {

			voidessence.fly_move();
			movement();

			if (game.level[1])
				fire.kill_check();
			if (game.level[2])
				bonuscoin.collect_check();
			if (game.level[3])
				toxicplat.toxic_check();
			if (game.level[4])
				jumpflower.flower_check();
			if (game.level[5])
				rocks.rock_kill();
			if (game.level[6])
				speedstar.star_check();
			if (game.level[7])
				lightning.lightning_check();

			voidessence.essence_check();
			if (game.level[9])
				raincloud.cloud_check();

			if (player.hit)
				return;

			if (player.posy + player.radius >= getmaxy()) { //c with floor
				player.deathtype = "Killed by void";
				player.hit = true;
				player.dead = true;
				return;

			}

		}
	}
#pragma endregion void essence functions

}bonuscoin, jumpflower, speedstar, voidessence;

void genprops() { //generate points

	circ.circ_props();
	//////////////////////////////////////////////////
	fire.fire_props();
	toxicplat.plat_props();
	rocks.rock_props();
	lightning.lightning_props();
	raincloud.cloud_props();
	/////////////////////////////////////////////////
	bonuscoin.coin_props();
	jumpflower.flower_props();
	speedstar.star_props();
	voidessence.essence_props();
	/////////////////////////////////////////////////
	platform.plat_props();

}

void holdkey() {


	if (global.press) {
		if (global.VirtualKey == VK_A) {
			player.posx -= player.speedx;
		}
		if (global.VirtualKey == VK_D) {
			player.posx += player.speedx;
		}
		if (global.VirtualKey == VK_ESCAPE)
			exit(1);
		global.press = false;
	}
	if (global.hold) {
		if (global.VirtualKey == VK_A) {
			player.posx -= player.speedx;
		}
		if (global.VirtualKey == VK_D) {
			player.posx += player.speedx;
		}
		if (global.VirtualKey == VK_ESCAPE)
			exit(1);
		global.hold = false;

	}

}

void movement() { //put whatever object you want to move in here

	//p1.scorecount(); //counts score in top left
	platform.plat_move();//adding to position

	if (!other.squishing && !fly.start)
		player.posy += player.speedy;

	if (game.level[1])
		fire.fire_move();
	if (game.level[2])
		bonuscoin.coin_move();
	if (game.level[3])
		toxicplat.plat_move();
	if (game.level[4])
		jumpflower.flower_move();
	if (game.level[5])
		rocks.rock_move();
	if (game.level[6])
		speedstar.star_move();
	if (game.level[7])
		lightning.lightning_move();
	if (game.level[8])
		voidessence.essence_move();
	if (game.level[9])
		raincloud.cloud_move();

	platform.plat_spawn();//platform 
	circ.circ_spawn(); //ball

	if (game.level[1])
		fire.fire_spawn();
	if (game.level[2])
		bonuscoin.coin_spawn();
	if (game.level[3])
		toxicplat.plat_spawn();
	if (game.level[4])
		jumpflower.flower_spawn();
	if (game.level[5])
		rocks.rock_spawn();
	if (game.level[6])
		speedstar.star_spawn();
	if (game.level[7])
		lightning.lightning_spawn();
	if (game.level[8])
		voidessence.essence_spawn();
	if (game.level[9])
		raincloud.cloud_spawn();


	Sleep(7); //sleep

	platform.plat_destroy(); //platform
	circ.circ_destroy(); //ball

	if (game.level[1])
		fire.fire_destroy();
	if (game.level[2])
		bonuscoin.coin_destroy();
	if (game.level[3])
		toxicplat.plat_destroy();
	if (game.level[4])
		jumpflower.flower_destroy();
	if (game.level[5])
		rocks.rock_destroy();
	if (game.level[6])
		speedstar.star_destroy();
	if (game.level[8])
		voidessence.essence_destroy();
	if (game.level[9])
		raincloud.cloud_destroy();

}

void squish() { //squishing animation
	other.squishing = true;
	for (int i = 0; i < 30; i++) {
		if (i < 15) {
			player.rx++;
			player.ry--;
		}
		else {
			player.rx--;
			player.ry++;
		}
		holdkey();
		movement();

#pragma region commented
		/*platform.plat_move();
		if (i < 10) {
		player.rx++;
		player.ry--;
		}
		else {
		player.rx--;
		player.ry++;
		}
		holdkey();
		if (game.level[1])
		fire.fire_move();
		if (game.level[2])
		bonuscoin.coin_move();
		if (game.level[3])
		toxicplat.plat_move();
		if (game.level[4])
		jumpflower.flower_move();
		if (game.level[5])
		rocks.rock_move();
		if (game.level[6])
		speedstar.star_move();

		platform.plat_spawn();
		circ.circ_spawn();

		if (game.level[1])
		fire.fire_spawn();
		if (game.level[2])
		bonuscoin.coin_spawn();
		if (game.level[3])
		toxicplat.plat_spawn();
		if (game.level[4])
		jumpflower.flower_spawn();
		if (game.level[5])
		rocks.rock_spawn();
		if (game.level[6])
		speedstar.star_spawn();

		Sleep(8);


		platform.plat_destroy();
		circ.circ_destroy();

		if (game.level[1])
		fire.fire_destroy();
		if (game.level[2])
		bonuscoin.coin_destroy();
		if (game.level[3])
		toxicplat.plat_destroy();
		if (game.level[4])
		jumpflower.flower_destroy();
		if (game.level[5])
		rocks.rock_destroy();
		if (game.level[6])
		speedstar.star_destroy();
		}*/
#pragma endregion

	}
	other.squishing = false;
}

void collisionCheck() {
	if (game.level[1])
		fire.kill_check();
	if (game.level[2])
		bonuscoin.collect_check();
	if (!player.movingup) {
		if (game.level[3])
			toxicplat.toxic_check();
	}
	if (game.level[4])
		jumpflower.flower_check();
	if (game.level[5])
		rocks.rock_kill();
	if (game.level[6])
		speedstar.star_check();
	if (game.level[7])
		lightning.lightning_check();
	if (game.level[8]) {
		voidessence.essence_check();
		voidessence.essence_power();
	}
	if (game.level[9])
		raincloud.cloud_check();
}

void downcurve() {
	// i made this a function because i needed to break the loop easily without using break
	// basically just the speed curve for falling
	for (int q = 0; q < 4; q++) {
		for (int j = 0; j <= player.acc[player.y]; j += player.speedy) { //speed curve

			collisionCheck();

			if (player.hit)
				return;


			for (int i = 0; i < amount.amt; i++) {
				if (player.posy + player.radius >= plat[i].top && player.posx + (player.radius / 2) > plat[i].left  && player.posx - (player.radius / 2) < plat[i].right && player.posy + player.radius <= plat[i].bottom) { //c with platform
					player.hit = true;
					return;
				}
			}
			if (player.posy + player.radius >= getmaxy()) { //c with floor
				player.deathtype = "Killed by void";
				player.hit = true;
				player.dead = true;
				return;
			}

			holdkey();
			movement();


		} //for array data
		player.y--; //array curve --
		player.speedy++;


	} // for array element

}

void play() { //circle movement
	while (!player.dead) {

#pragma region up
		if (player.movingup) { //while the player is rising
			player.speedy = -5; //up at fastest
			player.y = 4; //speed curve
			for (int i = 0; i < 4; i++) {
				for (int i = 0; i <= player.acc[player.y] * player.heightx; i -= player.speedy) {

					holdkey();
					movement();// movement

					collisionCheck();

					if (player.dead)
						return;

				}
				player.y--;
				player.speedy++;
			}
			player.movingup = false;
		}
#pragma endregion ball moving up

		if (!player.movingup) { //if the player is falling
			player.speedy = 1; //set speed to lowest
			player.y = 4; //speed curve array 


			downcurve(); //falling down but with a speed increase curve


			while (!player.hit) { //runs while not colliding

				collisionCheck();


				for (int i = 0; i < amount.amt; i++) {
					if (player.posy + player.radius >= plat[i].top && player.posx + (player.radius / 2) > plat[i].left  && player.posx - (player.radius / 2) < plat[i].right && player.posy + player.radius <= plat[i].bottom)//c with platform
						player.hit = true;
				}
				if (player.posy + player.radius >= getmaxy()) { //c with floor
					player.deathtype = "Killed by void";
					player.hit = true;
					player.dead = true;
				}

				holdkey();
				movement();

			} //while !player.hit

			player.movingup = true;
			player.hit = false;


			if (!player.dead)
				squish();

		} //if !movingup

		platform.speed_check();
		game.level_check();
		p1.scorecount();

	}//while
	p1.totalscore += p1.score;
}

#pragma endregion the actual gameplay of the game lol

void countdown() {
	string die = "nothing", three = "THREE", two = "TWO", one = "ONE", go = "GO !";
	settextstyle(6, 0, 5);

	setcolor(4);
	outtextxy(CENTERX - 100, CENTERY, three.c_str());
	outtextxy(CENTERX - 100, CENTERY, three.c_str()); //three
	Sleep(250);
	cleardevice();

	setcolor(14);
	outtextxy(CENTERX - 100, CENTERY, two.c_str()); //two
	Sleep(250);
	cleardevice();

	setcolor(2);
	outtextxy(CENTERX - 100, CENTERY, one.c_str()); //one
	Sleep(250);
	cleardevice();

	setcolor(1);
	outtextxy(CENTERX - 100, CENTERY, go.c_str()); //go
	Sleep(250);
	cleardevice();

	settextstyle(0, 0, 1);
	setcolor(15);
}

void gameover() {
	settextstyle(6, 0, 2);
	string deathtext[13] = { "G", "A", "M", "E", " ", "O", "V", "E", "R", ".", ".", ".", "." };
	string scoredisp = "Score: " + p1.dispscore;
	string tscoredisp = "Total: " + to_string(p1.totalscore);

	int textpos = (getmaxx() / 3) - 50;

	setcolor(15);
	outtextxy(CENTERX - 250, CENTERY + 250, player.deathtype.c_str());

	outtextxy(CENTERX - 250, CENTERY + 350, scoredisp.c_str());
	outtextxy(CENTERX - 250, CENTERY + 450, tscoredisp.c_str());

	for (int i = 0; i < 13; i++) {
		setcolor(4);

		outtextxy(textpos, CENTERY, deathtext[i].c_str());
		textpos += 55;
		Sleep(100);

	}
	Sleep(1000);


}

#pragma region color select

struct select {
	bool selecting = true;
	int slot;
	int shopslot;
	int menuslot;
	string strung;

	string upgradenames[3];

}selecter;

void holdkeygame() {

	if (global.press) {
		if (global.VirtualKey == VK_A) {
			selecter.slot -= 1;
		}
		if (global.VirtualKey == VK_D) {
			selecter.slot += 1;
		}
		if (global.VirtualKey == VK_S) {
			selecter.slot += 3;
		}
		if (global.VirtualKey == VK_W) {
			selecter.slot -= 3;
		}

		if (global.VirtualKey == VK_E) {

			switch (selecter.slot) {
			case 1:
				player.color = 15;
				selecter.selecting = false;
				break;
			case 2:
				player.color = 14;
				selecter.selecting = false;
				break;
			case 3:
				player.color = 5;
				selecter.selecting = false;
				break;
			case 4:
				player.color = 10;
				selecter.selecting = false;
				break;
			case 5:
				player.color = RED;
				selecter.selecting = false;
				break;
			case 6:
				player.color = 1;
				selecter.selecting = false;
				break;

			}
		}

		global.press = false;
	}
}

void colorselect() {
	selecter.slot = 1;
	selecter.strung = "Select a Color:";
	string sel = "Press 'E' to Select";
	settextstyle(2, 0, 5);
	setcolor(15);
	outtextxy(10, getmaxy() - 20, sel.c_str());
	while (selecter.selecting) {
		settextstyle(6, 0, 3);
		setcolor(15);
		outtextxy((CENTERX)-100, 50, selecter.strung.c_str());
		setcolor(15); //white
		fillellipse(getmaxx() / 4, getmaxy() / 3, 55, 55);
		setcolor(14); //yellow
		fillellipse((getmaxx() / 4) * 2, getmaxy() / 3, 55, 55);
		setcolor(5); //magenta
		fillellipse((getmaxx() / 4) * 3, getmaxy() / 3, 55, 55);
		setcolor(10); //light green
		fillellipse(getmaxx() / 4, (getmaxy() / 3) * 2, 55, 55);
		setcolor(RED); //red
		fillellipse((getmaxx() / 4) * 2, (getmaxy() / 3) * 2, 55, 55);
		setcolor(1); //blue
		fillellipse((getmaxx() / 4) * 3, (getmaxy() / 3) * 2, 55, 55);


		//rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);  
		//rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
		//rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

		//rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
		//rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
		//rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);

		holdkeygame();

		switch (selecter.slot) { //if not a slot
		case -2:
			selecter.slot = 4;
			break;
		case -1:
			selecter.slot = 5;
			break;
		case 0:
			selecter.slot = 6;
			break;
		case 7:
			selecter.slot = 1;
			break;
		case 8:
			selecter.slot = 2;
			break;
		case 9:
			selecter.slot = 3;
			break;
		}
#pragma region mess
		switch (selecter.slot) {
		case 1:
			setcolor(15);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			setcolor(0);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		case 2:
			setcolor(15);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		case 3:
			setcolor(15);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);

			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		case 4:
			setcolor(15);
			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		case 5:
			setcolor(15);
			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		case 6:
			setcolor(15);
			rectangle(((getmaxx() / 4) * 3) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 3) + 55, ((getmaxy() / 3) * 2) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 3) - 55, (getmaxx() / 4) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 2) + 55, (getmaxy() / 3) + 55);
			rectangle(((getmaxx() / 4) * 3) - 55, (getmaxy() / 3) - 55, ((getmaxx() / 4) * 3) + 55, (getmaxy() / 3) + 55);

			rectangle((getmaxx() / 4) - 55, ((getmaxy() / 3) * 2) - 55, (getmaxx() / 4) + 55, ((getmaxy() / 3) * 2) + 55);
			rectangle(((getmaxx() / 4) * 2) - 55, ((getmaxy() / 3) * 2) - 55, ((getmaxx() / 4) * 2) + 55, ((getmaxy() / 3) * 2) + 55);
			break;
		}
#pragma endregion
	}
	cleardevice();
	selecter.selecting = true;
}

#pragma endregion selecting color

#pragma region shop
//a lot of this stuff i could have done more efficently but ehhhh

void holdkeyshop() {
	string scoredisp;
	if (global.press) {
		string error;
		if (global.VirtualKey == VK_A) {
			selecter.shopslot--;
		}
		if (global.VirtualKey == VK_D) {
			selecter.shopslot++;
		}
		if (global.VirtualKey == VK_E) {
			switch (selecter.shopslot) {
			case 1:

				if (p1.totalscore >= 50000 && player.upgrade.speedxlevel[0]) {
					p1.totalscore -= 50000;
					player.upgrade.speedxlevel[0] = false;
					player.upgrade.speedxlevel[1] = true;
					break;
				}
				else if (p1.totalscore >= 250000 && player.upgrade.speedxlevel[1]) {
					p1.totalscore -= 250000;
					player.upgrade.speedxlevel[1] = false;
					player.upgrade.speedxlevel[2] = true;
					break;
				}
				else if (p1.totalscore >= 500000 && player.upgrade.speedxlevel[2]) {
					p1.totalscore -= 500000;
					player.upgrade.speedxlevel[2] = false;
					player.upgrade.speedxlevel[3] = true;
					break;
				}
				else if (player.upgrade.speedxlevel[3]) {
					error = "Sold out !";
					setcolor(MAGENTA);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}
				else {
					error = "Not enough points !";
					setcolor(GREEN);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}


				break;
			case 2:

				if (p1.totalscore >= 50000 && player.upgrade.sizelevel[0]) {
					p1.totalscore -= 50000;
					player.upgrade.sizelevel[0] = false;
					player.upgrade.sizelevel[1] = true;
					break;
				}
				else if (p1.totalscore >= 250000 && player.upgrade.sizelevel[1]) {
					p1.totalscore -= 250000;
					player.upgrade.sizelevel[1] = false;
					player.upgrade.sizelevel[2] = true;
					break;
				}
				else if (p1.totalscore >= 500000 && player.upgrade.sizelevel[2]) {
					p1.totalscore -= 500000;
					player.upgrade.sizelevel[2] = false;
					player.upgrade.sizelevel[3] = true;
					break;
				}
				else if (player.upgrade.sizelevel[3]) {
					error = "Sold out !";
					setcolor(MAGENTA);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}
				else {
					error = "Not enough points !";
					setcolor(GREEN);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}


				break;
			case 3:

				if (p1.totalscore >= 50000 && player.upgrade.heightxlevel[0]) {
					p1.totalscore -= 50000;
					player.upgrade.heightxlevel[0] = false;
					player.upgrade.heightxlevel[1] = true;
					break;
				}
				else if (p1.totalscore >= 250000 && player.upgrade.heightxlevel[1]) {
					p1.totalscore -= 250000;
					player.upgrade.heightxlevel[1] = false;
					player.upgrade.heightxlevel[2] = true;
					break;
				}
				else if (p1.totalscore >= 500000 && player.upgrade.heightxlevel[2]) {
					p1.totalscore -= 500000;
					player.upgrade.heightxlevel[2] = false;
					player.upgrade.heightxlevel[3] = true;
					break;
				}
				else if (player.upgrade.heightxlevel[3]) {
					error = "Sold out !";
					setcolor(MAGENTA);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}
				else {
					error = "Not enough points !";
					setcolor(GREEN);
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
					Sleep(500);
					error = "                                                   ";
					outtextxy(CENTERX, CENTERY + 200, error.c_str());
				}


				break;
			}
		}
		if (global.VirtualKey == VK_ESCAPE) {
			selecter.selecting = false;
		}

		global.press = false;
	}
	scoredisp = "Points:" + to_string(p1.totalscore);
	setcolor(GREEN);
	outtextxy(10, 10, scoredisp.c_str());
}

void shop() {
	selecter.strung = "Shop";
	selecter.upgradenames[0] = "Speed Increase";
	selecter.upgradenames[1] = "Size Decrease";
	selecter.upgradenames[2] = "Jump Height Increase";
	string exit = "Press ESC to Exit";
	string sel = "Press 'E' to buy";

	settextstyle(2, 0, 5);
	setcolor(15);
	outtextxy((CENTERX), 50, selecter.strung.c_str());

	outtextxy((getmaxx() / 4) - 60, CENTERY + 120, selecter.upgradenames[0].c_str());
	outtextxy((getmaxx() / 4 * 2) - 45, CENTERY + 80, selecter.upgradenames[1].c_str());
	outtextxy((getmaxx() / 4 * 3) - 75, CENTERY + 80, selecter.upgradenames[2].c_str());

	outtextxy(10, getmaxy() - 20, exit.c_str());
	outtextxy(10, getmaxy() - 40, sel.c_str());

	selecter.shopslot = 1;

	string scoredisp = "Points:" + to_string(p1.totalscore);
	setcolor(GREEN);
	outtextxy(10, 10, scoredisp.c_str());
	while (selecter.selecting) {

#pragma region costs
		string speedcost, sizecost, heightcost, speedlvl, sizelvl, heightlvl;

		if (player.upgrade.speedxlevel[0]) {
			speedcost = "Cost: 50000";
			speedlvl = "Lvl 1 -> Lvl 2";
		}
		if (player.upgrade.speedxlevel[1]) {
			speedcost = "Cost: 250000";
			speedlvl = "Lvl 2 -> Lvl 3";
		}
		if (player.upgrade.speedxlevel[2]) {
			speedcost = "Cost: 500000";
			speedlvl = "Lvl 3 -> Lvl MAX";
		}
		if (player.upgrade.speedxlevel[3]) {
			speedcost = "Sold out!";
			speedlvl = "Level MAX";
		}

		///

		if (player.upgrade.sizelevel[0]) {
			sizecost = "Cost: 50000";
			sizelvl = "Lvl 1 -> Lvl 2";
		}
		if (player.upgrade.sizelevel[1]) {
			sizecost = "Cost: 250000";
			sizelvl = "Lvl 2 -> Lvl 3";
		}
		if (player.upgrade.sizelevel[2]) {
			sizecost = "Cost: 500000";
			sizelvl = "Lvl 3 -> Lvl MAX";
		}
		if (player.upgrade.sizelevel[3]) {
			sizecost = "Sold out!";
			sizelvl = "Level MAX";
		}

		///

		if (player.upgrade.heightxlevel[0]) {
			heightcost = "Cost: 50000";
			heightlvl = "Lvl 1 -> Lvl 2";
		}
		if (player.upgrade.heightxlevel[1]) {
			heightcost = "Cost: 250000";
			heightlvl = "Lvl 2 -> Lvl 3";
		}
		if (player.upgrade.heightxlevel[2]) {
			heightcost = "Cost: 500000";
			heightlvl = "Lvl 3 -> Lvl MAX";
		}
		if (player.upgrade.heightxlevel[3]) {
			heightcost = "Sold out!";
			heightlvl = "Level MAX";
		}

		setcolor(15);
		outtextxy((getmaxx() / 4) - 60, CENTERY + 140, speedcost.c_str());
		outtextxy((getmaxx() / 4 * 2) - 45, CENTERY + 100, sizecost.c_str());
		outtextxy((getmaxx() / 4 * 3) - 75, CENTERY + 100, heightcost.c_str());

		outtextxy((getmaxx() / 4) - 60, CENTERY + 160, speedlvl.c_str());
		outtextxy((getmaxx() / 4 * 2) - 45, CENTERY + 120, sizelvl.c_str());
		outtextxy((getmaxx() / 4 * 3) - 75, CENTERY + 120, heightlvl.c_str());

#pragma endregion

		setcolor(MAGENTA);
		fillellipse(getmaxx() / 4, getmaxy() / 2, 55, 55); //speed
		setcolor(YELLOW);
		line((getmaxx() / 4) - 55, (getmaxy() / 2) + 85, (getmaxx() / 4) + 55, (getmaxy() / 2) + 85); //arrow
		line((getmaxx() / 4) - 55, (getmaxy() / 2) + 85, (getmaxx() / 4) - 35, (getmaxy() / 2) + 65); line((getmaxx() / 4) - 55, (getmaxy() / 2) + 85, (getmaxx() / 4) - 35, (getmaxy() / 2) + 105);
		line((getmaxx() / 4) + 55, (getmaxy() / 2) + 85, (getmaxx() / 4) + 35, (getmaxy() / 2) + 65); line((getmaxx() / 4) + 55, (getmaxy() / 2) + 85, (getmaxx() / 4) + 35, (getmaxy() / 2) + 105);

		setcolor(MAGENTA);
		fillellipse((getmaxx() / 4) * 2, getmaxy() / 2, 55, 55); //size
		setcolor(YELLOW);
		line(((getmaxx() / 4) * 2) + 85, (getmaxy() / 2) + 55, ((getmaxx() / 4) * 2) + 85, (getmaxy() / 2) - 55);  //arrow
		line(((getmaxx() / 4) * 2) + 85, (getmaxy() / 2) + 55, ((getmaxx() / 4) * 2) + 65, (getmaxy() / 2) + 35); line(((getmaxx() / 4) * 2) + 85, (getmaxy() / 2) + 55, ((getmaxx() / 4) * 2) + 105, (getmaxy() / 2) + 35);

		setcolor(MAGENTA);
		fillellipse((getmaxx() / 4) * 3, getmaxy() / 2, 55, 55); //jump height
		setcolor(YELLOW);
		line(((getmaxx() / 4) * 3) + 85, (getmaxy() / 2) + 55, ((getmaxx() / 4) * 3) + 85, (getmaxy() / 2) - 55); //arrow
		line(((getmaxx() / 4) * 3) + 85, (getmaxy() / 2) - 55, ((getmaxx() / 4) * 3) + 65, (getmaxy() / 2) - 35); line(((getmaxx() / 4) * 3) + 85, (getmaxy() / 2) - 55, ((getmaxx() / 4) * 3) + 105, (getmaxy() / 2) - 35);

		holdkeyshop();

		if (selecter.shopslot < 1)
			selecter.shopslot = 3;
		if (selecter.shopslot > 3)
			selecter.shopslot = 1;

		//rectangle((getmaxx() / 4) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4) + 55, (getmaxy() / 2) + 105); 
		//rectangle((getmaxx() / 4 * 2) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 2) + 105, (getmaxy() / 2) + 55);
		//rectangle((getmaxx() / 4 * 3) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 3) + 105, (getmaxy() / 2) + 55);

		switch (selecter.shopslot) {
		case 1:
			setcolor(15);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4) + 55, (getmaxy() / 2) + 105);
			setcolor(0);
			rectangle((getmaxx() / 4 * 2) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 2) + 105, (getmaxy() / 2) + 55);
			rectangle((getmaxx() / 4 * 3) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 3) + 105, (getmaxy() / 2) + 55);
			break;
		case 2:
			setcolor(15);
			rectangle((getmaxx() / 4 * 2) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 2) + 105, (getmaxy() / 2) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4) + 55, (getmaxy() / 2) + 105);
			rectangle((getmaxx() / 4 * 3) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 3) + 105, (getmaxy() / 2) + 55);
			break;
		case 3:
			setcolor(15);
			rectangle((getmaxx() / 4 * 3) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 3) + 105, (getmaxy() / 2) + 55);
			setcolor(0);
			rectangle((getmaxx() / 4 * 2) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4 * 2) + 105, (getmaxy() / 2) + 55);
			rectangle((getmaxx() / 4) - 55, (getmaxy() / 2) - 55, (getmaxx() / 4) + 55, (getmaxy() / 2) + 105);
			break;

		}

	}
	cleardevice();
	selecter.selecting = true;
}

#pragma endregion shop

#pragma region info

struct infosection {
	void displaytext(string title, string line1, string line2, string line3, int xoffset) {
		settextstyle(6, 0, 7);
		outtextxy(CENTERX - xoffset, 50, title.c_str());

		settextstyle(6, 0, 3);
		outtextxy(100, (getmaxy() / 4) * 3, line1.c_str());
		outtextxy(100, ((getmaxy() / 4) * 3) + 30, line2.c_str());
		outtextxy(100, ((getmaxy() / 4) * 3) + 60, line3.c_str());

		string left = "'A' <- Previous Page", right = "'D' -> Next Page";
		outtextxy(5, getmaxy() - 25, left.c_str()); outtextxy(getmaxx() - 250, getmaxy() - 25, right.c_str()); outtextxy(getmaxx() - 250, getmaxy() - 25, right.c_str());
	}
	void gameinfo() {
		string title = "Ball Runner";
		string l1 = "This is you. You can move using the 'A' and 'D' keys.";
		string l2 = "The goal of the game is to survive as long as possible whithout getting hit or touching the floor.";
		string l3 = "Over time, the game gets progressively harder but in turn will yield higher rewards.";


		setcolor(MAGENTA);
		fillellipse(CENTERX, CENTERY, 90, 90);
		setcolor(MAGENTA);
		fillellipse(CENTERX, CENTERY, 90, 90);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 100);

	}
	void fireinfo() {
		string title = "Fireball";
		string l1 = "Ah, the infamous fireball. One of the simplest but most deadly enemies.";
		string l2 = "Small in size, it can sneak upon an unsuspecting player and kill them in an instant.";
		string l3 = "Appears at level 2.";

		setcolor(RED);
		fillellipse(CENTERX, CENTERY, 20, 20);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 100);

	}
	void coininfo() {
		string title = "Coin";
		string l1 = "Woah! Free money?! Where is it coming from?";
		string l2 = "Snatch this little sucker and you'll get an extra 5,000 bonus points.";
		string l3 = "Appears at level 3.";


		setcolor(YELLOW);
		fillellipse(CENTERX, CENTERY, 20, 20);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 100);

	}
	void toxinfo() {
		string title = "Toxic Platform";
		string l1 = "Something seems... Odd about this platform. It's slower, but doesn't exactly seem safe.";
		string l2 = "Acts like a really slow platform, but flips your controls for 3.5 seconds.";
		string l3 = "Appears at level 4.";


		setcolor(GREEN);
		bar(CENTERX - 100, CENTERY - 25, CENTERX + 100, CENTERY);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 250);
	}
	void flowerinfo() {
		string title = "Flower";
		string l1 = "This flower will make you high. High in the sense that you'll be bouncing higher in the air.";
		string l2 = "Consume one of these and you'll gain a .75 bonus to your height multiplier for 5 seconds.";
		string l3 = "Appears at level 5.";


		setcolor(YELLOW);
		fillellipse(CENTERX, CENTERY, 21, 21);
		setcolor(BLUE);
		fillellipse(CENTERX + 27, CENTERY, 15, 15);
		fillellipse(CENTERX - 27, CENTERY, 15, 15);
		fillellipse(CENTERX, CENTERY + 27, 15, 15);
		fillellipse(CENTERX, CENTERY - 27, 15, 15);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 100);

	}
	void rockinfo() {
		string title = "Rock";
		string l1 = "The minerals in these rocks could be very valuble. It's too bad theyll knock you out if they hit you.";
		string l2 = "Slightly larger and faster than the fireball, and falls from the sky.";
		string l3 = "Appears at level 6.";


		setcolor(15);
		fillellipse(CENTERX, CENTERY, 40, 40);
		page.displaytext(title, l1, l2, l3, 100);

	}
	void starinfo() {
		string title = "Falling Star";
		string l1 = "The sky is falling! Make a wish!";
		string l2 = "Catch one of these hard to get stars to gain +5 speed for 5 seconds.";
		string l3 = "Appears at level 7.";


		setcolor(14);
		fillellipse(CENTERX, CENTERY, 21, 21);
		line(CENTERX, CENTERY, CENTERX - 36, CENTERY - 36);
		line(CENTERX, CENTERY, CENTERX + 36, CENTERY - 36);
		line(CENTERX, CENTERY, CENTERX - 36, CENTERY + 36);
		line(CENTERX, CENTERY, CENTERX + 36, CENTERY + 36);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 250);
	}
	void lightinfo() {
		string title = "Lightning";
		string l1 = "They say lightning can't strike the same place twice. Maybe you'll be (un)lucky.";
		string l2 = "If you see a red '!', you might want to move out of the way.";
		string l3 = "Appears at level 8.";


		setcolor(RED);
		bar(CENTERX + 68, CENTERY + 15, CENTERX + 77, CENTERY + 60);
		bar(CENTERX + 68, CENTERY + 66, CENTERX + 77, CENTERY + 75);
		setcolor(14);
		bar(CENTERX - 20, CENTERY - 300, CENTERX, CENTERY + 150);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 200);
	}
	void voidinfo() {
		string title = "Void Essence";
		string l1 = "With this dark magical essence, you'll be able to harness the power of the void.";
		string l2 = "Absorb one of these spirits and gain complete flight for 5 seconds.";
		string l3 = "Appears at level 9.";


		setcolor(MAGENTA);
		fillellipse(CENTERX, CENTERY, 30, 30);
		setcolor(BLACK);
		fillellipse(CENTERX, CENTERY, 15, 15);
		setcolor(BLUE);
		circle(CENTERX, CENTERY, 33);
		arc(CENTERX, CENTERY, 115, 165, 60); //top left
		arc(CENTERX, CENTERY, 15, 435, 60); //top right
		arc(CENTERX, CENTERY, 195, 255, 60); //bottom left
		arc(CENTERX, CENTERY, 285, 345, 60); //bottom right
		setcolor(MAGENTA);
		page.displaytext(title, l1, l2, l3, 300);



	}
	void raininfo() {
		string title = "Acid Rain";
		string l1 = "Due to external influences on the environment, the water has become polluted and the sky now rains acid.";
		string l2 = "Don't even try to get past this. Stay behind or ahead of it when it spawns if you want to live.";
		string l3 = "Appears at level 10.";


		cloud.radius = 25;
		cloud.posx = CENTERX;
		cloud.posy = getmaxy() / 3;

		setcolor(MAGENTA);
		fillellipse(cloud.posx, cloud.posy, cloud.radius, cloud.radius);
		setcolor(MAGENTA);
		fillellipse(cloud.posx, cloud.posy, cloud.radius, cloud.radius);
		fillellipse(cloud.posx - 23, cloud.posy - 10, cloud.radius, cloud.radius);
		fillellipse(cloud.posx + 23, cloud.posy + 10, cloud.radius, cloud.radius);
		fillellipse(cloud.posx + 12, cloud.posy - 15, cloud.radius, cloud.radius);
		fillellipse(cloud.posx - 23, cloud.posy + 10, cloud.radius, cloud.radius);

		setcolor(GREEN);
		for (int i = 0; i < 10; i++)
			bar(cloud.posx - 20, getmaxy() / 3 + 62 + i * 100, cloud.posx - 15, getmaxy() / 3 + 87 + i * 100);
		for (int i = 0; i < 10; i++)
			bar(cloud.posx + 10, getmaxy() / 3 + 120 + i * 100, cloud.posx + 15, getmaxy() / 3 + 145 + i * 100);
		for (int i = 0; i < 10; i++)
			bar(cloud.posx + 30, getmaxy() / 3 + 153 + i * 100, cloud.posx + 35, getmaxy() / 3 + 178 + i * 100);
		setcolor(15);
		page.displaytext(title, l1, l2, l3, 200);
	}

	int infoslot;
}page;

void holdkeyinfo() {
	if (global.press) {
		if (global.VirtualKey == VK_A) {
			page.infoslot--;
			cleardevice();
			if (page.infoslot > 10)
				page.infoslot = 1;
			if (page.infoslot < 1)
				page.infoslot = 10;
			switch (page.infoslot) {
			case 1:
				page.gameinfo();
				break;
			case 2:
				page.fireinfo();
				break;
			case 3:
				page.coininfo();
				break;
			case 4:
				page.toxinfo();
				break;
			case 5:
				page.flowerinfo();
				break;
			case 6:
				page.rockinfo();
				break;
			case 7:
				page.starinfo();
				break;
			case 8:
				page.lightinfo();
				break;
			case 9:
				page.voidinfo();
				break;
			case 10:
				page.raininfo();
				break;
			}
		}
		if (global.VirtualKey == VK_D) {
			page.infoslot++;
			cleardevice();
			if (page.infoslot > 10)
				page.infoslot = 1;
			if (page.infoslot < 1)
				page.infoslot = 10;
			switch (page.infoslot) {
			case 1:
				page.gameinfo();
				break;
			case 2:
				page.fireinfo();
				break;
			case 3:
				page.coininfo();
				break;
			case 4:
				page.toxinfo();
				break;
			case 5:
				page.flowerinfo();
				break;
			case 6:
				page.rockinfo();
				break;
			case 7:
				page.starinfo();
				break;
			case 8:
				page.lightinfo();
				break;
			case 9:
				page.voidinfo();
				break;
			case 10:
				page.raininfo();
				break;
			}
		}
		if (global.VirtualKey == VK_ESCAPE)
			selecter.selecting = false;

		global.press = false;
	}
}

void infopages() {
	selecter.selecting = true;
	page.infoslot = 1;
	page.gameinfo();
	while (selecter.selecting) {
		holdkeyinfo();
	}
	selecter.selecting = true;
}

#pragma endregion info functions

#pragma region main menu
void masterkeylistener() {
	while (true) {
		if (KEYBOARD(VK_A))
			cout << "";
		if (KEYBOARD(VK_D))
			cout << "";
		if (KEYBOARD(VK_W))
			cout << "";
		if (KEYBOARD(VK_S))
			cout << "";
		if (KEYBOARD(VK_E))
			cout << "";
		if (KEYBOARD(VK_M))
			cout << "";
		if (KEYBOARD(VK_I))
			cout << "";
		if (KEYBOARD(VK_RETURN))
			cout << "";
		if (KEYBOARD(VK_ESCAPE))
			cout << "";
	}
}

void holdkeymenu() {
	if (global.press) {
		if (global.VirtualKey == VK_A) {
			selecter.menuslot--;
		}
		if (global.VirtualKey == VK_D) {
			selecter.menuslot++;
		}
		if (global.VirtualKey == VK_M) {
			reset();
		}
		if (global.VirtualKey == VK_I) {
			demo();
		}
		if (global.VirtualKey == VK_ESCAPE) {
			exit(1);
		}
		if (global.VirtualKey == VK_RETURN) {
			switch (selecter.menuslot) {
			case 1:
				shopthread();
				break;
			case 2:
				gamethread();
				break;
			case 3:
				infothread();
				break;

			}
		}
		global.press = false;
	}
}

void mainmenu() {
	bool up = true, right = true;
	int speedx, speedy, posx = CENTERX, posy = CENTERY;
	string title = "circle quest";
	string shop = "shop", play = "play", info = "info";
	string sel = "Press Enter to Select";
	string res = "Press 'M' to Reset Game";


	while (true) {
		settextstyle(2, 0, 5);
		outtextxy(10, getmaxy() - 20, sel.c_str());
		outtextxy(10, getmaxy() - 40, res.c_str());

		int color1 = (rand() % 14 + 1);
		int color2 = (rand() % 14 + 1);
		int color3 = (rand() % 14 + 1);

		for (int i = 0; i < 100; i++) {
			setcolor(color1);
			settextstyle(6, 0, 15);
			outtextxy(CENTERX - 450, CENTERY / 4, title.c_str());

			settextstyle(6, 0, 6);
			setcolor(color2);
			outtextxy((getmaxx() / 4), (getmaxy() / 4) * 3, shop.c_str());
			outtextxy((getmaxx() / 4) * 2, (getmaxy() / 4) * 3, play.c_str());
			outtextxy((getmaxx() / 4) * 3, (getmaxy() / 4) * 3, info.c_str());
			Sleep(1);


			holdkeymenu();
			if (selecter.menuslot < 1)
				selecter.menuslot = 3;
			if (selecter.menuslot > 3)
				selecter.menuslot = 1;


			switch (selecter.menuslot) {
			case 1:
				setcolor(15);
				rectangle((getmaxx() / 4) - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) + 150, (getmaxy() / 4) * 3 + 63);
				setcolor(0);
				rectangle((getmaxx() / 4) * 2 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 2 + 130, (getmaxy() / 4) * 3 + 63);
				rectangle((getmaxx() / 4) * 3 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 3 + 125, (getmaxy() / 4) * 3 + 63);
				break;
			case 2:
				setcolor(15);
				rectangle((getmaxx() / 4) * 2 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 2 + 130, (getmaxy() / 4) * 3 + 63);
				setcolor(0);
				rectangle((getmaxx() / 4) - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) + 150, (getmaxy() / 4) * 3 + 63);
				rectangle((getmaxx() / 4) * 3 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 3 + 125, (getmaxy() / 4) * 3 + 63);
				break;
			case 3:
				setcolor(15);
				rectangle((getmaxx() / 4) * 3 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 3 + 125, (getmaxy() / 4) * 3 + 63);
				setcolor(0);
				rectangle((getmaxx() / 4) - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) + 150, (getmaxy() / 4) * 3 + 63);
				rectangle((getmaxx() / 4) * 2 - 1, (getmaxy() / 4) * 3 - 1, (getmaxx() / 4) * 2 + 130, (getmaxy() / 4) * 3 + 63);
				break;
			}

			if (up)
				speedy = -4;
			else
				speedy = 4;
			if (right)
				speedx = 4;
			else
				speedx = -4;

			//setcolor(0);
			//fillellipse(posx, posy, 30, 30);
			posx += speedx; posy += speedy;

			setcolor(color3);
			fillellipse(posx, posy, 30, 30);



			if (posx >= getmaxx())
				right = false;
			if (posx <= 0)
				right = true;

			if (posy >= (getmaxy() / 4) * 3 - 35)
				up = true;
			if (posy <= getmaxy() / 4 + 50)
				up = false;

		}

	}
}
#pragma endregion main menu functions

#pragma region saves

void save() {

	string filename = "save.txt";
	ofstream fout;

	fout.open(filename.c_str());

	if (fout.is_open()) {
		for (int i = 0; i < 4; i++)
			fout << player.upgrade.sizelevel[i] << endl;
		for (int i = 0; i < 4; i++)
			fout << player.upgrade.speedxlevel[i] << endl;
		for (int i = 0; i < 4; i++)
			fout << player.upgrade.heightxlevel[i] << endl;
		fout << p1.totalscore << endl;
	}
	else
		cout << "oh noooooo";

	fout.close();

}

void load() {
	ifstream fin;

	fin.open("save.txt");

	if (fin.is_open()) {
		for (int i = 0; i < 4; i++)
			fin >> player.upgrade.sizelevel[i];
		for (int i = 0; i < 4; i++)
			fin >> player.upgrade.speedxlevel[i];
		for (int i = 0; i < 4; i++)
			fin >> player.upgrade.heightxlevel[i];
		fin >> p1.totalscore;


	}

	else
		cout << "uh oh";

	fin.close();

	//for (int i = 0; i < 4; i++)
	//	cout << player.upgrade.sizelevel[i] << endl;
	//for (int i = 0; i < 4; i++)
	//	cout << player.upgrade.speedxlevel[i] << endl;
	//for (int i = 0; i < 4; i++)
	//	cout << player.upgrade.heightxlevel[i] << endl;
	//cout << p1.totalscore << endl;



	//
}

void reset() {

	string filename = "save.txt";
	ofstream fout;

	fout.open(filename.c_str());

	if (fout.is_open()) {
		cout << "reset successfully" << endl;
		for (int i = 0; i < 3; i++) {
			fout << 1 << endl;
			for (int i = 0; i < 3; i++) {
				fout << 0 << endl;
			}
		}
		fout << 0 << endl;
	}
	else
		cout << "oh noooooo";

	fout.close();
}

#pragma endregion save file functions

#pragma region threads

void gamethread() {
	load();

	cleardevice();


	thread selectc(colorselect);


	selectc.join();

	countdown();

	genprops();

	for (int i = 0; i < 10; i++)
		game.level[i] = false;

	if (other.demoa) {
		for (int i = 0; i < 10; i++)
			game.level[i] = true;
		p1.totalscore = 2400000;
	}

	other.runtime = clock();
	p1.bonus = 0;

	thread game(play);

	global.press = global.hold = false;
	game.join();


	gameover();

	save();

	cleardevice();
}

void shopthread() {

	cleardevice();

	load();

	thread shoop(shop);

	shoop.join();


	save();

	cleardevice();

}

void infothread() {

	cleardevice();

	thread infodisp(infopages);

	infodisp.join();

	cleardevice();
}

#pragma endregion game threads

void demo() {
	if (other.demoa) {
		p1.totalscore = 0;
		other.demoa = false;
	}
	else if (!other.demoa) {
		other.demoa = true;
	}
}

int main() {
	gr_start(GrDriver, GrMode, ErrorCode);
	srand(time(NULL));

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);


	thread menu(mainmenu);
	thread menuinput(masterkeylistener);

	menu.join();
	menuinput.join();

	//while (true) {


	//}

	return 0;
}