#pragma once
#include <stdint.h>
#define MAX_SIZE 128
#define MAX_NAME 50

struct Directory;

typedef struct File {
    uint8_t id;
    uint8_t permission;    
    char name[MAX_NAME];
    char content[MAX_SIZE];
    struct Directory* par_dir;
} File;



File* file_create(const char* name); // touch 1/2

int file_add_to_dir(struct Directory* dir, File* file); // touch 2/2

File* file_find(const struct Directory* dir, const char* name); // find maxdepth 1

int file_read(const File* file); // cat

int file_write(File* file, const char* content); // echo > file

int file_delete(File* file); // rm 

int file_set_permission(File* file, uint8_t perm); // chmod

int file_has_permission(const File* file, uint8_t perm); // check perm

