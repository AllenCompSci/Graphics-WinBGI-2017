#include "Utility.h"

void main() {
	thread Connect4(game); /// CHECK GAME PROTOCHECK 
	thread KeyListener(Listener);
	KeyListener.join();
	Connect4.join();
}