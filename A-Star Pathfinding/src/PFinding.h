#ifndef PFINDING_H
#define PFINDING_H

#include <SDL.h>
#include <SDL_opengl.h>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <cmath>
#include <climits>
using namespace std;

//---------------------------------------------------------------------
#define For(i,N)	for (int (i) = 0; (i) < (N); (i)++)
//---------------------------------------------------------------------

class Node {
public:
	enum { EMPTY, WALL, START, TARGET, PATH } state;
	Node* ptrParent;
	Node() : state(EMPTY), ptrParent(NULL), g(0), h(0) {};
	int row;
	int col;
	int g;
	int h;
};

class PFinding {
public:
	static const int M = 16;					// rows
	static const int N = 40;					// columns
	static const int CELL = 28;				// cell size
	static const float GRID_X, GRID_Y;		// starting grid location
	float wall_percentage;						// % walls in grid
	vector<Node*> openList;
	vector<Node*> closeList;
	Node* ptrStart;								// starting square
	Node* ptrTarget;								// target square				
	Node matrix[M][N];							
	bool noPathFound;
	float mW, mH;									// window size
	char (*mMtxFont)[7][5];
	//-------------------------------------------------------
	PFinding();
	~PFinding();
	//-------------------------------------------------------
	bool	findPath();
	void	showPath();
	bool	isNodeInList(Node* node, vector<Node*> list);
	void	check_neighbors(Node* current);
	void	evaluateNeighbor(Node* current, Node* neighbor);
	int	calculateDistance(Node* nodeA, Node* nodeB);
	int	calculate_G(Node* node);
	int	calculate_H(Node* current);
	Node* lowest_F();
	//-------------------------------------------------------
	void	cleanMatrix(bool keepWalls);
	void	placeStartTarget();
	void  flipSquare(int row, int col);
	bool  markSquare(int row, int col);
	void	randomMap();
	void  draw();
	//-------------------------------------------------------
	void  changeSize(int w, int h);
	void  initMtxFont();
	void  draw_MtxText(float x, float y, const char *fmt,...);
	void  draw_MtxFont(float x, float y, Uint8 c);
};

#endif