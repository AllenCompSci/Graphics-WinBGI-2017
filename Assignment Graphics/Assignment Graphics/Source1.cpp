#include<iostream>
#include<string>
#include<winbgi.cpp>
#include<graphics.h>
#include<ctime>
#include<Windows.h>
using namespace std;

int maxX, maxY;
int GrDriver, GrMode, ErrorCode;
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
bool evens(){
	return rand() % 2 == 0;
}
struct Shape {
	int count;
	float poiX, poiY;
	bool intersect;
	bool overlap;
	struct Point {
		int X;
		int Y;
		void init(int x, int y) {
			X = x;
			Y = y;
		}
	};
	struct Line {
		int x1, x2, y1, y2;
		float b;
		float m;
		int pencolor;
		void init() {
			x1 = rand() % (maxX - 35) + 15 + (rand() % 20);
			x2 = rand() % (maxX - 35) + 15 + (rand() % 20);
			y1 = rand() % (maxY - 35) + 15 + (rand() % 20);
			y2 = rand() % (maxY - 35) + 15 + (rand() % 20);
			m = (y2 - y1) / (float)(x2 - x1);
			// y = mx+b -> y - mx = b
			b = y1 - (m * x1);
			pencolor = rand() % 15 + 1;
		}
		void tick() {
			pencolor = rand() % 15 + 1;
		}
		void render() {
			setcolor(pencolor);
			line(x1, y1, x2, y2);
		}
	}line1, line2;
	struct Rect {
		int x, y, h, w;
		Point TopLeft, BottomRight;
		int bColor, outline;
		void construct(int xc, int yc, int r) {
			TopLeft.init(xc - r, yc - r);
			BottomRight.init(xc + r, yc + r);
		}
		void init() {
			x = (rand() % (maxX - 50)) + (rand() % 20) + 5;
			y = (rand() % (maxY - 50)) + (rand() % 20) + 5;
			w = (rand() % (maxX - 25 - x)) + (rand() % 20) + 5;
			h = (rand() % (maxY - 25 - y)) + (rand() % 20) + 5;
			bColor = rand() % 15 + 1;
			TopLeft.init(x, y);
			BottomRight.init(x + w, y + h);
			oL();
		}
		void tick() {
			oL();
		}
		void render() {
			setcolor(bColor);
			bar(x, y, x + w, y + h);
			setcolor(outline);
			rectangle(x, y, x + w, y + h);
		}
		void oL() {
			do {
				outline = rand() % 15 + 1;
			} while (outline != bColor);
		}
		bool intersect(Rect check) {
			/*
			TopLeft(x1,y1)   (x2,y1)
			___________________
			| A				 B|
			|				  |
			|				  |
			|				  |
			|				  |
			|_D______________C|
			(x1,y2)			BottomRight(x2,y2)
			*/
			Point corner[4];
			corner[0] = check.TopLeft;
			corner[1].init(check.BottomRight.X, check.TopLeft.Y);
			corner[2].init(check.TopLeft.X, check.BottomRight.Y);
			corner[3] = check.BottomRight;

			for (int i = 0; i < 4; i++) {
				if (corner[i].X >= TopLeft.X && corner[i].X <= BottomRight.X) {
					if (corner[i].Y >= TopLeft.Y && corner[i].Y <= BottomRight.Y)
						return true;
				}
				if (i % 2 == 0) {
					while (corner[i].X <= corner[i + 1].X) {
						corner[i].X += 1;
						if (corner[i].X >= TopLeft.X && corner[i].X <= BottomRight.X) {
							if (corner[i].Y >= TopLeft.Y && corner[i].Y <= BottomRight.Y)
								return true;
						}
					}
				}
				else if (i == 1) {
					while (corner[i].Y <= corner[i + 1].Y) {
						corner[i].Y += 1;
						if (corner[i].X >= TopLeft.X && corner[i].X <= BottomRight.X) {
							if (corner[i].Y >= TopLeft.Y && corner[i].Y <= BottomRight.Y)
								return true;
						}
					}
				}
			}
			return false;
		}
	}rect, rect2, rect3;
	struct Circle {
		Rect boundBox;
		int x, y, r;
		int dx, dy;
		int C;
		bool reset;
		void init() {
			x = maxX / 2;
			y = maxY / 2;
			set();
			boundBox.construct(x, y, r);
		}
		void set() {
			reset = false;
			r = rand() % 20 + 15;
			C = rand() % 15 + 1;
			dx = rand() % 20;
			if (evens())
				dx *= -1;
			dy = rand() % 20;
			if (evens())
				dy *= -1;
			if (dx == 0 && dx == dy) {
				set();
			}
		}
		void wall() {
			if (x - r < 5) {
				x = r + 5;
				reset = true;
			}
			if (x > (maxX - r - 3)) {
				x = maxX - r - 3;
				reset = true;
			}
			if (y - r < 3)
			{
				y = r+3;
				reset = true;
			}
			if (y >(maxY - r - 3)) {
				y = maxY - r + 3;
				reset = true;
			}
		}
		void tick() {
			if (reset)
				set();
			x += dx;
			y += dy;
			wall();
		}
		void render() {
			setcolor(C);
			fillellipse(x, y, r, r);
			boundBox.construct(x, y, r);
		}
	} circ;
	void Line_init() {
		line1.init();
		line2.init();
		intersect = false;
		lineCross(line1, line2);
		count = 0;
	}
	void Rect_init() {
		rect.init();
		rect2.init();
		rect3.init();
		overlap = check(rect, rect2, rect3);
	}
	void lineCross(Line l1, Line l2) {
		if (l1.m != l2.m) {
			// b2 + m2x = m1x + b1
			// b2 - b1 = (m1 - m2) x
			// x = (b2 - b1) / (m1 - m2)
			poiX = ((l2.b - l1.b) / (l1.m - l2.m));
			poiY = ((l1.m * poiX) + l1.b);
			intersect = contained((int)poiX, l1.x1, l1.x2) && contained((int)poiX, l2.x1, l2.x2);
		}
		if (intersect) {
			poi();
		}
	}
	void poi() {
		setcolor(14);
		fillellipse((int)poiX, (int)poiY, 7, 7);
	}
	bool check(Rect r1, Rect r2, Rect r3) {
		return r1.intersect(r3) || r2.intersect(r3) || r3.intersect(r1) || r3.intersect(r2) || r1.intersect(r2) || r2.intersect(r1);
	}
	bool contained(int p, int bound1, int bound2) {
		if (p == bound1 || p == bound2) {
			return true;
		}
		if (bound1 < bound2) {
			return bound1 < p && p < bound2;
		}
		return bound2 < p && p < bound1;
	}
	void init() {
		Line_init();
		circ.init();
		Rect_init();
	}
	void tick(Line &l1, Line &l2) {
		if (!intersect) {
			Line_init();
		}
		else {
			l1.tick();
			l2.tick();
			
		}
	}
	void render(Line l1, Line l2) {
		l1.render();
		l2.render();
		if (intersect)
			poi();
	}
	void tick(Rect &r1, Rect &r2, Rect &r3) {
		if (overlap) {
			Rect_init();
		}
		r1.tick();
		r2.tick();
		r3.tick();
	}
	void render(Rect r1, Rect r2, Rect r3) {
		r1.render();
		r2.render();
		r3.render();
	}
	void tick() {
		tick(line1, line2);
		circ.tick();
		tick(rect, rect2, rect3); 
		count++;
		if (count > 30) {
			Line_init();
			Rect_init();
		}
		// sort of checks collision between circle and rectangles
		//circ.reset = circ.reset || circ.boundBox.intersect(rect) || circ.boundBox.intersect(rect2) || circ.boundBox.intersect(rect3);

	}
	void render() {
		cleardevice();
		render(line1, line2);
		circ.render();
		render(rect, rect2, rect3);
		Sleep(40);
	}
}assignment;
void main() {
	srand((unsigned int)time(NULL));
	gr_Start(GrDriver, GrMode, ErrorCode);
	assignment.init();
	while (true) {
		assignment.tick();
		assignment.render();
		Sleep(35);
	}
}