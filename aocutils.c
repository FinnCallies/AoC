#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

int count_lines_in_file(FILE *file)
{
        int lines = 0;
        char c;

        while (!feof(file))
        {
                c = fgetc(file);
                if (c == '\n')
                {
                        lines++;
                }
        }

        return lines;
}

int getLinF(char *filename)
{
        int line_count = 0;

        FILE *f = fopen(filename, "r");
        if (!f)
        {
                printf("File \"%s\" not opened, early exit.\n", filename);
                exit(EXIT_FAILURE);
        }

        line_count = count_lines_in_file(f);
        printf("%d lines counted in file \"%s\"\n", line_count, filename);

        if (fclose(f) != 0)
                printf("WARNING: fclose() failed for file \"%s\"\n", filename);

        return line_count;
}

bool is_sorted(int *list, int len)
{
        for (int i = 0; i < len - 1; i++)
        {
                if (list[i] > list[i + 1])
                {
                        return false;
                }
        }
        return true;
}

void bubble_sort(int *list, int len)
{
        int sorted = 0;
        for (int i = 0; i < len; i++)
        {
                int idx = 0;
                for (int j = 0; j < len - i - 1; j++)
                {
                        if (list[j] > list[j + 1])
                        {
                                int temp = list[j + 1];
                                list[j + 1] = list[j];
                                list[j] = temp;
                        }
                }
        }

        if (!is_sorted(list, len))
                printf("WARNING: bubble sort broken\n");
}

void print_list(int *list, int len)
{
        printf("[");
        for (int i = 0; i < len; i++)
        {
                printf("%d", list[i]);
                if ((i + 1) < len)
                        printf(", ");
        }
        printf("]");
}
