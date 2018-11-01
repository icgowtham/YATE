/* Contains header, function and variables declarations. */

#ifndef __SOURCE_H
#define __SOURCE_H 

#include <ncurses.h>
#include <menu.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#define ESC 	27
#define CTRL_D   4
#define CTRL_K  11
#define CTRL_L  12
#define CTRL_N  14
#define CTRL_O  15
#define CTRL_W  23
#define CTRL_Y  25
#define SPACE	20

int ESCDELAY   = 0;
int INS	       = 1;	
int col	       = 0;
int row        = 1;
int choice     = 0;
int tab_stop   = 8;
int line       = 1;

WINDOW *Main, *menu_bar, *status_bar;
PANEL *mainP, *mbP, *sbP;
bool changed = FALSE;

FILE *file, *curFile;
char filename [256];
char *Fname;
char *curF = "Untitled";

struct nodex
{
	chtype info;
	struct nodex *next, *prev;
};
typedef struct nodex NODEX;

struct nodey
{
	NODEX *columns;
	struct nodey *up, *down;
};
typedef struct nodey NODEY;

NODEY     *curY = NULL;
NODEY *scrfirst = NULL;
NODEY   *Screen = NULL;
NODEX     *curX = NULL;

NODEY *Create ();
NODEY *InsLine (NODEY *, NODEX *);
NODEY *Up   (NODEY *);
NODEY *Down (NODEY *);
NODEY *Pageup   (NODEY *);
NODEY *Pagedown (NODEY *);
NODEX *InsBefore (NODEX *, chtype);
NODEX *InsMiddle (NODEX *, chtype);
NODEX *InsAfter  (NODEX *, chtype);
NODEX *Left  (NODEX *);
NODEX *Right (NODEX *);
NODEX *Backspace (NODEX *);
NODEX *Del (NODEX *);

int   file_menu ();
int   edit_menu ();
int   options_menu ();
int   help_menu ();
void  Startup ();
void  Show_Screen ();
void  Shutdown ();
void  Display (NODEY *);
void  DispScreen (NODEY *);
void  End  ();
void  Home ();
void  scrollup   ();
void  scrolldown ();
void  New ();
void  Open (char *);
void  Close ();
void  Save (char *);
void  SaveAs ();
void  Delete ();
void  shell ();
void  Quit ();
void  delLine ();
void  insertWord ();
void  textColour ();
void  bkColour ();
void  about ();
void  Help ();
void  msgbox (char *, char *);
void  confirm (char *);
void  Cleanup ();
char  *input (char *, char *, char *);

#endif
