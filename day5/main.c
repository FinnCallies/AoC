#include "../aocutils.h"

struct rule {
        int a;
        int z;
};

bool check_rules(int nu, int nr, int *u, struct rule *r)
{
        // for num in update
        for(int i = 0; i < nu; i++) {
                // for rule in rules
                for(int j = 0; j < nr; j++) {
                        if(u[i] != r[j].a)
                                continue;

                        // for num in update BEFORE update[i]
                        for(int k = 0; k < i; k++) {
                                if(u[k] == r[j].z)
                                        return false;
                        }
                }
        }

        return true;
}

bool is_before(int a, int z, int nr, struct rule *r) 
{
        for(int i = 0; i < nr; i++) {
                if(r[i].a == a && r[i].z == z) 
                        return true;
                if(r[i].a == z && r[i].z == a)
                        return false;
        }

        return true;
}

int sort(int nu, int nr, int *u, struct rule *r)
{
        for(int i = 0; i < nu; i++) {
                for(int j = 0; j < (nu - i - 1); j++) {
                        if(!is_before(u[j], u[j + 1], nr, r)) {
                                int temp = u[j + 1];
                                u[j + 1] = u[j];
                                u[j] = temp;
                        }
                }
        }

        print_list(u, nu);
        printf("\n");

        return u[(nu - 1) / 2];
}

int main(int argc, char *argv[])
{
        FILE *fp;
        char *filename;
        int lines;
        struct rule *rules;
        char *line;
        size_t len = 64;
        int line_num = 1;
        int rule_num;
        int sum = 0;
        int sum2 = 0;

        if (argc > 1)
                filename = argv[1];
        else
                filename = TESTFILE;

        lines = getLinF(filename);
        if (lines == 0)
                exit(EXIT_SUCCESS);

        fp = fopen(filename, "r");
        if (fp == NULL)
        {
                printf("fopen() of file \"%s\" failed, early exit", filename);
                exit(EXIT_FAILURE);
        }

        rules = (struct rule *) calloc(lines, sizeof(struct rule));

        line = malloc(len);
        while(getline(&line, &len, fp) != -1 && strncmp(line, "\n", 1) != 0) {
                if (sscanf(line, "%d|%d", &(rules[line_num - 1].a), &(rules[line_num - 1].z)) < 2)
                        continue;

                line_num++;
        }
        
        rule_num = line_num - 1;

        printf("rules: %d\n", rule_num);

        while(getline(&line, &len, fp) != -1) {
                char *tok; 
                int *update;
                int line_len = strlen(line);
                int count = 0;

                update = (int *) calloc(line_len, sizeof(int));
                tok = strtok(line, ",");

                while(tok != NULL) {
                        update[count] = atoi(tok);

                        count++;
                        tok = strtok(NULL, ",");
                }

                if(check_rules(count, rule_num, update, rules)) {
                        printf("Line %d: correct\n", line_num - rule_num);
                        sum += update[(count - 1) / 2];
                } else {
                        printf("Line %d: incorrect\n", line_num - rule_num);
                        sum2 += sort(count, rule_num, update, rules);
                }

                line_num++;
                free(update);
        }

        printf("sum: %d\n", sum);
        printf("sum2: %d\n", sum2);

        fclose(fp);
        free(line);
        exit(EXIT_SUCCESS);
}
