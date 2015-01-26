#include <curses.h>

#include "game.h"
#include "renderer.h"
#include "actor.h"

void gameLoop()
{
	Renderer renderer;
	renderer.initWindow();

	Actor* player = new Actor("*", 1, 1);
	
	char ch;
	while((ch = getch()) != 'q') {
		renderer.draw(player);
	}
}

void update()
{
}

