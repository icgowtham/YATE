/* Contains function definitions. */

#include "source.h"

#ifndef __SOURCE_C
#define __SOURCE_C 

void Startup()
{
	initscr();
	start_color();
	keypad(stdscr, TRUE);
	noecho();
	cbreak();
	
	init_pair(1,  COLOR_WHITE,   COLOR_BLUE);
	init_pair(2,  COLOR_YELLOW,  COLOR_WHITE);
	init_pair(3,  COLOR_GREEN ,  COLOR_WHITE);
	init_pair(4,  COLOR_GREEN,   COLOR_BLUE);
	init_pair(5,  COLOR_MAGENTA, COLOR_WHITE);
	init_pair(6,  COLOR_RED,     COLOR_WHITE);
	init_pair(7,  COLOR_BLUE,    COLOR_WHITE);
	init_pair(8,  COLOR_CYAN,    COLOR_BLACK);
	init_pair(9,  COLOR_RED,     COLOR_BLACK);
	init_pair(10, COLOR_WHITE,   COLOR_BLACK);
	init_pair(11, COLOR_YELLOW,  COLOR_BLACK);
	init_pair(12, COLOR_WHITE,   COLOR_CYAN);
	
	Main = newwin(23, 80, 1, 0);
	mainP = new_panel(Main);
	
	menu_bar = newwin(1, 80, 0, 0);
	mbP = new_panel(menu_bar);
	status_bar = newwin(1, 80, 24, 0);
	sbP = new_panel(status_bar);
	Show_Screen();
	curs_set(0);
	wmove(Main, row, 1);
}

void Show_Screen()
{
	curs_set(0);
	wbkgd(Main, COLOR_PAIR(1));
	wattron(Main, A_BOLD);
	wborder(Main, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26,      	ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);

	wbkgd(menu_bar, COLOR_PAIR(8));
	wmove(menu_bar, 0, 5);
	waddch(menu_bar, 'F' | A_BOLD);
	waddstr(menu_bar, "ile");
	wmove(menu_bar, 0, 15);
	waddch(menu_bar, 'E' | A_BOLD);
	waddstr(menu_bar, "dit");
	wmove(menu_bar, 0, 25);
	waddch(menu_bar, 'O' | A_BOLD);
	waddstr(menu_bar, "ptions");
	wmove(menu_bar, 0, 37);
	waddch(menu_bar, 'H' | A_BOLD);
	waddstr(menu_bar, "elp");
	wmove(menu_bar, 0, 72);
	wattron(menu_bar, COLOR_PAIR(10) | A_BOLD);
	waddstr(menu_bar, "YATE");
	wattroff(menu_bar, COLOR_PAIR(10) | A_BOLD);
	
	wbkgd(status_bar, COLOR_PAIR(8));
	wmove(status_bar, 0, 0);
	waddstr(status_bar, "F1 - Help   F2 - Save   F3 - Open");
	wmove(status_bar, 0, 36);
	waddstr(status_bar, "^O - Shell   F10 - About ");

	update_panels();
	doupdate();
}

void Shutdown()
{
	delwin(Main);
	delwin(menu_bar);
	delwin(status_bar);
	del_panel(mainP);
	del_panel(mbP);
	del_panel(sbP);

	endwin();
}

NODEY *Create()
{
	NODEY *new;
	new = (NODEY *) malloc(sizeof(NODEY));
	if(new == NULL)
	{
		msgbox("Fatal Error","YATE - Out Of Memory");
		endwin();
		system("clear");
		exit(0);
	}
	new->up = new->down = NULL;
	new->columns = NULL;

	return new;
}

NODEX *InsBefore(NODEX *node, chtype c)
{
	NODEX *temp = (NODEX *) malloc(sizeof(NODEX));
	temp->info = c;

	node->prev = temp;
	temp->next = node;
	temp->prev = NULL;

	return temp;
}

NODEX *InsMiddle(NODEX *node, chtype c)
{
	NODEX *temp = (NODEX *) malloc(sizeof(NODEX));
	temp->info = c;
	if(node == NULL)
		return temp;

	temp->next = node->next;
	node->next->prev = temp;
	temp->prev = node;
	node->next = temp;

	return temp;
}

NODEY *InsLine(NODEY *node, NODEX *xnd)
{
	NODEY *New;
	NODEX *tmp;
	New = Create();
	tmp = node->columns;
	New->columns = (NODEX *) malloc(sizeof(NODEX));

	if(xnd != NULL)
	{
		New->columns = xnd->next;
		xnd->next = NULL;
	}

	else
	{
		New->columns = node->columns;
		node->columns = NULL;
	}

	curX = NULL;
	New->up = node;
	New->down = node->down;
	if(node->down != NULL)
		node->down->up = New;
	node->down = New;

	return New;
}

NODEX *InsAfter(NODEX *node, chtype c)
{
	NODEX *temp = (NODEX *) malloc(sizeof(NODEX));
	temp->info = c;
	temp->next = temp->prev = NULL;

	if(node == NULL)
	{
		temp->next = curY->columns;
		curY->columns = temp;
		return temp;
	}

	if(node != NULL && node->next != NULL)
	{
		InsMiddle(node, c);
		return node->next;
	}

	node->next = temp;
	temp->prev = node;
	return temp;
}

