#include <curses.h>

#include "renderer.h"
#include "actor.h"

Renderer::~Renderer()
{
	endwin();
}

void Renderer::initWindow()
{
	initscr(); //typical ncurses setup
	cbreak();
	noecho();
	curs_set(0);
	
	getmaxyx(stdscr, _termSizeY, _termSizeX);
	refresh();

	_win = newwin(_termSizeY-15, _termSizeX, 0, 0);
	box(_win,0,0);
	wrefresh(_win);

        _inventory = newwin(16, _termSizeX, _termSizeY-16, 0);
        wborder(_inventory, 0, 0, 0, 0, ACS_LTEE, ACS_RTEE, 0, 0);
	wrefresh(_inventory);
}

void Renderer::draw(Actor *actor)
{
	mvwprintw(_win, actor->_xPos, actor->_yPos, actor->_token);
	wrefresh(_win);
}
