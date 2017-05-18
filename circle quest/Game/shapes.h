#pragma once
#ifndef SHAPES_H
#define SHAPES_H

#include <thread>

thread t1(leftright);
thread t2(movement);




void gr_start(int&, int&, int&);

void displayv();

bool KEYBOARD(int);

void genprops();
void movement();
void plats();
void squish();
void leftright();











#endif 