NODEX *Left(NODEX *temp)
{
	if(temp == NULL)
	{
		beep();
		return NULL;
	}

	temp = temp->prev;
	col--;
	return temp;
}

NODEX *Right(NODEX *temp)
{
	if(temp == NULL)
	{
		if(curY->columns != NULL)
			col++;
		else
			beep();
		return curY->columns;
	}

	if(temp->next == NULL)
	{
		beep();
		return temp;
	}

	col++;
	return temp->next;
}

NODEY *Up(NODEY *temp)
{
	int i = 0;

	if(temp->up == NULL)
	{
		beep();
		return temp;
	}

	temp = temp->up;

	line--;
	curX = temp->columns;

	if(curX == NULL)
		col = 0;

	else
	{
		while(curX->next != NULL && i < col)
		{
			i++;
			curX = curX->next;
		}
		curX = curX->prev;
		if(col == 0)
			curX = NULL;
		else if(i < col)
		col = i;
       }

	if(row != 1)
		row--;
	else
		scrollup();

	return temp;
}

NODEY *Down(NODEY *temp)
{
	int i = 0;

	if(temp->down == NULL)
	{
		beep();
		return temp;
	}

	temp = temp->down;
	line++;
	curX = temp->columns;

	if(curX == NULL)
		col = 0;
	else
	{
		while(curX->next != NULL && i < col)
		{
			i++;
			curX = curX->next;
		}
		curX = curX->prev;

		if(col == 0)
			curX = NULL;
		else if(i < col)
			col = i;
	}
	if(row != 21)
		row++;
	else
	{
		row = 21;
		scrolldown();
	}

	return temp;
}

void Home()
{
	beep();
	col = 0;
	curX = NULL;
}

void End()
{
	if(curX == NULL)
	{
		if(curY->columns == NULL)
			 return;
		curX = curY->columns;
		col++;
	}

	while(curX->next != NULL)
	{
		curX = curX->next;
		col++;
	}
}

NODEY *Pageup(NODEY *temp)
{
	int i;
	if(temp == NULL)
	{
		beep();
		return;
	}

	for(i = 0; i < 20; i++)
		temp = Up(temp);

	return temp;
}

NODEY *Pagedown(NODEY *temp)
{
	int i;
	if(temp->down == NULL)
	{
		beep();
		return temp;
	}

	for(i = 0; i < 20; i++)
		temp = Down(temp);

	return temp;
}

void delNDY(NODEY *temp)
{
	if(temp->up != NULL)
		temp->up->down = temp->down;

	if(temp->down != NULL)
		temp->down->up = temp->up;

	free(temp);
}

NODEX *Backspace(NODEX *temp)
{
	NODEX *temp1 = temp;

	if(temp == NULL)
	{
		if(curY->up != NULL)
		{
			NODEY *ty;
			curY = curY->up;
			ty = curY->down;
			line--;
			End();
			if(curY->columns == NULL)
			{
				curY->columns = curY->down->columns;
				delNDY(curY->down);
			}
			else
			{
				if(ty->columns != NULL)
				{
					curX->next = ty->columns;
					delNDY(ty);
				}
			}
			row--;
			return curX;
		}
	}

	/* If there is no node. */
	if(temp == NULL)
	{
		beep();
		return NULL;
	}

	/* If there is only one node. */
	if(temp->prev == NULL)
	{
		curY->columns = temp->next;
		free(temp);
		col = 0;
		return NULL;
	}

	{
		if(temp->next != NULL)
			temp->next->prev = temp->prev;
		if(temp->prev != NULL)
			temp->prev->next = temp->next;
		temp = temp->prev;
		free(temp1);

		col--;
		return temp;
	}
}

NODEX *Del(NODEX *temp)
{
	if(temp == NULL)
	{
		if(curY->columns != NULL)
		{
			NODEX *temp1  = curY->columns;
			curY->columns = temp1->next;
			free(temp1);
		}
		else
			if(curY->down != NULL)
			{
				curY->columns = curY->down->columns;
				delNDY(curY->down);
			}
		return NULL;
	}

	if(temp->next == NULL)
	{
		if(curY->down != NULL)
		{
			temp->next = curY->down->columns;
			delNDY(curY->down);
		}

		return temp;
	}
	{
		NODEX *temp1 = temp->next;
		temp->next = temp1->next;
		if(temp1->next != NULL)
			temp1->next->prev = temp;
		free(temp1);
		return temp;
	}
}

void scrollup()
{
	if(scrfirst->up != NULL)
		scrfirst = scrfirst->up;
}

void scrolldown()
{
	if(curY->down != NULL)
		scrfirst = scrfirst->down;
}


void textWrap()
{
	if(col == 78)
	{
		curY = InsLine(curY, curX);
		wborder(Main, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26, ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);
		row++;
		line++;
		col = 0;
		if(line > 21)
		{
			row -= 1;
			scrfirst = scrfirst->down;
		}
		curs_set(1);
	}
}

void Display(NODEY *temp)
{
	NODEX *x;
	int i = 1;
	x = temp->columns;
	if(x == NULL)
		return;
	while(x != NULL && i <= 78)
	{
		wprintw(Main, "%c",  x->info);
		x = x->next;
		i++;
	}
}

