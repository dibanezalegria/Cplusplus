//---------------------------------------------------------------------
//---------------------------------------------------------------------
//  Introduction to Artificial Intelligence for Game Programming, 15 hp
//  Gotland University, Sweden
//	 Lab 3.3 - Genetic Algorithms
//  Daniel Ibanez Alegria 
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "DNA.h"
DNA dna;
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
	gScreen = SDL_SetVideoMode ((int)dna.mW, (int)dna.mH, 0, SDL_OPENGL);
	SDL_WM_SetCaption("TSP problem", NULL);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void mainLoop () {
	SDL_Event event;
	bool done = false;
	int key;
	int generation = 1;

	while ( !done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					key = event.key.keysym.sym;
					//state = SDL_GetModState();
					//---------------------------------
					if (key == SDLK_SPACE) { 
						dna.resetCity();
						dna.initializeMatrix();
						generation = 1;
					}				
					break; 
				case SDL_QUIT: 
					done = true; 
					break;
			}
		}
	
		dna.sortMatrixByFx();
		int percentage = (int)(dna.calculateFx(dna.mC[0]) * 100 / dna.initialBest);
		dna.draw(0, generation, percentage);
		SDL_GL_SwapBuffers();
		SDL_Delay(500);

		if (percentage > dna.PERCENT_GOAL) {
			dna.evolutionaryStep();
			generation++;
		}
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
int main(int argc, char* argv[]) {
	createSurface();
	dna.changeSize((int)dna.mW, (int)dna.mH);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);	
	mainLoop();
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



