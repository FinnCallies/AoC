#include "../aocutils.h"

bool doing;

char *get_tok_in_line(char *line, int line_len, int *ret) 
{
        for(int i = 0; i < (line_len - 8); i++) {
                // char *a, *b;
                int x, y;

                if(!doing && strncmp(line + i, "do()", 4) == 0) {
                        doing = true;
                        i += 3;
                        continue;
                } else if(!doing) {
                        continue;
                }

                if(strncmp(line + i, "don't()", 7) == 0) {
                        doing = false;
                        i += 6;
                        continue;
                }

                if(sscanf(line + i, "mul(%d,%d)", &x, &y) != 2) {
                        continue;
                }

                // x = atoi(a);
                // y = atoi(b);

                if(x > 999 || y > 999)
                        continue;

                char *str = malloc(13);
                int len = snprintf(str, 12 + 1, "mul(%d,%d)", x, y);
                // printf("replica: %s\n", str);

                if(len > 12 || strncmp(line + i, str, len) != 0)
                        continue;

                *ret = x * y;
                
                printf("%12s => %3d * %3d = %5d\n", str, x, y, *ret);
                
                return (strchr(line + i, ')') + 1);
        }

        return NULL;
}

int main(int argc, char *argv[])
{
        FILE *fp;
        char *line;
        size_t line_len = 64;
        char *filename;
        int sum = 0;
        doing = true;

        // read line
        // for line
        //      tokenize into "mul(X, Y)"


        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;
        
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit\n", filename);
                exit(EXIT_FAILURE);
        }

        while(getline(&line, &line_len, fp) != -1) {
                int val;
                while((line = get_tok_in_line(line, strchr(line, '\n') - line, &val)) != NULL) {
                        sum += val;
                }
        }

        // exe1: 179571322
        printf("sum = %d\n", sum);

        exit(EXIT_SUCCESS);
}
