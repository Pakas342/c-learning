#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Counts how many times the characters in delim occur in str.
*/
int count_occurences(char* str, char* delims);

/*
 * Breaks a string into an array of sentences,
 * separated by whatever characters are in delims
 * For instance, "All over the leagues of waste..." is returned as:
 * ["All", "over", "the", "leagues", "of", "waste...", NULL]
 * Edits the string in place, but mallocs its own memory for the array of char* it returns.
 * The array is null terminated.
 * Returns null on error.
*/
char** deconstruct_sentence(char* str, char* delims);

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
 * This edits the string in place, but mallocs its own memory
 * for the array it returns.
*/
char*** deconstruct_command(char* str);

// Frees all the memory associated with a deconstructed command.
// Does not free the root string.
void free_decon_command(char*** decon_cmd);

void test_deconstruct_command();

void test_deconstruct_sentence();

#endif
