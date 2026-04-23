#include "file_system.h"
#include "tree.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree_node *create_file(char *name, bool dir) {
    struct file *file = calloc(1, sizeof(struct file));
    if (!file) {
        errno = 12;
    };
    strcpy(file->name, name);
    file->directory = dir;

    struct tree_node *node = create_node(file);

    return node;
}

struct stack *create_stack(void *first_value) {
    struct stack *new_stack = malloc(sizeof(struct stack));
    if (!new_stack) {
        errno = 12;
        return NULL;
    }

    // I should realloc this if I feel like the user might have a more than 10
    // dir nesting, but this is just a student chill ver
    void **stack_data = malloc(sizeof(void *) * 10);
    if (!stack_data) {
        errno = 12;
        return NULL;
    }

    stack_data[0] = first_value;

    new_stack->capacity = 10;
    new_stack->data = stack_data;
    new_stack->top_index = 0;

    return new_stack;
}

void destroy_stack(struct stack *stack) {
    free(stack->data);
    free(stack);
}

void get_directory(struct stack *dir_stack, char *str) {
    strcpy(str, "> ");
    for (int i = 0; i <= dir_stack->top_index; i++) {
        strcat(str,
               (((struct file *)((struct tree_node *)dir_stack->data[i])->value)
                    ->name));
        strcat(str, "/");
    }
}

struct tree_node *search_child(char *name, bool directory,
                               struct tree_node *node_to_search) {
    for (int i = 0; i < node_to_search->children.used; i++) {
        struct tree_node *file_node =
            (struct tree_node *)node_to_search->children.values[i];

        struct file *file = (struct file *)file_node->value;

        if (strcmp(file->name, name) == 0 && file->directory == directory) {
            return file_node;
        }
    }
    return NULL;
}

struct tree_node *search_descendant_helper(char *name, bool directory,
                                           struct tree_node *node_to_start,
                                           struct stack *stack) {
    struct tree_node *tree_node = NULL;
    struct file *file = node_to_start->value;

    if (strcmp(file->name, name) == 0 && file->directory == directory) {
        tree_node = node_to_start;
        return tree_node;
    }
    for (int i = 0; i < node_to_start->children.used; i++) {
        tree_node = search_descendant_helper(
            name, directory, node_to_start->children.values[i], stack);
        if (tree_node) {
            if (strcmp(((struct file *)node_to_start->value)->name, "~") != 0) {
                stack->top_index++;
                stack->data[stack->top_index] = node_to_start;
            }
            break;
        }
    }
    return tree_node;
}

void search_descendant(char *name, bool directory,
                       struct tree_node *node_to_start) {
    struct stack *visited = create_stack(node_to_start);
    struct tree_node *tree_node =
        search_descendant_helper(name, directory, node_to_start, visited);
    if (tree_node) {
        char path[400] = {0};
        get_directory(visited, path);
        printf("%s is at %s\n", name, path);
        destroy_stack(visited);
    } else {
        printf("%s not found\n", name);
        destroy_stack(visited);
    }
}

void destroy_file(struct file *file) { free(file); }
destroy_value_t dfn = (destroy_value_t)destroy_file;

int main() {
    char command[20] = {0};
    char argument[100] = {0};
    char prompt[120] = {0};

    char current_path[400] = {0};

    struct tree_node *root_node = create_file("~", true);
    struct stack *dir_stack = create_stack(root_node);

    while (1) {
        get_directory(dir_stack, current_path);
        struct tree_node *current_path_node =
            (struct tree_node *)dir_stack->data[dir_stack->top_index];
        printf("%s\n", current_path);

        fgets(prompt, 120, stdin);
        if (sscanf(prompt, "%s %s", command, argument) < 1) {
            printf("sorry, I couldn't get that\n");
        };

        if (strcmp(command, "ls") == 0) {
            for (int i = 0; i < current_path_node->children.used; i++) {
                struct tree_node *file_node =
                    (current_path_node->children.values[i]);
                struct file *file = (struct file *)file_node->value;
                printf("%s\n", file->name);
            }
        }

        if (strcmp(command, "cd") == 0) {
            if (strcmp(argument, "..") == 0) {
                if (dir_stack->top_index == 0) {
                    printf("Already at root\n");
                    continue;
                }
                dir_stack->top_index--;
                continue;
            }

            struct tree_node *node =
                search_child(argument, true, current_path_node);
            if (node) {
                dir_stack->top_index++;
                dir_stack->data[dir_stack->top_index] = node;
                continue;
            }
            printf("Directory not found\n");
        }

        if (strcmp(command, "touch") == 0) {
            if (strlen(argument) < 1) {
                printf("invalid file name\n");
                continue;
            }

            struct tree_node *node =
                search_child(argument, false, current_path_node);
            if (node) {
                printf("File already exist\n");
                continue;
            }
            struct tree_node *new_file_node = create_file(argument, false);
            add_child(current_path_node, new_file_node);
        }

        if (strcmp(command, "mkdir") == 0) {
            if (strlen(argument) < 1) {
                printf("invalid dir name\n");
                continue;
            }

            struct tree_node *node =
                search_child(argument, true, current_path_node);
            if (node) {
                printf("Dir already exist\n");
                continue;
            }

            struct tree_node *new_file_node = create_file(argument, true);
            add_child(current_path_node, new_file_node);
        }

        if (strcmp(command, "rm") == 0) {
            if (strlen(argument) < 1) {
                printf("invalid name to remove\n");
                continue;
            }

            struct tree_node *node =
                search_child(argument, false, current_path_node);
            if (node) {
                remove_child(current_path_node, node, dfn);
                printf("%s Deleted\n", argument);
            } else {
                struct tree_node *node =
                    search_child(argument, true, current_path_node);
                if (node) {
                    remove_child(current_path_node, node, dfn);
                    printf("%s Deleted\n", argument);
                } else {
                    printf("%s Not found\n", argument);
                }
            }
        }

        if (strcmp(command, "ffind") == 0) {
            if (strlen(argument) < 1) {
                printf("invalid name to remove\n");
                continue;
            }
            search_descendant(argument, false, current_path_node);
        }

        if (strcmp(command, "dfind") == 0) {
            if (strlen(argument) < 1) {
                printf("invalid name to remove\n");
                continue;
            }
            search_descendant(argument, true, current_path_node);
        }

        if (strcmp(command, "kill") == 0) {
            printf("Bye!\n");
            break;
        }
    }

    destroy_tree(root_node, dfn);
    destroy_stack(dir_stack);
}
