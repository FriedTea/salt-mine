#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <glob.h>
#include <git2.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include "util.h"
#include "mine.h"


void lastElement(char **ptr, char *str, char *delimiter)
{
	char *str_copy = strdup(str);
	char *token;
	while ((token = strtok_r(str_copy, delimiter, &str_copy)))
	{
		if (token)
		{
			*ptr = strdup(token);
		}
	}
}


int mkdirR(char *dir)
{
	struct stat s;
	stat(dir, &s);
	if (S_ISDIR(s.st_mode))
	{
		return 0;
	}

	char *dir_copy = strdup(dir);
	char *partial_dir = strtok(dir_copy, "/");
	char *complete_dir = malloc(strlen(dir)*sizeof(char)+1);
	strcpy(complete_dir, "/");
	while (partial_dir != NULL)
	{
		strcat(complete_dir, partial_dir);
		strcat(complete_dir, "/");
		mkdir(complete_dir, 0777);
		partial_dir = strtok(NULL, "/");
	}

	printf("Made directory %s\n", complete_dir);

	free(complete_dir);
	free(dir_copy);
	return 1;
}


int curlFile(char *url, char *filename)
{
	CURL *curl;
	FILE *file;
	CURLcode res;

	curl = curl_easy_init();
	if (curl)
	{
		file = fopen(filename, "w");
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(file);
	}

	if (res <= 0)
	{
		printf("Downloaded %s\n", filename);
	}

	return res;
}



char *curlData(char *url)
{
	CURL *curl;
	FILE *stream;
	char *str;
	size_t size;
	CURLcode res;

	curl = curl_easy_init();
	if (curl)
	{
		stream = open_memstream(&str, &size);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "request");

		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		fclose(stream);
	}
	return str;
}


int cloneRepo(char *url, char *path, int bare)
{
	int error;
	git_libgit2_init();

	git_repository *repo = NULL;
	git_clone_options opts = GIT_CLONE_OPTIONS_INIT;

	if (bare) // if (bare == positive int)
	{
		opts.bare = 1;
	}

	error = git_clone(&repo, url, path, &opts);
	return error;
}


int checkDir(char *dir)
{
	struct stat s;
	stat(dir, &s);
	if (S_ISDIR(s.st_mode))
		return 1;

	return 0;
}


char *concat(int strings, ...)
{
	va_list list;
	va_start(list, strings);

	FILE *stream;
	char *str;
	size_t size;


	stream = open_memstream(&str, &size);
	for (int i = 0; i < strings; i++)
	{
		fprintf(stream, "%s", va_arg(list, char *));
	}
	fclose(stream);

	va_end(list);
	return str;
};


char *catchCommand(const char *cmd)
{
	char *str = malloc(1);
	char c;
	FILE *stream;

	stream = popen(cmd, "r");
	str[0] = fgetc(stream);

	int counter = 0;
	while ((c = fgetc(stream)) != EOF)
	{
		str = (char*)realloc(str, strlen(str)+1);
		counter++;
		str[counter] = c;
	}
	str[strlen(str)-1] = '\0';

	printf("%s", str);
	pclose(stream);
	return str;
};


int checkDependency(char *program)
{
	char *path = strdup(getenv("PATH"));
	char *dir;

	dir = strtok(path, ":");
	char *glob_str;
	while (dir != NULL)
	{
		glob_str = (char *) malloc(strlen(dir) + strlen(program) + 3);
		sprintf(glob_str, "%s/%s", dir, program);

		glob_t globtmp;
		if (glob(glob_str, 0, NULL, &globtmp) == 0)
		{
			globfree(&globtmp);
			free(path);
			return 1;
		}

		dir = strtok(NULL, ":");
	}
	free(path);

	return 0;
}

