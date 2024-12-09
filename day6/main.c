#include <unistd.h>
#include "../aocutils.h"

struct position
{
        int l;
        int o;
        char d;
};

void clearScreen()
{
        printf("\e[1;1H\e[2J");
}

int count_Xs(char **m, int c, int l)
{
        return 0;
}

char check_for_move(char **m, int c, int l, struct position p)
{
        if (p.d == 'u')
                return (p.l == 0 ? ' ' : *(m[p.l - 1] + p.o));
        else if (p.d == 'r')
                return (p.o == l - 1 ? ' ' : *(m[p.l] + p.o + 1));
        else if (p.d == 'd')
                return (p.l == c - 1 ? ' ' : *(m[p.l + 1] + p.o));
        else if (p.d == 'l')
                return (p.o == 0 ? ' ' : *(m[p.l] + p.o - 1));
        else
                return false;
}

void set_d(char *d)
{
        if (*d == 'u')
                *d = 'r';
        else if (*d == 'r')
                *d = 'd';
        else if (*d == 'd')
                *d = 'l';
        else if (*d == 'l')
                *d = 'u';
}

void draw_map(char **m, int c)
{
        // return;

        // clearScreen();

        for (int i = 0; i < c; i++)
        {
                printf("%s", m[i]);
        }
        printf("\n");

        // usleep(500000);
}

void update(char **m, struct position p, char c)
{
        *(m[p.l] + p.o) = c;
}

void move(char **m, int c, int l, struct position *p, bool *loop)
{
        char ch = 'X';
        char prev = check_for_move(m, c, l, *p);

        // update p
        if (p->d == 'u')
        {
                p->l--;
                ch = 'u';
        }
        else if (p->d == 'r')
        {
                p->o++;
                ch = 'r';
        }
        else if (p->d == 'd')
        {
                p->l++;
                ch = 'd';
        }
        else if (p->d == 'l')
        {
                p->o--;
                ch = 'l';
        }

        if (prev == ch)
                *loop = true;

        // update p.d
        while (check_for_move(m, c, l, *p) == '#' || check_for_move(m, c, l, *p) == 'O')
        {
                ch = 't';
                set_d(&p->d);
        }

        // update m
        update(m, *p, ch);

        return;
}

struct position get_pos(char **m, int c, int l)
{
        struct position p = {.l = 0, .o = 0, .d = 'u'};

        for (int i = 0; i < c; i++)
        {
                for (int j = 0; j < l; j++)
                {
                        if (*(m[i] + j) == '^')
                        {
                                p.l = i;
                                p.o = j;
                                return p;
                        }
                }
        }

        return p;
}

void reset_map(char **m, int c, struct position s)
{
        for (int i = 0; i < c; i++)
        {
                for (int j = 0; *(m[i] + j) != '\n'; j++)
                {
                        if (*(m[i] + j) == 'u' || *(m[i] + j) == 'd' || *(m[i] + j) == 'r' || *(m[i] + j) == 'l' || *(m[i] + j) == 't')
                                *(m[i] + j) = '.';
                }
        }
        *(m[s.l] + s.o) = '^';
}

int unique_fields(char **m, int c)
{
        int sum = 0;

        for (int i = 0; i < c; i++)
        {
                for (int j = 0; j < (strlen(m[0]) - 1); j++)
                {
                        if (*(m[i] + j) != '.' && *(m[i] + j) != '#' && *(m[i] + j) != 'O')
                                sum++;
                }
        }

        return sum;
}

int get_unique_fields(char **m, int c, struct position *p, int n)
{
        int ret = 0;

        for (int i = 0; i < c; i++)
        {
                for (int j = 0; j < (strlen(m[0]) - 1); j++)
                {
                        if (*(m[i] + j) != '.' && *(m[i] + j) != '#' && *(m[i] + j) != 'O')
                        {
                                p[ret].l = i;
                                p[ret].o = j;
                                ret++;
                        }
                }
        }

        return ret + 1;
}

bool check_log(struct position *l, int s, int i)
{
        for (int j = (i + 1) % s; j < i; j = (j + 1) % s)
        {
                if(l[j].l == l[i].l && l[j].o == l[i].o && l[j].d == l[i].d)
                        return true;
        }

        return false;
}

