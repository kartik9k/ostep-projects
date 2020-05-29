#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

char *grep(char line[], char key[]);

int main(int argc, char** argv)
{
    // printf ("argc: %d\n", argc);
    if (argc < 2)
    {
        printf ("wgrep: searchterm [file ...]\n");
        return 1;
    }

    else if (argc == 2)
    {
        FILE *fp = NULL;
        size_t line_sz = 0;
        char *line = NULL;
        int rd_status = -1;
        fp = fdopen(STDIN_FILENO, "r");
        if (NULL != fp)
        {
            rd_status = getline(&line, &line_sz, fp);
            while (-1 != rd_status)
            {
                if (NULL != strstr(line, argv[1]))
                {
                    printf ("%s", line);
                }
                rd_status = getline(&line, &line_sz, fp);
                if (line[0] == '\n')
                {
                    break;
                }
            }
        }
        else
        {
            printf ("wgrep: cannot open file\n");
            return 1;
        }

        free(line);
        fclose(fp); 
    }

    else
    {
        int iter;
        for (iter = 2; iter < argc; iter++)
        {
            FILE *fp = NULL;
            size_t line_sz = 0;
            char *line = NULL;
            int rd_status = -1;
            fp = fopen(argv[iter], "r");
            if (NULL != fp)
            {
                rd_status = getline(&line, &line_sz, fp);
                while (-1 != rd_status)
                {
                    if (NULL != strstr(line, argv[1]))
                    {
                        printf ("%s", line);
                    }
                    rd_status = getline(&line, &line_sz, fp);
                }
            }
            else
            {
                printf ("wgrep: cannot open file\n");
                return 1;
            }

            free(line);
            fclose(fp);   
        }
    }
    

    return 0;
}


char *grep(char line[], char key[])
{
    return strstr(line, key);
}