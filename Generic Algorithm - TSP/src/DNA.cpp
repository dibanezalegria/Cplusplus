#include "DNA.h"
#include <ctime>
#include <cmath>
#include <algorithm>
using namespace std;

// static members
float DNA::x_city[N];
float DNA::y_city[N];

// constants members 
const bool DNA::ELITISM = true;				// best fitness path moves to next gen
const float DNA::PARENTS = 0.50f;			// % population allowed to reproduce 
const float	DNA::CROSSOVER = 0.90f;			// % crossovers
const float DNA::MUTATION = 0.10f;			// % mutations
const int DNA::PERCENT_GOAL = 70;			// algorithm stops when path length reaches 
														// %PERCENT_GOAL of the best initial path

DNA::DNA() {
	// fonts
	mMtxFont = new char[128][7][5];
	initMtxFont();
	// screen dimensions
	mW = 1272, mH = 732;	
	// seed
	srand( (unsigned int)time(NULL) );
	
	// assign coordinates to cities
	resetCity();

	// initialize matrix with M random paths
	initializeMatrix();
}

DNA::~DNA() {
	delete [] mMtxFont;
}

void DNA::resetCity() {
	// cities location 
	for (int i = 0; i < N; ++i) {
		x_city[i] = (float)rand() / RAND_MAX * 1000;	 	
		y_city[i] = (float)rand() / RAND_MAX * 500;
	}
}

void DNA::initializeMatrix() {
	// initialize each path with numbers from 0 to N-1
	for (int m = 0; m < M; ++m) 
		for (int n = 0; n < N; ++n) 
			mC[m][n] = n;

	// randomize each path from n[1] (n[0] is the starting point)
   int temp, index;
	for (int m = 0; m < M; ++m) 
		for (int n = 1; n < N; ++n) {
			index = rand() % (N - 1) + 1;	
			temp = mC[m][n];
			mC[m][n] = mC[m][index];
			mC[m][index] = temp;
		}

	// calculate best initial path
	sortMatrixByFx();
	initialBest = calculateFx(mC[0]);
}

void DNA::copyRow(int* from, int* to) {
	for (int n = 0; n < N; ++n) 
		to[n] = from[n];
}

float DNA::calculateFx(int* ptr) {
	float total = 0;

	// from starting point to next city until last city in path
	for (int i = 0; i < N - 1; ++i) {
		total += sqrt((x_city[ptr[i]] - x_city[ptr[i + 1]]) * (x_city[ptr[i]] - x_city[ptr[i + 1]]) + 
			(y_city[ptr[i]] - y_city[ptr[i + 1]]) * (y_city[ptr[i]] - y_city[ptr[i + 1]]));
	}

	// from last city to starting point
	total += sqrt((x_city[ptr[N - 1]] - x_city[ptr[0]]) * (x_city[ptr[N - 1]] - x_city[ptr[0]]) + 
			(y_city[ptr[N - 1]] - y_city[ptr[0]]) * (y_city[ptr[N - 1]] - y_city[ptr[0]]));

	return total;
}

int DNA::comparator(const void* ptrA, const void* ptrB) {
	int *arg1 = (int*)(ptrA);
   int *arg2 = (int*)(ptrB);
	return calculateFx(arg1) > calculateFx(arg2);
}

// sort paths by fitness 
void DNA::sortMatrixByFx() {
	qsort(mC, sizeof(mC)/sizeof(mC[0]), sizeof(mC[0]), comparator);
}

void DNA::evolutionaryStep() {
	//sortMatrixByFx();
	int mParents[M][N]; 
	
	// generate pool of parents with only the fittest
	int nfit= (int)(M * PARENTS);
	for (int m = 0; m < M; ++m) 
		copyRow(mC[m % nfit], mParents[m]);
	
	// elitism
	int nelit = 0;
	if (ELITISM) {
		copyRow(mParents[0], mC[0]);
		nelit = 1;
	}
		
	// crossovers and mutations
	for (int m = nelit; m < M; ++m) {
		int* p0 = mParents[rand() % M];
		int* p1 = mParents[rand() % M];
	
		if (prob(CROSSOVER)) 
			crossover(p0, p1);
		
		if (prob(MUTATION)) 
			mutate(p0);
	
		copyRow(p0, mC[m]);
	}
}

bool DNA::prob(float percent) {
	int p = rand() % 100 + 1;
	if (p <= (percent * 100))
		return true;
	else 
		return false;
}

void DNA::mutate(int* ptr) {
	int pos0 = rand() % (N - 1) + 1;	// starting point at 0!
	int pos1 = rand() % (N - 1) + 1;
	int temp = ptr[pos0];
	ptr[pos0] = ptr[pos1];
	ptr[pos1] = temp;
}

