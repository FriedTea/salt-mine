#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mine.h"

#define MSG  "↑/k: Move Up  ↓/j: Move Down"
#define MSG2 "a: Select All  Space/Enter: Select"

#define miner_len 22
const char *miner[] = {
"      ▄▄▄▄            ",
"         ██▄          ",
"        /  █          ",
"       /   █          ",
"      /               ",
"     /     ▄███▄      ",
"┌───┐     ▄█████▄     ",
"│ ••│     ████████    ",
"└───┘   ▄██████████   ",
" ███  ▄▄████████████▄▄"
};



const char *miner2[] = {
"                      ",
"                      ",
"          ▄           ",
"           █          ",
"     _______█⁺.       ",
"┌───┐   ·.  █.█▄      ",
"│ --│  ⁺.· █⁺███▄     ",
"└───┘   ⁺·▀.██████    ",
" ███    ▄██████████   ",
"      ▄▄████████████▄▄"
};


const char *title[][9] = {
{
"   ▄████████    ▄████████  ▄█           ███            ▄▄▄▄███▄▄▄▄    ▄█  ███▄▄▄▄      ▄████████ ",
"  ███    ███   ███    ███ ███       ▀█████████▄      ▄██▀▀▀███▀▀▀██▄ ███  ███▀▀▀██▄   ███    ███ ",
"  ███    █▀    ███    ███ ███          ▀███▀▀██      ███   ███   ███ ███▌ ███   ███   ███    █▀  ",
"  ███          ███    ███ ███           ███   ▀      ███   ███   ███ ███▌ ███   ███  ▄███▄▄▄     ",
"▀███████████ ▀███████████ ███           ███          ███   ███   ███ ███▌ ███   ███ ▀▀███▀▀▀     ",
"         ███   ███    ███ ███           ███          ███   ███   ███ ███  ███   ███   ███    █▄  ",
"   ▄█    ███   ███    ███ ███▌    ▄     ███          ███   ███   ███ ███  ███   ███   ███    ███ ",
" ▄████████▀    ███    █▀  █████▄▄██    ▄████▀         ▀█   ███   █▀  █▀    ▀█   █▀    ██████████ ",
"                          ▀                                                                      ",
},
{
"                   ▄▄                            ▄▄                    ",
" ▄█▀▀▀█▄█        ▀███   ██      ▀████▄     ▄███▀ ██                    ",
"▄██    ▀█          ██   ██        ████    ████                         ",
"▀███▄    ▄█▀██▄    ██ ██████      █ ██   ▄█ ██ ▀███ ▀████████▄   ▄▄█▀█▄",
"  ▀█████▄█   ██    ██   ██        █  ██  █▀ ██   ██   ██    ██  ▄█▀   █",
"▄     ▀██▄█████    ██   ██        █  ██▄█▀  ██   ██   ██    ██  ██▀▀▀▀▀",
"██     ███   ██    ██   ██        █  ▀██▀   ██   ██   ██    ██  ██▄    ",
"█▀█████▀▀████▀██▄▄████▄ ▀████   ▄███▄ ▀▀  ▄████▄████▄████  ████▄ ▀█████",
},
{
" ▄█▀▀▀▄█          ▀██    ▄      ▀██    ██▀  ██                  ",
" ██▄▄  ▀   ▄▄▄▄    ██  ▄██▄      ███  ███  ▄▄▄  ▄▄ ▄▄▄     ▄▄▄▄ ",
"  ▀▀███▄  ▀▀ ▄██   ██   ██       █▀█▄▄▀██   ██   ██  ██  ▄█▄▄▄██",
"▄     ▀██ ▄█▀ ██   ██   ██       █ ▀█▀ ██   ██   ██  ██  ██     ",
"█▀▄▄▄▄█▀  ▀█▄▄▀█▀ ▄██▄  ▀█▄▀    ▄█▄ █ ▄██▄ ▄██▄ ▄██▄ ██▄  ▀█▄▄▄▀"
},
};
int title_len[4] = {96, 71, 64};
int title_h[4] = {9, 8, 5};


void drawMiner(int start, int *counter) {
	int center_x;
	center_x = (getmaxx(stdscr) - miner_len)/2;
	int miner_h;

	if (*counter == 0) {
		miner_h = sizeof miner/sizeof *miner;
		for (int i = 0; i < miner_h; i++) {
			mvprintw(start+i, center_x, miner[i]);
		}
	}
	else {
		miner_h = sizeof miner2/sizeof *miner2;
		for (int i = 0; i < miner_h; i++) {
			mvprintw(start+i, center_x, miner2[i]);
		}
	}
}


void drawMsg(int min_h) {
	int scr_h, scr_w;
	getmaxyx(stdscr, scr_h, scr_w);


	int center1;
	int center2;
	center1 = (scr_w - strlen(MSG)) / 2;
	center2 = (scr_w - strlen(MSG2)) / 2;
	if (center1 % 2 > 0) {
		center1 = center1 + 1;
	}

	if (center2 % 2 > 0) {
		center2 = center2 + 1;
	}

	if (scr_h > min_h+2 && scr_w > strlen(MSG) && scr_w > strlen(MSG2)) {
		mvprintw(scr_h - min_h - 2, center1, MSG);
		mvprintw(scr_h - min_h - 1, center2, MSG2);
	}
}


