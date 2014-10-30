#include <SDL.h>
#include <SDL_opengl.h>
//---------------------------------------------------------------------
#define For(i,N)	for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------

class DNA {
public:
	const static int M = 30;				// number of paths 
	const static int N = 20;				// number of cities 
	const static int PERCENT_GOAL;		// sets the limit for the algorithm	
	const static bool ELITISM;
	const static float	PARENTS,
								CROSSOVER, 
								MUTATION;		// percentages

	static float x_city[N];					// city coordinates
	static float y_city[N];
	float initialBest;						// best path 1st generation
	int mC[M][N];				
	char (*mMtxFont)[7][5];					// fonts
	float mW, mH;								// screen size
	//-------------------------------------------------------
	DNA();
	~DNA();
	//-------------------------------------------------------
	static float calculateFx(int* ptr);
	static int comparator(const void* ptrA, const void* ptrB);
	//
	void  draw(int m, int generation, int percentage);
	void  copyRow(int* from, int* to);
	void	resetCity();
	void	initializeMatrix();
	void	sortMatrixByFx();
	void  evolutionaryStep();
	void	mutate(int* ptr);
	void	crossover(int* ptrA, int* ptrB);
	bool	prob(float percentage);
	//-------------------------------------------------------
	void  changeSize(int w, int h);
	void  initMtxFont();
	void  draw_MtxText(float x, float y, const char *fmt,...);
	void  draw_MtxFont(float x, float y, Uint8 c);
};

	