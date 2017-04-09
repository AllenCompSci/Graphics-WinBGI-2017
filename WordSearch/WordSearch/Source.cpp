#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <windows.h>

const int MAXSIZE = 20;
using namespace std;

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT, 
	UP_RIGHT, 
	DOWN_LEFT, 
	DOWN_RIGHT
};

struct Point {
	int row;
	int col;
	void init(int i, int j) {
		row = i;
		col = j;
	}
	void init(Point gen) {
		row = gen.getRow();
		col = gen.getCol();
	}
	int getRow() {
		return row;
	}
	int getI() {
		return row;
	}
	int getCol() {
		return col;
	}
	int getJ() {
		return col;
	}
};
struct PUZZLEWORD {
	Point begin;
	direction dir;
	string word;
	void init(Point t, direction layout, string str) {
		update(t, layout);
		word = str;
	}
	void update(Point t) {
		begin.init(t);
	}
	void update(direction layout) {
		dir = layout;
	}
	void update(Point t, direction layout) {
		update(t);
		update(layout);
	}
	bool existsWithin(int i, int j) {
		int startI = begin.getI();  /// ROW
		int startJ = begin.getJ();  /// COL
		switch (dir) {
		case UP:
			if (j == startJ) {
				if (startI >= i && i >= (startI - ((int)word.length() - 1))) {
					return true;
				}
			}
			return false;
		case DOWN:
			if (j == startJ) {
				if (startI <= i && i <= (startI - ((int)word.length() - 1))) {
					return true;
				}
			}
			return false;
		case LEFT:
			if (i == startI) {

			}
			return false;
		case RIGHT:
			if (i == startI) {

			}
			return false;
		}
		return false;
	}
	bool existsWithin(Point curr) {
		return existsWithin(curr.getI(), curr.getJ());
	}
	
};
class Generator {
	string filePath;
	string rescFolder;
	char ** wordsearch;
	int h, w;
	vector<string> words;
	vector<string> resourcefiles;
	vector<string> categories;
public :
	Generator();
	void rescManage(string fileName);
	char randCharGen();
};

Generator::Generator() { // called at declation of object without any necessary statements.
	cout << "CREATION \n";
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	cout << buffer << endl;
	string::size_type pos = string(buffer).find_last_of("\\/");
	filePath = string(buffer).substr(0, pos);
	cout << filePath << endl;
}
void Generator::rescManage(string fileName) {

}
char Generator::randCharGen() {
	return 'A' + (rand() % 26);
}

void main() {
	srand((unsigned int)time(NULL));

	Generator gen;
	system("pause");
	gen.rescManage("t");
	system("pause");

}