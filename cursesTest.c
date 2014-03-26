#include <curses.h>

int **WALLS;
int numWallVerts = 0;

typedef struct room{
	char name[10];
	unsigned int doors[4][10];
	int y, x, height, width;
	int tl;
	int tr;
	int bl;
	int br;
} room;

void wmakeroom(WINDOW *w, room *r);
void drawroom(WINDOW *w, room *r);
void makecollidable(room *r);
void fillwallarray(int start, int end, int initY, int initX, int doors[40], bool vertical);
void initdefaultroom(room *def);
void wmoveplayer(WINDOW *w, int *playerY, int *playerX, char input);


int main()
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	
	int termSizeX, termSizeY;
	getmaxyx(stdscr, termSizeY, termSizeX);
	
	static room DEFAULT_ROOM;
	initdefaultroom(&DEFAULT_ROOM);
	
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

	struct room newRoom = DEFAULT_ROOM;
	newRoom.name[0] = 'T';
	newRoom.doors[0][4] = 1;
	newRoom.y = 4;
	newRoom.x = 4;
	newRoom.height = 10;
	newRoom.width = 10;

	wmakeroom(win, &newRoom);
	wrefresh(win);

	while((ch = getch()) != 'q')
	{	
		moveplayer(win, &playerY, &playerX, ch);
		wrefresh(win);
	}


	endwin();
	return 0;
}

void initdefaultroom(room *def)
{
	def->tl = ACS_ULCORNER;
	def->tr = ACS_URCORNER;
	def->bl = ACS_LLCORNER;
	def->br = ACS_LRCORNER;
}

void wmakeroom(WINDOW *w, room *r)
{
	drawroom(w, r);
	makecollidable(r);
}

void drawroom(WINDOW *w, room *r)
{
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

        //ADD DOORS//
        int i, j;
        for (i = 0; i < 4; i++)
        {
                for (j = 0; j < 10; j++)
                {
                        if (r->doors[i][j] == 1)
                        {
                                switch (i) 
                                {
                                        case 0: mvwaddch(w, r->y, r->x+j, ' '); break;
                                        case 1: mvwaddch(w, r->y+j, r->x+r->width, ' '); break;
                                        case 2: mvwaddch(w, (r->y+r->height)-1, (r->x+r->width)-j, ' '); break;
                                        case 3: mvwaddch(w, (r->y+r->height)-(1+j), r->x, ' '); break;
                                }
                        }
                }
        }

}

void makecollidable(room *r)
{
	int tNVs = numWallVerts;
	
	int doorArray[40];	//TODO: refactor this block
	int i;
	for (i = 0; i < 40; i++)
	{
			if (i < 10 && r->doors[0][i] == 1)
				doorArray[i] = i;

			else if (i < 20 && r->doors[1][i] == 1)
				doorArray[i] = r->width+i;

			else if (i < 30 && r->doors[2][i] == 1)
					doorArray[i] = r->width+r->height+i;
			
			else if (i < 40 && r->doors[3][i] == 1)
					doorArray[i] = (2*r->width)+r->height+i;
	}
	
	if (WALLS == NULL)
	{	
		WALLS = malloc(2*(r->height+r->width)*sizeof(int*));
		int i = 0;
		for (i; i < 2*(r->height+r->width); i++)
			WALLS[i] = malloc(2 * sizeof(int));
	}
	
	else
	{
		int **tmpPtr;
		tmpPtr = realloc (WALLS, (numWallVerts + (2*(r->height+r->width) * sizeof(int*))));
		if (tmpPtr != NULL)
			WALLS = tmpPtr;

		int i = numWallVerts;
		for(i; i < numWallVerts + 2*(r->height+r->width); i++)
			WALLS[i] = malloc(2* sizeof(int));
	}

	numWallVerts += 2*(r->height+r->width);

	fillwallarray(0, r->width, r->y, r->x, doorArray, false);

/*	int off = 0;
	for (i; i < r->width+r->height; i++) //data for the two vertical lines;
	{
		WALLS[i][0] = r->y+off;
		WALLS[i][1] = r->x;
		WALLS[i+r->height][0] = r->y+off;
		WALLS[i+r->height][1] = r->x+r->width;
		off++;
	}*/	
}

void fillwallarray(int start, int end, int initY, int initX, int doors[40], bool vertical)
{
	int i;
	int off = -1;
        for (i = start; i < end; i++) //data for top and bottom lines
        {
		off++;
                int j;
                bool matchdoor = FALSE;
                for (j = 1; j < 40; j++)
                {
                        if (i == doors[j])
                                matchdoor = TRUE;
                }
                if (matchdoor)
                        continue;

		if (vertical)
		{
                	WALLS[i][0] = initY+off;
                	WALLS[i][1] = initX;
		}

		else
		{
			WALLS[i][0] = initY;
			WALLS[i][1] = initX+off;
		}
        }

}

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
                                                if(WALLS[i][0] == *playerY && WALLS[i][1] == (*playerX - 1))
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
                                                if(WALLS[i][0] == *playerY && WALLS[i][1] == (*playerX + 1))
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
