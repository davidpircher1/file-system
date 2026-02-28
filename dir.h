#pragma once
#include <stdint.h>

#define READ  4   // 100
#define WRITE 2   // 010
#define EXEC  1   // 001
#define MAX_FILES 100
#define MAX_NAME 50

struct File;
struct IDPool;

typedef struct Directory
{
	uint8_t id;
	char name[MAX_NAME];
	struct Directory* sub_dirs[MAX_FILES];
	struct Directory* par_dir;
	struct File* file[MAX_FILES];
	int dir_count;
	int file_count;
	uint8_t permission;
	struct IDPool* child_ids;
}Directory;


Directory* dir_create(const char* name); // <=> mkdir 1/2

int dir_add_subdir(Directory* parent, Directory* child); // <=> mkdir 2/2

Directory* dir_find_subdir(Directory* dir, const char* name);

void dir_list(const Directory* dir); // <=> ls

int dir_delete(Directory* dir); // <=> rmdir

Directory* dir_change(Directory* current, const char* name); // <=> cd

int dir_has_permission(const Directory* dir, uint8_t perm); // check for perm

void dir_print_path(const Directory* dir); // <=> pwd

int dir_set_permission(Directory* dir, uint8_t perm); // <=> chmod


