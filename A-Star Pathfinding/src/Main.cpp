//---------------------------------------------------------------------
//---------------------------------------------------------------------
//  Introduction to Artificial Intelligence for Game Programming, 15 hp
//  Gotland University, Sweden
//  Lab 5 - Path Finding with A*
//  Daniel Ibanez Alegria 
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "PFinding.h"
PFinding pf;
SDL_Surface* gScreen;

//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void initAttributes() {
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	//Depth Buffer 24-bit
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);//FSAA
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);//FSAA
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);	//Enable Vsync
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void createSurface() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_EnableKeyRepeat(300, 80);
	initAttributes();	//Context Attr
	gScreen = SDL_SetVideoMode ((int)pf.mW, (int)pf.mH, 0, SDL_OPENGL);
	SDL_WM_SetCaption("A* Path Finding", NULL);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------

static void mainLoop () {
	SDL_Event event;
	bool done = false;
	int key;

	// place start/target
	pf.placeStartTarget();
	// generate random obstacles
	pf.randomMap();

	while ( !done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					key = event.key.keysym.sym;
					//---------------------------------
					if (key == SDLK_SPACE) { 
						if (pf.findPath())
							pf.showPath();
						else
							pf.noPathFound = true;
					}
					if (key == SDLK_UP && pf.wall_percentage < 0.50) {
						pf.wall_percentage = pf.wall_percentage + 0.05f;
						pf.cleanMatrix(false);
						pf.randomMap();
					}
					if (key == SDLK_DOWN && pf.wall_percentage > 0.11) {
						pf.wall_percentage = pf.wall_percentage - 0.05f;
						pf.cleanMatrix(false);
						pf.randomMap();
					}
					if (key == SDLK_r) {
						pf.cleanMatrix(false);
						pf.placeStartTarget();
						pf.randomMap();
					}
					break; 
				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT) {
						int row = (int)((event.motion.y - pf.GRID_Y) / pf.CELL);
						int col = (int)((event.motion.x - pf.GRID_X) / pf.CELL);
						pf.cleanMatrix(true);
						pf.flipSquare(row, col);
					}
					break;			
				case SDL_QUIT: 
					done = true; 
					break;
			}
		}

		// draw
		pf.draw();
		SDL_GL_SwapBuffers();
		SDL_Delay(200);	
	}
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
int main(int argc, char* argv[]) {
	createSurface();
	pf.changeSize((int)pf.mW, (int)pf.mH);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);	
	mainLoop();
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



