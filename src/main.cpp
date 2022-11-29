#include <iostream>
#include <game.h>

Game *game = nullptr;

int main(int argc, const char * argv[]) {

	game = new Game();

	game->fps = 60;


	game->init("My Game", 1200, 800);
	game->setup();
	while (game->running())
	{
		game->startClock = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		if((1000/ game->fps) > SDL_GetTicks() - game->startClock)
		{
			SDL_Delay(1000 / game->fps - (SDL_GetTicks() - game->startClock));
		}

		game->previous_time = game->current_time;
		game->current_time = SDL_GetTicks();
		game->delta_time = game->current_time - game->previous_time;
	}

	game->clean();
	
	return 0;
}
	