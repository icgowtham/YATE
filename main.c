#include "source.h"

int main (void)
{
	chtype ch;
	char *fname;
	int i;
	curY = Create();
	Screen = curY;
	scrfirst = curY;
	signal (SIGINT, SIG_IGN);

	while (1)
	{
		Startup();
		keypad(Main, TRUE);
		curs_set(1);
		row = 1;
		col = 0;

		while(ch = wgetch(Main))
		{
			show_panel(mbP);
			show_panel(sbP);
			show_panel(mainP);
			wmove(Main, row, col);

			switch(ch)
			{
				case 27:
					  wtimeout(Main, 1);
                                          ch = wgetch(Main);
                                          wtimeout(Main, -1);
				switch(ch)
				{
					case 'f':
					case 'F':
						curs_set(0);
						doupdate();
						update_panels();
						i = file_menu();
						break;

					case 'e':
					case 'E':
						curs_set(0);
						doupdate();
						update_panels();
						i = edit_menu();
						break;

					case 'o':
					case 'O':
						curs_set(0);
						doupdate();
						update_panels();
						i = options_menu();
						break;

					case 'h':
					case 'H':
						curs_set(0);
						doupdate();
						update_panels();
						i = help_menu();
						break;

					case 'x':
					case 'X':
						if(changed == TRUE)
				   confirm(" Save changes to the file? (Y/N) ");
						Shutdown();
						Cleanup();
						system("clear");
						exit(0);
                                      	case ERR:
						break;
				}
				curs_set(1);
				wmove(Main, row, col);
				break;

				case CTRL_D:
					Delete();
					break;

				case CTRL_O:
					shell();
					break;

				case CTRL_K:
					Close();
					break;

				case CTRL_N:
					New();
					break;

				case CTRL_L:
					curY = InsLine(curY, curX);
					row++;
					line++;
					col = 0;
					changed = TRUE;
					break;

				case CTRL_W:
					insertWord();
					break;

				case CTRL_Y:
					delLine();
					break;

				case KEY_F(1):
					Help();
					break;

				case KEY_F(10):
					about();
					break;

				case KEY_F(2):
				if((strcmp(curF, "Untitled")) == 0)
				{
			                fname = input(" Save File ", " Enter the filename ", filename);
					curF = fname;
					Save(fname);
				}
				else
					Save(fname);
					break;

				case KEY_F(3):
			      fname = input(" Open File "," Enter the filename ", filename);
					curF = fname;
					Open(fname);
					wborder(Main, ACS_VLINE+7, ACS_VLINE+7, ACS_VLINE+26, ACS_VLINE+26, ACS_VLINE+22, ACS_VLINE+8, ACS_VLINE+21, ACS_VLINE+9);
					break;

				case 10:
					col = 0;
					line++;
					row++;
					curY = InsLine(curY, curX);
					if(line > 21)
					{
						row -= 1;
						scrfirst = scrfirst->down;
					}
					changed = TRUE;
					break;

				case '\t':
					for(i = 0; i < tab_stop; i++)
					{
						curX = InsAfter(curX, ' ');
						col++;
					}
					changed = TRUE;
					break;

				case KEY_LEFT:
					curX = Left(curX);
					break;

				case KEY_RIGHT:
					curX = Right(curX);
					break;

				case KEY_PPAGE:
					curY = Pageup(curY);
					break;

				case KEY_NPAGE:
					curY = Pagedown(curY);
					break;

				case KEY_HOME:
					Home();
					break;

				case KEY_END:
					End();
					break;

				case KEY_BACKSPACE:
					curX = Backspace(curX);
					changed = TRUE;
					break;

				case SPACE:
					curX = InsAfter(curX, ' ');
					changed = TRUE;
					break;

				case KEY_UP:
					curY = Up(curY);
					break;

				case KEY_DOWN:
					curY = Down(curY);
					break;

				case KEY_DC:
					curX = Del(curX);
					changed = TRUE;
					break;

				//default:
				case ' ':
				case 33 ... 126:
					curX = InsAfter(curX, ch);
					col++;
					changed = TRUE;
					textWrap();
					break;
			}
			DispScreen(scrfirst);
			mvwprintw(Main, 22, 5, " %d : %d ", line, col);
			if(changed == TRUE)
				mvwprintw(Main, 22, 3, " * ");
			else
				mvwprintw(Main, 22, 3, "   ");
			mvwprintw(Main, 0, 34, " %s ", curF);
			wmove(Main, row, col+1);
			update_panels();
			doupdate();
		}
		Cleanup();
		Shutdown();
	}

	return 0;
}
