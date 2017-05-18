#include "Functions.h"

void main() {
	
	thread TICTACTOE(TicTacToe);
	thread KeyPress(Listener);
	TICTACTOE.join();
	KeyPress.join();

}