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
		update(ch, player);
		draw(player);
	}
}

void Game::update(char input, Actor *actor)
{
	switch(input){
		case 'w': actor->move(actor->_xPos,   actor->_yPos-1); break; 
		case 'a': actor->move(actor->_xPos-1, actor->_yPos  ); break;
		case 's': actor->move(actor->_xPos,   actor->_yPos+1); break;
		case 'd': actor->move(actor->_xPos+1, actor->_yPos  ); break;
		default: break;
	}
}

void Game::draw(Actor *actor)
{
	renderer.draw(actor->_xPos, actor->_yPos, actor->_oldXPos, actor->_oldYPos, actor->_token);
}

