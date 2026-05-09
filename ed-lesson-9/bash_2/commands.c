#include "commands.h"

/*
 * Counts how many times the characters in delim occur in str.
 * Returns -1 on error.
 */
int count_occurences(char *str, char *delims) {
    if (str == NULL || delims == NULL) {
        return -1;
    }
    int count = 0;
    for (int i = 0; str[i]; i++) {
        // if str[i] a delimiter
        if (strchr(delims, str[i])) {
            count++;
        }
    }
    return count;
}

/*
 * Breaks a string into an array of sentences,
 * separated by whatever characters are in delims
 * For instance, "All over the leagues of waste..." is returned as:
 * ["All", "over", "the", "leagues", "of", "waste...", NULL]
 * Edits the string in place, but mallocs its own memory for the array of char*
 * it returns. The array is null terminated. Returns null on error.
 */
char **deconstruct_sentence(char *str, char *delims) {
    int n = count_occurences(str, delims);
    if (n < 0) {
        return NULL;
    }

    // n+2 as each delimiter has a string before (n),
    // the last delimiter has a string after (+1),
    // and we need room for the null terminator (+1)
    char **array = malloc(sizeof(char *) * (n + 2));
    if (!array) {
        perror("malloc failed");
        return NULL;
    }
    memset(array, 0, sizeof(char *) * (n + 2));

    int i = 0;
    array[i] = strtok(str, delims);
    while (array[i]) {
        i++;
        array[i] = strtok(NULL, delims);
        // the above two lines could be merged as:
        // array[++i] = strtok(NULL, delims);
    }

    return array;
}

/*
 * Breaks str into a 2D array of strings.
 * Str is first broken into sentences, which are split apart by '|'.
 * Each sentence is then broken into words, separated by whitespace.
 * Each sentence array is null terminated.
 * For instance, the string "echo Hello World | cat -E" is returned as:
 * [
        ["echo", "Hello", "World", NULL],
        ["cat", "-E", NULL],
        NULL
        ]
 * This edits the string in place, but mallocs its own memory for the array it
 returns.
*/
char ***deconstruct_command(char *str) {
    int n = count_occurences(str, "|");
    if (n < 0) {
        return NULL;
    }

    // n+2 as each delimiter has a string before (n),
    // the last delimiter has a string after (+1),
    // and we need room for the null terminator (+1)
    char ***array = malloc(sizeof(char **) * (n + 2));
    if (!array) {
        perror("malloc failed");
        return NULL;
    }
    memset(array, 0, sizeof(char **) * (n + 2));

    // We first break it into a series of strings, removing the | delimiters.
    char **temp_array = deconstruct_sentence(str, "|");
    if (!temp_array) {
        perror("failed to split by |");
        return NULL;
    }

    // We then break each of these strings into their individual words.
    for (int i = 0; temp_array[i]; i++) {
        array[i] = deconstruct_sentence(temp_array[i], " ");
    }

    free(temp_array);
    return array;
}

// Should print out all the words on new lines.
void test_deconstruct_sentence() {
    // We use strdup because we need to be able to edit the string,
    // which we can't do for constant ones.
    char *str = strdup("Well I wonder if I'll get all these words");
    char **decon = deconstruct_sentence(str, " ");
    int i = 0;
    while (decon[i]) {
        printf("%s\n", decon[i]);
        i++;
    }
    free(str);
    free(decon);
}

// Frees all the memory associated with a deconstructed command.
// Does not free the root string.
void free_decon_command(char ***decon_cmd) {
    int i = 0;
    while (decon_cmd[i]) {
        free(decon_cmd[i]);
        i++;
    }
    free(decon_cmd);
}

void test_deconstruct_command() {
    // We use strdup because we need to be able to edit the string,
    // which we can't do for constant ones.
    char *str =
        strdup("Well I wonder if I'll get all these words |and then these two");
    char ***decon = deconstruct_command(str);
    int i = 0;
    while (decon[i]) {
        int j = 0;
        while (decon[i][j]) {
            printf("%s, ", decon[i][j]);
            j++;
        }
        printf("\nNew Command\n");
        i++;
    }
    free(str);
    free_decon_command(decon);
}
