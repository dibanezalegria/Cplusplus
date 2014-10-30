#include "Game.h"

// patterns
const bool Game::pattern[SIZE][5][5] = 
	{	{	{ 0, 1, 0, 0, 0 },	// GLIDER 
			{ 0, 0, 1, 0, 0 }, 
			{ 1, 1, 1, 0, 0 }, 
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 } },  
		{	{ 0, 1, 1, 0, 0 },	// RPENTO
			{ 1, 1, 0, 0, 0 }, 
			{ 0, 1, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 } },
		{	{ 0, 1, 0, 0, 0 },	// BOAT
			{ 1, 0, 1, 0, 0 }, 
			{ 0, 1, 1, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 } },
		{	{ 0, 0, 0, 0, 0 },	// OSCILLATOR
			{ 1, 1, 1, 0, 0 }, 
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0 } },
		{	{ 0, 1, 1, 1, 1 },	// FISH
			{ 1, 0, 0, 0, 1 }, 
			{ 0, 0, 0, 0, 1 },
			{ 1, 0, 0, 1, 0 },
			{ 0, 0, 0, 0, 0 } },
		{	{ 1, 0, 1, 0, 1 },	// EXPLODER
			{ 1, 0, 0, 0, 1 }, 
			{ 1, 0, 0, 0, 1 },
			{ 1, 0, 0, 0, 1 },
			{ 1, 0, 1, 0, 1 } }
};

// static members
const float Game::startX = 340.0f;	// grid start 
const float Game::startY = 40.0f;


Game::Game() {
	mMtxFont = new char[128][7][5];
	initMtxFont();
	mW = 1272, mH = 732;	
	// seed
	srand( (unsigned int)time(NULL) );
	// pause
	pause = false;
}

Game::~Game() {
	delete [] mMtxFont;
}

void Game::clearGrid() {
	// initalize arrays
	for (int m = 0; m < M; ++ m)
		for (int n = 0; n < N; ++n) {
			matrix[MATRIX_0][m][n] = false;
			matrix[MATRIX_1][m][n] = false;
		}
	// reset 
	generation = 1;
}

void Game::placePattern(int patternType, int row, int col) {
	// copy initial pattern to grid
	for (int r = 0; r < 5; ++r)
		for (int c = 0; c < 5; ++c)
			if (pattern[patternType][r][c])
				matrix[MATRIX_0][r + row][c + col] = true;
}

int Game::neighbors(int current, int m, int n) {
	int neig = 0;
	// upper left
	if (m > 0 && n > 0 && matrix[current][m - 1][n - 1]) 
		neig++;
	
	// upp
	if (m > 0 && matrix[current][m - 1][n]) 
		neig++;

	// upper right
	if (m > 0 && n < N - 1 && matrix[current][m - 1][n + 1]) 
		neig++;
	
	// left
	if (n > 0 && matrix[current][m][n - 1]) 
		neig++;
	
	// right
	if (n < N - 1 && matrix[current][m][n + 1]) 
		neig++;
	
	// under left
	if (m < M - 1 && n > 0 && matrix[current][m + 1][n - 1]) 
		neig++;
	
	// under 
	if (m < M - 1 && matrix[current][m + 1][n]) 
		neig++;

	// under right
	if (m < M - 1 && n < N - 1 && matrix[current][m + 1][n + 1]) 
		neig++;

	return neig;
}

void Game::evolve(int from, int to) {
	for (int m = 0; m < M; ++m)
		for (int n = 0; n < N; ++n) {
			int neig = neighbors(from, m, n);
			// Rules
			if (matrix[from][m][n])
				if (neig < 2 || neig > 3)
					matrix[to][m][n] = false;	// overcrowded
				else 
					matrix[to][m][n] = true;	// continues being alive

			if ( !matrix[from][m][n])
				if (neig == 3)
					matrix[to][m][n] = true;	// comes to life
				else 
					matrix[to][m][n] = false;	// continues being dead
		}
}

