#define _CRT_SECURE_NO_WARNINGS
#include "file.h"
#include "dir.h"
#include <stdlib.h>
#include <string.h>




File* file_create(const char* name) {
	File* file = malloc(sizeof(File));
	if (!file) return NULL;
	file->id = 255;
	file->par_dir = NULL;
	file->permission = READ | WRITE | EXEC;
	strcpy(file->name, name);
	file->content[0] = '\0';

	return file;
}

int file_add_to_dir(Directory* dir, File* file) {
	if (dir == NULL || file == NULL) return -1; // error parent or child doesnt exist

	// find space for file
	for (int i = 0; i < MAX_FILES; i++) {
		if (!dir->file[i]) {
			dir->file[i] = file;
			dir->file_count++;
			file->par_dir = dir;
			file->id = idpool_alloc(dir->child_ids);
			return 1; // everything correct
		}
	}
	free(file);
	return -2; // error
}

int file_read(const File* file) {
	if (!file) return -1;
	if (!file_has_permission(file, READ)) return -2; // no perm to read

	printf("%s\n", file->content);
	return 1;
}

File* file_find(const struct Directory* dir, const char* name) {
	for (int i = 0; i < MAX_FILES; i++) {
		if (strcmp(dir->file[i]->name, name) == 0) {
			return dir->file[i];
		}
	}
	return NULL;
}

int file_write(File* file, const char* content) {
	if (!file) return -1;

	if (strlen(content) >= MAX_SIZE) return -2; // overflow

	if (!file_has_permission(file, WRITE)) return -3; // no perm to write

	strcpy(file->content, content);

	return 1;
}

int file_delete(File* file) {
	for (int i = 0; i < MAX_FILES; i++) {
		if (file->par_dir->file[i]->id == file->id) {
			file->par_dir->file[i] = NULL;
			free(file);
			return 1;
		}
	}
	return -1;
}

int file_set_permission(File* file, uint8_t perm) {
	if (perm < 0 || perm > 7) return -1;
	if (!file) return -1;

	file->permission = perm;
	return 1;
}

int file_has_permission(const File* file, uint8_t perm) {
	if (perm < 0 || perm > 7) return -1;
	if (!file) return -1;

	if (file->permission & perm) {
		return 1;
	}
	else {
		return 0;
	}
}