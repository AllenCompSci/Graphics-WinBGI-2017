#include "FunctionProto.h"
#pragma region UTILITY_Section
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
/// Color Helper
void introColorSet(char value) {
	switch (value) {
	case 'B':
		setcolor(BLACK);
		break;
	case 'W':
		setcolor(WHITE);
		break;
	case 'G':
		setcolor(GREEN);
		break;
	case 'Y':
		setcolor(YELLOW);
		break;
	}
}
/// READBMP Utility
string toUpper(string s) {
	for (int i = 0; i < (int)s.length(); i++) {
		if (s[i] >= 97 && s[i] <= 122)
			s[i] -= 32;
	}
	return s;
}
/// Main Function LOADBMP
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
/// Start/Setup Graphics Window
void gr_Start(int &GrDriver, int&GrMode, int&ErrorCode) {
	GrDriver = VGA;
	GrMode = VGAMAX;
	initgraph(&GrDriver, &GrMode, "");
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
		system("pause");
		exit(0);
	}
	/// HIDE CONSOLE
	maxX = getmaxx();
	maxY = getmaxy();
	return;
}
/// GET INPUT KEY LISTENER
string StringBuilder() {
	string NAME = "";
	isName = true;
	string BufferedReader = "";
	int left, top, right, bottom;
	left = top = right = bottom = 0;
	int OFFSET = UNIT * 2;
	settextstyle(0, 0, 72);
	while (BufferedReader != "RETURN") {
		if (GLOBAL.isPressed) {
			BufferedReader = GLOBAL.Significance;
			if (BufferedReader != "RETURN" && BufferedReader != "Left Mouse Click"
				&& BufferedReader != "LEFT" && BufferedReader != "RIGHT"
				&& BufferedReader != "UP" && BufferedReader != "DOWN") {
				if (BufferedReader != "BACK") {
					NAME += BufferedReader;
				}
				else {
					NAME = NAME.substr(0, NAME.length() - 1);
				}
			}
			do { Sleep(3); } while (ActionListener(GLOBAL.VirtualKey));
			GLOBAL.resetKey();
			setcolor(BLACK);
			bar(left, top, right, bottom);
			setcolor(WHITE);
			left = (maxX - textwidth(NAME.c_str())) / 2;
			right = left + textwidth(NAME.c_str());
			top = (maxY - OFFSET - textheight(NAME.c_str())) / 2;
			bottom = top + textheight(NAME.c_str());
			outtextxy(left, top, NAME.c_str());
			//cout << NAME << "\n";

		}

		Sleep(15);
	}
	isName = false;
	return NAME;
}
/// AsynKeyState
bool ActionListener(int VirtualKey) {
	return ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
}
/// establishGAME - UTILITY FUNCTION
void updateCREATE(int index) {
	CREATION[index] = rand() % RANDOMIZER[index];
}
/// COLLSION BASED UTLITY FUNCTION FOR FROG.tick()
bool isSafe(Column frog, int top, int h) {
	bool safe = true;
	if (frog == Second) {
		for (int i = 0; i < (int)CARCOL2.size(); i++) {
			if (CARCOL2[i].top - 80 < top && CARCOL2[i].bottom > top) {
				CARCOL2[i].remove();
				return false;
			}
		}
	}
	else if (frog == Third) {
		for (int i = 0; i < (int)CARCOL3.size(); i++) {
			if (CARCOL3[i].top - 80 < top && CARCOL3[i].bottom > top) {
				CARCOL3[i].remove();
				return false;
			}
		}
	}
	else if (frog == Fourth) {
		for (int i = 0; i < (int)CARCOL4.size(); i++) {
			if (CARCOL4[i].top - 80 < top && CARCOL4[i].bottom > top) {
				CARCOL4[i].remove();
				return false;
			}
		}
	}
	else if (frog == Sixth) {
		for (int i = 0; i < (int)LOGCOL6.size(); i++) {
			if (LOGCOL6[i].top <= top && (LOGCOL6[i].bottom) >(top + h))
				return true;
		}
		return false;
	}
	else if (frog == Seventh) {
		for (int i = 0; i < (int)LOGCOL7.size(); i++) {
			if (LOGCOL7[i].top <= top && (LOGCOL7[i].bottom) >(top + h))
				return true;
		}
		return false;
	}
	else if (frog == Eight) {
		for (int i = 0; i < (int)LOGCOL8.size(); i++) {
			if (LOGCOL8[i].top <= top && (LOGCOL8[i].bottom) >(top + h))
				return true;
		}
		return false;
	}
	else if (frog == Ninth) {
		for (int i = 0; i < (int)LOGCOL9.size(); i++) {
			if (LOGCOL9[i].top <= top && (LOGCOL9[i].bottom) > (top+h))
				return true;
		}
		return false;
	}
	return true;
}
/// Gives Value when drawing CARs/Trucks - UTILITY FUNCTION
int adjust(CARTYPE color, int value) {
	if (value == 16)
		return 0;
	if (color == BLUECAR) {
		switch (value) {
		case primaryColor:
			swap1 = !swap1;
			if (swap1)
				return LIGHTGRAY;
			else
				return BLUE;
			break;
		case secondaryColor:
			return LIGHTCYAN;
		}
	}
	if (color == BLACKCAR) {
		switch (value) {
		case primaryColor:
			return WHITE;
			break;
		case secondaryColor:
			swap2 = !swap2;
			if (swap2)
				return LIGHTGRAY;
			else
				return DARKGRAY;
		}
	}
	if (color == GREENCAR) {
		switch (value) {
		case primaryColor:
			return GREEN;
			break;
		case secondaryColor:
			swap2 = !swap2;
			if (swap2)
				return YELLOW;
			else
				return GREEN;
		}
	}
	if (color == REDCAR) {
		switch (value) {
		case primaryColor:
			return RED;
			break;
		case secondaryColor:
			swap2 = !swap2;
			if (swap2)
				return YELLOW;
			else
				return RED;
		}
	}
	if (color == REDTRUCK) {
		switch (value) {
		case cabColor:
			return RED;
			break;
		case trailer:
			return LIGHTGRAY;
		}
	}
	if (color == YELLOWTRUCK) {
		switch (value) {
		case cabColor:
			return YELLOW;
			break;
		case trailer:
			return LIGHTGRAY;

		}
	}
	if (color == BLUETRUCK) {
		switch (value) {
		case cabColor:
			return CYAN;
			break;
		case trailer:
			return LIGHTGRAY;

		}
	}
	return 0;
}
#pragma endregion
#pragma region GAME_FUNCTIONALITY
/// LOAD BMP Call Function
void CREATE(string test) {
	w = 0;
	h = 0;
	/*Copy all the vector values color.at() HERE*/
#pragma region example
	/*FROGGER COLORS SET 4
	colors.at(0).init(255, 255, 255, 99);
	colors.at(1).init(71, 213, 0, 2);
	colors.at(2).init(255, 255, 0, 14);
	colors.at(3).init(255, 0, 255, 5);
/* WISE FROG
	colors.at(3).init(0, 0, 0, 0);        // BLACK
	colors.at(4).init(255, 255, 255, 99); // BACKGROUND
	colors.at(5).init(153, 229, 80, 14);    // YELLOWGREEN
	colors.at(0).init(106, 190, 48, 2);    // GREEN?
	colors.at(1).init(55, 148, 110, 1);    // BLUEGREEN?
	colors.at(2).init(50, 60, 57, 8);     // DARKGRAY
	*/
	/* LOG
	colors.at(0).init(0, 0, 0, 99);
	colors.at(1).init(252, 71, 25, BROWN);
	colors.at(2).init(106, 190, 48, CYAN);
	colors.at(3).init(255, 255, 255, WHITE);
	*/
	//const int LIGHTGREEN = 98;
	//const int GRAYGREEN = 97;
	//const int GRAYBLUE = 96;
	/* Lily Pad DINORIKO SCREEN SHOT 
	colors.at(5).init(0, 0, 0, CYAN + 16);
	colors.at(6).init(0, 0, 168, 99);
	colors.at(7).init(50, 60, 57, DARKGRAY + 16);
	colors.at(8).init(0, 148, 0, GREEN+16);
	colors.at(4).init(39, 175, 38, LIGHTGREENE);
	colors.at(0).init(153, 217, 234, CYAN+16);
	colors.at(1).init(0, 104, 94, GRAYGREEN);
	colors.at(2).init(255, 255, 255, WHITE);
	colors.at(3).init(34, 105, 156, GRAYBLUE);
	*/
	/*
	*/
#pragma endregion
	/*Make sure the *.bmp file is located in the same directory as the Source.cpp file*/
	ReadBMP(TEXT((LPTSTR)test.c_str())); // 24 bit depth .bmp
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
}
/// Thread 1
void game() {
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);
	//							GAME MAP CONCEPT BY COLUMNS
	///  LEFT    200    9*40	13*40  17*40    800   25*40  28*40    31*40  34*40	37*40
	///  TOP	   0							  0                                  7*40
	///  WIDTH	  80	 80      80    100      160      80                            80
	///  HEIGHT 1080	120     150    300     1080                                    80
	///  SPEED	   0	 -9      +6     -6        0
	///  COLOR     8    4/3       1     15       11                                 GREEN
	if (debug) {
		INTRO();
		QuestLog();
		string name = StringBuilder();
		cleardevice();
		settextstyle(0, 3, 72);
		/// Draws CHAR NAME ON SIDE GETS RID OF UNUSED SPACE
		outtextxy(43 * UNIT, 5 * UNIT + textwidth(name.c_str()), name.c_str());
	}
	/// Setup Side Menu
	notfrogger.create((int)(1.7 * UNIT), 3 * UNIT);
	/// GAME SETUP
	RIVER();
	SIDEWALK();
	SAFEZONE();
	ROAD.init(); /// Possibly need to redraw Lines when Frogger Jumps
	GLOBAL.reset();
	GLOBAL.resetKey();
	while (isRunning) {
		if (lillied) {
			if (!firstRun && (int)GOLDENFROGS.size() < 3) {
				if(frog.alive)
					GOLDENFROGS.push_back(frog);
				frog.create(220, 600);
				frog.currColumn = First;
			}
			else if (firstRun) {
				RIVER();
				LILYPAD();
				frog.create(220, 600);
				frog.currColumn = First;
			}
			lillied = false;
		}
#pragma region RENDER
		frog.tick();
		if (frog.currColumn == First) {
			frog.draw();
		}
		if (frog.currColumn == Second) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)CARCOL2.size(); i++) {
			CARCOL2[i].tick();
		}
		if (frog.currColumn == Third) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)CARCOL3.size(); i++) {
			CARCOL3[i].tick();
		}
		if (frog.currColumn == Fourth || frog.currColumn == Fifth) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)CARCOL4.size(); i++) {
			CARCOL4[i].tick();
		}
		for (int i = 0; i < (int)GOLDENFROGS.size(); i++) {
			GOLDENFROGS[i].draw();
		}
		for (int i = 0; frog.alive && i < (int)LOGCOL6.size(); i++) {
			LOGCOL6[i].tick();
		}
		if (frog.currColumn == Sixth) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)LOGCOL7.size(); i++) {
			LOGCOL7[i].tick();
		}
		if (frog.currColumn == Seventh) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)LOGCOL8.size(); i++) {
			LOGCOL8[i].tick();
		}
		if (frog.currColumn == Eight) {
			frog.draw();
		}
		for (int i = 0; frog.alive && i < (int)LOGCOL9.size(); i++) {
			LOGCOL9[i].tick();
		}
		if (frog.currColumn == Ninth) {
			frog.draw();
		}
