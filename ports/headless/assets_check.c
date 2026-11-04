/**
 *    Name:        assets_check.c
 *    Description: the first run's check: the assets the C takes out of a ROM set, compared file by file with the
 *                 ones tools/extract_assets.py wrote
 *    Version:     0.1.0
 *    Created:     04-11-2026
 *    Modified:    04-11-2026
 *    Author:      Sebastian Toyos
 *
 *    license:BSD-3-Clause
 *    Usage: tmnt_assets_check ROM_SET PYTHON_ASSETS [OUT]
 *
 *    Runs the first run's extraction (runtime/src/extract.c) of the ROM set ROM_SET (its .zip or a folder) into OUT
 *    (build/assets_c by default), then compares every file under PYTHON_ASSETS (the folder extract_assets.py wrote,
 *    with --out) with the file of the same name under OUT, byte for byte, and says which differ and which are on
 *    one side only (the version file is the first run's own). 0 when every file is the same. A development tool:
 *    the game's executables do not carry it.
 */
#include "game/extract.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#define PATH_SIZE   1200
#define SHOWN_MOST  20

typedef struct
{
	char **paths;
	size_t count, capacity;
} file_list;

static void add_path(file_list *list, const char *path)
{
	if (list->count == list->capacity)
	{
		list->capacity = list->capacity * 2 + 64;
		list->paths = realloc(list->paths, list->capacity * sizeof(*list->paths));
		if (!list->paths)
		{
			fprintf(stderr, "no memory\n");
			exit(2);
		}
	}
	list->paths[list->count] = malloc(strlen(path) + 1);
	strcpy(list->paths[list->count++], path);
}

/* every file under 'root'/'relative', by its path from 'root' (folders apart by '/') */
static void list_files(const char *root, const char *relative, file_list *list)
{
	char path[PATH_SIZE], inner[PATH_SIZE];

	snprintf(path, sizeof(path), "%s%s%s", root, relative[0] ? "/" : "", relative);
#ifdef _WIN32
	{
		WIN32_FIND_DATAA found;
		HANDLE search;
		char pattern[PATH_SIZE + 4];

		snprintf(pattern, sizeof(pattern), "%s/*", path);
		if ((search = FindFirstFileA(pattern, &found)) == INVALID_HANDLE_VALUE)
			return;
		do
		{
			if (!strcmp(found.cFileName, ".") || !strcmp(found.cFileName, ".."))
				continue;
			snprintf(inner, sizeof(inner), "%s%s%s", relative, relative[0] ? "/" : "", found.cFileName);
			if (found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				list_files(root, inner, list);
			else
				add_path(list, inner);
		} while (FindNextFileA(search, &found));
		FindClose(search);
	}
#else
	{
		DIR *folder = opendir(path);
		struct dirent *entry;

		if (!folder)
			return;
		while ((entry = readdir(folder)) != NULL)
		{
			char full[PATH_SIZE * 2];
			struct stat status;

			if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
				continue;
			snprintf(inner, sizeof(inner), "%s%s%s", relative, relative[0] ? "/" : "", entry->d_name);
			snprintf(full, sizeof(full), "%s/%s", root, inner);
			if (stat(full, &status) == 0 && S_ISDIR(status.st_mode))
				list_files(root, inner, list);
			else
				add_path(list, inner);
		}
		closedir(folder);
	}
#endif
}

static int by_path(const void *one, const void *other)
{
	return strcmp(*(char *const *)one, *(char *const *)other);
}

static unsigned char *read_whole(const char *path, size_t *size)
{
	FILE *file = fopen(path, "rb");
	unsigned char *data = NULL;
	long length;

	if (!file)
		return NULL;
	if (fseek(file, 0, SEEK_END) == 0 && (length = ftell(file)) >= 0 && fseek(file, 0, SEEK_SET) == 0
		&& (data = malloc((size_t)length + 1)) != NULL && fread(data, 1, (size_t)length, file) != (size_t)length)
	{
		free(data);
		data = NULL;
	}
	if (data)
		*size = (size_t)length;
	fclose(file);
	return data;
}

static int contains(const file_list *list, const char *path)
{
	return bsearch(&path, list->paths, list->count, sizeof(*list->paths), by_path) != NULL;
}

int main(int argc, char **argv)
{
	const char *out = argc > 3 ? argv[3] : "build/assets_c";
	file_list python = { 0 }, ours = { 0 };
	size_t same = 0, differ = 0, missing = 0, extra = 0;
	const char *problem;
	clock_t started;

	if (argc < 3 || argc > 4)
	{
		fprintf(stderr, "Usage: tmnt_assets_check ROM_SET PYTHON_ASSETS [OUT]\n");
		return 2;
	}
	started = clock();
	if ((problem = game_assets_extract(argv[1], out)) != NULL)
	{
		fprintf(stderr, "The first run cannot take the assets out of %s: %s\n", argv[1], problem);
		return 1;
	}
	printf("the first run took the assets out of %s into %s in %.1f s\n", argv[1], out,
		(double)(clock() - started) / CLOCKS_PER_SEC);
	list_files(argv[2], "", &python);
	list_files(out, "", &ours);
	qsort(python.paths, python.count, sizeof(*python.paths), by_path);
	qsort(ours.paths, ours.count, sizeof(*ours.paths), by_path);
	for (size_t index = 0; index < python.count; index++)
	{
		char one[PATH_SIZE * 2], other[PATH_SIZE * 2];
		unsigned char *expected, *got;
		size_t expected_size = 0, got_size = 0;

		snprintf(one, sizeof(one), "%s/%s", argv[2], python.paths[index]);
		snprintf(other, sizeof(other), "%s/%s", out, python.paths[index]);
		if (!contains(&ours, python.paths[index]))
		{
			if (missing++ < SHOWN_MOST)
				printf("  not written: %s\n", python.paths[index]);
			continue;
		}
		expected = read_whole(one, &expected_size);
		got = read_whole(other, &got_size);
		if (expected && got && expected_size == got_size && !memcmp(expected, got, got_size))
			same++;
		else
		{
			size_t at = 0;

			while (expected && got && at < expected_size && at < got_size && expected[at] == got[at])
				at++;
			if (differ++ < SHOWN_MOST)
				printf("  differs: %s (%zu bytes, %zu written; the first difference at %zu)\n", python.paths[index],
					expected_size, got_size, at);
		}
		free(expected);
		free(got);
	}
	for (size_t index = 0; index < ours.count; index++)
		if (strcmp(ours.paths[index], GAME_ASSETS_VERSION_FILE) != 0 && !contains(&python, ours.paths[index]))
			if (extra++ < SHOWN_MOST)
				printf("  only the first run's: %s\n", ours.paths[index]);
	printf("%zu of extract_assets.py's %zu files the same, byte for byte; %zu differ, %zu not written, %zu only the "
		"first run's (and its %s)\n", same, python.count, differ, missing, extra, GAME_ASSETS_VERSION_FILE);
	return same == python.count && !extra ? 0 : 1;
}
