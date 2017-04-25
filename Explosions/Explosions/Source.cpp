#include <winbgi.cpp>
#include <graphics.h>
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>

using namespace std;
int screenX, screenY, halfX, halfY;
bool isCircle(int Xc, int Yc, int R, int Ex, int Ey) {
	int dx2 = Ex - Xc;
	int dy2 = Ey - Yc;
	dx2 *= dx2;
	dy2 *= dy2;
	R *= R;
	return R >= (dx2 + dy2);
}
string COLORVAL(int c) {
	switch (c) {
	case 1:
		return "BLUE";
	case 2:
		return "GREEN";
	case 3:
		return "CYAN";
	case 4:
		return "RED";
	case 5:
		return "MAGENTA";
	case 6:
		return "BROWN";
	case 7:
		return "LIGHTGRAY";
	case 8:
		return "DARKGRAY";
	case 9:
		return "LIGHTBLUE";
	case 10:
		return "LIGHTGREEN";
	case 11:
		return "LIGHTCYAN";
	case 12:
		return "LIGHTRED";
	case 13:
		return "LIGHTMAGENTA";
	case 14:
		return "YELLOW";
	case 15:
		return "WHITE";
	}
	return "BLACK";
}
void colorWheel() {
	bool toggle = true;
	int radius = screenX / 2;
	int FREQ = 1;
	int FREQ0 = 1;
	
	int color1 = WHITE;
	int color2 = MAGENTA;
	for (int t = 1; t <= radius; t++) {
		color1 = rand() % 16;
		do {
			color2 = rand() % 16;
		} while (color2 == color1);
		string str1, str2;
		str1 = COLORVAL(color1);
		str2 = COLORVAL(color2);
		setcolor(15);
		settextstyle(0, 0, 72);
		outtextxy(20, 10, str1.c_str());
		outtextxy(screenX - 20 - textwidth(str2.c_str()), 10, str2.c_str());
		for (int j = 0; j <= t; j++) {
			for (int i = 0; i <= t; i++)
			{
				if (toggle) {
					if (rand() % FREQ0 == 0)
						putpixel(halfX + i, halfY, color1);
					if (rand() % FREQ0 == 0)
						putpixel(halfX, halfY + j, color2);
					if (rand() % FREQ0 == 0)
						putpixel(halfX - i, halfY, color1);
					if (rand() % FREQ0 == 0)
						putpixel(halfX, halfY - j, color2);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX + i, halfY + j))
							putpixel(halfX + i, halfY + j, color2);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX - i, halfY - j))
							putpixel(halfX - i, halfY - j, color1);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX + i, halfY - j))
							putpixel(halfX + i, halfY - j, color1);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX - i, halfY + j))
							putpixel(halfX - i, halfY + j, color2);

				}
				else {

					if (rand() % FREQ0 == 0)
						putpixel(halfX + i, halfY, color2);
					if (rand() % FREQ0 == 0)
						putpixel(halfX, halfY + j, color1);
					if (rand() % FREQ0 == 0)
						putpixel(halfX - i, halfY, color2);
					if (rand() % FREQ0 == 0)
						putpixel(halfX, halfY - j, color1);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX + i, halfY + j))
							putpixel(halfX + i, halfY + j, color1);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX - i, halfY - j))
							putpixel(halfX - i, halfY - j, color2);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX + i, halfY - j))
							putpixel(halfX + i, halfY - j, color2);
					if (rand() % FREQ == 0)
						if (isCircle(halfX, halfY, t, halfX - i, halfY + j))
							putpixel(halfX - i, halfY + j, color1);

				}
				toggle = !toggle;
				//Sleep(2);
			}
		}
		cout << color1 << " : " << color2 << endl;
		setcolor(BLACK);
		Sleep(350);
		bar(20, 10, 20 + textwidth(str1.c_str()), 10 + textheight(str1.c_str()));
		bar(screenX - 20 - textwidth(str2.c_str()), 10, screenX - 20, 10 + textheight(str2.c_str()));
		
	}
}


void main() {
	int driver = VGA;
	int mode = VGAMAX;
	initgraph(&driver, &mode, "");
	srand((unsigned int)time(NULL));
	screenX = getmaxx();
	screenY = getmaxy();
	halfX = screenX / 2;
	halfY = screenY / 2;

	colorWheel();

	system("pause");


}