void DispScreen(NODEY *scr)
{
	NODEY *temp = scr;
	int r;
	if(scr == NULL)
		return;
	for(r = 1; r <= 21; r++)
		mvwhline(Main, r, 1, ' ', 78);
	r = 1;

	while(temp != NULL && r <= 21)
	{
		wmove(Main, r, 0);
		Display(temp);
		temp = temp->down;
		r++;
	}
	update_panels();
	doupdate();
}

void msgbox(char *title, char *message)
{
	WINDOW *msg_win, *msg_shadow;

	curs_set(0);

	msg_win = newwin(10, 40, 8, 20);

	msg_shadow = newwin(10, 40, 9, 21);
	wbkgd(msg_shadow, COLOR_BLACK | A_DIM);
	wrefresh(msg_shadow);

	wbkgd(msg_win, COLOR_PAIR(8));
	box(msg_win, ACS_VLINE, ACS_HLINE);
	mvwprintw(msg_win, 0, 2, "%s", title);
	mvwprintw(msg_win, 4, 7, "%s", message);

	wrefresh(msg_win);

	noecho();
	cbreak();

	keypad(msg_win, TRUE);
	wgetch(msg_win);

	delwin(msg_shadow);
	delwin(msg_win);
}

void New()
{
	clearok(Main, TRUE);
	Close();
	touchwin(Main);
	wrefresh(Main);
	if((strcmp(curF, "Untitled")) != 0)
		strcpy(curF, "Untitled");
	line = 1;
	row = 1;
	col = 0;
	wmove(Main, row, col);
}

void Open(char *fname)
{
	chtype  ch;
	int i;
	NODEY  *temp;
	NODEX  *tmp;
	strcpy(filename, fname);
	curFile = fopen(filename, "r+w");
	if(curFile == NULL)
	{
		touchwin(Main);
		wrefresh(Main);
		msgbox(" YATE Error ", "File requested does not exist");
		touchwin(Main);
		wrefresh(Main);
		wmove(Main, row, col);
		curs_set(1);
		return;
	}
	fseek(curFile, 0, SEEK_SET);
	temp = Create();
	curY = temp;
	scrfirst = temp;
	tmp = NULL;
	row  = 0;
	col  = 0;

	while((ch = fgetc(curFile)) != EOF)
	{
		if(ch == '\t')
		{
			for(i = 0; i < tab_stop; i++)
			{
				tmp = InsAfter(tmp, ' ');
				col++;
			}
		}

		else if(ch == '\n' || col == 78)
		{
			curY = InsLine(curY, tmp);
			tmp = NULL;
			row++;
			col = 0;
		}

		else
		{
			tmp = InsAfter(tmp, ch);
			col++;
			DispScreen(scrfirst);
			doupdate();
			update_panels();
	       }
	       textWrap();
	}
	fclose(curFile);
	wborder(Main, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26,     	ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);
	curY = temp;
	curX = NULL;
	row  = 1;
	line = 1;
	col  = 0;
}

void Close()
{
	if(changed == TRUE)
		confirm(" Save changes to the file? (Y/N) ");
	touchwin(Main);
	wrefresh(Main);
	Cleanup();
	curY = Create();
	scrfirst = curY;
	Screen   = curY;
	curX = NULL;
	clearok(Main, TRUE);
	wborder(Main, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26,		ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);
	row = 1;
	col = 0;
	line = 1;
	wmove(Main, row, col);
}

void Save(char *fname)
{
	WINDOW *status;
	NODEY *temp;
	NODEX *tmp;
	curs_set(0);
	status = newwin(1, 80, 24, 0);
	wbkgd(status, COLOR_PAIR(8));
	strcpy(filename, fname);
	file = fopen(filename, "w");
	if (file == NULL)
	{
		mvwprintw (status, 0, 1, " Unable to save file: ");
		mvwprintw (status, 0, 23, "%s. ", strerror (errno));
		waddstr (status, "Choose another name/path. ");

		file = fopen (filename, "r");
		return;
	}

	temp = curY;
	while(temp->up != NULL)
		temp = temp->up;

	while(temp != NULL)
	{
		tmp = temp->columns;
		while(tmp != NULL)
		{
			fprintf(file, "%c", tmp->info);
			tmp = tmp->next;
		}
		fputc('\n', file);
		temp = temp->down;
	}
	fclose(file);
	delwin(status);
	touchwin(Main);
	wrefresh(Main);
	curs_set(1);
}

void SaveAs()
{
	Fname = input(" Save As ", " Enter the name of the file ", filename);
	Save(Fname);
	touchwin(Main);
	wrefresh(Main);
}


