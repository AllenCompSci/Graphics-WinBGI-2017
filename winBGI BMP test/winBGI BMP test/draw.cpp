#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include <iomanip>
#include <stdio.h>
#include <winbgi.cpp>
#include <thread>
#include <graphics.h>
#include <vector>
#include <string>
#include "images.h"
#include "frontANI.h"
using namespace std;


int GrDriver;
int GrMode;
int ErrorCode;
struct RGB {
	int r;
	int g;
	int b;
	int colorVal;
	void init(int R, int G, int B){
		r = R;
		g = G;
		b = B;
	}

	bool isEqual(RGB t){
		return r == t.r && g == t.g && b == t.b;
	}

};
vector <RGB> colors;
RGB REDPIXEL, PURPLEPIXEL, BLACKPIXEL, WHITEPIXEL, NULLPIXEL, LIGHTGREY2PIXEL, LIGHTGREYPIXEL, LIGHTREDPIXEL, BROWNPIXEL, BROWN2PIXEL, DARKGREYPIXEL, YELLOWPIXEL, WHITE2PIXEL;
/*
0  BLACK
1  BLUE
2  GREEN
3  CYAN
4  RED
5  MAGENTA
6  BROWN
7  LIGHTGRAY
8  DARKGRAY
9  LIGHTBLUE
10 LIGHTGREEN
11 LIGHTCYAN
12 LIGHTRED
13 LIGHTMAGENTA
14 YELLOW
15 WHITE

*/
unsigned char* ReadBMP(const char*, string num);
unsigned char* ReadBMP(char*);
void drawSquirrel(int, int);
void gr_Start(int&, int&, int&);
void eraseSquirrel(int, int);
void drawSquare(int, int);
void drawRight(int, int);
void drawEagle(int, int);
void drawAni(int, int);
void drawANIF(int, int);
void drawFrameAni(const int frame[aniHeigh][aniWidth], int x, int y);
void backgroundSave(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y);
void redrawBackGround(int background[aniHeigh][aniWidth], int x, int y);
void drawFrameMulti(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration);
void drawFrameAniR(const int frame[aniHeigh][aniWidth], int x, int y);
void backgroundSaveR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y);
void drawFrameMultiR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration);
string convert(int);