#pragma endregion
		
		Sleep(150);
		establishGAME();
	}
	getch();
	setcolor(10);
	rectangle(0, 120, 160, 560);
	bar(0, 120, 160, 560);
	setcolor(2);
	/*T*/
	line(0, 600, 160, 600);
	line(80, 600, 80, 720);
	/*I*/
	line(80, 840, 80, 760);
	/*M*/
	line(0, 1000, 0, 920);
	line(40, 1000, 0, 920);
	line(40, 1000, 80, 920);
	line(80, 920, 80, 1000);
	/*E*/
	line(120, 920, 120, 1000);
	line(120, 1000, 160, 1000);
	line(120, 960, 160, 960);
	line(120, 920, 160, 920);
	/*NotFrogger movements*/
}
/// Thread 2
void Listener() {
	GLOBAL.reset();
	while (isRunning) {
		if (!GLOBAL.isPressed) {
			/// Mouse Listener
			if (ActionListener(VK_LBUTTON)) {
				// Left Mouse Button
				GetCursorPos(&GLOBAL.Cursor);
				GLOBAL.VirtualKey = VK_LBUTTON;
				GLOBAL.Significance = "Left Mouse Click";
				do { Sleep(100); } while (ActionListener(VK_LBUTTON));
				GLOBAL.isClicked = true;
			}
			/// Irregular KeyListener Build String
			if (isName) {
				if (ActionListener(VK_A)) {
					GLOBAL.VirtualKey = VK_A;
					GLOBAL.Significance = "A";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_B)) {
					GLOBAL.VirtualKey = VK_B;
					GLOBAL.Significance = "B";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_C)) {
					GLOBAL.VirtualKey = VK_C;
					GLOBAL.Significance = "C";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_D)) {
					GLOBAL.VirtualKey = VK_D;
					GLOBAL.Significance = "D";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_E)) {
					GLOBAL.VirtualKey = VK_E;
					GLOBAL.Significance = "E";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_F)) {
					GLOBAL.VirtualKey = VK_F;
					GLOBAL.Significance = "F";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_G)) {
					GLOBAL.VirtualKey = VK_G;
					GLOBAL.Significance = "G";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_H)) {
					GLOBAL.VirtualKey = VK_H;
					GLOBAL.Significance = "H";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_I)) {
					GLOBAL.VirtualKey = VK_I;
					GLOBAL.Significance = "I";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_J)) {
					GLOBAL.VirtualKey = VK_J;
					GLOBAL.Significance = "J";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_K)) {
					GLOBAL.VirtualKey = VK_K;
					GLOBAL.Significance = "K";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_L)) {
					GLOBAL.VirtualKey = VK_L;
					GLOBAL.Significance = "L";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_M)) {
					GLOBAL.VirtualKey = VK_M;
					GLOBAL.Significance = "M";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_N)) {
					GLOBAL.VirtualKey = VK_N;
					GLOBAL.Significance = "N";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_O)) {
					GLOBAL.VirtualKey = VK_O;
					GLOBAL.Significance = "O";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_P)) {
					GLOBAL.VirtualKey = VK_P;
					GLOBAL.Significance = "P";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Q)) {
					GLOBAL.VirtualKey = VK_Q;
					GLOBAL.Significance = "Q";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_R)) {
					GLOBAL.VirtualKey = VK_R;
					GLOBAL.Significance = "R";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_S)) {
					GLOBAL.VirtualKey = VK_S;
					GLOBAL.Significance = "S";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_T)) {
					GLOBAL.VirtualKey = VK_T;
					GLOBAL.Significance = "T";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_U)) {
					GLOBAL.VirtualKey = VK_U;
					GLOBAL.Significance = "U";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_V)) {
					GLOBAL.VirtualKey = VK_V;
					GLOBAL.Significance = "V";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_W)) {
					GLOBAL.VirtualKey = VK_W;
					GLOBAL.Significance = "W";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_X)) {
					GLOBAL.VirtualKey = VK_X;
					GLOBAL.Significance = "X";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Y)) {
					GLOBAL.VirtualKey = VK_Y;
					GLOBAL.Significance = "Y";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_Z)) {
					GLOBAL.VirtualKey = VK_Z;
					GLOBAL.Significance = "Z";
					GLOBAL.isAlpha = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_1) || ActionListener(VK_NUMPAD1)) {
					GLOBAL.VirtualKey = VK_1;
					GLOBAL.Significance = "1";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_2) || ActionListener(VK_NUMPAD2)) {
					GLOBAL.VirtualKey = VK_2;
					GLOBAL.Significance = "2";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_3) || ActionListener(VK_NUMPAD3)) {
					GLOBAL.VirtualKey = VK_3;
					GLOBAL.Significance = "3";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_4) || ActionListener(VK_NUMPAD4)) {
					GLOBAL.VirtualKey = VK_4;
					GLOBAL.Significance = "4";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_5) || ActionListener(VK_NUMPAD5)) {
					GLOBAL.VirtualKey = VK_5;
					GLOBAL.Significance = "5";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_6) || ActionListener(VK_NUMPAD6)) {
					GLOBAL.VirtualKey = VK_6;
					GLOBAL.Significance = "6";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_7) || ActionListener(VK_NUMPAD7)) {
					GLOBAL.VirtualKey = VK_7;
					GLOBAL.Significance = "7";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_8) || ActionListener(VK_NUMPAD8)) {
					GLOBAL.VirtualKey = VK_8;
					GLOBAL.Significance = "8";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_9) || ActionListener(VK_NUMPAD9)) {
					GLOBAL.VirtualKey = VK_9;
					GLOBAL.Significance = "9";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_0) || ActionListener(VK_NUMPAD0)) {
					GLOBAL.VirtualKey = VK_0;
					GLOBAL.Significance = "0";
					GLOBAL.isNumeric = true;
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_RETURN)) {
					GLOBAL.VirtualKey = VK_RETURN;
					GLOBAL.Significance = "RETURN";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_SPACE)) {
					GLOBAL.VirtualKey = VK_SPACE;
					GLOBAL.Significance = " ";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_BACK)) {
					GLOBAL.VirtualKey = VK_BACK;
					GLOBAL.Significance = "BACK";
					GLOBAL.isPressed = true;
				}
			}
			/// Regular Movements for FROGGER
			else {
				if (ActionListener(VK_LEFT) || ActionListener(VK_A)) {
					GLOBAL.VirtualKey = VK_LEFT;
					GLOBAL.Significance = "LEFT";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_RIGHT) || ActionListener(VK_D)) {
					GLOBAL.VirtualKey = VK_RIGHT;
					GLOBAL.Significance = "RIGHT";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_UP) || ActionListener(VK_W)) {
					GLOBAL.VirtualKey = VK_UP;
					GLOBAL.Significance = "UP";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_DOWN) || ActionListener(VK_S)) {
					GLOBAL.VirtualKey = VK_DOWN;
					GLOBAL.Significance = "DOWN";
					GLOBAL.isPressed = true;
				}
				else if (ActionListener(VK_F6)) {
					GLOBAL.VirtualKey = VK_F6;
					GLOBAL.Significance = "F6";
					GLOBAL.isPressed = true;
				}  /// Novelty EASTER EGG insta kill yourself button
				else if (ActionListener(VK_ESCAPE)) {
					isRunning = false;
					exit(0);
				}
			}
		}
		Sleep(15);
	}
}
/// CONTAINER BUILD 
void establishGAME() {
	if (firstRun) {
		Type1.create(Second);
		CARCOL2.push_back(Type1);
		Type1.create(Third);
		CARCOL3.push_back(Type1);
		Type1.create(Fourth);
		CARCOL4.push_back(Type1);
		platform.create(Sixth);
		LOGCOL6.push_back(platform);
		platform.create(Seventh);
		LOGCOL7.push_back(platform);
		platform.create(Eight);
		LOGCOL8.push_back(platform);
		platform.create(Ninth);
		LOGCOL9.push_back(platform);
		firstRun = false;
		for (int i = 0; i < 7; i++) {
			updateCREATE(i);
		}
	}
	if ((int)CARCOL2.size() > 0) {
		if (CARCOL2[0].bottom < 6) {
			CARCOL2.erase(CARCOL2.begin());
		}
		if (CARCOL2[(int)CARCOL2.size() - 1].bottom < maxY - (CAR_Height / 4) - (CREATION[0] * CAR_Height)) {
			Type1.create(Second);
			CARCOL2.push_back(Type1);
			updateCREATE(0);
		}
	}
	if ((int)CARCOL3.size() > 0) {
		if (CARCOL3[0].top >(maxY - 6)) {
			CARCOL3.erase(CARCOL3.begin());
		}
		if (CARCOL3[(int)CARCOL3.size() - 1].top > CAR_Height / 4 + (CREATION[1] * CAR_Height)) {
			Type1.create(Third);
			CARCOL3.push_back(Type1);
			updateCREATE(1);
		}
	}
	if ((int)CARCOL4.size() > 0) {
		if (CARCOL4[0].bottom < 6) {
			CARCOL4.erase(CARCOL4.begin());
		}
		if (CARCOL4[(int)CARCOL4.size() - 1].bottom < maxY - (TRUCK_Height / 4) - (CREATION[2] * TRUCK_Height)) {
			Type1.create(Fourth);
			CARCOL4.push_back(Type1);
			updateCREATE(2);
		}
	}
	if ((int)LOGCOL6.size() > 0) {
		if (LOGCOL6[0].bottom < 6) {
			LOGCOL6.erase(LOGCOL6.begin());
		}
		if (LOGCOL6[(int)LOGCOL6.size() - 1].bottom < maxY - (250 / 3) - (CREATION[3] * 250 - (20 * CREATION[3] + 20))) {
			platform.create(Sixth);
			LOGCOL6.push_back(platform);
			updateCREATE(3);
		}
	}
	if ((int)LOGCOL7.size() > 0) {
		if (LOGCOL7[0].top > maxY - 20) {
			LOGCOL7.erase(LOGCOL7.begin());
		}
		if (LOGCOL7[(int)LOGCOL7.size() - 1].top > (250 / 3) + (CREATION[4] * 250) - (20 * CREATION[3])) {
			platform.create(Seventh);
			LOGCOL7.push_back(platform);
			updateCREATE(4);
		}
	}
	if ((int)LOGCOL8.size() > 0) {
		if (LOGCOL8[0].top > maxY - 20) {
			LOGCOL8.erase(LOGCOL8.begin());
		}
		if (LOGCOL8[(int)LOGCOL8.size() - 1].top > (250 / 4) + (CREATION[5] * 250) - (20 * CREATION[3])) {
			platform.create(Eight);
			LOGCOL8.push_back(platform);
			updateCREATE(5);
		}
	}
	if ((int)LOGCOL9.size() > 0) {
		if (LOGCOL9[0].bottom < 6) {
			LOGCOL9.erase(LOGCOL9.begin());
		}
		if (LOGCOL9[(int)LOGCOL9.size() - 1].bottom < maxY - (250 / 4) - (CREATION[6] * 250)) {
			platform.create(Ninth);
			LOGCOL9.push_back(platform);
			updateCREATE(6);
		}
	}
}
#pragma endregion
#pragma region DRAW_HELPERS
/// Draw River 
void RIVER() {
	setcolor(BLUE);
	bar(24 * UNIT, 0, 42 * UNIT, maxY);
}
/// Draw SideWalk
void SIDEWALK() {
	setcolor(LIGHTGRAY);
	bar(20 * UNIT, 0, 24 * UNIT, maxY);
}
/// Draw SafeZone
void SAFEZONE() {
	setcolor(LIGHTGRAY);
	bar(5 * UNIT, 0, 8 * UNIT, maxY);
}
/// Draw LilyPads
void LILYPAD() {
	pads[0].create(37, 7);
	pads[1].create(37, 11);
	pads[2].create(37, 15);
	pads[3].create(37, 19);
}
/// Draws Intro
void INTRO() {
	setcolor(RED);
	bar(0, 0, maxX, maxY);
	int Sx = (maxX - (17 * UNIT * 2)) / 2;
	int Sy = (maxY - (12 * UNIT * 2)) / 2;
	bool yg = false;
	for (int i = 0; i < Title_Height; i++) {
		for (int j = 0; j < Title_Width; j++) {
			if (Title[i][j] != 'D') {
				if (Title[i][j] != 'Y') {
					introColorSet(Title[i][j]);
					bar(Sx + (j * 2 * UNIT), Sy + (i * 2 * UNIT), Sx + (j * 2 * UNIT) + (2 * UNIT), Sy + (i * 2 * UNIT) + (2 * UNIT));
					bar(maxX - (Sx + (j * 2 * UNIT) + (2 * UNIT)), Sy + (i * 2 * UNIT), maxX - (Sx + (j * 2 * UNIT)), Sy + (i * 2 * UNIT) + (2 * UNIT));
				}
				else {
					for (int k = Sx + (j * 2 * UNIT); k <= Sx + (j * 2 * UNIT) + (2 * UNIT); k++) {
						for (int l = Sy + (i * 2 * UNIT); l <= Sy + (i * 2 * UNIT) + (2 * UNIT); l++) {
							if (yg)
							{
								putpixel(k, l, YELLOW);
								putpixel(maxX - (k), (l), GREEN);
							}
							else {
								putpixel(k, l, GREEN);
								putpixel(maxX - (k), (l), YELLOW);
							}
							yg = !yg;

						}
					}
				}
				Sleep(10);
			}
		}
	}
	Sleep(200);
	GLOBAL.reset();
	do {
		Sleep(10);
	} while (GLOBAL.isClicked == false);
	GLOBAL.reset();
	cleardevice();
}
/// Establishs the Intro Dialogue
void QuestLog() {
	string Dialogue[] = {
		"RIBIT! Watch it!",
		"Oh, you there. . .",
		"Where are you headed?",
		" . . . ",
		"You don't say!?",
		"Once I was your age",
		"I jumped everywhere",
		"Many, many places",
		"Jumped over things",
		"Jumped around them",
		"Jumped over them",
		"Jumped under . . .",
		"but today. . .",
		"Well old frogs",
		"we could never make it.",
		"The world,",
		"it's a changing!",
		"Cars! Trucks!",
		"Why some of my best",
		"friends never made it",
		"to the Lily Pads",
		"YOU go out there and",
		"Show the world what",
		"Frogs are really made of",
		"Try not to get smashed,",
		"TIRE Tread lightly!",
		"RIBIT! *CHUCKLE",
		"Sorry Croak joke.",
		"You make a name",
		"for yourself",
		"Say. . . ",
		"What was your name?",
		" "
	};
	notfrogger.create(maxX / 2, maxY / 2 + 35);
	int x = 0;
	int y = notfrogger.bottom - 80;
	int limitX = notfrogger.left - 130;
	int sleepTimer = 350;
	frog.create(x, y);
	Sleep(sleepTimer);
	int addition = limitX / 5;
	for (int i = 0; i < 5; i++) {
		frog.remove();
		if (i % 2 == 0) {
			frog.repos((i + 1)*addition, y - 40);
		}
		else {
			frog.repos((i + 1)*addition, y);
		}
		Sleep(sleepTimer);
	}
	frog.remove();
	frog.repos(limitX, y);
	string NAME;
	int left, top, right, bottom;
	left = top = right = bottom = 0;
	int OFFSET = UNIT * 2;
	settextstyle(0, 0, 72);
	for (int i = 0; i < sizeof(Dialogue) / sizeof(string); i++) {
		NAME = "";
		for (int j = 0; j < (int)Dialogue[i].length(); j++) {
			NAME += Dialogue[i][j];
			setcolor(BLACK);
			bar(left, top, right, bottom);
			setcolor(WHITE);
			left = (maxX - textwidth(NAME.c_str())) / 2;
			right = left + textwidth(NAME.c_str());
			top = (maxY - OFFSET - textheight(NAME.c_str())) / 2;
			bottom = top + textheight(NAME.c_str());
			outtextxy(left, top, NAME.c_str());
			Sleep(85);
		}
		Sleep(1000);
	}
	Sleep(1000);
}
/// Draws Truck
void drawTruck(int x, int y, CARTYPE vehicle) {
	for (int i = 0; i < TRUCK_Height; i++)
		for (int j = 0; j < TRUCK_Width; j++) {
			if (TRUCK_ARRY[i][j] != 99)
				putpixel(x + j, y + i, adjust(vehicle, TRUCK_ARRY[i][j]));
		}
}
/// Draws Cars in reverse direction
void drawRevCar(int x, int y, CARTYPE vehicle) {
	for (int i = 0; i < CAR_Height; i++)
		for (int j = 0; j < CAR_Width; j++) {
			if (vehicle != MASTERCAR) {
				if (CAR_ARRY[i][j] != 99)
					putpixel(x + j, y + CAR_Height - i - 1, adjust(vehicle, CAR_ARRY[i][j]));
			}
			else
				if(MasterCAR_ARRY[i][j] != 99)
					putpixel(x + j, y + CAR_Height - i - 1, MasterCAR_ARRY[i][j]);
		}
}
/*x,y are the top left corner of the image*/ /// Draws all Cars 
void drawCar(int x, int y, CARTYPE vehicle) {
	for (int i = 0; i < CAR_Height; i++)
		for (int j = 0; j < CAR_Width; j++) {
			if (CAR_ARRY[i][j] != 99)
				putpixel(x + j, y + i, adjust(vehicle, CAR_ARRY[i][j]));
		}
}
/// Draws Single MasterCar Type
void drawMasterCar(int x, int y) {
	for (int i = 0; i < CAR_Height; i++)
		for (int j = 0; j < CAR_Width; j++) {
			if (MasterCAR_ARRY[i][j] != 99)
				putpixel(x + j, y + i, MasterCAR_ARRY[i][j]);
		}
	putpixel(0, 0, DARKGRAY);
}
/// NEW DRAW for all cars other than Reverse
void draw(int x, int y, CARTYPE vehicle) {
	switch (vehicle) {
	case MASTERCAR:
		drawMasterCar(x, y);
		return;
	case REDTRUCK:
	case BLUETRUCK:
	case YELLOWTRUCK:
		drawTruck(x, y, vehicle);
		return;
	case BLACKCAR:
	case GREENCAR:
	case REDCAR:
	case BLUECAR:
		drawCar(x, y, vehicle);
		return;
	}
}
#pragma endregion
/// LINE 1119