#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "mine.h"
#include "draw.h"


// Function to clone personal repositories
int cloneSuckless(char *program) {
	char *path = concat(3, getenv("HOME"), "/.local/rice/", program);
	char *base_url = "https://gitlab.com/FriedTeaCP/tea-";

	// https://gitlab.com/FriedTeaCP/tea-${PROGRAM}.git
	char *url = concat(3, base_url, program, ".git");
	if (cloneRepo(url, path, 0) <= 0) {
		printf("Cloned %s\n", program);
	}
	return 0;
}


// Function to download, extract, and patch, other programs
// That aren't in personal repositories
void downloadMisc(char *url, char **patches, int patches_amount) {
	char *dir = concat(2, getenv("HOME"), "/.local/rice/"); // $HOME/.local/rice/

	char *basename;
	lastElement(&basename, url, "/");
	char *tar_path = concat(2, dir, basename);

	mkdirR(dir);
	curlFile(url, concat(2, dir, basename));

	// tar xf /dir/herbe.tar.gz -C $dir
	puts("extract");
	char *extract = concat(4, "tar xf ", tar_path, " -C ", dir);
	system(extract);

	// Download and patch a patch
	if (patches_amount != 0) {
		// Finds the directory where the files are going to be extracted to
		char *tarDir = catchCommand(concat(4, "tar tf ", dir, basename, " | head -n1"));
		char *current_patch;
		printf("Downloading patches\n");
		for (int i = 0; i < patches_amount; i++) {
			lastElement(&current_patch, patches[i], "/");
			curlFile(patches[i], concat(3, dir, tarDir, current_patch));
			printf("Patching %s\n", current_patch);
			system(concat(7, "patch -d ", dir, tarDir, " < ", dir, tarDir, current_patch));
		}
		free(current_patch);
		free(tarDir);
		printf("Finished patching\n");
	}

	if (remove(tar_path)) {
		printf("Removed %s\n", tar_path);
	}

	free(basename);
}


void dwm() {cloneSuckless("dwm");}
void st() {cloneSuckless("st");}
void dmenu() {cloneSuckless("dmenu");}
void dwmblocks() {cloneSuckless("dwmblocks");}


void herbe() {
	char *url = "https://github.com/dudik/herbe/archive/1.0.0.tar.gz";
	char *patches[] = {
		"https://patch-diff.githubusercontent.com/raw/dudik/herbe/pull/11.diff",
		"https://patch-diff.githubusercontent.com/raw/dudik/herbe/pull/19.diff"
	};

	downloadMisc(url, patches, sizeof patches/sizeof *patches);
}


void slock() {
	char *url = "https://dl.suckless.org/tools/slock-1.4.tar.gz";
	char *patches[] = {
		"https://tools.suckless.org/slock/patches/message/slock-message-20191002-b46028b.diff",
		"https://tools.suckless.org/slock/patches/capscolor/slock-capscolor-20170106-2d2a21a.diff",
		"https://tools.suckless.org/slock/patches/xresources/slock-xresources-20191126-53e56c7.diff"
	};
	downloadMisc(url, patches, sizeof patches/sizeof *patches);
}


void devour() {
	char *url = "https://github.com/salman-abedin/devour/archive/refs/tags/12.tar.gz";
	downloadMisc(url, NULL, 0);
}


Choice suckless[7] = {
	{"Dynamic Window Manager (dwm)", dwm, 0},
	{"Simple Terminal (st)", st, 0},
	{"Dmenu", dmenu, 0},
	{"DWM Blocks", dwmblocks, 0},
	{"Herbe", herbe, 0},
	{"Slock", slock, 0},
	{"Devour", devour, 0}
};


// Draw suckless choices, same as drawing main choices
int drawSuckless(WINDOW *win) {
	int min_width, min_height;
	int choice_amount = sizeof suckless / sizeof *suckless;
	int min_w = getmaxx(win);

	int counter = 0;
	int highlighted = 0;
	int input_ret;
	while (1) {
		drawAll(win, suckless, choice_amount, &highlighted, &counter, 1);

		input_ret = handleInput(win, suckless, choice_amount, &highlighted);

		if (input_ret == 0) //quit
		  return 1;

		if (input_ret == 1) //continue/back
		  return 0;
	}
}


// Function to install selected/toggled suckless utilites
void installSuckless() {
	int repo_len = sizeof suckless / sizeof *suckless;
	for (int i = 0; i < repo_len; i++) {
		if (suckless[i].toggled) {
			suckless[i].exec();
		}
	}
}
