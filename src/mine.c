#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <unistd.h>
#include <ncurses.h>
#include <dirent.h>
#include <sys/stat.h>
#include "draw.h"
#include "suckless.h"
#include "fonts.h"
#include "util.h"


int checkOS()
{
    #ifdef __linux__
    return 1;

    #elif __APPLE__ || __MACH__
    return 1;

    #elif __FreeBSD__
    return 1;

    #elif __unix || __unix__
    return 1;

    #else
    return 0;

    #endif
}


// Found in stack overflow
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}


void gitdirRename(char *gitdir, char *targetdir)
{
	if (!checkDir(targetdir))
	{
		mkdirR(targetdir);
	}

	char *targetfile;
	char *gitfile;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (gitdir)) != NULL)
	{
	  while ((ent = readdir (dir)) != NULL)
	  {
		if (
			strcmp(ent->d_name, "..") == 0
			||
			strcmp(ent->d_name, ".") == 0
			)
		{
			continue;
		}

		targetfile = (char *)malloc(strlen(ent->d_name)+strlen(targetdir));
		gitfile = (char *)malloc(strlen(ent->d_name)+strlen(gitdir));

		sprintf(targetfile, "%s/%s", targetdir, ent->d_name);
		sprintf(gitfile, "%s/%s", gitdir, ent->d_name);

		if (strcmp(ent->d_name, ".git") == 0)
		{
			nftw(gitfile, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
			printf("Removed %s\n", gitfile);
		} else
		{
			rename(gitfile, targetfile);
			printf("%s -> %s\n", gitfile, targetfile);
		}
	  }

	  closedir (dir);
	} else
	{
	  fprintf(stderr, "Could not access directory\n");
	}

	free(targetfile);
	free(gitfile);
}


void dotfiles()
{
	char *url = "https://gitlab.com/FriedTeaCP/dotfiles.git";
	char *home = getenv("HOME");
	char *path = concat(2, home, "/.local/dots/");
	char *tmp_path = concat(2, home, "/tmp-gitdir");

	printf("Cloning temporary dotfiles repository...\n");
	if (cloneRepo(url, tmp_path, 0) <= 0)
	{
		printf("Cloned temporary dotfiles repository!\n");
		printf("Moving files...\n");
		gitdirRename(tmp_path, home);
		printf("Moved files!\n");

		printf("Removing temporary dotfiles repository...\n");
		if (remove(tmp_path) == 0)
		{
			printf("Removed temporary dotfiles repository!\n");
		} else
		{
			printf("Failed to remove temporary dotfiles repository!\n");
		}
	} else
	{
		printf("Failed to clone temporary dotfiles repository!\n");
	}


	printf("Cloning bare dotfiles repository...\n");
	if (cloneRepo(url, path, 1) <= 0)
	{
		printf("Cloned bare dotfiles repository!\n");
	} else
	{
		printf("Failed to clone bare dotfiles repository!\n");
	}
}


void Wallpapers()
{
	char *base_path = concat(2, getenv("HOME"), "/.local/rice/Wallpapers/");
	if (mkdirR(base_path))
		printf("Made directory %s\n", base_path);

	char *url = "https://images2.alphacoders.com/950/950452.png";

	curlFile(url, concat(2, base_path, "Rock Thing.png"));
};


int maxChoiceLen(Choice choice[], int choice_amount)
{
	int max = 0;
	for (int i = 0; i < choice_amount; i++)
		if (strlen(choice[i].name) > max)
		  max = strlen(choice[i].name);

	return max;
}


int main()
{
	if (!checkOS())
	{
		printf("Operating system is not supported\n");
		return 1;
	}

	int choice_amount;


	Choice main_choice[4] =
	{
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

	int win_h, win_y;
	int highlighted = 0;
	int counter = 0;
	int input_ret;
	int success = 0;
	while (1)
	{
		drawAll(win, main_choice, choice_amount, &highlighted, &counter, 0);

		input_ret = handleInput(win, main_choice, choice_amount, &highlighted);

		if (input_ret == 0) //quit
		  break;

		if (input_ret == 1)
		{ //continue/back
		  success = 1;
		  break;
		}
	}

	endwin();
	if (success == 1)
	{
		for (int i = 0; i < choice_amount; i++)
			if (main_choice[i].toggled || main_choice[i].is_submenu)
				main_choice[i].exec();
	}
	return 0;
}
