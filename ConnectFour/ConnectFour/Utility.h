#include "Proto.h"

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
/// AsynKeyState
bool ActionListener(int VirtualKey) {
	return ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
}
/// LOAD BMP Call Function
void CREATE(string test) {
	w = 0;
	h = 0;
	/*Copy all the vector values color.at() HERE*/
#pragma region example
	
	colors.at(0).init(0, 0, 0, BLACK);
	colors.at(1).init(255, 255, 255, 99);
	colors.at(2).init(255, 0, 0, RED);
	colors.at(3).init(168, 39, 39, BROWN);
	colors.at(4).init(240, 240, 240, LIGHTGRAY);
	colors.at(5).init(49, 78, 80, DARKGRAY);
	colors.at(6).init(255, 0, 255, MAGENTA);
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
	int x = maxX / 2;
	int y = 100;
	int radius = 45;
	string temp = "";
	bool cards = false;
	if (cards) {
		for (int k = 0; k < 39; k++) {
			setcolor(WHITE);
			bar(0, 0, maxX, maxY);
			drawMinion((Minion)k, (maxX - IMGWidth) / 2, (maxY - IMGHeight) / 2);
			getch();
			if (k < 19) {
				bar(0, 0, maxX, maxY);
				drawToken((Token)k, (maxX - IMGWidth) / 2, (maxY - IMGHeight) / 2);
				getch();
			}
			if (k < 26) {
				bar(0, 0, maxX, maxY);
				drawSpell((Spell)k, (maxX - IMGWidth) / 2, (maxY - IMGHeight) / 2);
				getch();
			}

		}
	}
	GAME.gamesetup();
	GAME.drop((Column)0, Red);
	GAME.drop((Column)1, Black);
	GAME.drop((Column)1, Red);
	GAME.drop((Column)1, Black);
	GAME.drop((Column)1, Red);
	GAME.drop((Column)1, Black);
	GAME.drop((Column)1, Red);
	GAME.drop((Column)1, Black);
	Turn = Red;
	for (int i = 0; i < 99; i++) {
		GAME.drop((Column)(rand() % 8), Turn);
		if (GAME.win(Turn)) {
			temp = (Turn == Red) ? "RED" : "BLACK";
			cout << temp << " wins!!! ";
			getch();
			GAME.gamesetup();
		}
		if (Turn == Red) {
			Turn = Black;
		}
		else {
			Turn = Red;
		}
		getch();
	}
	/*
	int y = 100;
	int x = maxX / 2;
	setcolor(YELLOW);
	bar(0, 0, maxX, maxY);
	setcolor(BLACK);
	int UNIT = 100;
	x = (maxX - 800) / 2;

	for (i = 0; i < 8; i++)
		for (int j = 1; j < 7; j++) {
			rectangle(x + i*UNIT, y + j*UNIT, x + (i + 1)*UNIT, y + (j + 1)*UNIT);
			draw(x + 50 + i*UNIT, y + 50 + j*UNIT, 45, BLACK);
		}

	*/
	/*
	while (isRunning) {

	}
	*/
	system("pause");
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
///
/// Draws Intro
void INTRO() {

}
/// Distance Forumla
float distance(int x1, int y1, int x2, int y2) {
	return abs(sqrt((float)(pow((float)(x2 - x1),2.0)) + (float)(pow((float)(y2 - y1), 2.0))));
}
/// DRAW CIRCLE
void draw(int centerX, int centerY, int Radius, int color) {
	for (int i = -Radius; i <= Radius; i++)
		for (int j = -Radius; j <= Radius; j++) {
			if (Radius >= distance(centerX, centerY, centerX + i, centerY + j)) {
				putpixel(centerX + i, centerY + j, color);
			}
		}
}
void boundedCircle(int centerX, int centerY, int Radius, int color, int topX, int topY, int bottomX, int bottomY) {
	for (int i = -Radius; i <= Radius; i++)
		for (int j = -Radius; j <= Radius; j++) {
			if (Radius >= distance(centerX, centerY, centerX + i, centerY + j) && (Radius >= distance(topX, topY, centerX +i, centerY +j)) || (Radius >= distance(centerX, centerY, centerX + i, centerY + j)) && (Radius >= distance(bottomX, bottomY, centerX +i, centerY + j))){
				putpixel(centerX + i, centerY + j, color);
			}
		}
}
void topCircle(int centerX, int centerY, int Radius, int color, int topX, int topY, int bottomX, int bottomY) {
	for (int i = -Radius; i <= Radius; i++)
		for (int j = -Radius; j <= Radius; j++) {
			if (Radius >= distance(centerX, centerY, centerX + i, centerY + j) && (centerY + j < (topY + 50) && centerY + j > topY - 50)  || (Radius >= distance(centerX, centerY, centerX + i, centerY + j)) && (Radius >= distance(bottomX, bottomY, centerX + i, centerY + j))) {
				putpixel(centerX + i, centerY + j, color);
			}
		}
}
void cpyCard(const int tbC[][IMGWidth]) {
	for (int i = 0; i < IMGHeight; i++)
		for (int j = 0; j < IMGWidth; j++)
			BLANKCARD[i][j] = tbC[i][j];
}
void drawCard(int x, int y) {
	for (int i = 0; i < IMGHeight; i++)
		for (int j = 0; j < IMGWidth; j++)
			if (BLANKCARD[i][j] != 99)
				putpixel(x + j, y + i, BLANKCARD[i][j]);
}
void drawMinion(Minion card, int x, int y) { // 38
	switch (card) {
	case Antasma:
		cpyCard(Antasma_ARRY);
		break;
	case Bill_Blaster:
		cpyCard(BillBlaster_ARRY);
		break;
	case Block:
		cpyCard(block_ARRY);
		break;
	case Boo:
		cpyCard(Boo_ARRY);
		break;
	case Boomarang_Bro:
		cpyCard(BoomarangBro_ARRY);
		break;
	case Bowser_Jr:
		cpyCard(BowserJr_ARRY);
		break;
	case Bowser_Statue:
		cpyCard(BowserStatue_ARRY);
		break;
	case Captain_Toad:
		cpyCard(CaptainToad_ARRY);
		break;
	case Cataquak:
		cpyCard(CheepCheep_ARRY);
		break;
	case Cheep_Cheep:
		cpyCard(CheepCheep_ARRY);
		break;
	case Dry_Bones:
		cpyCard(DryBowser_ARRY);
		break;
	case Dry_Bowser:
		cpyCard(DryBowser_ARRY);
		break;
	case Fawful:
		cpyCard(Fawful_ARRY);
		break;
	case FireBro:
		cpyCard(FireBro_ARRY);
		break;
	case Fuzzy:
		cpyCard(fuzzy_ARRY);
		break;
	case Goomba:
		cpyCard(Goomba_ARRY);
		break;
	case Goomba_King:
		cpyCard(GoombaKing_ARRY);
		break;
	case Goomba_Tower:
		cpyCard(GoombaTower_ARRY);
		break;
	case Green_Koopa:
		cpyCard(GreenKoopa_ARRY);
		break;
	case Hammer_Bro:
		cpyCard(hammerbro_ARRY);
		break;
	case Honey_Queen:
		cpyCard(HoneyQueen_ARRY);
		break;
	case King_Boo:
		cpyCard(KingBoo_ARRY);
		break;
	case Koopatrol:
		cpyCard(Koopatrol_ARRY);
		break;
	case Lakitu:
		cpyCard(Lakitu_ARRY);
		break;
	case Luma:
		cpyCard(Luma_ARRY);
		break;
	case Magikoopa:
		cpyCard(Magikoopa_ARRY);
		break;
	case Mini_Goomba:
		cpyCard(MiniGoomba_ARRY);
		break;
	case Para_Goomba:
		cpyCard(Paragoomba_ARRY);
		break;
	case Pipe_Pirahna_Plant:
		cpyCard(PipePirahnaPlant_ARRY);
		break;
	case Pokey:
		cpyCard(pokey_ARRY);
		break;
	case Red_Koopa:
		cpyCard(RedKoopa_ARRY);
		break;
	case Rocky_Wench:
		cpyCard(RockyWrench_ARRY);
		break;
	case Shy_Guy:
		cpyCard(shyGuy_ARRY);
		break;
	case Sledge_Bro:
		cpyCard(SledgeBro_ARRY);
		break;
	case Toad:
		cpyCard(toad_ARRY);
		break;
	case Toadette:
		cpyCard(Toadette_ARRY);
		break;
	case Toadsworth:
		cpyCard(Toadsworth_ARRY);
		break;
	case Wiggler:
		cpyCard(Wiggler_ARRY);
		break;
	case Yoshi_Egg:
		cpyCard(YoshiEgg_ARRY);
		break;
	}
	drawCard(x, y);
}
void drawToken(Token card, int x, int y) { // 18
	switch (card) {
	case Ball:
		cpyCard(Ball_ARRY);
		break;
	case Bat:
		cpyCard(Bat_ARRY);
		break;
	case Bullet_Bill:
		cpyCard(BulletBill_ARRY);
		break;
	case Coin:
		cpyCard(coin_ARRY);
		break;
	case Green_Shell:
		cpyCard(GreenShell_ARRY);
		break;
	case Iggy:
		cpyCard(Iggy_ARRY);
		break;
	case Larry:
		cpyCard(Larry_ARRY);
		break;
	case Lava_Bubble:
		cpyCard(LavaBubble_ARRY);
		break;
	case Lemmy:
		cpyCard(Lemmy_ARRY);
		break;
	case Ludwig_Von_Koopa:
		cpyCard(LudwigVonKoopa_ARRY);
		break;
	case Morton:
		cpyCard(morton_ARRY);
		break;
	case Red_Shell:
		cpyCard(RedShell_ARRY);
		break;
	case Ring:
		cpyCard(Ring_ARRY);
		break;
	case Roy:
		cpyCard(Roy_ARRY);
		break;
	case Sonic:
		cpyCard(Sonic_ARRY);
		break;
	case Spiny:
		cpyCard(Spiny_ARRY);
		break;
	case War_Toad:
		cpyCard(WarToad_ARRY);
		break;
	case Wendy:
		cpyCard(Wendy_ARRY);
		break;
	case Yoshi:
		cpyCard(Yoshi_ARRY);
		break;
	}
	drawCard(x, y);
}
void drawSpell(Spell card, int x, int y) { // 25
	switch (card) {
	case OneUp:
		cpyCard(ONEup_ARRY);
		break;
	case Blue_Shell:
		cpyCard(BlueShell_ARRY);
		break;
	case Castle:
		cpyCard(Castle_ARRY);
		break;
	case Damsel_in_Distress:
		cpyCard(DamselinDistress_ARRY);
		break;
	case Dr_Mario_Pills:
		cpyCard(DrMarioPills_ARRY);
		break;
	case Fire_Flower:
		cpyCard(FireFlower_ARRY);
		break;
	case FLUDD:
		cpyCard(FLUDD_ARRY);
		break;
	case Fused_Bombomb:
		cpyCard(FusedBombomb_ARRY);
		break;
	case Fuzzy_Maddness:
		cpyCard(FuzzyMadness_ARRY);
		break;
	case Gotta_Go_Fast:
		cpyCard(GottaGoFast_ARRY);
		break;
	case Hammer:
		cpyCard(Hammer_ARRY);
		break;
	case Haunted_Mansion:
		cpyCard(HauntedMansion_ARRY);
		break;
	case Koopalings_Mischief:
		cpyCard(KoopalingsMischief_ARRY);
		break;
	case LavaPool:
		cpyCard(LavaPool_ARRY);
		break;
	case Lightning:
		cpyCard(Lightning_ARRY);
		break;
	case Marios_Cap:
		cpyCard(MariosCap_ARRY);
		break;
	case Mega_Mushroom:
		cpyCard(MegaMushroom_ARRY);
		break;
	case Metal_Cap:
		cpyCard(MetalCap_ARRY);
		break;
	case POW_Block:
		cpyCard(POWBlock_ARRY);
		break;
	case Question_Block:
		cpyCard(QuestionBlock_ARRY);
		break;
	case Super_Mushroom:
		cpyCard(SuperMushroom_ARRY);
		break;
	case Super_Star:
		cpyCard(SuperStar_ARRY);
		break;
	case Thwomp_Stomp:
		cpyCard(ThwompStomp_ARRY);
		break;
	case Warios_Bike:
		cpyCard(WariosBike_ARRY);
		break;
	case Wassup_Bro:
		cpyCard(WassupBro_ARRY);
		break;
	case Whomp_Plomp:
		cpyCard(WhompPlomp_ARRY);
		break;
	}
	drawCard(x, y);
}