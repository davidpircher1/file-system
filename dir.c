#define _CRT_SECURE_NO_WARNINGS
#include "dir.h"
#include "stdlib.h"
#include "string.h"
#include "idpool.h"
#include "file.h"
#include <stdio.h>

Directory* dir_create(const char* name) {
	Directory* dir = malloc(sizeof(Directory));
	IDPool* pool = malloc(sizeof(IDPool));
	idpool_init(pool);

	if (dir == NULL) {return NULL;}

	strcpy(dir->name, name);
	dir->id = 255;
	dir->dir_count = 0;
	dir->file_count = 0;
	dir->permission = READ | WRITE | EXEC;
	dir->par_dir = NULL;
	dir->child_ids = pool;

	for (int i = 0; i < MAX_FILES; i++) {
		dir->sub_dirs[i] = NULL;
		dir->file[i] = NULL;
	}

	return dir;
}

int dir_add_subdir(Directory* parent, Directory* child) {
	if (parent == NULL || child == NULL) return -1; // error parent or child doesnt exist

	// find space for child parent subdir array
	for (int i = 0; i < MAX_FILES; i++) {
		if (!parent->sub_dirs[i]) {
			parent->sub_dirs[i] = child;
			parent->dir_count++;
			child->par_dir = parent;
			child->id = idpool_alloc(parent->child_ids);
			return 1; // everything correct
		}
	}
	free(child);
	return -2; // error maximum subdirs created, free the new subdir
}

Directory* dir_find_subdir(Directory* dir, const char* name) {
	if (dir == NULL) { return NULL; }

	for (int i = 0; i < MAX_FILES; i++) {
		if (dir->sub_dirs[i] == NULL)
			continue;

		if (strcmp(dir->sub_dirs[i]->name, name) == 0)
			return dir->sub_dirs[i];
	}

	return NULL;
}

void print_permission(uint8_t perm) {
	printf("%c", (perm & READ) ? 'r' : '-');
	printf("%c", (perm & WRITE) ? 'w' : '-');
	printf("%c", (perm & EXEC) ? 'x' : '-');
}

void dir_list(const Directory* dir) {
	// print subdirs
	for (int i = 0; i < MAX_FILES; i++) {
		if (dir->sub_dirs[i] != NULL) {
			print_permission(dir->sub_dirs[i]->permission);
			printf(" %s\n", dir->sub_dirs[i]->name);
		}
		
	}

	// print files
	for (int i = 0; i < MAX_FILES; i++) {
		if (dir->file[i] != NULL) {
			print_permission(dir->file[i]->permission);
			printf(" %s\n",dir->file[i]->name);
		}
	}
}

int dir_delete(Directory* dir) {
	if (dir->par_dir == NULL) // cannot delete root
		return -1;

	// if loop wont start, we are on the list of the tree, otherwise, we are still traveling
	for (int i = 0; i < dir->dir_count; i++) {
		if (dir->sub_dirs[i] != NULL) {
			dir_delete(dir->sub_dirs[i]);
			dir->sub_dirs[i] = NULL;
		}
	}

	// free epstein files
	for (int i = 0; i < dir->file_count; i++) {
		if (dir->file[i] != NULL) {
			free(dir->file[i]);
			dir->file[i] = NULL;
		}
	}
	dir->file_count = 0;
	
	// free current list dir
	for (int i = 0; i < MAX_FILES; i++) {
		if (!dir->par_dir->sub_dirs[i]) continue;

		if (strcmp(dir->par_dir->sub_dirs[i]->name, dir->name) == 0) {
			dir->par_dir->sub_dirs[i] = NULL;
			break;
		}
	}
	dir->par_dir->dir_count--;

	free(dir->child_ids); // memory leak fix
	free(dir);

	return 1; // deleted correctly
}


Directory* dir_change(Directory* current, const char* name) {
	if (strcmp(name, "..") == 0) {
		if (current->par_dir != NULL)
			return current->par_dir;
		else
			return NULL;
	}
	
	for (int i = 0; i < MAX_FILES; i++) {
		if (!current->sub_dirs[i]) continue;

		if (strcmp(current->sub_dirs[i]->name, name) == 0) {
			return current->sub_dirs[i];
		}
	}
	return NULL;
}

void dir_print_path(const Directory* dir) {
	const char* stack[64];
	int count = 0;

	while (dir != NULL) {
		stack[count++] = dir->name;
		dir = dir->par_dir;
	}

	for (int i = count - 1; i >= 0; i--) {
		if(i == count - 1) 
			printf("%s", stack[i]);
		else 
			printf("/%s", stack[i]);
	}
	printf("\n");
}


int dir_has_permission(const Directory* dir, uint8_t perm) {
	if (!dir)
		return -2;

	if (dir->permission & perm) // permission correct
		return 1;

	return -1; // permission not set
}

int dir_set_permission(Directory* dir, uint8_t perm) {
	if (!dir) return -1;
	if (perm >= 0 && perm <= 7) { // perm can be only to from 0 to 7, 0 nothing and 7 all
		dir->permission = perm;
		return 1;
	}
	return -2;
}