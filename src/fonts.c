#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "mine.h"
#include "draw.h"
#include "util.h"

void cozette();
void jetBrains();

Choice fonts[2] = {
  {"Cozzete", cozette, 0},
  {"JetBrains Mono", jetBrains, 0}
};


void cozette() {
	char *url = "https://github.com/slavfox/Cozette/releases/download/v.1.9.3/CozetteVector.ttf";
	char *home = getenv("HOME");
	char *base_path = concat(2, getenv("HOME"), "/.local/share/fonts/");
	if (mkdirR(base_path))
		printf("Made directory %s\n", base_path);

	if (curlFile(url, concat(2, base_path, "CozetteVector.ttf")) == 0) {
		printf("Downloaded CozetteVector.ttf\n");
	}
}


void jetBrains() {
	char *font_base = "JetBrainsMono-";
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
	int len = sizeof fonts / sizeof *fonts;
	maxStrLen(max_len, fonts);

	char *base_path = concat(2, getenv("HOME"), "/.local/share/fonts/JetBrainsMono/");
	char *base_url = "https://github.com/JetBrains/JetBrainsMono/raw/master/fonts/otf/";
	mkdirR(base_path);


	char *url = malloc(strlen(base_url)+max_len);
	char *path = malloc(strlen(base_path)+max_len);
	for (int i = 0; i < len; i++) {
		strcpy(url, base_url);
		strcat(url, fonts[i]);

		strcpy(path, base_path);
		strcat(path, fonts[i]);

		curlFile(url, path);
	}
	free(url);
	free(path);
}


int drawFonts(WINDOW *win) {
	int min_width, min_height;
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
