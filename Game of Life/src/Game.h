#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <time.h>
#include <cmath>

//---------------------------------------------------------------------
#define For(i,N)	for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------

class Game {
public:
	static const float startX;	// grid start 
	static const float startY;
	static const int cellSize = 6;
	static const int M = 104;	// M = grid rows + 4 extra (padding)	
	static const int N = 144;	// N = grid cols + 4 extra (padding)
	enum { MATRIX_0, MATRIX_1 };
	bool matrix[2][M][N];
	enum { GLIDER, RPENTO, BOAT, OSCILLATOR, FISH, EXPLODER, PARTY, SIZE };
	static const bool pattern[SIZE][5][5];
	int menuOption;
	int generation;		// counter
	bool pause;
	char (*mMtxFont)[7][5];
	float mW, mH;
	//-------------------------------------------------------
	Game();
	~Game();
	//-------------------------------------------------------
	//void  initGame(int patternType);
	void	clearGrid();
	void	placePattern(int patternType, int row, int col);
	int   neighbors(int current, int m, int n);
	void  evolve(int from, int to);
	void  draw(int current);
	//-------------------------------------------------------
	void  changeSize(int w, int h);
	void  initMtxFont();
	void  draw_MtxText(float x, float y, const char *fmt,...);
	void  draw_MtxFont(float x, float y, Uint8 c);
};

#endif