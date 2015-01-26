#include <curses.h> //using for getch(), will probably toss for stdin

#include "game.h"
#include "renderer.h"
#include "actor.h"


namespace{
	Renderer renderer;
}

void Game::gameLoop()
{
	renderer.initWindow();

	Actor* player = new Actor(1, 1, "*");
	
	char ch;
	while((ch = getch()) != 'q') {
		draw(player);
	}
}

void Game::update()
{
}

void Game::draw(Actor *actor)
{
	renderer.draw(actor->_xPos, actor->_yPos, actor->_token);
}