void Delete()
{
	char c;
	char system_cmd[273];
	int retval, i;
	WINDOW *del;
	PANEL  *delp;
	del = newwin(3, 52, 11, 14);
	delp  = new_panel(del);
	wbkgd(del, COLOR_PAIR(8));
	box(del, ACS_VLINE, ACS_HLINE);
	mvwprintw(del, 1, 3, " Do you really want to delete the file? (Y/N) ");
	echo();
	cbreak();
	c = wgetch(del);
	show_panel(delp);
	doupdate();
	update_panels();

	if(c == 'Y' || c == 'y')
	{
		strcpy (system_cmd, "rm -f ");
		strcat (system_cmd, curF);
		strcat (system_cmd, " 2> /dev/null");
		retval = system (system_cmd);

		if (retval == 0)
		{
			touchwin(Main);
			wrefresh(Main);
			Cleanup();
			curY = Create();
			scrfirst = curY;
			Screen = curY;
			curX = NULL;
			msgbox(" Success ", " File deleted successfully. ");
			touchwin(Main);
			wrefresh(Main);
			row = 1;
			col = 0;
			wmove(Main, row, col);
		}
		else
		{
			msgbox (" Failure"," Directory possibly read-only. ");
			touchwin(Main);
			wrefresh(Main);
		}
	}

	if(c == 'N' || c == 'n')
	{
		touchwin(Main);
		wrefresh(Main);
	}

	if(c == ESC)
	{
		touchwin(Main);
		wrefresh(Main);
		return;
	}

	del_panel(delp);
	delwin(del);
}


void Cleanup()
{
	NODEY *y, *p;
	NODEX *x, *q;

	y = curY;
	while(y->up != NULL)
		y = y->up;

	while(y != NULL)
	{
		p = y->down;
		x = y->columns;
		while(x != NULL)
		{
			q = x->next;
			free(x);
			x = q;
		}
		free(y);
		y = p;
		beep();
	}
}

void insertWord()
{
	char word[30];
	int i = 0;
	WINDOW *iW;
	PANEL  *iwp;
	iW = newwin(5, 40, 11, 20);
	iwp  = new_panel(iW);
	wbkgd(iW, COLOR_PAIR(8));
	box(iW, ACS_VLINE, ACS_HLINE);
	mvwprintw(iW, 0, 2, " Insert Word ");
	mvwprintw(iW, 2, 5, " Enter the word: ");
	echo();
	cbreak();
	wmove(iW, 3, 5);
	wscanw(iW, "%s", word);
	show_panel(iwp);
	doupdate();
	update_panels();

	curX = InsAfter(curX, ' ');
	col++;
	while(word[i] != '\0')
	{
		curX = InsAfter(curX, word[i]);
		col++;
		i++;
	}
	curX = InsAfter(curX, ' ');
	col++;

	touchwin(Main);
	wrefresh(Main);
	del_panel(iwp);
	delwin(iW);
}

void delLine()
{
	if(curY->up == NULL && curY->down == NULL)
	{
		delNDY(curY);
		curY = Create();
		scrfirst = curY;
		Screen = scrfirst;
		curX = NULL;
		update_panels();
		doupdate();
		row = 1;
		col = 0;
		line = 1;
	}

	else
	if(curY->down != NULL)
	{
		if(curY->up == NULL)
		{
			curY = Down(curY);
			scrfirst = curY;
			Screen = curY;
			delNDY(curY->up);
			line--;
			row--;
			update_panels();
			doupdate();
		}
		else
		{
			curY = Down(curY);
			delNDY(curY->up);
			row--;
		}
	}

	else
	{
		if(curY->up != NULL)
			curY = Up(curY);
		delNDY(curY->down);
	}
}


void bkColour()
{
	short cur_fg, cur_bg;
	short new_bg;
	chtype c;
	WINDOW* win, *status;

	pair_content (1, &cur_fg, &cur_bg);
	curs_set(0);
	status = newwin(1, 80, 24, 0);
	wbkgd(status, COLOR_PAIR(8));

	win = newwin (3, 33, 10, 24);
	box (win, ACS_VLINE, ACS_HLINE);
	wbkgd (win, COLOR_PAIR(8));
	mvwaddstr (win, 0, 1, " Background ");

	mvwaddstr (status, 0, 1, " Press ENTER to apply, ESC to quit. ");
	wrefresh (status);

	curs_set (0);
	mvwaddstr (win, 1, 1, "Press PGUP and PGDN to change.");
	noecho();
	keypad (win, TRUE);
	wmove (win, 1, 1);
	new_bg = cur_bg;

	init_pair (1, cur_fg, cur_bg);
	wbkgd (win, COLOR_PAIR(1));

	while ((c = wgetch (win)) != '\n')
	{
		switch (c)
		{
			case KEY_PPAGE:
				if (new_bg == 0)
					new_bg = 7;
				else
					new_bg--;
				init_pair (1, cur_fg, new_bg);
				wbkgd (win, COLOR_PAIR(1));
				break;

			case KEY_NPAGE:
				if (new_bg == 7)
					new_bg = 0;
				else
					new_bg++;
					init_pair (1, cur_fg, new_bg);
					wbkgd (win, COLOR_PAIR(1));
				break;

			case 27:
				init_pair (1, cur_fg, cur_bg);
				wbkgd (Main, COLOR_PAIR(1));
				delwin (win);
				delwin (status);
				curs_set (1);
				touchwin (Main);
				wrefresh (Main);
		}
	}


	wbkgd (Main, COLOR_PAIR(1));

	mvwaddstr (status, 0, 1, " Applied changes successfully. ");

	delwin (win);
	delwin (status);
	curs_set (1);
	touchwin (Main);
	wrefresh (Main);
}

