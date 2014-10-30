#include "PFinding.h"

// --------------------------------------------------------------------------------------
// static members
// --------------------------------------------------------------------------------------
const float PFinding::GRID_X = 70.0f;
const float PFinding::GRID_Y = 50.0f;

// --------------------------------------------------------------------------------------
// constructor/destructor
// --------------------------------------------------------------------------------------
PFinding::PFinding() {
	// seed
	srand((unsigned int)time(NULL));
	// window size
	mW = 1272.0f;
	mH = 732.0f;
	// fonts
	mMtxFont = new char[128][7][5];
	initMtxFont();
	
	// % of obstacles/ total space in grid
	wall_percentage = 0.10f;

	// initialize matrix
	for (int m = 0; m < M; ++m) {
		for (int n = 0; n < N; ++n) {
			matrix[m][n].row = m;
			matrix[m][n].col = n;
		}
	}
}

PFinding::~PFinding() {
	delete [] mMtxFont;
}

// --------------------------------------------------------------------------------------
// starts the A* path finding algorithm
// --------------------------------------------------------------------------------------
bool PFinding::findPath() {
	Node* current = ptrStart;
	openList.push_back(current);
			
	while ( !openList.empty() && !isNodeInList(ptrTarget, closeList)) {
		// for each neighbor
		check_neighbors(current);
		openList.erase(remove(openList.begin(), openList.end(), current), openList.end());
		closeList.push_back(current);
		current = lowest_F();
	}
		
	if (isNodeInList(ptrTarget, closeList))
		return true;
	else 
		return false;
}


// --------------------------------------------------------------------------------------
// marks the state of squares in the path as PATH
// --------------------------------------------------------------------------------------
void PFinding::showPath() {
	Node* ptr = ptrTarget;
	while (ptr != NULL) {
		if (ptr != ptrTarget && ptr != ptrStart)
			ptr->state = Node::PATH;
		ptr = ptr->ptrParent;
	}
}

// --------------------------------------------------------------------------------------
// returns true if a node is in the list
// --------------------------------------------------------------------------------------
bool PFinding::isNodeInList(Node* node, vector<Node*> list) {
	if ( find(list.begin(), list.end(), node) != list.end() )
		return true;
	else
		return false;
}

// --------------------------------------------------------------------------------------
// calls evaluate function for each of the 8 neighbors to the current square
// --------------------------------------------------------------------------------------
void PFinding::check_neighbors(Node* current ) {
	// upp left (cutting corners is not allowed)
	if (current->row - 1 >= 0 && current->col - 1 >= 0
		&& matrix[current->row - 1][current->col].state != Node::WALL
		&& matrix[current->row][current->col - 1].state != Node::WALL) {

		evaluateNeighbor(current, &matrix[current->row - 1][current->col - 1]);
	}
	// upp
	if (current->row - 1 >= 0) 
		evaluateNeighbor(current, &matrix[current->row - 1][current->col]);
	
	// upp right
	if (current->row - 1 >= 0 && current->col + 1 < N
		&& matrix[current->row - 1][current->col].state != Node::WALL
		&& matrix[current->row][current->col + 1].state != Node::WALL) {
		
		evaluateNeighbor(current, &matrix[current->row - 1][current->col + 1]);
	}
	// right
	if (current->col + 1 < N) 
		evaluateNeighbor(current, &matrix[current->row][current->col + 1]);
	
	// down right
	if (current->row + 1 < M && current->col + 1 < N
			&& matrix[current->row][current->col + 1].state != Node::WALL
			&& matrix[current->row + 1][current->col].state != Node::WALL) {

		evaluateNeighbor(current, &matrix[current->row + 1][current->col + 1]);
	}
	// down
	if (current->row + 1 < M) 
		evaluateNeighbor(current, &matrix[current->row + 1][current->col]);
	
	// down left
	if (current->row + 1 < M && current->col - 1 >= 0 
			&& matrix[current->row][current->col - 1].state != Node::WALL
			&& matrix[current->row + 1][current->col].state != Node::WALL) {
	
		evaluateNeighbor(current, &matrix[current->row + 1][current->col - 1]);
	}
	// left
	if (current->col - 1 >= 0) 
		evaluateNeighbor(current, &matrix[current->row][current->col - 1]);
	
}

// --------------------------------------------------------------------------------------
// checks if the neighbor is walkable, if true then it adds it to the openList
// --------------------------------------------------------------------------------------
void PFinding::evaluateNeighbor(Node* current, Node* neighbor) {
	if ( !(neighbor->state == Node::WALL) && !isNodeInList(neighbor, closeList)) {
		if ( !isNodeInList(neighbor, openList)) {
			// set G H 
			neighbor->ptrParent = current; // current becomes parent of neighbor	
			neighbor->g = calculate_G(neighbor);
			neighbor->h = calculate_H(neighbor);		
			openList.push_back(neighbor);
		}
		else {
			// check if better path through current 
			int newPath = current->g + calculateDistance(current, neighbor);
			if (newPath < neighbor->g) {
				// better path found
				neighbor->ptrParent = current;
				neighbor->g = calculate_G(neighbor);
			}
		}
	}
}