void DNA::crossover(int* ptrA, int* ptrB) {
	// check if paths to cross are the same
	if (ptrA == ptrB)
		return;

	int segment = 2;	// length of the segment to swap
	int pos = rand() % (N - 1) + 1;	// starting point at 0!
	
	// check collisions in A
	int colA[N];
	int icol = 0;
	int b = pos;
	while (b < pos + segment && b < N) {
		for (int a = 0; a < N; ++a) {		
			// check collision excluding range to be swapped
			if (ptrA[a] == ptrB[b] && 
				(a < pos || a >= pos + segment)) {	
				ptrA[a] = -1;
				colA[icol++] = ptrB[b];
			}
		}
		b++;
	}
	// check collision in B
	int colB[N];
	icol = 0;
	int a = pos;
	while (a < pos + segment && a < N) {
		for (int b = 0; b < N; ++b) {			
			// check collision excluding range to be swapped
			if (ptrA[a] == ptrB[b] &&
				(b < pos || b >= pos + segment)) {	// collision
				ptrB[b] = -1;
				colB[icol++] = ptrA[a];
			}
		}
		a++;
	}
	// swap
	int temp;
	a = pos;
	while (a < pos + segment && a < N) {
		temp = ptrA[a];
		ptrA[a] = ptrB[a];
		ptrB[a] = temp;
		a++;
	}
	// remove Hs
	icol = 0;
	for (int a = 0; a < N; ++a) 
		if (ptrA[a] == -1) 
			ptrA[a] = colB[icol++];
		
	icol = 0;
	for (int b = 0; b < N; ++b) 
		if (ptrB[b] == -1)
			ptrB[b] = colA[icol++];

}

void DNA::changeSize(int w, int h) {
	mW = (float)w, mH = (float)h;
	glViewport(0, 0, w, h);										// Reset Viewport
	glMatrixMode(GL_PROJECTION); glLoadIdentity();		// Reset The Projection Matrix
	glOrtho(0.0f, w, h, 0.0f, -1.0f, 1.0f);	         // Create Ortho View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();		// Reset The Modelview Matrix
}