void textColour()
{
	short cur_fg, cur_bg;
	short new_fg;
	chtype c;
	WINDOW* win, *status;

	pair_content (1, &cur_fg, &cur_bg);

	curs_set(0);
	status = newwin(1, 80, 24, 0);
	wbkgd(status, COLOR_PAIR(8));
	win = newwin (3, 33, 10, 24);
	box (win, ACS_VLINE, ACS_HLINE);
	wbkgd (win, COLOR_PAIR(1));
	mvwaddstr (win, 0, 1, " Foreground ");

	mvwaddstr (status, 0, 1, " Press ENTER to apply, ESC to quit. ");
	wrefresh (status);

	curs_set (0);
	mvwaddstr (win, 1, 1, "Press PGUP and PGDN to change.");
	noecho();
	keypad (win, TRUE);
	wmove (win, 1, 1);
	new_fg = cur_fg;

	init_pair (1, cur_fg, cur_bg);
	wbkgd (win, COLOR_PAIR(1));

	while ((c = wgetch (win)) != '\n')
	{
		switch (c)
		{
			case KEY_PPAGE:
				if (new_fg == 0)
					new_fg = 7;
				else
					new_fg--;
				init_pair (1, new_fg, cur_bg);
				wbkgd (win, COLOR_PAIR(1));
				break;

			case KEY_NPAGE:
				if (new_fg == 7)
					new_fg = 0;
				else
					new_fg++;
				init_pair (1, new_fg, cur_bg);
				wbkgd (win, COLOR_PAIR(1));
				break;

			case 27:
				init_pair (1, cur_fg, cur_bg);
				wbkgd (Main, COLOR_PAIR(1));
				delwin (win);
				delwin (status);
				curs_set (1);
				touchwin (Main);
				wrefresh (Main);
		  }
	}

	wbkgd (Main, COLOR_PAIR(1));

	mvwaddstr (status, 0, 1, " Applied changes successfully. ");

	delwin (win);
	delwin (status);
	curs_set (1);
	touchwin (Main);
	wrefresh (Main);
}

int file_menu()
{
	MENU *file;
	ITEM *file_items[11];
	int file_rows, file_cols, ch, i;
	WINDOW *filewin, *file_subwin, *file_shadow;

	file_items[0]  = new_item("New",	"    ^N");
	file_items[1]  = new_item("Open",	"    F3");
	file_items[2]  = new_item("Close",	"    ^K");
	file_items[3]  = new_item(" ",		"      ");
	file_items[4]  = new_item("Save",	"    F2");
	file_items[5]  = new_item("Save As ..", "      ");
	file_items[6]  = new_item("Delete",	"    ^D");
	file_items[7]  = new_item(" ",		"      ");
	file_items[8]  = new_item("OS Shell",	"    ^O");
	file_items[9]  = new_item("Exit",	" ALT+X");
	file_items[10]  = NULL;

	file = new_menu(file_items);
	scale_menu(file, &file_rows, &file_cols);

	filewin = newwin(file_rows+2, file_cols+2, 1, 5);

	file_shadow = newwin(file_rows+2, file_cols+2, 2, 6);
	wbkgd(file_shadow, COLOR_BLACK);
	wrefresh(file_shadow);

	box(filewin, ACS_VLINE, ACS_HLINE);
	file_subwin = derwin(filewin, file_rows, file_cols, 1, 1);

	wbkgd(filewin, COLOR_PAIR(8));


	set_menu_win(file, filewin);
	set_menu_sub(file, file_subwin);
	set_menu_back(file, COLOR_PAIR(8));
	set_menu_fore(file, COLOR_PAIR(8)  | A_REVERSE);
	set_menu_grey(file, COLOR_PAIR(11));
	set_menu_mark(file, " ");
	menu_opts_off(file, O_NONCYCLIC);

	item_opts_off(file_items[3], O_SELECTABLE);
	item_opts_off(file_items[7], O_SELECTABLE);

	if(file == NULL || curY == NULL)
	{
		item_opts_off(file_items[2], O_SELECTABLE);
		item_opts_off(file_items[4], O_SELECTABLE);
		item_opts_off(file_items[5], O_SELECTABLE);
		item_opts_off(file_items[6], O_SELECTABLE);
	}

	post_menu(file);
	wrefresh(filewin);
	wrefresh(file_subwin);

	noecho();
	cbreak();

	keypad(file_subwin, TRUE);

	while((ch = wgetch(file_subwin)) != ESC)
	{
		switch(ch)
		{
			case KEY_UP:
				menu_driver(file, REQ_UP_ITEM);
			while(item_opts(current_item(file)) != O_SELECTABLE)
				menu_driver(file, REQ_UP_ITEM);
				break;

			case KEY_DOWN:
				menu_driver(file, REQ_DOWN_ITEM);
			while(item_opts(current_item(file)) != O_SELECTABLE)
				menu_driver(file, REQ_DOWN_ITEM);
				break;

			case KEY_LEFT:
				ungetch('h');
				ungetch(27);
				break;

			case KEY_RIGHT:
				ungetch('e');
				ungetch(27);
				break;

			case 10:
				touchwin(Main);
				wrefresh(Main);
				curs_set(1);
				choice = item_index(current_item(file));
				switch(choice)
				{
					case 0:
						New();
						break;
					case 1:
		Fname = input(" Open File "," Enter the file name ", filename);
						Open(Fname);
						break;
					case 2:
						Close();
						break;
					case 4:
		Fname = input(" Save File "," Enter the file name ", filename);
						Save(Fname);
						break;
					case 5:
						SaveAs();
						break;
					case 6:
						Delete();
						break;
					case 8:
						shell();
						break;
					case 9:
						curs_set(1);
						Quit();
						break;
				}
				break;
		}
	}
	ungetch(27);
	wmove(Main, row, col);

	unpost_menu(file);
	for(i = 0; i < 11; i++)
		free_item(file_items[i]);

	free_menu(file);
	delwin(file_shadow);
	delwin(filewin);
	delwin(file_subwin);

	return 0;
}

