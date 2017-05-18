#include "Record.h"
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode) {
	//set the graphics driver
	GrDriver = VGA; //800x600 by monitor
	GrMode = VGAMAX;//set graphics mode
	initgraph(&GrDriver, &GrMode, "");//start graphics
									  //check for problems
	ErrorCode = graphresult();
	if (ErrorCode != grOk)
	{
		cout << "Error:" << ErrorCode;
	}
	else {
		maxX = getmaxx();
		maxY = getmaxy();
	}
}
bool ActionPress(int VirtualKey) {
	return ((GetAsyncKeyState(VirtualKey) & 0x8000) != 0);
}
void TicTacToe() {
	gr_Start(GrDriver, GrMode, ErrorCode);
	bool changeStates = true;
	int GAMESEQ = 0;
	int seq = 0;
	GAME = Start_Menu;
	while (isRunning) {
		switch (GAME) {
		case Start_Menu:
			if (changeStates) {
				seq = 0;
				GAMESEQ = 0;
				test.value = -1;
				test.create((maxX - UNIT * 3) / 2, (maxY - UNIT * 3) / 2);
				changeStates = false;
			}
			cleardevice();
			if (GAMESEQ == 0) {
				test.value = -1;
				test.currValue = X;
			}
			if (GAMESEQ == 1) {
				test.currValue = Empty;
				test.value = 99;
			}
			if (GAMESEQ == 2) {
				test.value = -1;
				test.currValue = O;
				seq++;
			}
			if (GAMESEQ == 3) {
				test.currValue = Empty;
				test.value = 100;
				GAMESEQ = -1;
				seq++;
			}
			test.draw();
			putpixel(0, 0, 0);
			
			GAMESEQ++;
			Sleep(450);
			if (seq == 7) {
				changeStates = true;
				GAME = Game;
			}
			break;
		case Game:
			if (changeStates)
			{
				Input.reset();
				grid.init();
				changeStates = false;
				GAMESEQ = 0;
			}

			if (Input.isPressed) {
				bool MOUSE = false;
				int VAL = -1;
				if (Input.VirtualKey == VK_LBUTTON) {
					Input.getClick();
					MOUSE = true;
				}
				else if (Input.VirtualKey == VK_1 || Input.VirtualKey == VK_NUMPAD1) {
					VAL = 1;
				}
				else if (Input.VirtualKey == VK_2 || Input.VirtualKey == VK_NUMPAD2) {
					VAL = 2;
				}
				else if (Input.VirtualKey == VK_3 || Input.VirtualKey == VK_NUMPAD3) {
					VAL = 3;
				}
				else if (Input.VirtualKey == VK_4 || Input.VirtualKey == VK_NUMPAD4) {
					VAL = 4;
				}
				else if (Input.VirtualKey == VK_5 || Input.VirtualKey == VK_NUMPAD5) {
					VAL = 5;
				}
				else if (Input.VirtualKey == VK_6 || Input.VirtualKey == VK_NUMPAD6) {
					VAL = 6;
				}
				else if (Input.VirtualKey == VK_7 || Input.VirtualKey == VK_NUMPAD7) {
					VAL = 7;
				}
				else if (Input.VirtualKey == VK_8 || Input.VirtualKey == VK_NUMPAD8) {
					VAL = 8;
				}
				else if (Input.VirtualKey == VK_9 || Input.VirtualKey == VK_NUMPAD9) {
					VAL = 9;
				}
				else {
					Input.reset();
				}
				if (Input.isPressed) {
					for(int i = 0; i < SQR_GRID; i++)
						for(int j = 0; j < SQR_GRID; j++)
							if (MOUSE && grid.board[i][j].isClicked(Input.click) || grid.board[i][j].isPressed(VAL)) {
								grid.board[i][j].SWITCH();
								GAMESEQ++;
								if (grid.check()) {
									GAME = GameOver;
									changeStates = true;
								}
								else if (GAMESEQ == 9) {
									TURN = Empty;
									GAME = GameOver;
									changeStates = true;
								}
								else {
									grid.ENDTurn();
								}
							}
				}
				Input.reset();

			}

			break;
		case GameOver:
			if (changeStates) {
				cleardevice();
				string temp = (TURN == X) ? "X Wins" : (TURN == O) ? "O Wins" : " DRAW! ";
				settextstyle(0, 0, 72);
				int x = (grid.screenX - textwidth(temp.c_str())) / 2;
				int y = (grid.screenY - textheight(temp.c_str())) / 2;
				outtextxy(x, y, temp.c_str());
			}
			Input.reset();
			do {
				Sleep(10);
			} while (Input.isPressed == false);
			GAME = Start_Menu;
			break;
		}
		Sleep(10);
	}
}
void Listener() {
	Input.reset();
	while (isRunning) {
		if (Input.isPressed == false) {
			if (ActionPress(VK_LBUTTON)) {
				Input.VirtualKey = VK_LBUTTON;
				Input.isPressed = true;
			}
			if (ActionPress(VK_1) || ActionPress(VK_NUMPAD1)) {
				Input.VirtualKey = VK_1;
				Input.isPressed = true;
			}
			if (ActionPress(VK_2) || ActionPress(VK_NUMPAD2)) {
				Input.VirtualKey = VK_2;
				Input.isPressed = true;
			}
			if (ActionPress(VK_3) || ActionPress(VK_NUMPAD3)) {
				Input.VirtualKey = VK_3;
				Input.isPressed = true;
			}
			if (ActionPress(VK_4) || ActionPress(VK_NUMPAD4)) {
				Input.VirtualKey = VK_4;
				Input.isPressed = true;
			}
			if (ActionPress(VK_5) || ActionPress(VK_NUMPAD5)) {
				Input.VirtualKey = VK_5;
				Input.isPressed = true;
			}
			if (ActionPress(VK_6) || ActionPress(VK_NUMPAD6)) {
				Input.VirtualKey = VK_6;
				Input.isPressed = true;
			}
			if (ActionPress(VK_7) || ActionPress(VK_NUMPAD7)) {
				Input.VirtualKey = VK_7;
				Input.isPressed = true;
			}
			if (ActionPress(VK_8) || ActionPress(VK_NUMPAD8)) {
				Input.VirtualKey = VK_8;
				Input.isPressed = true;
			}
			if (ActionPress(VK_9) || ActionPress(VK_NUMPAD9)) {
				Input.VirtualKey = VK_9;
				Input.isPressed = true;
			}
		}
		Sleep(15);
	}
}