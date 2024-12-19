#include "../aocutils.h"
#include <limits.h>

void print_arr(int *array, int len) 
{
        for(int i = 0; i < len; i++)
                if(array[i] == INT_MAX)
                        printf(". ");
                else
                        printf("%d ", array[i]);
        
        printf("\n");
}

int *dm2data(char *dm, int len, int *ret_len) 
{
        int *data = NULL;
        unsigned long size = 0;
        bool alt = true;
        int id = 0;
        unsigned long idx = 0;

        for(int i = 0; i < len; i++) {
                size += *(dm + i) - '0';
                // printf("%d ", *(dm + i) - '0');
        }
        printf("data size: %lu\n", size);
        // printf("\n");

        data = (int *) calloc(size, sizeof(int));
        if(data == NULL)
                exit(EXIT_FAILURE);

        for(int i = 0; i < size; i++)
                data[i] = INT_MAX;

        for(unsigned long i = 0; i < len && idx < size; i++) {
                int c = *(dm + i) - '0';

                if(!alt) {
                        idx += c;
                        alt = true;
                        continue;
                }

                for(int j = 0; j < c; j++) {
                        data[idx + j] = id;
                }

                idx += c;
                id++;
                alt = false;
        }

        *ret_len = size;

        return data;
}

int get_last_data(int *data, int len)
{
        int ret = 0;

        for(int i = len - 1; i > 0; i--)
                if(data[i] != INT_MAX)
                        return i;

        return -1;
}

int get_first_free(int *data, int len) 
{
        int ret = 0;

        while(data[ret] != INT_MAX && ret < len) 
                ret++;

        return ret;
}

unsigned long data2hash(int *data, int len)
{
        unsigned long ret = 0;

        for(int i = 0; i < len; i++) {
                if(data[i] == INT_MAX)
                        continue;

                ret += i * data[i];
        }

        return ret;
}

void get_last_file(int *data, int len, int *start, int *size) 
{
        int end = 0;

        *size = 0;
        *start = 0;

        for(int i = len - 1; i >= 0; i--) {
                if(data[i] != INT_MAX && end == 0) {
                        end = i;
                        for(int j = end - 1; j >= 0; j--) {
                                if(data[j] != data[end]) {
                                        *start = j + 1;
                                        *size = end - *start + 1;
                                        return;
                                }
                        }
                }
        }
}

int get_first_free_of_size(int *data, int len, int size)
{
        for(int i = 0; i < len; i++) {
                int j;

                if(data[i] != INT_MAX) 
                        continue;
                
                for(j = i + 1; j < len && data[j] == INT_MAX; j++)
                        continue;

                if(j - i >= size)
                        return i;
        }

        return -1;
}

int main(int argc, char *argv[])
{
        FILE *fp;
        char *filename;
        size_t len = 0;
        int sum = 0;
        char *line = NULL;
        int *data;
        int data_len = 0;
        int file_idx = 0;
        int file_len = 0;
        int mem_idx = 0;
        int end_idx;

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

        if(getline(&line, &len, fp) == -1) {
                printf("Error on getline()\n");
                exit(EXIT_FAILURE);
        }

        len = strchr(line, '\n') - line + 1;

        printf("%lu\n", len);

        fclose(fp);

        // build data from hash
        // while first dot before last char
        //      shift last char to first dot
        // data2hash
        data = dm2data(line, len - 1, &data_len);
        // print_arr(data, data_len);

        // printf("%s\n", data);
        while(get_first_free(data, data_len) < get_last_data(data, data_len)) {
                data[get_first_free(data, data_len)] = data[get_last_data(data, data_len)];
                data[get_last_data(data, data_len)] = INT_MAX;
        }

        // print_arr(data, data_len);

        printf("HASH: %lu\n", data2hash(data, data_len));

        free(data);
        data = NULL;
        data = dm2data(line, len - 1, &data_len);
        // print_arr(data, data_len);

        // get last file
        // get first matching free space
        // move data
        get_last_file(data, data_len, &file_idx, &file_len);
        end_idx = file_idx;
        mem_idx = get_first_free_of_size(data, end_idx, file_len);
        while(end_idx > 0) {
                if(mem_idx == -1) {
                        get_last_file(data, end_idx, &file_idx, &file_len);
                        end_idx = file_idx;
                        mem_idx = get_first_free_of_size(data, end_idx, file_len);
                        continue;
                }

                for(int i = 0; i < file_len; i++) {
                        data[mem_idx + i] = data[file_idx + i];
                        data[file_idx + i] = INT_MAX;
                }

                get_last_file(data, end_idx, &file_idx, &file_len);
                end_idx = file_idx;
                mem_idx = get_first_free_of_size(data, end_idx, file_len);

                // print_arr(data, data_len);
        }

        printf("HASH: %lu\n", data2hash(data, data_len));

        free(data);
        free(line);
        exit(EXIT_SUCCESS);
}