void main()
{
	/*
	int numBMPS = 35;
	// bmpLayers is file name creates numBMPS * 2 .txt files  
	for (int i = 1; i <= numBMPS; i++){
		string file = "bmpLayers/" + convert(i) + ".bmp";
		char * s = new char[file.length() + 1];
		strcpy(s, file.c_str());
		ReadBMP(file.c_str(), convert(i));
	}
	// creates a single 3 single txt files 
	ReadBMP(TEXT("file.bmp"));
	system("pause");
	*/
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);
	int maxY = getmaxy();
	int maxX = getmaxx();


	while (true){
		drawSquirrel(200, 200);
		drawSquirrel(400, 400);
		drawSquirrel(600, 500);
		Sleep(5000);
		eraseSquirrel(200, 200);
		Sleep(1000);
		eraseSquirrel(400, 400);
		Sleep(1000);
		eraseSquirrel(600, 500);
		int count = 0;
		
		for (int j = 0; j + square_width <= maxX+square_width; j += square_width){
			for (int i = 0; i + square_height <= maxY+square_height; i += square_height){
				int sqr = rand() % 3;
				switch (sqr){
				case 0:	drawSquare(j, i);
					break;
				case 1:	drawRight(j, i);
					break;
				case 2:  drawEagle(j, i);
					break;
				}
			}
		}
		Sleep(200);
		drawAni(200, 200);
		drawAni(600, 500);
		drawAni(400, 400);
		drawANIF(200, 200);
		cleardevice();
	}
	system("pause");
	closegraph();
}
void drawRight(int x, int y){
	for (int i = 0; i < square_height; i++)
		for (int j = 0; j < square_width; j++)
		{
			if (square_correct[i][j] != 99)
				putpixel(x + j, y + i, square_correct[i][j]);
		}
}
void drawSquare(int x, int y){
	for (int i = 0; i < square_height; i++)
		for (int j = 0; j < square_width; j++)
		{
			if (square_bit[i][j] != 99)
				putpixel(x + j, y + i, square_bit[i][j]);
		}
}
void drawEagle(int x, int y){
	for (int i = 0; i < 51; i++)
		for (int j = 0; j < 51; j++)
		{
			putpixel(x + j, y + i, eglhks[i][j]);
		}
}
void drawSquirrel(int x, int y){
	for (int i = 0; i < squirrel_height; i++)
		for (int j = 0; j < squirrel_width; j++)
		{
			if (squirrel[i][j] != 99)
				putpixel(x + j, y + i, squirrel[i][j]);
		}
}
void drawAni(int x, int y){
	int background[aniHeigh][aniWidth];
	int count = 0;
	while (count < 15){
		drawFrameMulti(frame1, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame2, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame3, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame2, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame1, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame2, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame4, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame1, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame2, background, x, y, 300);
		x += 3;
		drawFrameMulti(frame3, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame1, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame2, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame3, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame1, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame2, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame4, background, x, y, 300);
		x -= 3;
		drawFrameMultiR(frame1, background, x, y, 300);

	count++;
	}
}
void drawANIF(int x, int y){
	int background[aniHeigh][aniWidth];
	int count = 0;
	int large = getmaxx();
	while ((x + 100) <= large){
		for (int i = 0; (x + 100 <= large) && i < 36; i++){
			drawFrameMulti(frameA1, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA2, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA3, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA4, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA5, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA6, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA7, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA8, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA9, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA10, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA11, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA12, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA13, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA14, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA15, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA16, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA17, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA18, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA19, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA20, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA21, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA22, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA23, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA24, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA25, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA26, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA27, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA28, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA29, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA30, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA31, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA32, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA33, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA34, background, x, y, 300); 	x += 3;
			drawFrameMulti(frameA35, background, x, y, 300); 	x += 3;
			
		}
		
	}
}
void drawFrameMultiR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration){
	backgroundSaveR(frame, background, x, y);
	drawFrameAniR(frame, x, y);
	Sleep(duration);
	redrawBackGround(background, x, y);

}
void drawFrameMulti(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y, int duration){
	backgroundSave(frame, background, x, y);
	drawFrameAni(frame, x, y);
	Sleep(duration);
	redrawBackGround(background, x, y);

}
void drawFrameAni(const int frame[aniHeigh][aniWidth], int x, int y){
	for (int i = 0; i < aniHeigh; i++)
		for (int j = 0; j < aniWidth; j++)
		{
		if (frame[i][j] != 99)
			putpixel(x + j, y + i, frame[i][j]);
		}

}
void backgroundSave(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y){
	for (int i = 0; i < aniHeigh; i++){
		for (int j = 0; j < aniWidth; j++)
		{
			if (frame[i][j] == 99){
				background[i][j] = 99;
			}
			else
				background[i][j] = getpixel(x + j, y + i);
		}
	}
}
void backgroundSaveR(const int frame[aniHeigh][aniWidth], int background[aniHeigh][aniWidth], int x, int y){
	int Reverse = aniWidth - 1;
	for (int i = 0; i < aniHeigh; i++){
		for (int j = 0; j < aniWidth; j++)
		{
			if (frame[i][Reverse - j] == 99){
				background[i][j] = 99;
			}
			else
				background[i][j] = getpixel(x + j, y + i);
		}
	}
}
void drawFrameAniR(const int frame[aniHeigh][aniWidth], int x, int y){
	int Reverse = aniWidth - 1;
	for (int i = 0; i < aniHeigh; i++)
		for (int j = 0; j < aniWidth; j++)
		{
		if (frame[i][Reverse - j] != 99)
			putpixel(x + j, y + i, frame[i][Reverse - j]);
		}

}
void redrawBackGround(int background[aniHeigh][aniWidth], int x, int y){
	for (int i = 0; i < aniHeigh; i++)
		for (int j = 0; j < aniWidth; j++)
		{
		if (background[i][j] != 99)
			putpixel(x + j, y + i, background[i][j]);
		}

}
void gr_Start(int&Gr_Driver, int&GrMode, int&ErrorCode)
{

	GrDriver = VGA;

	GrMode = VGAMAX;

	initgraph(&GrDriver, &GrMode, " ");

	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error: " << ErrorCode;
		getch();
		exit(1);
	}
}
void eraseSquirrel(int x, int y){
	for (int i = 0; i < squirrel_height; i++)
		for (int j = 0; j < squirrel_width; j++)
		{
			if (squirrel[i][j] != 99)
				putpixel(x + j, y + i, 0);
		}
}
unsigned char* ReadBMP(char* filename)
{
	//REDPIXEL, PURPLEPIXEL, BLACKPIXEL, WHITEPIXEL, NULLPIXEL, LIGHTGREY2PIXEL, LIGHTGREYPIXEL, LIGHTREDPIXEL, BROWNPIXEL, BROWN2PIXEL, DARKGREYPIXEL, YELLOWPIXEL, WHITE2PIXEL;
	NULLPIXEL.init(163, 73, 164);
	NULLPIXEL.colorVal = 99;
	REDPIXEL.init(255, 8, 8);
	WHITE2PIXEL.init(64, 64, 192);
	BLACKPIXEL.init(0, 0, 0);
	WHITEPIXEL.init(255, 255, 255);
	WHITE2PIXEL.init(252, 252, 252);
	WHITE2PIXEL.colorVal = 15;
	BROWN2PIXEL.init(148, 66, 0);
	BROWNPIXEL.init(82, 41, 0);
	BROWN2PIXEL.colorVal = 6;
	BROWNPIXEL.colorVal = 6;
	LIGHTREDPIXEL.init(255, 102, 102);
	LIGHTREDPIXEL.colorVal = 12;
	LIGHTGREY2PIXEL.init(229, 220, 212);
	LIGHTGREYPIXEL.init(240, 240, 240);
	LIGHTGREYPIXEL.colorVal = LIGHTGREY2PIXEL.colorVal = 7;
	DARKGREYPIXEL.init(152, 136, 136);
	YELLOWPIXEL.init(255, 186, 0);
	YELLOWPIXEL.colorVal = 14;
	DARKGREYPIXEL.colorVal = 8;
	REDPIXEL.colorVal = 4;
	PURPLEPIXEL.colorVal = 5;
	BLACKPIXEL.colorVal = 0;
	WHITEPIXEL.colorVal = 15;
	vector <RGB> myColor;
	myColor.push_back(NULLPIXEL);
	myColor.push_back(WHITE2PIXEL);
	myColor.push_back(BROWNPIXEL);
	myColor.push_back(BROWN2PIXEL);
	myColor.push_back(YELLOWPIXEL);
	myColor.push_back(REDPIXEL);
	myColor.push_back(WHITEPIXEL);
	myColor.push_back(DARKGREYPIXEL);
	myColor.push_back(LIGHTGREYPIXEL);
	myColor.push_back(LIGHTGREY2PIXEL);
	myColor.push_back(LIGHTREDPIXEL);
	myColor.push_back(BLACKPIXEL);

	
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;
	ofstream files;
	ofstream paintArray;
	files.open("RGB.txt");
	paintArray.open("paintByNum.txt");
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		paintArray << "{ ";
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			files << "[" << (int)data[j] << ", " << (int)data[j + 1] << "," << (int)data[j + 2] << "] ";
			RGB n;
			n.init(data[j], data[j + 1], data[j + 2]);
			bool exits = false;
			for (int i = 0; !exits && i < (int)colors.size(); i++)
				if (colors.at(i).isEqual(n))
					exits = true;

			if (!exits)
				colors.push_back(n);
			for (int k = 0; k < (int)myColor.size(); k++)
				if (myColor.at(k).isEqual(n)){
				paintArray << myColor.at(k).colorVal;
				}
			if (j + 3 != width * 3){
				paintArray << ", ";
			}
		}
		paintArray << "}, \n";
		files << '\n';
	}
	paintArray.close();
	files.close();
	files.open("KEYRGB.txt");
	for (int i = 0; i < (int)colors.size(); i++)
		files << i << "[ " << colors.at(i).r << ", " << colors.at(i).g << ", " << colors.at(i).b << "] \n";
	files.close();
	ifstream reverse;
	reverse.open("paintByNum.txt");
	vector <string> hold;
	string g;
	while (!reverse.eof()){
		getline(reverse, g);
		hold.push_back(g);
	}
	reverse.close();
	paintArray.open("paintByNum.txt");
	while (hold.size() > 0){
		paintArray << hold.back() << '\n';
		hold.pop_back();
	}
	paintArray.close();
	fclose(f);
	return data;
}
unsigned char* ReadBMP(const char* filename, string num)
{
	//REDPIXEL, PURPLEPIXEL, BLACKPIXEL, WHITEPIXEL, NULLPIXEL, LIGHTGREY2PIXEL, LIGHTGREYPIXEL, LIGHTREDPIXEL, BROWNPIXEL, BROWN2PIXEL, DARKGREYPIXEL, YELLOWPIXEL, WHITE2PIXEL;
	RGB nu1, bl1, dg1, br1, lg1, nu2, nu3, dg2, nu4, br2, ye1, lg2, lg3, wh1, wh2, rd1, lg4, rd2, lird1, nu5;
	RGB nu6, nu7;
	nu6.init(223, 50, 212); nu6.colorVal = 99;
	nu7.init(222, 50, 212); nu7.colorVal = 99;
	nu1.init(223,50,213); nu1.colorVal = 99;
	nu2.init(223,49,212); nu2.colorVal = 99;
	nu3.init(222,49,212); nu3.colorVal = 99;
	nu4.init(223,49,213); nu4.colorVal = 99;
	nu5.init(221,49,211); nu5.colorVal = 99;
	bl1.init(0, 0, 0); bl1.colorVal = 0;
	dg1.init(69, 69, 69); dg1.colorVal = 8;
	dg2.init(152, 136, 136); dg2.colorVal = 8;
	br1.init(82, 41, 0); br1.colorVal = 6;
	br2.init(255, 138, 0); br2.colorVal = 6;
	lg1.init(148,66,0); lg1.colorVal = 7;
	lg2.init(229,220,212); lg2.colorVal = 7;
	lg3.init(255,227,99); lg3.colorVal = 7;
	lg4.init(240,224,88); lg4.colorVal = 7;
	ye1.init(255, 186, 0); ye1.colorVal = 14;
	rd1.init(173, 0, 0); rd1.colorVal = 4;
	rd2.init(255, 8, 8); rd2.colorVal = 4;
	lird1.init(255, 102, 102); lird1.colorVal = 12;
	wh1.init(252,252,252); wh1.colorVal = 15;
	wh2.init(240,240,240); wh2.colorVal = 15;

	vector <RGB> myColor;
	
	myColor.push_back(wh1);
	myColor.push_back(wh2);
	myColor.push_back(rd1);
	myColor.push_back(rd2);
	myColor.push_back(lird1);
	myColor.push_back(ye1);
	myColor.push_back(lg1);
	myColor.push_back(lg2);
	myColor.push_back(lg3);
	myColor.push_back(lg4);
	myColor.push_back(br1);
	myColor.push_back(br2);
	myColor.push_back(dg1);
	myColor.push_back(dg2);
	myColor.push_back(bl1);
	myColor.push_back(nu1);
	myColor.push_back(nu2);
	myColor.push_back(nu3);
	myColor.push_back(nu4);
	myColor.push_back(nu5);
	myColor.push_back(nu6);
	myColor.push_back(nu7);
	
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	int width = *(int*)&info[18];
	int height = *(int*)&info[22];

	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;
	ofstream files;
	ofstream paintArray;
	files.open("RGB.txt");
	num += ".txt";
	paintArray.open(num.c_str());
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;

	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		paintArray << "{ ";
		for (int j = 0; j < width * 3; j += 3)
		{
			// Convert (B, G, R) to (R, G, B)
			tmp = data[j];
			data[j] = data[j + 2];
			data[j + 2] = tmp;

			files << "[" << (int)data[j] << ", " << (int)data[j + 1] << "," << (int)data[j + 2] << "] ";
			RGB n;
			n.init(data[j], data[j + 1], data[j + 2]);
			bool exits = false;
			for (int i = 0; !exits && i < (int)colors.size(); i++)
				if (colors.at(i).isEqual(n))
					exits = true;

			if (!exits)
				colors.push_back(n);
			for (int k = 0; k < (int)myColor.size(); k++)
				if (myColor.at(k).isEqual(n)){
				paintArray << myColor.at(k).colorVal;
				}
			if (j + 3 != width * 3){
				paintArray << ", ";
			}
		}
		paintArray << "}, \n";
		files << '\n';
	}
	paintArray.close();
	files.close();
	
	ifstream reverse;
	reverse.open(num.c_str());
	vector <string> hold;
	string g;
	while (!reverse.eof()){
		getline(reverse, g);
		hold.push_back(g);
	}
	reverse.close();
	paintArray.open(num.c_str());
	while (hold.size() > 0){
		paintArray << hold.back() << '\n';
		hold.pop_back();
	}
	paintArray.close();
	num = num.substr(0, num.length() - 4);
	num += "KEY.txt";
	files.open(num.c_str());
	for (int i = 0; i < (int)colors.size(); i++){
		bool notFound = true;
		for (int j = 0; notFound && j < (int)myColor.size(); j++)
			if (colors.at(i).isEqual(myColor.at(j)))
			{
				notFound = false;
			}
		if (notFound)
			files << i << "[ " << colors.at(i).r << ", " << colors.at(i).g << ", " << colors.at(i).b << "] \n";
	}
	files.close();
	fclose(f);
	return data;
}
string convert(int x){
	return to_string(x);
}

