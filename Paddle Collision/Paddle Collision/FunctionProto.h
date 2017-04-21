#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <ctime>
#include <Windows.h> // KEY PRESS MOUSE LISTENER NECESSITY 
#include <thread>
#include <mmsystem.h> /// WAV
#include <mciapi.h> /// MP3
#pragma comment(lib, "winmm.lib") /// WAV also maybe MP3
using namespace std;
void gr_Start(int &GrDriver, int &GrMode, int &ErrorCode);
void PLAYER1();
void MOUSE();
void PADDLEGAME();
void BACKGROUND();
void getCursor(POINT &p, int VirtualKey);
bool mousePress(int);
bool KEYBOARD1(int);
void playSound(string, int);
void playSoundSFX(string, int);
int level1();
POINT pitBrick(int left, int top, int right, int bottom);
POINT crackBrick(int left, int top, int right, int bottom);
void pitBrick(int left, int top, int right, int bottom, int randomX, int randomY);
void crackBrick(int left, int top, int right, int bottom, int randomX, int randomY);