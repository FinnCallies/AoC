#include "../aocutils.h"

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

        // do stuff

        printf("sum: %d\n", sum);

        exit(EXIT_SUCCESS);
}