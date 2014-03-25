#include <curses.h>


//void wmakeroom(WINDOW *w, int y, int x, int height, int width, int ulc, int urc, int llc, int lrc);
//void wmakeroom(WINDOW *w, room *r);

void wmoveplayer(WINDOW *w, int *playerY, int *playerX, char input);

int *wallsY;
int *wallsX;
int numWallVerts = 0;
int termSizeX, termSizeY;

typedef struct room{
	char name[10];
	int door[4][10];
	int y, x, height, width;
	int tl;
	int tr;
	int bl;
	int br;
} room;

void wmakeroom(WINDOW *w, room *r);

int main()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	getmaxyx(stdscr, termSizeY, termSizeX);
		
	int playerY = 10;
	int playerX = 10;
	int ch;
	char *player = "*";

	refresh();
		
	WINDOW *win, *inventory;

	win = newwin(termSizeY-15, termSizeX, 0, 0);
	box(win,0,0);
	wmove(win, playerY, playerX);
	wprintw(win, player);
	wrefresh(win);

        inventory = newwin(16, termSizeX, termSizeY-16, 0);
        wborder(inventory, 0, 0, 0, 0, ACS_LTEE, ACS_RTEE, 0, 0);
        wrefresh(inventory);

	struct room newRoom;
	newRoom.name[0] = 'T';
	newRoom.door[0][3] = 1;
	newRoom.y = 4;
	newRoom.x = 4;
	newRoom.height = 10;
	newRoom.width = 10;
	newRoom.tl = ACS_ULCORNER;
	newRoom.tr = ACS_URCORNER;
	newRoom.bl = ACS_LLCORNER;
	newRoom.br = ACS_LRCORNER;

	wmakeroom(win, &newRoom);

	

//	wmakeroom(win, 5, 12, 8, 10, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
//	wmakeroom(win, 12, 12, 5, 10, ACS_LTEE, ACS_RTEE, ACS_LLCORNER, ACS_LRCORNER);
//	wmakeroom(win, 5, 30, 10, 10, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER);
	wrefresh(win);

	while((ch = getch()) != 'q')
	{	
		moveplayer(win, &playerY, &playerX, ch);
		wrefresh(win);
	}


	endwin();
	return 0;
}

void wmakeroom(WINDOW *w, room *r)
{
	int tNVs = numWallVerts;

	wmove(w, r->y, r->x); //top horiz
	whline(w, 0, r->width);
	
	wmove(w, (r->y+r->height)-1, r->x); //bot horiz
	whline(w, 0, r->width);

	wmove(w, r->y, r->x); //left vertical
	wvline(w, 0, r->height);

	wmove(w, r->y, (r->x+r->width)); //right vert
	wvline(w, 0, r->height);

	mvwaddch(w, r->y, r->x, r->tl); //corners clockwise
	mvwaddch(w, r->y, r->x+r->width, r->tr);
	mvwaddch(w, (r->y+r->height)-1, r->x, r->bl);
	mvwaddch(w, (r->y+r->height)-1, r->x+r->height, r->br);
}

/*
void wmakeroom(WINDOW *w, int y, int x, int height, int width, int ulc, int urc, int llc, int lrc)
{
	int tNVs =  numWallVerts;

	wmove(w, y, x); //top horiz
	whline(w, 0, width);

	wmove(w, (y+height)-1, x); //bot horiz
	whline(w, 0, width);

	wmove(w, y, x); //left vert
	wvline(w, 0, height);

	wmove(w, y, x+width); // right vert
	wvline(w, 0, height);

	mvwaddch(w, y, x, ulc);
        mvwaddch(w, y, x+width, urc);
        mvwaddch(w, (y+height)-1, x, llc);
        mvwaddch(w, (y+height)-1, x+width, lrc);

	
	if (wallsY == NULL)
	{
		wallsY = (int*) malloc (((2 * width) + (2*height)) * sizeof(int));
        	wallsX = (int*) malloc (((2 * width) + (2*height)) * sizeof(int));
	}
	else
	{
		int* tmpPtrY;
		int* tmpPtrX;
	        tmpPtrY = (int*) realloc (wallsY, (numWallVerts + (2 * width) + (2*height)) * sizeof(int)); //TODO: we are adding double data for each corner
                tmpPtrX = (int*) realloc (wallsX, (numWallVerts + (2 * width) + (2*height)) * sizeof(int)); //SOLN: dont draw or store corner horiz verts, requires editing numWallVerts as well	
		if (tmpPtrY != NULL && tmpPtrX != NULL)
		{
			wallsY = tmpPtrY;
			wallsX = tmpPtrX;
		}
	}

	numWallVerts += ((2*width)+(2*height));

	int i = 0;
	int tempX = x;
	for(i; i < width; i++) //add top horiz line to walls //FIXME: the last horiz line tile is not drawn, also applies to bot hriz line
	{
		wallsY[i+tNVs] = y;
		wallsX[i+tNVs] = tempX++;
	}

	tempX = x;
	for(i; i < 2*width; i++) //add bot horiz line
	{
		wallsY[i+tNVs] = y+height-1;
		wallsX[i+tNVs] = tempX++;
	}	

	int tempY = y;
	for(i; i < (2*width)+height; i++) //add left vert line
	{
		wallsY[i+tNVs] = tempY++;
		wallsX[i+tNVs] = x; 
	}

	tempY = y;
	for(i; i < (2*width) + (2*height); i++) //add right vert line
	{	
		wallsY[i+tNVs] = tempY++;
		wallsX[i+tNVs] = x+width;
	}
}
*/

void moveplayer(WINDOW *w, int *playerY, int *playerX, char input)
{
	bool col = false;
	switch(input)
                {       
			case 'a':
                                if(*playerX > 1) //TODO: collions with outer walls (1 , 68, 1, 24) is hardcodded to current size
					         //SOLN: easiest fix is to store these in wallsY and wallsX
                                {
                                        int i = 0;
                                        for(i; i < numWallVerts; i++)
                                        {
                                                if(wallsY[i] == *playerY && wallsX[i] == (*playerX - 1))
                                                {
                                                        col = true;
                                                        break;
                                                }

                                                col =  false;
                                        }

                                        if (!col)
                                        {
                                                mvwprintw(w, *playerY, *playerX, " ");
                                                mvwprintw(w, *playerY, --*playerX, "*");
                                        }
                                }
                                break;

                        case 'd':
                                if(*playerX < 68)
                                {
                                        int i = 0;
                                        for(i; i < numWallVerts; i++)
                                        {
                                                if(wallsY[i] == *playerY && wallsX[i] == (*playerX + 1))
                                                {
                                                        col = true;
                                                        break;
                                                }

                                                col = false;
                                        }

                                        if(!col)
                                        {
                                                mvwprintw(w, *playerY, *playerX, " ");
                                                mvwprintw(w, *playerY, ++*playerX, "*");
                                        }
                                }
                                break;

                                break;
                        case 'w':
                                if(*playerY > 1)
                                {
                                        mvwprintw(w, *playerY, *playerX, " ");
                                        mvwprintw(w, --*playerY, *playerX, "*");
                                }
                                break;
                        case 's':
                                if(*playerY < 24)
                                {
                                        mvwprintw(w, *playerY, *playerX, " ");
                                        mvwprintw(w, ++*playerY, *playerX, "*");
                                }
                                break;
                }

}