void Game::draw(int current) {
	// text
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255, 128, 128);
	draw_MtxText(20, 20, "PATTERN MENU");
	glColor3ub(0, 0, 255);
	draw_MtxText(20, 60, "1. BOAT - STILL LIFE");
	draw_MtxText(20, 90, "2. GLIDER");
	draw_MtxText(20, 120, "3. OSCILLATOR");
	draw_MtxText(20, 150, "4. R-PENTOMINO");
	draw_MtxText(20, 180, "5. FISH");
	draw_MtxText(20, 210, "6. EXPLODER");
	draw_MtxText(20, 240, "7. PARTY");

	glColor3ub(0, 0, 255);
	draw_MtxText(380, 20, "GEN %4d", generation);
	
	// highlight selected menu option
	glColor3ub(128, 128, 255);
	switch (menuOption) {
		case BOAT:
			draw_MtxText(20, 60, "1. BOAT - STILL LIFE");
			break;
		case GLIDER:
			draw_MtxText(20, 90, "2. GLIDER");
			break;
		case OSCILLATOR:
			draw_MtxText(20, 120, "3. OSCILLATOR");
			break;
		case RPENTO:
			draw_MtxText(20, 150, "4. R-PENTOMINO");
			break;
		case FISH:
			draw_MtxText(20, 180, "5. FISH");
			break;
		case EXPLODER:
			draw_MtxText(20, 210, "6. EXPLODER");
			break;
		case PARTY:
			draw_MtxText(20, 240, "7. PARTY");
			break;
	}
	glColor3ub(200, 200, 200);
	draw_MtxText(20, 700, "PRESS SPACE BAR TO PAUSE");

	if (pause) {
		glColor3ub(255, 0, 0);
		draw_MtxText(600, 20, "-- GAME PAUSED -- ");
	}
	
	//
	// Grid is smaller than matrix. It leaves out 2 rows up, 
	// 2 rows down, 2 col up and 2 col down.
	// This padding gets rid off GLIDERS stuck on the walls
	//
	glColor3ub(120, 120, 120);
	glBegin(GL_LINES);
	for (int m = 2; m < M - 1; ++m) {
		// horizontal
		glVertex2f(startX + (2 * cellSize), startY + (m * cellSize));
		glVertex2f(startX + ((N - 2) * cellSize), startY + (m * cellSize));
	}
	for (int n = 2; n < N - 1; ++n) {
			// vertical
			glVertex2f(startX + (n * cellSize), startY + (2 * cellSize));
			glVertex2f(startX + (n * cellSize), startY + ((M - 2) * cellSize)); 
		}
	glEnd();
	
	// display active matrix
	for (int m = 2; m < M - 2; ++m)
		for (int n = 2; n < N - 2; ++n)
			if (matrix[current][m][n]) {
				// yellow rectangle
				glColor3ub(200, 200, 100);	// yellow
				glBegin(GL_QUADS);
				glVertex2f(startX + n * cellSize, startY + m * cellSize);
				glVertex2f(startX + (n + 1) * cellSize, startY + m * cellSize);
				glVertex2f(startX + (n + 1) * cellSize, startY + (m + 1) * cellSize); 
				glVertex2f(startX + n * cellSize, startY + (m + 1) * cellSize);
				glEnd();
			}
}

void Game::changeSize(int w, int h) {
	mW = (float)w, mH = (float)h;
	glViewport(0, 0, w, h);										// Reset Viewport
	glMatrixMode(GL_PROJECTION); glLoadIdentity();		// Reset The Projection Matrix
	glOrtho(0.0f, w, h, 0.0f, -1.0f, 1.0f);	         // Create Ortho View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();		// Reset The Modelview Matrix
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Font
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Game::initMtxFont() {

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
void Game::draw_MtxText(float x, float y, const char *fmt,...) {
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
void inline Game::draw_MtxFont(float x, float y, Uint8 c) {
	if (c == ' ') 
		return;	

	For (m,7) 
		For (n,5) {
			if (mMtxFont[c][m][n] == '0') 
				continue;
			glVertex2f(x + 2 * n, y + 2 * m);
		}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
