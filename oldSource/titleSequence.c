#include <curses.h>
#include <signal.h>

void setup(); //turns on curses options
void tearDown(); //tears down screen etc
void* resizeHandler(int); // handles resizing the terminal

int currH, currW; //current width and height of the terminal

int main()
{
	setup();
	//signal(SIGWINCH, resizeHandler);
	start_color();
	init_pair(1,COLOR_GREEN,COLOR_BLACK);
	bkgd(COLOR_PAIR(1));
	refresh();

	getmaxyx(stdscr, currH, currW);

	WINDOW *win, *titleMarq;
	//char *title0, title1[20], title2[20], title3[20];

	char title0[35] = "   _______  ___   ___   __ \0";      //SIMULA with escape sequences
	char title1[35] = "/// __/ / \\/ | | | | | |  \\\\\\\0";
	char title2[35] = "//__ / / /\\/ | |_| | |_| - \\\\\0";
	char title3[35] = "/___/_/_/  |_|\\___/\\___\\_|\\_\\\0";


 

	win = newwin(currH, currW, 0,0);
	box(win, 0, 0);
        mvwprintw(win, 11, (currW/2)-2, "v1.0");
	mvwprintw(win, 12, (currW/2)-9, "By: Marcus Tennant");
	//wmove(win, 17, (currW/2)-20);
	//waddstr(win, "Press Enter To Begin" | A_BLINK);
	wattron(win, A_BLINK);
	mvwprintw(win, 17, (currW/2)-10, "Press Enter to Begin");
//	wattroff(win, A_BLINK);

	wbkgd(win, COLOR_PAIR(1));
	wrefresh(win);

	titleMarq = newwin(6, currW-14, 5, 7);
	box(titleMarq, 0, 0);
	wbkgd(titleMarq, COLOR_PAIR(1));

	int ch;
	int off = 1;
	int i = 2;
	//while ((ch = getch()) != 'q');

	while(1)
	{
		//signal(SIGWINCH, resizeHandel);
		mvwprintw(titleMarq, 1, i, "%s", title0);
        	mvwprintw(titleMarq, 2, i, "%s", title1); 
        	mvwprintw(titleMarq, 3, i, "%s", title2); 
        	mvwprintw(titleMarq, 4, i, "%s", title3); 
		wrefresh(titleMarq);
		off = (i == 1 || i == currW-44) ? -off : off;
		i += off;
		napms(100);
	}

	getch();
	
	tearDown(); // deletes screens etc
	return 0;
}



void setup()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);	
}

void tearDown()
{
	clear();
	endwin();
}

void* resizeHandler(int signal)
{
	int newH, newW;
	getmaxyx(stdscr, newH, newW);

	currH = newH;
	currW = newW;
}
