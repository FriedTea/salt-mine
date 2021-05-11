#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include "draw.h"
#include "suckless.h"
#include "fonts.h"
#include "util.h"


void dotfiles() {
	int error;
	char *url = "https://gitlab.com/FriedTeaCP/dotfiles.git";
	char *path = concat(2, getenv("HOME"), "/.local/dots/");
	printf("Cloning dotfiles\n");
	error = cloneRepo(url, path, 1);
	if (error > 0)
		printf("Failed to clone or checkout repository\n");

	// Replace with checkout function
	// git --git-dir=$HOME/.local/dots --work-tree=$HOME
    char *checkout_cmd = "\
git --git-dir=$HOME/.local/dots --work-tree=$HOME checkout ||\
echo 'Backing up existing dotfiles' && mkdir --verbose $HOME/dot-backup &&\
git --git-dir=$HOME/.local/dots --work-tree=$HOME checkout 2>&1 | grep -E \"\\s+\\.\" | awk {'print $1'} | xargs -I{} mv --verbose {} $HOME/dot-backup/;\
git --git-dir=$HOME/.local/dots --work-tree=$HOME config --local status.showUntrackedFile no &&\
echo 'Succesfully installed dotfiles' || echo 'Failed to install dotfiles'";
	system(checkout_cmd);

}

void Wallpapers() {
	char *base_path = concat(2, getenv("HOME"), "/.local/rice/Wallpapers/");
	if (mkdirR(base_path))
		printf("Made directory %s\n", base_path);

	char *url = "https://images2.alphacoders.com/950/950452.png";

	curlFile(url, concat(2, base_path, "Rock Thing.png"));
};


int maxChoiceLen(Choice choice[], int choice_amount) {
	int max = 0;
	for (int i = 0; i < choice_amount; i++)
		if (strlen(choice[i].name) > max)
		  max = strlen(choice[i].name);

	return max;
}


int main() {
	int choice_amount;

	Choice main_choice[4] = {
	  {"Dotfiles", dotfiles, 0},
	  {"Wallpapers", Wallpapers, 0},
	  {"Suckless Utilites", installSuckless, 1, drawSuckless},
	  {"Fonts", downloadFonts, 1, drawFonts}
	};
	choice_amount = sizeof main_choice / sizeof *main_choice;

	setlocale(LC_ALL, "");
	initscr();
	noecho();
	curs_set(0);

	WINDOW *win = newwin(0, 0, 0, 0);

	int highlighted = 0;
	int counter = 0;
	int input_ret;
	int success = 0;
	while (1) {
	drawAll(win, main_choice, choice_amount, &highlighted, &counter, 0);

	input_ret = handleInput(win, main_choice, choice_amount, &highlighted);

	if (input_ret == 0) //quit
	  break;

	if (input_ret == 1) { //continue/back
	  success = 1;
	  break;
	}

	}

	endwin();
	if (success == 1) {
		for (int i = 0; i < choice_amount; i++)
			if (main_choice[i].toggled || main_choice[i].is_submenu)
				main_choice[i].exec();
	}
	return 0;
}