void DNA::draw(int m, int generation, int percentage) {
	glClear(GL_COLOR_BUFFER_BIT);
	//--------------------------------------------Points
	glPointSize(10.0);
	glBegin(GL_POINTS); 
	glColor3ub(0, 255, 0);
	glVertex2f(x_city[mC[m][0]], y_city[mC[m][0]]);
	glColor3ub(0, 0, 255);
	for (int n = 1; n < N; ++n) 
		glVertex2f(x_city[mC[m][n]], y_city[mC[m][n]]);
	glEnd();
	//-------------------------------------------- Lines
	glLineWidth(3.0);
	glColor3ub(255, 0, 0);
	for (int n = 0; n < N - 1; ++n) {
		glBegin(GL_LINES);
		glVertex2f(x_city[mC[m][n]], y_city[mC[m][n]]);
		glVertex2f(x_city[mC[m][n + 1]], y_city[mC[m][n + 1]]);
		glEnd();
	}

	glBegin(GL_LINES);
	glVertex2f(x_city[mC[m][N - 1]], y_city[mC[m][N - 1]]);
	glVertex2f(x_city[mC[m][0]], y_city[mC[m][0]]);
	glEnd();
	
	
	//--------------------------------------------Text
	float a = mW - 600;
	float b = mH - 180;

	glColor3ub(0, 0, 255);

	if (percentage > PERCENT_GOAL) {
		glColor3ub(0, 0, 255);
		draw_MtxText(a, b + 60, "SHOWING BEST PATH FOR GENERATION:");
		glColor3ub(100, 128, 250);
		draw_MtxText(a + 480, b + 60, "%3d", generation);
		glColor3ub(100, 128, 250);
		draw_MtxText(a, b + 30, "-- PERCENT GOAL %2d", PERCENT_GOAL);
		glColor3ub(0, 0, 255);
		draw_MtxText(a, b + 90, "PATH LENGTH:");
		glColor3ub(100, 128, 250);
		draw_MtxText(a + 180, b + 90, "%5d", (int)calculateFx(mC[m]));
		glColor3ub(0, 0, 255);
		draw_MtxText(a, b + 120, "THIS PATH IS");
		glColor3ub(100, 128, 250);
		draw_MtxText(a + 180, b + 120, "%2d", percentage);
		glColor3ub(0, 0, 255);
		draw_MtxText(a + 230, b + 120, "PERCENT OF INITIAL PATH");
	}
	else {
		glColor3ub(100, 128, 250);
		draw_MtxText(a, b + 30, "-- %2d PERCENT IMPROVEMENT REACHED", PERCENT_GOAL);
		glColor3ub(0, 0, 255);
		draw_MtxText(a, b + 60, "GENERATIONS: %3d", generation);
		draw_MtxText(a, b + 90, "BEST PATH: %5d", (int)calculateFx(mC[m]));
		draw_MtxText(a, b + 120, "THIS PATH IS %2d PERCENT OF INITIAL PATH", percentage);
	}
	
	draw_MtxText(a, b, "INITIAL BEST PATH: %5d", (int)initialBest);
	glColor3ub(150, 150, 150);
	draw_MtxText(60, b + 120, "PRESS SPACE BAR TO RESET");
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Font
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void DNA::initMtxFont() {

	For (i,128) For (j,7) For (k,5) mMtxFont[i][j][k] = 1;

	const char F0[] =
		"00000" "00000" "00000" "11111" "00000" "00000" "00000"//-
		"00000" "00000" "00000" "00000" "00000" "01100" "01100"//.
		"11111" "11111" "11111" "11111" "11111" "11111" "11111"//
		"01110" "10001" "10011" "10101" "11001" "10001" "01110"//0
		"00100" "01100" "00100" "00100" "00100" "00100" "01110"//1
		"01110" "10001" "00001" "00010" "00100" "01000" "11111"//2
		"01110" "10001" "00001" "00110" "00001" "10001" "01110"//3
		"00010" "00110" "01010" "10010" "11111" "00010" "00111"//4
		"11111" "10000" "11110" "00001" "00001" "10001" "01110"//5
		"01110" "10001" "10000" "11110" "10001" "10001" "01110"//6
		"11111" "10001" "00010" "00010" "00100" "00100" "00100"//7
		"01110" "10001" "10001" "01110" "10001" "10001" "01110"//8
		"01110" "10001" "10001" "01111" "00001" "00001" "01110"//9
		"00000" "01100" "01100" "00000" "01100" "01100" "00000"//:
		"11111" "11111" "11111" "11111" "11111" "11111" "11111"//
		"11111" "11111" "11111" "11111" "11111" "11111" "11111"//
		"00000" "00000" "11111" "00000" "11111" "00000" "00000"//=
		"11111" "11111" "11111" "11111" "11111" "11111" "11111"//
		"01110" "10001" "10001" "00010" "00100" "00000" "00100"//?
		"11111" "11111" "11111" "11111" "11111" "11111" "11111"//
		"01110" "10001" "10001" "11111" "10001" "10001" "10001"//A
		"11110" "10001" "10001" "11110" "10001" "10001" "11110"//B
		"01110" "10001" "10000" "10000" "10000" "10001" "01110"//C
		"11110" "10001" "10001" "10001" "10001" "10001" "11110"//D
		"11111" "10000" "10000" "11110" "10000" "10000" "11111"//E
		"11111" "10000" "10000" "11110" "10000" "10000" "10000"//F
		"01110" "10001" "10000" "10111" "10001" "10001" "01110"//G
		"10001" "10001" "10001" "11111" "10001" "10001" "10001"//H
		"01110" "00100" "00100" "00100" "00100" "00100" "01110"//I
		"00001" "00001" "00001" "00001" "10001" "10001" "01110"//J
		"10001" "10010" "10100" "11000" "10100" "10010" "10001"//K
		"10000" "10000" "10000" "10000" "10000" "10000" "11111"//L
		"10001" "11011" "10101" "10101" "10001" "10001" "10001"//M
		"10001" "10001" "11001" "10101" "10011" "10001" "10001"//N
		"01110" "10001" "10001" "10001" "10001" "10001" "01110"//O
		"11110" "10001" "10001" "11110" "10000" "10000" "10000"//P
		"01110" "10001" "10001" "10001" "10101" "10010" "01101"//Q
		"11110" "10001" "10001" "11110" "10100" "10010" "10001"//R
		"01111" "10000" "10000" "01110" "00001" "00001" "11110"//S
		"11111" "00100" "00100" "00100" "00100" "00100" "00100"//T
		"10001" "10001" "10001" "10001" "10001" "10001" "01110"//U
		"10001" "10001" "10001" "10001" "10001" "01010" "00100"//V
		"10001" "10001" "10001" "10101" "10101" "10101" "01010"//W
		"10001" "10001" "01010" "00100" "01010" "10001" "10001"//X
		"10001" "10001" "10001" "01010" "00100" "00100" "00100"//Y
		"11111" "00001" "00010" "00100" "01000" "10000" "11111";//Z

	For (i,46) 
		For (j,7) 
			For (k,5) 
				mMtxFont[i + 45][j][k] = F0[35 * i + 5 * j + k];
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void DNA::draw_MtxText(float x, float y, const char *fmt,...) {
	//--------------------
	glPushMatrix();
	glLoadIdentity();
	//--------------------
	char text[256]; 
	va_list ap;
	if (fmt == NULL) return;
	va_start(ap, fmt); 
	vsprintf_s(text, fmt, ap); 
	va_end(ap);
	//--------------------
	int n = 0;
	glPointSize(2.f);
	glBegin(GL_POINTS);
	while (text[n] != 0) {
		draw_MtxFont(x + 14.f * float(n), y, text[n]);
		n++;
	}
	glEnd();
	//--------------------
	glPopMatrix();
	//--------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void inline DNA::draw_MtxFont(float x, float y, Uint8 c) {
	if (c == ' ') 
		return;	

	For (m,7) 
		For (n,5) {
			if (mMtxFont[c][m][n] == '0') 
				continue;
			glVertex2f(x + 2 * n, y + 2 * m);
		}
}