int edit_menu()
{
	MENU *edit;
	ITEM *edit_items[4];
	int edit_rows, edit_cols, ch, i;
	WINDOW *editwin, *edit_subwin, *edit_shadow;

	edit_items[0]  = new_item("Insert Line","^L");
	edit_items[1]  = new_item("Delete Line","^Y");
	edit_items[2]  = new_item("Insert Word","^W");
	edit_items[3]  = NULL;

	edit = new_menu(edit_items);
	scale_menu(edit, &edit_rows, &edit_cols);

	editwin = newwin(edit_rows+2, edit_cols+2, 1, 15);

	edit_shadow = newwin(edit_rows+2, edit_cols+2, 2, 16);
	wbkgd(edit_shadow, COLOR_BLACK | A_DIM);
	wrefresh(edit_shadow);

	box(editwin, ACS_VLINE, ACS_HLINE);
	edit_subwin = derwin(editwin, edit_rows, edit_cols, 1, 1);

	wbkgd(editwin, COLOR_PAIR(8));


	set_menu_win(edit, editwin);
	set_menu_sub(edit, edit_subwin);
	set_menu_back(edit, COLOR_PAIR(8));
	set_menu_fore(edit, COLOR_PAIR(8) | A_REVERSE);
	set_menu_grey(edit, COLOR_PAIR(11));
	set_menu_mark(edit, " ");
	menu_opts_off(edit, O_NONCYCLIC);


	if(curY == NULL)
	{
		item_opts_off(edit_items[0], O_SELECTABLE);
		item_opts_off(edit_items[1], O_SELECTABLE);
		item_opts_off(edit_items[2], O_SELECTABLE);
		item_opts_off(edit_items[3], O_SELECTABLE);
	}

	post_menu(edit);
	wrefresh(editwin);
	wrefresh(edit_subwin);

	noecho();
	cbreak();

	keypad(edit_subwin, TRUE);

	while((ch = wgetch(edit_subwin)) != ESC)
	{
		switch(ch)
		{
			case KEY_UP:
				menu_driver(edit, REQ_UP_ITEM);
			while(item_opts(current_item(edit)) != O_SELECTABLE)
				menu_driver(edit, REQ_UP_ITEM);
				break;

			case KEY_DOWN:
				menu_driver (edit, REQ_DOWN_ITEM);
			while(item_opts(current_item(edit)) != O_SELECTABLE)
				menu_driver(edit, REQ_DOWN_ITEM);
				break;

			case KEY_LEFT:
				ungetch('f');
				ungetch(27);
				break;

			case KEY_RIGHT:
				ungetch('o');
				ungetch(27);
				break;

			case 10:
				touchwin(Main);
				wrefresh(Main);
				curs_set(1);
				choice = item_index(current_item(edit));
				switch(choice)
				{

					case 0:
						curY = InsLine(curY, curX);
						row++;
						line++;
						col = 0;
						break;
					case 1:
						delLine();
						break;
					case 2:
						insertWord();
						break;
				}
				touchwin(Main);
				wrefresh(Main);
				break;
		}
	}
	ungetch(27);

	unpost_menu(edit);
	for(i = 0; i < 4; i++)
		free_item(edit_items[i]);

	free_menu(edit);
	delwin(edit_shadow);
	delwin(editwin);
	delwin(edit_subwin);

	return choice;
}


int options_menu()
{
	MENU *options;
	ITEM *opts_items[3];
	int opts_rows, opts_cols, ch, i;
	WINDOW *optswin, *opts_subwin, *opts_shadow;

	opts_items[0] = new_item("Text Colour",		"  ");
	opts_items[1] = new_item("Background Colour",	"  ");
	opts_items[2] = NULL;

	options = new_menu(opts_items);
	scale_menu(options, &opts_rows, &opts_cols);

	optswin = newwin(opts_rows+2, opts_cols+2, 1, 25);

	opts_shadow = newwin(opts_rows+2, opts_cols+2, 2, 26);
	wbkgd(opts_shadow, COLOR_BLACK | A_DIM);
	wrefresh(opts_shadow);

	box(optswin, ACS_VLINE, ACS_HLINE);
	opts_subwin = derwin(optswin, opts_rows, opts_cols, 1, 1);

	wbkgd(optswin, COLOR_PAIR(8));

	set_menu_win(options, optswin);
	set_menu_sub(options, opts_subwin);
	set_menu_back(options, COLOR_PAIR(8));
	set_menu_fore(options, COLOR_PAIR(8) | A_REVERSE);
	set_menu_grey(options, COLOR_PAIR(11));
	set_menu_mark(options, " ");
	menu_opts_off(options, O_NONCYCLIC);

	post_menu(options);
	wrefresh(optswin);
	wrefresh(opts_subwin);

	noecho();
	cbreak();

	keypad(opts_subwin, TRUE);

	while((ch = wgetch(opts_subwin)) != ESC)
	{
		switch(ch)
		{
			case KEY_UP:
				menu_driver(options, REQ_UP_ITEM);
			while(item_opts(current_item(options)) != O_SELECTABLE)
				menu_driver(options, REQ_UP_ITEM);
				break;

			case KEY_DOWN:
				menu_driver(options, REQ_DOWN_ITEM);
			while(item_opts(current_item(options)) != O_SELECTABLE)
				menu_driver(options, REQ_DOWN_ITEM);
				break;

			case KEY_LEFT:
				ungetch('e');
				ungetch(27);
				break;

			case KEY_RIGHT:
				ungetch('h');
				ungetch(27);
				break;

			case 10:
				touchwin(Main);
				wrefresh(Main);
				choice = item_index(current_item(options));
				switch(choice)
				{
					case 0:
						textColour();
						break;
					case 1:
						bkColour();
						break;
				}
				break;
		}
	}
	ungetch(27);

	wmove(Main, row, col);

	unpost_menu(options);
	for(i = 0; i < 3; i++)
		free_item(opts_items[i]);

	free_menu(options);
	delwin(opts_shadow);
	delwin(optswin);
	delwin(opts_subwin);

	return choice;
}


