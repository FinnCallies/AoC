#include <unistd.h>
#include "../aocutils.h"

struct position {
        int l;
        int o;
        char d;
};

void clearScreen() {
        printf("\e[1;1H\e[2J");
}

int count_Xs(char **m, int c, int l)
{
        return 0;
}

char check_for_move(char **m, int c, int l, struct position p)
{
        if(p.d == 'u') 
                return (p.l == 0 ? ' ' : *(m[p.l - 1] + p.o));
        else if (p.d == 'r') 
                return (*(m[p.l] + p.o) == l - 1 ? ' ' : *(m[p.l] + p.o + 1));
        else if (p.d == 'd') 
                return (p.l == c - 1 ? ' ' : *(m[p.l + 1] + p.o));
        else if (p.d == 'l') 
                return (*(m[p.l] + p.o) == 0 ? ' ' : *(m[p.l] + p.o - 1));
        else 
                return false;
}

void set_d(char *d) 
{
        if(*d == 'u')
                *d = 'r';
        else if(*d == 'r')
                *d = 'd';
        else if(*d == 'd')
                *d = 'l';
        else if(*d == 'l')
                *d = 'u';
}

void draw_map(char **m, int c) 
{
        // return;

        clearScreen();

        for(int i = 0; i < c; i++) {
                printf("%s", m[i]);
        }
        printf("\n");

        usleep(500000);
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
        if(p->d == 'u') {
                p->l--;
                ch = '|';
        } else if (p->d == 'r') {
                p->o++;
                ch = '-';
        } else if (p->d == 'd') {
                p->l++;
                ch = '|';
        } else if (p->d == 'l') {
                p->o--;
                ch = '-';
        }

        if(prev == ch)
                *loop = true;
        

        // update p.d
        if(check_for_move(m, c, l, *p) == '#' || check_for_move(m, c, l, *p) == 'O') {
                ch = '+';
                set_d(&p->d);
        }

        // update m
        update(m, *p, ch);

        return;
}

struct position get_pos(char **m, int c, int l) 
{
        struct position p = {.l = 0, .o = 0, .d = 'u'};

        for(int i = 0; i < c; i++) {
                for(int j = 0; j < l; j++) {
                        if(*(m[i] + j) == '^') {
                                p.l = i;
                                p.o = j;
                                return p;
                        }
                }
        }

        return p;
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
        int sum = 0;
        bool loop;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;
        
        l_count = getLinF(filename) - 1;

        map = calloc(l_count, sizeof(char *));

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit", filename);
                exit(EXIT_FAILURE);
        }

        while(getline(&map[ctr], &len, fp) != -1 && ctr < l_count)
                ctr++;

        for(int i = 0; i < l_count; i++) {
                for(int j = 0; j < (strlen(map[0]) - 1); j++) {
                        *(map[i] + j) = 'O';

                        draw_map(map, l_count);

                        p = get_pos(map, l_count, strlen(map[0]) - 1);

                        update(map, p, '|');
                        while(check_for_move(map, l_count, strlen(map[0]) - 1, p) != ' ' && !loop) {
                                loop = false;
                                move(map, l_count, strlen(map[0]) - 1, &p, &loop);
                                draw_map(map, l_count);
                        }

                        for(int i = 0; i < l_count; i++) {
                                for(int j = 0; j < (strlen(map[0]) - 1); j++) {
                                        if(*(map[i] + j) == '-' || *(map[i] + j) == '|' || *(map[i] + j) == '+')
                                                sum++;
                                }
                        }

                        printf("%3d, %3d: Xs: %d\n", i, j, sum);
                }
        }

        fclose(fp);
        exit(EXIT_SUCCESS);
}
