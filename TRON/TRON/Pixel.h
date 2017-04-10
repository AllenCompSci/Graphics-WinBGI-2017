#include <winbgi.cpp>
#include <graphics.h>
#include "Title.h"

void drawTitle(int X, int Y) {
	int h = title_Height;
	int w = title_Width;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (title_ARRY[i][j] != 99)
				putpixel((X + j), (Y + i), title_ARRY[i][j]);
		}
	}
}
void drawTitleE(int X, int Y) {
	int h = title_Height;
	int w = title_Width;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (title_ARRY[i][j] == WHITE || title_ARRY[i][j] == 99)
				putpixel((X + j), (Y + i), WHITE);
		}
	}
}
void drawGrid(int unitSize, int maxX, int maxY, int bkgrnd, int lines) {
	setbkcolor(bkgrnd);
	cleardevice();
	setbkcolor(BLACK);
	setcolor(lines);
	for (int i = 0; i < maxX; i++) {
		if( i % (unitSize+1) == 0)
			line(i, 0, i, maxY);
	}
	for (int j = 0; j < maxY; j++) {
		if( j % (unitSize + 1) == 0)
			line(0, j, maxX, j);
	}
}