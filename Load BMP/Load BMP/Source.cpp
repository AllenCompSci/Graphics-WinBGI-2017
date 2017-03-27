#include "test.h"
#include <fstream>
#include <vector>
#include <string>
const int NUMVECTOR = 62; /// HERE 
struct RGB {
	int r;
	int g;
	int b;
	int colorVal;
	void init(int R, int G, int B) {
		r = R;
		g = G;
		b = B;
	}
	void init(int R, int G, int B, int c) {
		r = R;
		g = G;
		b = B;
		colorVal = c;
	}
	bool isEqual(RGB t) {
		return r == t.r && g == t.g && b == t.b;
	}
};
int w, h;
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
vector <RGB> colors(NUMVECTOR);
string toUpper(string s) {
	for (int i = 0; i < (int)s.length(); i++) {
		if (s[i] >= 97 && s[i] <= 122)
			s[i] -= 32;
	}
	return s;
}
unsigned char* ReadBMP(char* filename)
{
	
	FILE* f = fopen(filename, "rb");

	if (f == NULL)
		throw "Argument Exception";

	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
	int width, height;
	// extract image height and width from header
	if (w == 0 || h == 0) {
		width = *(int*)&info[18];
		height = *(int*)&info[22];
	}
	else {
		width = w;
		height = h;
	}
	string n(filename);
	n = n.substr(0, n.length() - 4);
	cout << "FILE : " << n << endl;
	cout << endl;
	cout << "  Name: " << filename << endl;
	cout << " Width: " << width << endl;
	cout << "Height: " << height << endl;
	ofstream files, paint;
	files.open("RGB.txt");
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char* data = new unsigned char[row_padded];
	unsigned char tmp;
	paint.open("paint.txt");
	for (int i = 0; i < height; i++)
	{
		fread(data, sizeof(unsigned char), row_padded, f);
		paint << "{ ";
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
			for (int index = 0; !exits && index < (int)colors.size(); index++)
				if (colors.at(index).isEqual(n))
				{
					exits = true;
					paint << colors.at(index).colorVal;
				}
			if (j + 3 >= (width * 3)) {
				paint << "}," << endl;
			}
			else
				paint << ", ";
			if (!exits)
				colors.push_back(n);
		}
		files << '\n';
	}
	files.close();
	paint.close();
	files.open("vectorBuild.txt");
	for (int i = 0; i < (int)colors.size(); i++)
		files << "colors.at(" << i << ").init( " << colors.at(i).r << ", " << colors.at(i).g << ", " << colors.at(i).b << ", ); \n";
	files.close();
	vector <string> hold;
	ifstream reverse;
	reverse.open("paint.txt");
	while (!reverse.eof()) {
		string line;
		getline(reverse, line);
		hold.push_back(line);
	}
	reverse.close();
	paint.open("paint.txt");
	paint << "const int " << n << "_Height = " << height << "; \n";
	paint << "const int " << n << "_Width = " << width << "; \n";
	paint << "#pragma region " << toUpper(n) << "_DRAW \n";
	paint << "const int " << n << "_ARRY [" << height << "][" << width << "] = { \n";
	while (!hold.empty()) {
		paint << hold.back() << "\n";
		hold.pop_back();
	}
	paint << "}; \n \n #pragma endregion \n";
	paint.close();
	fclose(f);
	return data;
}
void main() {
	w = 0;
	h = 0;
	/*Copy all the vector values color.at() HERE*/
#pragma region example
	
	
	colors.at(0).init(31, 236, 243, 99);
	colors.at(1).init(182, 108, 147, 12);
	colors.at(2).init(3, 7, 7, 0);
	colors.at(3).init(204, 202, 202, 7);
	colors.at(4).init(230, 105, 39, 6);
	colors.at(5).init(232, 108, 40, 6);
	colors.at(6).init(0, 4, 4, 0);
	colors.at(7).init(228, 102, 38, 6);
	colors.at(8).init(254, 156, 143, 12);
	colors.at(9).init(229, 104, 39, 6);
	colors.at(10).init(250, 234, 27, 14);
	colors.at(11).init(234, 152, 20, 6);
	colors.at(12).init(228, 103, 38, 6);
	colors.at(13).init(234, 156, 18, 6);
	colors.at(14).init(223, 95, 35, 6);
	colors.at(15).init(234, 154, 19, 6);
	colors.at(16).init(234, 155, 18, 6);
	colors.at(17).init(234, 153, 19, 6);
	colors.at(18).init(234, 144, 24, 6);
	colors.at(19).init(234, 144, 25, 6);
	colors.at(20).init(234, 150, 21, 6);
	colors.at(21).init(234, 148, 22, 6);
	colors.at(22).init(234, 143, 25, 6);
	colors.at(23).init(234, 146, 23, 6);
	colors.at(24).init(234, 147, 22, 6);
	colors.at(25).init(234, 145, 24, 6);
	colors.at(26).init(234, 143, 26, 6);
	colors.at(27).init(234, 145, 23, 6);
	colors.at(28).init(234, 142, 26, 6);
	colors.at(29).init(234, 142, 27, 6);
	colors.at(30).init(234, 151, 21, 6);
	colors.at(31).init(234, 146, 24, 6);
	colors.at(32).init(234, 149, 21, 6);
	colors.at(33).init(234, 144, 23, 6);
	colors.at(34).init(234, 147, 23, 6);
	colors.at(35).init(231, 107, 40, 6);
	colors.at(36).init(234, 152, 21, 6);
	colors.at(37).init(234, 158, 17, 6);
	colors.at(38).init(234, 149, 22, 6);
	colors.at(39).init(234, 157, 18, 6);
	colors.at(40).init(224, 97, 36, 6);
	colors.at(41).init(234, 155, 19, 14);
	colors.at(42).init(212, 80, 30, 6);
	colors.at(43).init(234, 157, 17, 6);
	colors.at(44).init(221, 93, 35, 6);
	colors.at(45).init(234, 151, 20, 14);
	colors.at(46).init(215, 84, 32, 6);
	colors.at(47).init(229, 104, 38, 6);
	colors.at(48).init(16, 201, 234, 3);
	colors.at(49).init(226, 99, 36, 6);
	colors.at(50).init(222, 94, 35, 6);
	colors.at(51).init(224, 96, 36, 6);
	colors.at(52).init(222, 95, 35, 6);
	colors.at(53).init(216, 85, 32, 6);
	colors.at(54).init(234, 156, 19, 6);// or 14
	colors.at(55).init(213, 82, 30, 6);
	colors.at(56).init(255, 255, 255, 15);
	colors.at(57).init(4, 8, 9, 0);
	colors.at(58).init(5, 11, 11, 8);
	colors.at(59).init(6, 12, 12, 8);
	colors.at(60).init(7, 14, 14, 8);
	colors.at(61).init(0, 0, 0, 0);
	


#pragma endregion
	/*Make sure the *.bmp file is located in the same directory as the Source.cpp file*/
	ReadBMP(TEXT("bigbird.bmp")); // 24 bit depth .bmp
	/*
	Creates multiple files, find the vector
	use this to normalize the RGB values from what they are to
	the 16 valid choices with WINBGI
	copy and paste all those values.
	colors.at(0).init(#,#,#, ); 
	colors.at(1).init(#,#,#, );
	...
	colors.at(#).init(#,#,#, );
	replace the variable NUMVECTOR Value with the last # + 1;
	also after each 3rd number in the parameter list of 
	init()
	i.e. init(#, #, #, _HERE_);
	Use the number value for the associated color from the 
	WINBGI List and the value must coorespond.
	Run this program again. After copying and pasting all those values above where 
	you call ReadBMP(*);

	This will creat a text file called paintByNum.
	Copy the information from there. 
	*/
	system("pause");

	test();
	system("pause");
}