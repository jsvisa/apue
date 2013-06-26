#include <string.h>
#include <ncurses.h>
#include <stdio.h>

void Init() {
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
}

int main()
{
	int ch;
	int i;
	char buf[31];

	Init();
//	printw("Type any character to see it in bold\n");

	buf[30] = 0;
	move(5, 1);
	memset(buf, '-', 30);
	printw(buf);

	getch();
	for(i=0; i<30; i++) {
		move(5, 1);
		if(i==0) {
			buf[0] = '+';
			printw(buf);
		}
		else {
			buf[i-1] = '-';
			buf[i] = '+';
			printw(buf);
		}
		refresh();
		sleep(1);
	}
	/*
	move(5, 1);
	printw("Input here:");
	move(10,1);
	printw("jump to here.");
	move(1, 1);
	printw("This is the last");
	*/

	refresh();			/* Print it on to the real screen */
	getch();			/* Wait for user input */
	endwin();			/* End curses mode		  */
	return 0;
}