int drawTitle(int win_h) {
	int chosen = 0;
	int title_amount;
	int scr_h, scr_w;
	int center_x, center_y;
	int miner_h = sizeof miner/sizeof *miner;
	int ret_value;
	getmaxyx(stdscr, scr_h, scr_w);

	title_amount = sizeof title/sizeof *title;

	if (scr_h-win_h-1-title_h[0] > miner_h) {
		chosen = 0;
		center_y = 1;
		ret_value = (scr_h-win_h+title_h[0]-miner_h)/2;
	}
	else {
		while (
			(title_len[chosen] > scr_w
			||
			title_h[chosen] > scr_h-win_h-1)
			&&
			chosen <= title_amount) {
		  chosen++;
		}

		center_y = (scr_h - win_h - title_h[chosen])/2;
		ret_value = 0;
	}

	center_x = (scr_w - title_len[chosen])/2;
	if (chosen < title_amount) {
		for (int i = 0; i < title_h[chosen]; i++) {
			mvprintw(i+center_y-1, center_x, title[chosen][i]);
		}
	}

	return ret_value;
	refresh();
}


int drawChoices(WINDOW *win, Choice *choice, int choice_amount, int *highlighted, int is_submenu) {
	for (int i = 0; i <= choice_amount; i++) {
		if (i == *highlighted) wattron(win, A_REVERSE);

		if (i == choice_amount) {
			if (is_submenu) {
				mvwprintw(win, i*2+1, (getmaxx(win)-6)/2, " BACK ");
			}
			else {
				mvwprintw(win, i*2+1, (getmaxx(win)-10)/2, " CONTINUE ");
			}
		}

		else if (choice[i].is_submenu) {
			mvwprintw(win, i*2+1, 1, " * %s  ", choice[i].name);
		}

		else if (choice[i].toggled) {
			mvwprintw(win, i*2+1, 1, "[X] %s  ", choice[i].name);
		}
		else {
			mvwprintw(win, i*2+1, 1, "[ ] %s  ", choice[i].name);
		}


		wattroff(win, A_REVERSE);
	}
	refresh();
	wrefresh(win);
	return 0;
}


int handleInput(WINDOW *win, Choice *choice, int choice_amount, int *highlighted) {
	int selected_all = 1;
	int input;
	int submenu_ret;
	input = wgetch(win);
	switch (input) {
    case KEY_UP:
		if (*highlighted != 0) {
			*highlighted = *highlighted - 1;
		}
		break;

    case KEY_DOWN:
		if (*highlighted != choice_amount) {
			*highlighted = *highlighted + 1;
		}
		break;

    case 'k':
		if (*highlighted != 0) {
			*highlighted = *highlighted - 1;
		}
		break;

    case 'j':
		if (*highlighted != choice_amount) {
			*highlighted = *highlighted + 1;
		}
		break;

    case 'a': //select all
		for (int i = 0; i < choice_amount; i++)
		if (!choice[i].toggled) {
			selected_all = 0;
		}

		for (int i = 0; i < choice_amount; i++) {
			choice[i].toggled = !selected_all;
		}

		break;

	case 'g': // Go to the very bottom
		*highlighted = 0;
		break;

	case 'G': // Go to the very bottom
		*highlighted = choice_amount;
		break;

    case 'q': // quit
		return 0;
}

	if (input == 10 || input == ' ') { // Ebter or space
		if (*highlighted == -1) {
			for (int i = 0; i < choice_amount; i++) {
				if (!choice[i].is_submenu) {
					choice[i].toggled = 1;
				}
			}
		}

		if (*highlighted == choice_amount) {
			return 1;
		}

		if (choice[*highlighted].is_submenu) {
			submenu_ret = choice[*highlighted].draw_submenu(win);
			if (submenu_ret == 1) {
				return 0;
			}
		}
		else if (choice[*highlighted].toggled) {
			choice[*highlighted].toggled = 0;
		}
		else {
			choice[*highlighted].toggled = 1;
		}
	}
	return 2;
}


void drawSalt(int amount) {
	int scr_h, scr_w;
	int x, y;

	getmaxyx(stdscr, scr_h, scr_w);

	time_t t;
	srand((unsigned) time(&t));

	for (int i = 0; i < amount; i++) {
		x = rand() % scr_w;
		y = rand() % scr_w;

		mvprintw(y, x, "·");
	}
}


int drawAll(WINDOW *win, Choice *choice, int choice_amount, int *highlighted, int *counter, int is_submenu) {
	int win_h, win_w;
	int win_y, win_x;
	int scr_h, scr_w;
	int title_ret = 0;

	getmaxyx(stdscr, scr_h, scr_w);
	win_h = choice_amount*2+2;
	win_w = maxChoiceLen(choice, choice_amount)+10;
	win_y = (scr_h - win_h);
	win_x = (scr_w - win_w) / 2;

	if (win_h >= scr_h || win_w >= scr_w) {
		return -1;
	}

	if (getmaxy(win) != win_h || getmaxx(win) != win_w
	  ||
	  getbegy(win) != win_y || getbegx(win) != win_x)
	{
		clear();
		wclear(win);
		drawSalt(scr_h*scr_w/6);
		drawMsg(win_h);
		wresize(win, win_h, win_w);
		mvwin(win, win_y, win_x);
		box(win, 0, 0);
	}

	title_ret = drawTitle(win_h);
	if (title_ret > 0) {
		if (*counter > 0) {
			*counter = 0;
		}
		else {
			*counter = 1;
		}

		halfdelay(10);
		drawMiner(title_ret, counter);
	}

	drawChoices(
		win,
		choice,
		choice_amount,
		highlighted,
		is_submenu);

  return 0;
}
