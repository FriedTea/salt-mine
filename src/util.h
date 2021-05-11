#ifndef UTIL_H
#define UTIL_H

#define maxStrLen(var, arr)\
	int var = 0;\
	for (int i = 0; i < sizeof arr / sizeof *arr; i++)\
		if (strlen(arr[i]) > var)\
			var = strlen(arr[i])


#include "mine.h"
void lastElement(char **ptr, char *str, char *delimiter);
int maxChoiceLen(Choice choice[], int choice_amount);
int mkdirR(char *dir);
int checkDir(char *dir);
int curlFile(char *url, char *filename);
char *curlData(char *url);
int cloneRepo(char *url, char *path, int bare);
int extractArchive(FILE *file);
char *concat(int strings, ...);
char *catchCommand(const char *cmd);

#endif // UTIL_H
