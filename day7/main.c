#include "../aocutils.h"
#include <math.h>

unsigned long recursive(int *nums, int c, unsigned long sum)
{
        if(c == 1)
                return nums[0];

        unsigned long x = recursive(nums, c - 1, sum - nums[c - 1]) + nums[c - 1];
        if(x == sum)
                return x;

        if((sum % nums[c - 1]) == 0) {
                x = recursive(nums, c - 1, sum / nums[c - 1]) * nums[c - 1];

                if(x == sum)
                        return x;
        }

        // part 2
        int l = log10(nums[c - 1]) + 1;
        int p = pow(10, l);
        x = (nums[c - 2] * p) + nums[c - 1];
        unsigned long new_sum = (sum - nums[c - 1]) / p; 
        x = recursive(nums, c - 1, new_sum);

        x = (x * p) + nums[c - 1];

        return x;
}

int main(int argc, char *argv[])
{
        FILE *fp;
        char *filename;
        size_t len = 64;
        unsigned long sum = 0;
        char *line;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \'%s\' failed, early exit\n", filename);
                exit(EXIT_FAILURE);
        }

        while(getline(&line, &len, fp) != -1) {
                char *ptr;

                if((ptr = strchr(line, ':')) != NULL) {
                        unsigned long s;
                        char *token = malloc(ptr - line);
                        int nums = 0;
                        int *ns;
                        int idx = 0;

                        memcpy(token, line, ptr - line);
                        s = atol(token);
                        free(token);

                        ptr += 2;

                        for(int i = 0; i < strlen(line); i++)
                                if(line[i] == ' ' || line[i] == '\n')
                                        nums++;
                        ns = (int *) calloc(nums, sizeof(int));

                        token = strtok(ptr, " ");
                        do {
                                ns[idx] = atoi(token);
                                idx++;
                        } while((token = strtok(NULL, " ")) != NULL);

                        if(recursive(ns, idx, s) == s) {
                                sum += s;
                        }

                        if(sum < 0)
                                printf("NEGATIVE\n");

                        free(ns);
                }
        }

        fclose(fp);

        // do stuff

        printf("sum: %ld\n", sum);

        exit(EXIT_SUCCESS);
}
