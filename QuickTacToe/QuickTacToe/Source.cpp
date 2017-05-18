#include "Functions.h"

void main() {
	gr_Start(GrDriver, GrMode, ErrorCode);
	test.create((maxX - UNIT * 3) / 2 , (maxY - UNIT * 3) / 2);
	test.currValue = O;
	test.draw(BLUE);
	int t = getch();
	test.currValue = Empty;
	test.draw(GREEN);
	t = getch();
	test.currValue = X;
	test.draw(RED);
	t = getch();
}