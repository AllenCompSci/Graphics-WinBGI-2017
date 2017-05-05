/*
Frogger Games Clean
*/

#include "LoadBMP.h"
int main() {
	//CREATE("frog.bmp"); // To Create Froger PixelMap
	//CREATE("MAINSCREEN.bmp"); // To Create WiseFrog PixelMap
	//CREATE("LOG.bmp"); // To Create LOG (PIXELMAP) // Prescaled
	thread FROGGER(game);
	thread LISTENER(Listener);
	FROGGER.join();
	LISTENER.join();
	system("pause");
}