// --------------------------------------------------------------------------------------
// calculate distance between two squares
// --------------------------------------------------------------------------------------
int PFinding::calculateDistance(Node* nodeA, Node* nodeB) {
	// horizontal or vertical (10), diagonal (14)
	float d = (float)abs(nodeA->row - nodeB->row) + abs(nodeA->col - nodeB->col);
	return (int)(sqrt(d) * 10);
}
	
// --------------------------------------------------------------------------------------
// G is the cost to move from one square to the starting square
// --------------------------------------------------------------------------------------
int PFinding::calculate_G(Node* node) {
	// distance to parent + G of parent
	return calculateDistance(node, node->ptrParent) + node->ptrParent->g;
}

// --------------------------------------------------------------------------------------
// H is the estimated cost to move from one square to the target square
// --------------------------------------------------------------------------------------
int PFinding::calculate_H(Node* current) {
	return (abs(current->row - ptrTarget->row) + 
		abs(current->col - ptrTarget->col)) * 10;
}

// --------------------------------------------------------------------------------------
// return pointer to node with lowest F in openList
// --------------------------------------------------------------------------------------
Node* PFinding::lowest_F() {
	int lowest = INT_MAX;
	Node* temp = NULL;
	
	for (unsigned int i = 0; i < openList.size(); ++i)
		if (openList[i]->g + openList[i]->h < lowest) {
			lowest = openList[i]->g + openList[i]->h;
			temp = openList[i];
		}
	
	return temp;
}

// --------------------------------------------------------------------------------------
// reset matrix (if keepWalls false then walls get also removed)
// --------------------------------------------------------------------------------------
void PFinding::cleanMatrix(bool keepWalls) {
	for (int m = 0; m < M; ++m) 
		for (int n = 0; n < N; ++n) {
			if (matrix[m][n].state == Node::WALL) {
				if (!keepWalls)
					matrix[m][n].state = Node::EMPTY;
			} else 
				matrix[m][n].state = Node::EMPTY;
			matrix[m][n].ptrParent = NULL;
			matrix[m][n].g = 0;
			matrix[m][n].h = 0;
		}
	// empty lists
	openList.clear();
	closeList.clear();
	// start and target squares
	ptrStart->state = Node::START;
	ptrTarget->state = Node::TARGET;
	// reset
	noPathFound = false;
}

// --------------------------------------------------------------------------------------
// sets start and target squares
// --------------------------------------------------------------------------------------
void PFinding::placeStartTarget() {
	if (ptrStart != NULL)
		ptrStart->state = Node::EMPTY;
	// place start on the left side of the grid
	int row = rand() % M;
	int col = rand() % (N / 2);
	matrix[row][col].state = Node::START;
	ptrStart = &matrix[row][col];
	// target on the right side of the grid
	if (ptrTarget != NULL)
		ptrTarget->state = Node::EMPTY;
	row = rand() % M;
	col = rand() % (N / 2) + N / 2;
	matrix[row][col].state = Node::TARGET;
	ptrTarget = &matrix[row][col];
}

// --------------------------------------------------------------------------------------
// sets one walkable square as unwalkable and the opposite
// --------------------------------------------------------------------------------------
void PFinding::flipSquare(int row, int col) {
	// outside grid?
	if (row < 0 || row >= M  || col < 0 || col >= N)
		return;

	// start or target squares?
	if (matrix[row][col].state == Node::START || 
		matrix[row][col].state == Node::TARGET)
		return;

	if (matrix[row][col].state == Node::EMPTY)
		matrix[row][col].state = Node::WALL;
	else
		matrix[row][col].state = Node::EMPTY;
}

// --------------------------------------------------------------------------------------
// sets one square as unwalkable
// return true when successful
// --------------------------------------------------------------------------------------
bool PFinding::markSquare(int row, int col) {
	if ((row < 0 || row >= M  || col < 0 || col >= N) ||	//outside grid?
		((matrix[row][col].state == Node::START) ||			//start or target squares?
		(matrix[row][col].state == Node::TARGET)))
			return false;

	if (matrix[row][col].state == Node::EMPTY) {
		matrix[row][col].state = Node::WALL;
		return true;
	}
	else 		
		return false;
}

