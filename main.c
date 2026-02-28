#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dir.h"
#include "file.h"

int main() {
    Directory* root = dir_create("root");
    if (!root) {
        printf("Failed to create root directory\n");
        return 1;
    }

    Directory* current = root;
    char input[64];

    printf("Simple FileSystem Menu (help for commands)\n");

    while (1) {
        printf("\n> "); // prompt
        if (scanf("%s", input) != 1) break;

        if (strcmp(input, "exit") == 0) {
            break;
        }
        else if (strcmp(input, "help") == 0) {
            printf("Commands:\n");
            printf("mkdir <name>\n");
            printf("touch <name>\n");
            printf("ls\n");
            printf("rmdir <name>\n");
            printf("cd <name>\n");
            printf("pwd\n");
            printf("chmod <name> <0-7>\n");
            printf("nano <name>\n");
            printf("exit\n");
        }
        else if (strcmp(input, "mkdir") == 0) {
            char name[MAX_NAME];
            scanf("%s", name);
            Directory* newdir = dir_create(name);
            if (!newdir) {
                printf("Failed to create directory\n");
                continue;
            }
            if (!dir_add_subdir(current, newdir)) {
                printf("Failed to add directory\n");
                continue;
            }
            printf("Directory '%s' created\n", name);
        }
        else if (strcmp(input, "touch") == 0) {
            char name[MAX_NAME];
            scanf("%s", name);
            File* newfile = file_create(name);
            if (!newfile) {
                printf("Failed to create file\n");
                continue;
            }
            if (!file_add_to_dir(current, newfile)) {
                printf("Failed to add file\n");
                continue;
            }
            printf("File '%s' created\n", name);
        }
        else if (strcmp(input, "ls") == 0) {
            if (!dir_has_permission(current, READ)) {
                printf("Permission denied\n");
                continue;
            }
            dir_list(current);
        }
        else if (strcmp(input, "rmdir") == 0) {
            char name[MAX_NAME];
            scanf("%s", name);
            Directory* to_delete = dir_find_subdir(current, name);
            if (!to_delete) {
                printf("Directory not found\n");
                continue;
            }
            if (!dir_has_permission(current, WRITE)) {
                printf("Permission denied\n");
                continue;
            }
            if (dir_delete(to_delete)) {
                printf("Directory '%s' deleted\n", name);
            }
            else {
                printf("Failed to delete directory\n");
            }
        }
        else if (strcmp(input, "cd") == 0) {
            char name[MAX_NAME];
            scanf("%s", name);
            Directory* next = dir_change(current, name);
            if (!next) {
                printf("Directory not found\n");
                continue;
            }
            if (!dir_has_permission(next, EXEC)) {
                printf("Permission denied\n");
                continue;
            }
            current = next;
        }
        else if (strcmp(input, "pwd") == 0) {
            dir_print_path(current);
        }
        else if (strcmp(input, "chmod") == 0) {
            char name[MAX_NAME];
            int perm;
            scanf("%s %d", name, &perm);
            Directory* target = dir_find_subdir(current, name);
            if (!target) {
                printf("Directory not found\n");
                continue;
            }
            if (perm < 0 || perm > 7) {
                printf("Invalid permission (0-7)\n");
                continue;
            }
            if (dir_set_permission(target, perm)) {
                printf("Permissions of '%s' set to %d\n", name, perm);
            }
            else {
                printf("Failed to set permissions\n");
            }
        }
        else if (strcmp(input, "nano") == 0) {
            char name[MAX_NAME];
            char content[MAX_SIZE];
            scanf("%s", name);
            getchar(); // remove \n
            printf("Write content\n");
            fgets(content, MAX_SIZE, stdin);
            File* file = file_find(current, name);

            if (file_write(file, content)) {
                printf("Content updated\n");
            }
            else {
                printf("Failed to update content\n");
            } 
        }
        else if (strcmp(input, "cat") == 0) {
            char name[MAX_NAME];
            char content[MAX_SIZE];
            scanf("%s", name);
            File* file = file_find(current, name);
            int result = file_read(file);

            if (result == -1) {
                printf("Failed to read file\n");
            }
            else if (result == -2) {
                printf("No permission to read\n");
            }
        }
        else {
            printf("Unknown command: %s\n", input);
        }
    }

    printf("Exiting filesystem...\n");
    dir_delete(root); // uvolnenie vsetkeho
    return 0;
}
