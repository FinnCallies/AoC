#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define TESTFILE "test.data"

int count_lines_in_file(FILE *file);
int getLinF(char *filename);
bool is_sorted(int *list, int len);
void bubble_sort(int *list, int len);
void print_list(int *list, int len);
bool is_sorted_dec(int *list, int len);
