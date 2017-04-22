#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
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
RGB RED, PURPLE, BLACK, WHITE;
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
unsigned char* ReadBMP(char* filename)
{
	RED.init(224, 32, 64);
	PURPLE.init(64, 64, 192);
	BLACK.init(0, 0, 0);
	WHITE.init(255, 255, 255);
	RED.colorVal = 4;
	PURPLE.colorVal = 5;
	BLACK.colorVal = 0;
	WHITE.colorVal = 15; 
	vector <RGB> myColor;
	myColor.push_back(RED);
	myColor.push_back(WHITE);
	myColor.push_back(PURPLE);
	myColor.push_back(BLACK);

	int i;
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

			files << "[" << (int)data[j] << ", " << (int)data[j + 1] << "," << (int)data[j + 2] <<  "] ";
			RGB n;
			n.init(data[j], data[j + 1], data[j + 2]);
			bool exits = false;
			for (int i = 0; !exits && i < colors.size(); i++)
				if (colors.at(i).isEqual(n))
					exits = true;

			if (!exits)
				colors.push_back(n);
			for (int k = 0; k < myColor.size(); k++)
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
	for (int i = 0; i < colors.size(); i++)
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

void main(){

	ReadBMP(TEXT("TEST.bmp"));
	system("pause");

}