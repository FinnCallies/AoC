#include "../aocutils.h"

void get_N(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if (l < 3)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l - 1] + d, 1);
        memcpy(&r[2], f[l - 2] + d, 1);
        memcpy(&r[3], f[l - 3] + d, 1);
}

void get_O(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if (strlen(x) < 5)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l] + d + 1, 1);
        memcpy(&r[2], f[l] + d + 2, 1);
        memcpy(&r[3], f[l] + d + 3, 1);
}

void get_S(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if ((nl - l) < 4)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l + 1] + d, 1);
        memcpy(&r[2], f[l + 2] + d, 1);
        memcpy(&r[3], f[l + 3] + d, 1);
}

void get_W(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if (d < 3)
                return;

        r[0] = 'X';
        memcpy(&r[1], x - 1, 1);
        memcpy(&r[2], x - 2, 1);
        memcpy(&r[3], x - 3, 1);
}

void get_NO(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if (l < 3 || strlen(x) < 5)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l - 1] + d + 1, 1);
        memcpy(&r[2], f[l - 2] + d + 2, 1);
        memcpy(&r[3], f[l - 3] + d + 3, 1);
}

void get_SO(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if ((nl - l) < 4 || strlen(x) < 5)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l + 1] + d + 1, 1);
        memcpy(&r[2], f[l + 2] + d + 2, 1);
        memcpy(&r[3], f[l + 3] + d + 3, 1);
}

void get_SW(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if ((nl - l) < 4 || d < 3)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l + 1] + d - 1, 1);
        memcpy(&r[2], f[l + 2] + d - 2, 1);
        memcpy(&r[3], f[l + 3] + d - 3, 1);
}

void get_NW(char **f, int l, char *x, int nl, char *r)
{
        int d = x - f[l];

        if (l < 3 || d < 3)
                return;

        r[0] = 'X';
        memcpy(&r[1], f[l - 1] + d - 1, 1);
        memcpy(&r[2], f[l - 2] + d - 2, 1);
        memcpy(&r[3], f[l - 3] + d - 3, 1);
}

void xmas(char **f, int l, char *x, int nl, int *sum)
{
        void (*funcs[])(char **, int, char *, int, char *) = {
            &get_N,
            &get_NO,
            &get_O,
            &get_SO,
            &get_S,
            &get_SW,
            &get_W,
            &get_NW};
        char *strings[] = {
            "N", "NO", "O", "SO", "S", "SW", "W", "NW"};

        for (int i = 0; i < 8; i++)
        {
                char *str = malloc(4);

                memset(str, '.', 4);
                funcs[i](f, l, x, nl, str);

                if (strncmp(str, "XMAS", 4) == 0)
                {
                        // printf("%2s: \"%c%c%c%c\"\n", strings[i], str[0], str[1], str[2], str[3]);
                        (*sum)++;
                }

                free(str);
        }
}

void x_mas(char **f, int l, char *a, int nl, int *sum)
{
        int d = a - f[l];

        if (l < 1 || strlen(a) < 3 || (nl - l) < 2 || d < 1)
                return;

        char *nw_so = malloc(3);
        char *no_sw = malloc(3);

        nw_so[0] = f[l - 1][d - 1];
        nw_so[1] = 'A';
        nw_so[2] = f[l + 1][d + 1];
        no_sw[0] = f[l - 1][d + 1];
        no_sw[1] = 'A';
        no_sw[2] = f[l + 1][d - 1];

        if ((strncmp(nw_so, "MAS", 3) == 0 || strncmp(nw_so, "SAM", 3) == 0) && (strncmp(no_sw, "MAS", 3) == 0 || strncmp(no_sw, "SAM", 3) == 0))
        {
                // printf("true\n");
                (*sum)++;
        }

        free(nw_so);
        free(no_sw);
}

int main(int argc, char *argv[])
{
        int num_lines;
        FILE *fp;
        char *filename;
        char **file;
        size_t len = 64;
        int sum = 0;
        int xs = 0;
        int as = 0;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        num_lines = getLinF(filename);

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \'%s\' failed, early exit\n", filename);
                exit(EXIT_FAILURE);
        }

        file = (char **)calloc(num_lines, sizeof(char *));
        // for (int i = 0; i < num_lines; i++)
        //         file[i] = malloc(len);

        for (int i = 0; i < num_lines; i++)
        {
                file[i] = malloc(len);
                if (getline(&file[i], &len, fp) == -1)
                        exit(EXIT_FAILURE);
        }

        fclose(fp);

        for (int i = 0; i < num_lines; i++)
        {
                char *line = file[i];
                char *x;
                while ((x = strchr(line, 'X')) != NULL)
                {
                        xs++;
                        int diff = x - file[i];

                        // printf("X number %d\n", xs);
                        xmas(file, i, x, num_lines, &sum);
                        // printf("\n");

                        line = file[i] + diff + 1;
                }
        }

        printf("Count XMAS = %d\n\n---\n\n", sum);

        sum = 0;
        for (int i = 0; i < num_lines; i++)
        {
                char *line = file[i];
                char *a;
                while ((a = strchr(line, 'A')) != NULL)
                {
                        as++;
                        int diff = a - file[i];

                        x_mas(file, i, a, num_lines, &sum);

                        line = file[i] + diff + 1;
                }
        }

        printf("Count X-MAS = %d\n", sum);

        for (int i = 0; i < num_lines; i++)
                free(file[i]);
        free(file);
        exit(EXIT_SUCCESS);
}