#include <stdio.h>
#include <string.h>

// Define the grid size
#define ROWS 10
#define COLS 10

// Function prototypes
void search_horizontal(const char grid[ROWS][COLS], const char *str);
void search_vertical(const char grid[ROWS][COLS], const char *str);
void search_diagonal(const char grid[ROWS][COLS], const char *str);
int string_compare(const char *w1, const char *w2, unsigned int len);

int main() {
    // Example grid (outer rows and columns are null terminators)
    char grid[ROWS][COLS] = {
        {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'},
        {'\0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', '\0'},
        {'\0', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', '\0'},
        {'\0', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', '\0'},
        {'\0', 'y', 'z', 'a', 'b', 'c', 'd', 'e', 'f', '\0'},
        {'\0', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', '\0'},
        {'\0', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', '\0'},
        {'\0', 'w', 'x', 'y', 'z', 'a', 'b', 'c', 'd', '\0'},
        {'\0', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', '\0'},
        {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'}};

    search_horizontal(grid, "abc");
    search_vertical(grid, "tbj");
    search_diagonal(grid, "ajs");
}

void search_horizontal(const char grid[ROWS][COLS], const char *str) {
    int str_len = strlen(str);
    for (int i = 0; i < ROWS; i++) {
        for (int k = 0; k < COLS; k++) {
            if (!string_compare(&grid[i][k], str, str_len)) {
                printf("Found horizontal match for %s: row %d, column %d\n",
                       str, i, k);
            }
        }
    }
}

void search_vertical(const char grid[ROWS][COLS], const char *str) {
    int str_len = strlen(str);
    for (int i = 0; i < ROWS; i++) {
        for (int k = 0; k < COLS; k++) {
            char temporal_str[100] = {'\0'};
            for (int j = 0; j < str_len; j++) {
                if (grid[i + j][k] == 0) {continue;}
                temporal_str[j] = grid[i + j][k];
            }
            if (!string_compare(temporal_str, str, str_len)) {
                printf("Found vertical match for %s: row %d, column %d\n",
                       str, i, k);
            }
        }
    }
}

void search_diagonal(const char grid[ROWS][COLS], const char *str) {
    int str_len = strlen(str);
    for (int i = 0; i < ROWS; i++) {
        for (int k = 0; k < COLS; k++) {
            char temporal_str[100] = {'\0'};
            for (int j = 0; j < str_len; j++) {
                if (grid[i + j][k + j] == 0) {continue;}
                temporal_str[j] = grid[i + j][k+j];
            }
            if (!string_compare(temporal_str, str, str_len)) {
                printf("Found diagonal match for %s: row %d, column %d\n",
                       str, i, k);
            }
        }
    }
}

int string_compare(const char *w1, const char *w2, unsigned int len) {
    for (int i = 0; i < len; i++) {
        if (w1[i] != w2[i]) {
            return 1;
        }
    }
    return 0;
}