int help_menu()
{
	MENU *help;
	ITEM *help_items[3];
	int help_rows, help_cols, ch, i;
	WINDOW *helpwin, *help_subwin, *help_shadow;

	help_items[0]  = new_item("Using YATE", "F1 ");
	help_items[1]  = new_item("About",	"F10");
	help_items[2]  = NULL;

	help = new_menu(help_items);
	scale_menu(help, &help_rows, &help_cols);

	helpwin = newwin(help_rows+2, help_cols+2, 1, 37);

	help_shadow = newwin(help_rows+2, help_cols+2, 2, 38);
	wbkgd(help_shadow, COLOR_BLACK | A_DIM);
	wrefresh(help_shadow);

	box(helpwin, ACS_VLINE, ACS_HLINE);
	help_subwin = derwin(helpwin, help_rows, help_cols, 1, 1);

	wbkgd(helpwin, COLOR_PAIR(8));


	set_menu_win(help, helpwin);
	set_menu_sub(help, help_subwin);
	set_menu_back(help, COLOR_PAIR(8));
	set_menu_fore(help, COLOR_PAIR(8) | A_REVERSE);
	set_menu_mark(help, " ");
	menu_opts_off(help, O_NONCYCLIC);

	post_menu(help);
	wrefresh(helpwin);
	wrefresh(help_subwin);

	noecho();
	cbreak();

	keypad(help_subwin, TRUE);

	while((ch = wgetch(help_subwin)) != ESC)
	{
		switch(ch)
		{
			case KEY_UP:
				menu_driver(help, REQ_UP_ITEM);
				break;

			case KEY_DOWN:
				menu_driver(help, REQ_DOWN_ITEM);
				break;

			case KEY_LEFT:
				ungetch('o');
				ungetch(27);
				break;

			case KEY_RIGHT:
				ungetch('f');
				ungetch(27);
				break;

			case 10:
				touchwin(Main);
				wrefresh(Main);
				choice = item_index(current_item(help));
				switch(choice)
				{
					case 0 :
						Help();
						break;

					case 1:
						 ungetch(27);
						 about();
						 break;
				}
				break;
		}
	}
	ungetch(27);


	unpost_menu(help);
	for(i = 0; i < 3; i++)
		free_item(help_items[i]);

	free_menu(help);
	delwin(help_shadow);
	delwin(helpwin);
	delwin(help_subwin);

	return 0;

}

void about()
{
	WINDOW *about, *about_shadow;
	curs_set(0);

	about = newwin(13, 40, 6, 20);

	about_shadow = newwin(13, 40, 7, 21);
	wbkgd(about_shadow, A_DIM | COLOR_BLACK);
	wrefresh(about_shadow);

	wbkgd(about, COLOR_PAIR(8));
	wborder(about, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26, ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);

	mvwprintw(about, 0, 17, "About");
	mvwprintw(about, 3, 17, "YATE");
	wattron(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4,  8, "Y");
	wattroff(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4,  9, "et ");
	wattron(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 12, "A");
	wattroff(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 13, "nother ");
	wattron(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 20, "T");
	wattroff(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 21, "ext ");
	wattron(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 25, "E");
	wattroff(about, COLOR_PAIR(8) | A_BOLD);
	mvwprintw(about, 4, 26, "ditor");
	mvwprintw(about, 5, 14, "Version 1.0");
	mvwprintw(about, 7, 14, "Developed By:");
	mvwprintw(about, 8, 9,  "Ishwarachandra Gowtham");
	mvwprintw(about, 9, 16, "1DS00CS025");
	mvwprintw(about,10, 10, "V Semester CSE, DSCE");

	wrefresh(about);

	noecho();
	cbreak();

	keypad(about, TRUE);
	wgetch(about);
	curs_set(1);

	delwin(about);
	delwin(about_shadow);
}

