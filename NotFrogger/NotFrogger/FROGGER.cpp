/*
Frogger Games Clean
*/
#include "RPGUtility.h"
int main() {

	//debug = false; /// TO TEST CODE COMMENT THIS OUT THIS IS THE INTRO FOR FROGGER RPG
	/* CREATE THE PIXEL MAPS
	//CREATE("frog.bmp"); // To Create Froger PixelMap /// 
	//CREATE("MAINSCREEN.bmp"); // To Create WiseFrog PixelMap //
	//CREATE("LOG.bmp"); // To Create LOG (PIXELMAP) // Prescaled /// LOG GARYcXJK
	//CREATE("lily.bmp");
	*/
	CREATE("chicken.bmp");
	thread FROGGER(game);
	thread LISTENER(Listener);
	FROGGER.join();
	LISTENER.join();
	system("pause");
}
/// LINE 19 
