#include "../aocutils.h"

long exe1(int *list_l, int *list_r, int len)
{
        long sum = 0;

        for (int i = 0; i < len; i++)
        {
                int summand = list_r[i] - list_l[i];
                if (summand < 0)
                        sum -= summand;
                else
                        sum += summand;
        }

        return sum;
}

long exe2(int *list_l, int *list_r, int len)
{
        long sum = 0;

        for (int i = 0; i < len; i++)
        {
                int count_in_r = 0;
                for (int j = 0; j < len; j++)
                {
                        if (list_r[j] == list_l[i])
                                count_in_r++;
                }
                sum += list_l[i] * count_in_r;
        }

        return sum;
}

int main(int argc, char *argv[])
{
        int *list_left;
        int *list_right;
        char *filename;
        int lines = 0;
        char *line = NULL;
        int line_c = 1;
        size_t len = 64;
        FILE *fp;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        lines = getLinF(filename);
        if (lines == 0)
                exit(EXIT_SUCCESS);

        list_left = calloc(lines, sizeof(int));
        if (!list_left)
        {
                printf("Allocation failed\n");
                exit(EXIT_FAILURE);
        }
        list_right = calloc(lines, sizeof(int));
        if (!list_right)
        {
                printf("Allocation failed\n");
                exit(EXIT_FAILURE);
        }

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit", filename);
                exit(EXIT_FAILURE);
        }

        line = malloc(len);
        while (getline(&line, &len, fp) != -1)
        {
                char *str_l = malloc(5);
                char *str_r = malloc(5);

                if (line == NULL)
                        printf("Unable to get line\n");

                if (sscanf(line, "%s   %s\n", str_l, str_r) < 2)
                {
                        printf("Incomplete parsing of string in line %d\n", line_c);
                        lines--;
                        continue;
                }

                list_left[line_c - 1] = atoi(str_l);
                list_right[line_c - 1] = atoi(str_r);

                line_c++;
                free(str_l);
                free(str_r);
        }

        if (fclose(fp) != 0)
                printf("WARNING: fclose() failed for file \"%s\"\n", filename);

        bubble_sort(list_left, lines);
        bubble_sort(list_right, lines);

        /* DAY 1 EXERCISE 1 */
        // 2285373
        printf("Sum: %ld\n", exe1(list_left, list_right, lines));

        /* DAY 1 EXERCISE 2 */
        // 21142653
        printf("Sum: %ld\n", exe2(list_left, list_right, lines));

        free(list_left);
        free(list_right);
        exit(EXIT_SUCCESS);
}