int main(int argc, char *argv[])
{
        size_t len = 192;
        FILE *fp;
        char *filename;
        char *line;
        int l_count;
        char **map;
        int ctr = 0;
        struct position p;
        struct position s;
        int sum = 0;
        bool loop;
        int loops = 0;
        int n_uf = 0;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        l_count = getLinF(filename);

        map = calloc(l_count, sizeof(char *));

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit", filename);
                exit(EXIT_FAILURE);
        }

        while (getline(&map[ctr], &len, fp) != -1 && ctr < l_count)
                ctr++;

        s = get_pos(map, l_count, strlen(map[0]) - 1);

        /* PART 1 */
        p.l = s.l;
        p.o = s.o;
        p.d = s.d;
        update(map, p, 'u');
        while (check_for_move(map, l_count, strlen(map[0]) - 1, p) != ' ' && !loop)
        {
                loop = false;
                move(map, l_count, strlen(map[0]) - 1, &p, &loop);
                // draw_map(map, l_count);
                // printf("p: line = %d; offset = %d; direction = \'%c\'\n", p.l, p.o, p.d);
        }
        // draw_map(map, l_count);
        sum = unique_fields(map, l_count);
        printf("unique fields: %d\n", sum);
        struct position *uf = (struct position *) calloc(sum, sizeof(struct position));
        n_uf = get_unique_fields(map, l_count, uf, sum);
        reset_map(map, l_count, s);

        for (int i = 0; i < n_uf; i++)
        {
                struct position *log = (struct position *) calloc(500, sizeof(struct position));
                int idx = 0;

                sum = 0;
                loop = false;
                p.l = s.l;
                p.o = s.o;
                p.d = s.d;

                /* if (*(map[uf[i].l] + uf[i].o) == '#' || *(map[i] + j) == '^')
                        continue; */

                *(map[uf[i].l] + uf[i].o) = 'O';

                // draw_map(map, l_count);

                update(map, p, 'u');
                while (check_for_move(map, l_count, strlen(map[0]) - 1, p) != ' ' && !loop)
                {
                        move(map, l_count, strlen(map[0]) - 1, &p, &loop);
                        log[idx].l = p.l;
                        log[idx].o = p.o;
                        log[idx].d = p.d;
                        if(check_log(log, 500, idx))
                                loop = true;
                        idx = (idx + 1) % 500;
                        // draw_map(map, l_count);
                }

                // draw_map(map, l_count);

                sum = unique_fields(map, l_count);

                if (loop)
                {
                        printf("%3d, %3d: Xs: %d - loop \n", uf[i].l, uf[i].o, sum);
                        loops++;
                        /* draw_map(map, l_count);
                        printf("\n"); */
                }

                *(map[uf[i].l] + uf[i].o) = '.';
                reset_map(map, l_count, s);

                free(log);
        }


        /* for (int i = 0; i < l_count; i++)
        {
                for (int j = 0; j < strlen(map[0]) - 1; j++)
                {
                        sum = 0;
                        loop = false;
                        p.l = s.l;
                        p.o = s.o;
                        p.d = s.d;

                        if (*(map[i] + j) == '#' || *(map[i] + j) == '^')
                                continue;

                        *(map[i] + j) = 'O';

                        // draw_map(map, l_count);

                        update(map, p, 'u');
                        while (check_for_move(map, l_count, strlen(map[0]) - 1, p) != ' ' && !loop)
                        {
                                loop = false;
                                move(map, l_count, strlen(map[0]) - 1, &p, &loop);
                                // draw_map(map, l_count);
                        }

                        // draw_map(map, l_count);

                        sum = unique_fields(map, l_count);

                        if (loop)
                        {
                                printf("%3d, %3d: Xs: %d - loop \n", i, j, sum);
                                loops++;
                                draw_map(map, l_count);
                                printf("\n");
                        }

                        *(map[i] + j) = '.';
                        reset_map(map, l_count, s);
                }
        } */

        printf("loops: %d\n", loops);

        free(uf);
        for (int i = 0; i < l_count; i++)
                free(map[i]);
        fclose(fp);
        exit(EXIT_SUCCESS);
}