void Help()
{
	WINDOW *help, *hshad;
	PANEL  *hp;
	curs_set(0);
	help = newwin(20, 77, 2, 1);
	hp = new_panel(help);
	hshad = newwin(20, 77, 3, 2);
	wbkgd(hshad, COLOR_BLACK | A_DIM);
	wrefresh(hshad);

	wbkgd(help, COLOR_PAIR(8));
	box(help, ACS_VLINE, ACS_HLINE);

	wattron(help, A_BOLD);
	mvwprintw(help, 1, 34 , "YATE Help");
	wmove(help, 3, 3);
	waddstr(help, " Menu Operations ");
	wattroff(help, A_BOLD);
	wmove(help, 4, 3);
	wattron(help, COLOR_PAIR(10) | A_BOLD);
	waddstr(help, " KEY 		Operation ");
	wattroff(help, COLOR_PAIR(10) | A_BOLD);
	wmove(help, 5, 3);
	waddstr(help, " ALT + F		File Menu ");
	wmove(help, 6, 3);
	waddstr(help, " ALT + E		Edit Menu ");
	wmove(help, 7, 3);
	waddstr(help, " ALT + O	 	Options Menu ");
	wmove(help, 8, 3);
	waddstr(help, " ALT + H	 	Help Menu ");
	wmove(help, 9, 3);
	waddstr(help, " ALT + X		Exit ");
	wattron(help, A_BOLD);
	wmove(help, 3, 40);
	waddstr(help, " Short-Cut Keys ");
	wattroff(help, A_BOLD);
	wmove(help, 4, 40);
	wattron(help, COLOR_PAIR(10) | A_BOLD);
	waddstr(help, " KEY		Operation ");
	wattroff(help, COLOR_PAIR(10) | A_BOLD);
	wmove(help, 5, 40);
	waddstr(help, " CTRL + D	Delete File ");
	wmove(help, 6, 40);
	waddstr(help, " CTRL + K	Close File ");
	wmove(help, 7, 40);
	waddstr(help, " CTRL + L	Insert Line ");
	wmove(help, 8, 40);
	waddstr(help, " CTRL + N	New File ");
	wmove(help, 9, 40);
	waddstr(help, " CTRL + O	OS Shell ");
	wmove(help, 10, 40);
	waddstr(help, " CTRL + Y	Delete Line ");
	wattron(help, A_BOLD);
	wmove(help, 12, 3);
	waddstr(help, " Function Keys ");
	wattroff(help, A_BOLD);
	wmove(help, 13, 3);
	wattron(help, COLOR_PAIR(10) | A_BOLD);
	waddstr(help, " KEY		Operation ");
	wattroff(help, COLOR_PAIR(10) | A_BOLD);
	wmove(help, 14, 3);
	waddstr(help, " F1		Help ");
	wmove(help, 15, 3);
	waddstr(help, " F2		Save File ");
	wmove(help, 16, 3);
	waddstr(help, " F3		Open File");
	wmove(help, 17, 3);
	waddstr(help, " F10		About");


	show_panel(hp);

	noecho();
	cbreak();

	keypad(help, TRUE);
	wgetch(help);
	curs_set(1);

	del_panel(hp);
	delwin(help);
	delwin(hshad);
}

char *input(char *title, char *mesg, char *Input)
{
	WINDOW *inp, *ishad;
	PANEL  *ip;
	chtype ch;

	inp = newwin(10, 40, 7, 20);
	ip = new_panel(inp);

	ishad = newwin(10, 40, 8, 21);
	wbkgd(ishad, COLOR_BLACK);
	wrefresh(ishad);
	
	wbkgd(inp, COLOR_PAIR(8));
	box(inp, ACS_VLINE, ACS_HLINE);
	show_panel(ip);

	echo();
	cbreak();
	keypad(inp, TRUE);
	mvwprintw(inp, 0, 2, "%s", title);
	mvwprintw(inp, 4, 7, "%s", mesg);
	curs_set(1);
	mvwscanw(inp,  5, 9, "%s", Input);
	
	show_panel(ip);
	doupdate();
	update_panels();

	del_panel(ip);
	delwin(ishad);
	delwin(inp);

	return Input;
}

void confirm(char *msg)
{
	WINDOW *con;
	PANEL  *cp;
	chtype c;
	con = newwin(3, 46, 11, 17);
	cp  = new_panel(con);
	wbkgd(con, COLOR_PAIR(8));
	box(con, ACS_VLINE, ACS_HLINE);
	mvwprintw(con, 1, 3, " %s ", msg);
	echo();
	cbreak();
	c = wgetch(con);
	show_panel(cp);
	doupdate();
	update_panels();

	if(c == 'Y' || c == 'y')
	{
		touchwin(Main);
		wrefresh(Main);
		Fname = input(" Save File "," Enter the filename ",filename);
		Save(filename);
	}

	if(c == 'N' || c == 'n')
	{
		touchwin(Main);
		wrefresh(Main);
	}

	if(c == ESC)
	{
		touchwin(Main);
		wrefresh(Main);
		return;
	}

	del_panel(cp);
	delwin(con);
}


void shell()
{
	endwin();
	system("clear");
	system("echo Type EXIT to return to YATE");
	system("bash");
	system("clear");
	refresh();
	noecho();
	cbreak();
}

void Quit()
{
	if(changed == TRUE)
		confirm("Save changes to the file? (Y/N) ");

	Shutdown();
	Cleanup();
	exit(0);
	system("echo  Thankyou for using YATE. ");
}

#endif

