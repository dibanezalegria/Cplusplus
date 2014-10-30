//---------------------------------------------------------------------
//---------------------------------------------------------------------
//  Introduction to Artificial Intelligence for Game Programming, 15 hp
//  Gotland University, Sweden
//  Lab 4.1 The Game of Life
//  Daniel Ibanez Alegria 
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "Game.h"
Game game;
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
	gScreen = SDL_SetVideoMode ((int)game.mW, (int)game.mH, 0, SDL_OPENGL);
	SDL_WM_SetCaption("Game of Life", NULL);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
static void mainLoop () {
	SDL_Event event;
	int done = 0, key;
	int currentMatrix = 0;

	// start game
	game.menuOption = game.BOAT;
	game.clearGrid();
	game.placePattern(game.BOAT, game.M/2, game.N/2);

	while ( !done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					key   = event.key.keysym.sym;
					//state = SDL_GetModState();
					//---------------------------------
					if (key == SDLK_SPACE) { 
						game.pause = !game.pause;
					}
					if (key == SDLK_1) {
						game.menuOption = game.BOAT;
						game.clearGrid();
						game.placePattern(game.BOAT, game.M/2, game.N/2);
						currentMatrix = 0;
					}
					if (key == SDLK_2) {
						game.menuOption = game.GLIDER;
						game.clearGrid();
						game.placePattern(game.GLIDER, game.M/2, game.N/2);
						currentMatrix = 0;
					}
					if (key == SDLK_3) {
						game.menuOption = game.OSCILLATOR;
						game.clearGrid();
						game.placePattern(game.OSCILLATOR, game.M/2, game.N/2);
						currentMatrix = 0;
					}
					if (key == SDLK_4) {
						game.menuOption = game.RPENTO;
						game.clearGrid();
						game.placePattern(game.RPENTO, game.M/2 - 5, game.N/2 - 5);
						currentMatrix = 0;
					}
					if (key == SDLK_5) {
						game.menuOption = game.FISH;
						game.clearGrid();
						game.placePattern(game.FISH, game.M/2, game.N/2);
						currentMatrix = 0;
					}
					if (key == SDLK_6) {
						game.menuOption = game.EXPLODER;
						game.clearGrid();
						game.placePattern(game.EXPLODER, game.M/2, game.N/2);
						currentMatrix = 0;
					}
					if (key == SDLK_7) {
						game.menuOption = game.PARTY;
						game.clearGrid();
						for (int i = 0; i < 10; ++i) {
							int row = rand() % (game.M - 20) + 10;
							int col = rand() % (game.N - 20) + 10;
							int pat = rand() % (game.SIZE - 1);	// leave RANDOM out
							game.placePattern(pat, row, col);
						}
						currentMatrix = 0;
					}
					break; 
				case SDL_QUIT: 
					done = 1; 
					break;
			}
		}

		// draw
		if (currentMatrix == game.MATRIX_0) 
			game.draw(game.MATRIX_0);
		else 
			game.draw(game.MATRIX_1);
		
		SDL_GL_SwapBuffers();
		SDL_Delay(106);
		
		// evolve
		if (!game.pause) {
			if (currentMatrix == game.MATRIX_0) {
				game.evolve(game.MATRIX_0, game.MATRIX_1);
				currentMatrix = 1;
			}
			else {
				game.evolve(game.MATRIX_1, game.MATRIX_0);
				currentMatrix = 0;
			}	
			
			game.generation++;
		}
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
int main(int argc, char* argv[]) {
	createSurface();
	game.changeSize((int)game.mW, (int)game.mH);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);	
	mainLoop();
	return 0;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



