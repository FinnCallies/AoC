#include "../aocutils.h"
#include <string.h>

#define LEVEL_DIFF_MIN 1
#define LEVEL_DIFF_MAX 3

bool in_range(int *list, int len)
{
        if(len < 2)
                return true;

        for(int i = 0; i < (len - 1); i++) {
                int temp = list[i] - list[i + 1];
                if(temp == 0 || temp < -3 || temp > 3)
                        return false;
        }
        return true;
}

bool exe1(int *list, int len)
{
        if(!is_sorted(list, len) && !is_sorted_dec(list, len)) 
                return false;

        if(!in_range(list, len))
                return false;

        return true;
}

int main(int argc, char *argv[])
{
        FILE *fp;
        char *filename;
        char *line;
        size_t line_len = 64;
        int line_num = 0;
        int safe_lines = 0;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;
        
        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit", filename);
                exit(EXIT_FAILURE);
        }

        while(getline(&line, &line_len, fp) != -1) {
                char *token = NULL;
                int token_count = 0;
                int *levels = NULL;
                bool safe = false;

                line_num++;

                levels = (int *) calloc(strlen(line), sizeof(int));

                token = strtok(line, " ");
                if (!token)
                        printf("ERROR\n");

                do {
                        token_count++;
                        
                        levels[token_count - 1] = atoi(token);
                } while((token = strtok(NULL, " ")) != NULL);

                /*
                printf("%d: [ ", line_num);
                for(int i = 0; i < token_count; i++) {
                        printf("%d", levels[i]);
                        if (i + 1 < token_count)
                                printf(", ");
                }
                printf(" ]\n");
                */

                if(!exe1(levels, token_count)) {
                        for(int i = 0; i < token_count && !safe; i++) {
                                int *cpy = (int *) calloc(token_count, sizeof(int));

                                for(int j = 0; j < token_count; j++) {
                                        cpy[j] = levels[j];
                                }

                                for(int j = i; j < (token_count - 1); j++) {
                                        cpy[j] = cpy[j + 1];
                                }

                                if(exe1(cpy, token_count - 1))
                                        safe = true;

                                free(cpy);
                        }
                } else {
                        safe = true;
                }

                if(!safe)
                        goto out;

                safe_lines++;
                // check for min max
out:
                printf("[ %4d ]: %ssafe\n", line_num, safe ? "  " : "un");
                free(levels);
        }
        fclose(fp);

        printf("Number of safe lines: %d\n", safe_lines);

        exit(EXIT_SUCCESS);
}
