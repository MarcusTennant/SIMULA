#include <curses.h>

#include "renderer.h"

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

void Renderer::draw(int x, int y, int oldX, int oldY, char *token)
{
	mvwprintw(_win, oldY, oldX, " ");
	mvwprintw(_win, y, x, token);
	wrefresh(_win);
}
