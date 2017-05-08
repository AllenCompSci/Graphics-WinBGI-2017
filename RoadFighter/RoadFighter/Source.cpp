#include <vector>
#include <string>
#include <Windows.h> // KEY PRESS MOUSE LISTENER NECESSITY 
#include "Pixel.h"

void main() {
	int GrDriver, GrMode;
	initgraph(&GrDriver, &GrMode, "");
	setcolor(DARKGRAY);
	int x = 2;
while(true){
	bar(0, 0, getmaxx(), getmaxy());

	draw((getmaxx() - CAR_Width) / 2, (getmaxy() - CAR_Height) / 2, BLUECAR);
	getch();
	bar(0, 0, getmaxx(), getmaxy());

	draw((getmaxx() - CAR_Width) / 2, (getmaxy() - CAR_Height) / 2, GREENCAR);
	getch();
	bar(0, 0, getmaxx(), getmaxy());

	draw((getmaxx() - CAR_Width) / 2, (getmaxy() - CAR_Height) / 2, REDCAR);
	getch();
	bar(0, 0, getmaxx(), getmaxy());

	draw((getmaxx() - CAR_Width) / 2, (getmaxy() - CAR_Height) / 2, BLACKCAR);
	getch();
	bar(0, 0, getmaxx(), getmaxy());

	draw((getmaxx() - CAR_Width) / 2, (getmaxy() - CAR_Height) / 2, MASTERCAR);
	getch();

	bar(0, 0, getmaxx(), getmaxy());
	draw((getmaxx() - TRUCK_Width) / 2, (getmaxy() - TRUCK_Height) / 2, REDTRUCK);
	getch();

	bar(0, 0, getmaxx(), getmaxy());
	draw((getmaxx() - TRUCK_Width) / 2, (getmaxy() - TRUCK_Height) / 2, BLUETRUCK);
	getch();
	bar(0, 0, getmaxx(), getmaxy());
	chickenMOVE((getmaxx() - chicken_Width) / 2, (getmaxy() - chicken_Height) / 2);
	getch();
	bar(0, 0, getmaxx(), getmaxy());

	greatChicken((getmaxx() - chicken_Width*x) / 2, (getmaxy() - chicken_Height*x) / 2, x);
	x++;
	getch();
	setcolor(DARKGRAY);
}
	system("pause");
    
}