// --------------------------------------------------------------------------------------
// place n walls randomly in the grid
// a constant controls the percentage of walls
// --------------------------------------------------------------------------------------
void PFinding::randomMap() {
	// remove all walls
	for (int m = 0; m < M; ++m)
		for (int n = 0; n < N; ++n) {
			if ((matrix[m][n].state != Node::START) &&	//start or target squares?
				(matrix[m][n].state != Node::TARGET))
					matrix[m][n].state = Node::EMPTY;
		}

	int nwalls = (int)(M * N * wall_percentage);

	// leave space for start and target square 
	// only extreme cases: tiny grid or %wall very high 
	if (nwalls > M * N - 2)
		nwalls = nwalls - 2;


	for (int i = 0; i < nwalls; ++i) {
		int row = rand() % M;
		int col = rand() % N;
		if ( !markSquare(row, col))
			i--;
	}
}

// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
void PFinding::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	// paint squares 
	for (int m = 0; m < M; ++m)
		for (int n = 0; n < N; ++n) {
			// colors
			if (matrix[m][n].state == Node::WALL) 
				glColor3ub(190, 190, 80);	
			if (matrix[m][n].state == Node::START)
				glColor3ub(0, 200, 0);
			if (matrix[m][n].state == Node::TARGET)
				glColor3ub(200, 0, 0);	
			if (matrix[m][n].state == Node::PATH)
				glColor3ub(0, 0, 255);
			// filled rectangle
			if (matrix[m][n].state != Node::EMPTY) {
				glBegin(GL_QUADS);
				glVertex2f(GRID_X + n * CELL, GRID_Y + m * CELL);
				glVertex2f(GRID_X + (n + 1) * CELL, GRID_Y + m * CELL);
				glVertex2f(GRID_X + (n + 1) * CELL, GRID_Y + (m + 1) * CELL); 
				glVertex2f(GRID_X + n * CELL, GRID_Y + (m + 1) * CELL);
				glEnd();
			}
		}

	// grid lines
	glColor3ub(120, 120, 120);
	glBegin(GL_LINES);
	for (int m = 0; m < M + 1; ++m) {
		// horizontal
		glVertex2f(GRID_X, GRID_Y + (m * CELL));
		glVertex2f(GRID_X + (N * CELL), GRID_Y + (m * CELL));
	}
	for (int n = 0; n < N + 1; ++n) {
		// vertical
		glVertex2f(GRID_X + (n * CELL), GRID_Y);
		glVertex2f(GRID_X + (n * CELL), GRID_Y + (M * CELL)); 
	}
	glEnd();
	
	// text
	glColor3ub(0, 150, 255);
	draw_MtxText(GRID_X, 560, "UP-DOWN: "); 
	glColor3ub(0, 0, 255);
	draw_MtxText(GRID_X + 130, 560, "WALL PERCENTAGE ");
	glColor3ub(190, 190, 80);
	draw_MtxText(GRID_X + 370, 560, "0.%2d", (int)(wall_percentage * 100));
	glColor3ub(0, 150, 255);
	draw_MtxText(GRID_X, 590, "R: ");
	glColor3ub(0, 0, 255);
	draw_MtxText(GRID_X + 40, 590, "RESET"); 
	glColor3ub(0, 150, 255);
	draw_MtxText(GRID_X, 620, "SPACE BAR: ");
	glColor3ub(0, 0, 255);
	draw_MtxText(GRID_X + 160, 620, "FIND PATH");
	glColor3ub(0, 150, 255);
	draw_MtxText(GRID_X, 650, "MOUSE LEFT BUTTON:");
	glColor3ub(0, 0, 255);
	draw_MtxText(GRID_X + 270, 650, "SELECT INDIVIDUAL TILES");
	if (noPathFound) {
		glColor3ub(255, 0, 0);
		draw_MtxText(GRID_X + 300, 620, "-- NO PATH FOUND -- ");
	}
}

// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
void PFinding::changeSize(int w, int h) {
	mW = (float)w, mH = (float)h;
	glViewport(0, 0, w, h);										// Reset Viewport
	glMatrixMode(GL_PROJECTION); glLoadIdentity();		// Reset The Projection Matrix
	glOrtho(0.0f, w, h, 0.0f, -1.0f, 1.0f);	         // Create Ortho View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();		// Reset The Modelview Matrix
}

// --------------------------------------------------------------------------------------
//                              Font
// --------------------------------------------------------------------------------------
void PFinding::initMtxFont() {

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
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
void PFinding::draw_MtxText(float x, float y, const char *fmt,...) {
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
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
void inline PFinding::draw_MtxFont(float x, float y, Uint8 c) {
	if (c == ' ') 
		return;	

	For (m,7) 
		For (n,5) {
			if (mMtxFont[c][m][n] == '0') 
				continue;
			glVertex2f(x + 2 * n, y + 2 * m);
		}
}
// --------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------
