#include "../aocutils.h"

struct pos {
        int l;
        int o;
};

struct pos get_next_trailhead(char **f, int ls, struct pos s)
{
        struct pos r = {.l = -1, .o = -1};
        int j = s.o + 1;

        for(int i = s.l; i < ls; i++) {
                for(; j < strlen(f[i]); j++) {
                        if(*(f[i] + j) == '0') {
                                r.l = i;
                                r.o = j;
                                return r;
                        }
                }

                j = 0;
        }

        return r;
}

struct pos get_N(char **f, int ls, struct pos p)
{
        struct pos r = {.l = -1, .o = -1};

        if(p.l > 0) {
                r.l = p.l - 1;
                r.o = p.o;
        }

        return r;
}

struct pos get_O(char **f, int ls, struct pos p)
{
        struct pos r = {.l = -1, .o = -1};

        if(p.o < (strlen(f[p.l]) - 1)) {
                r.l = p.l;
                r.o = p.o + 1;
        }

        return r;
}

struct pos get_S(char **f, int ls, struct pos p)
{
        struct pos r = {.l = -1, .o = -1};

        if(p.l < (ls - 1)) {
                r.l = p.l + 1;
                r.o = p.o;
        }

        return r;
}

struct pos get_W(char **f, int ls, struct pos p)
{
        struct pos r = {.l = -1, .o = -1};

        if(p.o > 0) {
                r.l = p.l;
                r.o = p.o - 1;
        }

        return r;
}

void get_next(char **f, int ls, struct pos *a, struct pos p)
{
        a[0] = get_N(f, ls, p);
        a[1] = get_O(f, ls, p);
        a[2] = get_S(f, ls, p);
        a[3] = get_W(f, ls, p);
}

bool pina(struct pos p, struct pos *a, int l)
{
        for(int i = 0; i < l; i++)
                if(a[i].l == p.l && a[i].o == p.o)
                        return true;

        return false;
}

struct pos *es;
int l_es;

int calc_next(char **f, int ls, struct pos p)
{
        struct pos *a = (struct pos *) calloc(4, sizeof(struct pos));
        int ret = 0;

        if(*(f[p.l] + p.o) == '0') {
                free(es);
                es = NULL;
                l_es = 0;
        }

        if(*(f[p.l] + p.o) == '9') {
                if(pina(p, es, l_es))
                        return 0;
                l_es++;
                es = (struct pos *) realloc(es, l_es * sizeof(struct pos));
                es[l_es - 1].l = p.l;
                es[l_es - 1].o = p.o;
                // printf(" [ %d, %d ]", p.l, p.o);
                free(a);
                return 1;
        }

        get_next(f, ls, a, p);
        for(int i = 0; i < 4; i++) {
                if(a[i].l == -1 || a[i].o == -1)
                        continue;

                if(*(f[a[i].l] + a[i].o) == (*(f[p.l] + p.o)) + 1)
                        ret += calc_next(f, ls, a[i]);
        }

        free(a);
        return ret;
}

int main(int argc, char *argv[])
{
        int num_lines;
        FILE *fp;
        char *filename;
        char **file;
        size_t len = 64;
        int sum = 0;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        num_lines = getLinF(filename);

        file = (char **)calloc(num_lines, sizeof(char *));

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \'%s\' failed, early exit\n", filename);
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < num_lines; i++)
        {
                file[i] = malloc(len);
                if (getline(&file[i], &len, fp) == -1)
                        exit(EXIT_FAILURE);
        }

        fclose(fp);

        for(int i = 0; i < num_lines; i++) {
                printf("%s", file[i]);
        }

        struct pos p = {.l = 0, .o = -1};
        p = get_next_trailhead(file, num_lines, p);
        while(p.l != -1 && p.o != -1) {
                printf("[ %d, %d]", p.l, p.o);

                int s = calc_next(file, num_lines, p);
                printf(" = %d\n", s);
                sum += s;

                p = get_next_trailhead(file, num_lines, p);
        }

        printf("sum: %d\n", sum);

        exit(EXIT_SUCCESS);
}
