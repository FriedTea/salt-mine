#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "mine.h"
#include "draw.h"
#include "util.h"

void cozette();
void jetBrains();
void nerdFont();

Choice fonts[3] = {
  {"Cozzete", cozette, 0},
  {"JetBrains Mono", jetBrains, 0},
  {"Nerd Fonts", nerdFont, 0}
};


void getFont(char *url, char *dir, char *file) {
	char *font_dir = concat(2, getenv("HOME"), "/.local/share/fonts/");
	char *filename;
	int ret;

	mkdirR(font_dir);


	if (file != NULL) {
		filename = strdup(file);
	}
	else {
		lastElement(&filename, url, "/");
	}


	if (dir != NULL) {
		ret = curlFile(url, concat(3, font_dir, dir, filename));
	}
	else {
		ret = curlFile(url, concat(2, font_dir, filename));
	}


	free(filename);
}

void cozette() {
	char *url = "https://github.com/slavfox/Cozette/releases/download/v.1.9.3/CozetteVector.ttf";
	getFont(url, NULL, NULL);
}


void jetBrains() {
	char *fonts[] = {
		"JetBrainsMono-Bold.otf",
		"JetBrainsMono-BoldItalic.otf",
		"JetBrainsMono-ExtraBold.otf",
		"JetBrainsMono-ExtraBoldItalic.otf",
		"JetBrainsMono-ExtraLight.otf",
		"JetBrainsMono-ExtraLightItalic.otf",
		"JetBrainsMono-Italic.otf",
		"JetBrainsMono-LightItalic.otf",
		"JetBrainsMono-Medium.otf",
		"JetBrainsMono-MediumItalic.otf",
		"JetBrainsMono-Regular.otf",
		"JetBrainsMono-Thin.otf",
		"JetBrainsMono-ThinItalic.otf"
	};
	char *base_url = "https://github.com/JetBrains/JetBrainsMono/raw/master/fonts/otf/";


	for (int i = 0; i < sizeof fonts/sizeof *fonts; i++) {
		getFont(concat(2, base_url, fonts[i]), "JetBrainsMono", NULL);
	}
}


void nerdFont() {
	char *license = "https://github.com/ryanoasis/nerd-fonts/raw/master/LICENSE";
	char *url = "https://github.com/ryanoasis/nerd-fonts/raw/master/src/glyphs/Symbols-1000-em%20Nerd\%20Font\%20Complete.ttf";
	getFont(url, NULL, "Symbols-1000-em Nerd Font Complete.ttf");
}


int drawFonts(WINDOW *win) {
	int choice_amount = sizeof fonts / sizeof *fonts;
	int min_w = getmaxx(win);

	int counter = 0;
	int highlighted = 0;
	int input_ret;
	while (1) {
		drawAll(win, fonts, choice_amount, &highlighted, &counter, 1);

		input_ret = handleInput(win, fonts, choice_amount, &highlighted);

		if (input_ret == 0) //quit
			return 1;

		if (input_ret == 1) //continue/back
			return 0;
	}
}


void downloadFonts() {
	int choice_amount = sizeof fonts / sizeof *fonts;
	for (int i = 0; i < choice_amount; i++)
		if (fonts[i].toggled)
			fonts[i].exec();
}
