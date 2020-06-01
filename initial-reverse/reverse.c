#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    // printf ("%d", argc);
    FILE *fp = NULL;
    FILE *out_fp = NULL;
    if (argc == 1)
    {
        fp = fdopen (STDIN_FILENO, "r");
        out_fp = fdopen (STDOUT_FILENO, "w");
    }

    else if (argc == 2)
    {
        fp = fopen (argv[1], "r");
        out_fp = fdopen (STDOUT_FILENO, "w");
    }
    else if (argc == 3)
    {
        if (strcmp(argv[1], argv[2]) == 0)
        {
            fprintf (stderr, "reverse: input and output file must differ\n");
            return 1;
        }

        fp = fopen (argv[1], "r");
        out_fp = fopen (argv[2], "w");
    }

    else
    {
        fprintf (stderr ,"usage: reverse <input> <output>\n");
        return 1;
    }
    
    if (NULL == fp)
    {
        fprintf (stderr, "reverse: cannot open file '%s'\n", argv[1]);
        return 1;
    }
    if (NULL == out_fp)
    {
        fprintf (stderr, "reverse: cannot open file '%s'\n", argv[2]);
        return 1;
    }

    if (argc == 3)
    {
        struct stat in_stat;
        struct stat out_stat;
        stat (argv[1], &in_stat);
        stat (argv[2], &out_stat);

        if ((long) in_stat.st_ino == (long) out_stat.st_ino)
        {
            fprintf (stderr, "reverse: input and output file must differ\n");
            return 1;        
        }
    }


    size_t line_sz = 0;

    char **file_line[2000];
    char *line = NULL;

    int rd_status = -1;
    rd_status = getline(&line, &line_sz, fp);

    int itr = 0;
    while (-1 != rd_status)
    {
        file_line[itr] = (char *)malloc(line_sz * sizeof(char));

        if (file_line[itr] == NULL)
        {
            fprintf (stderr, "malloc failed\n");
            return 1;
        }
        strcpy (file_line[itr], line);
        itr += 1;
        rd_status = getline(&line, &line_sz, fp);
    }

    itr --;
    while (itr >= 0)
    {
        fwrite (file_line[itr], sizeof(char), strlen(file_line[itr]), out_fp);
        itr -= 1;
    }
    free(line);
    fclose(fp); 
    return 0;
}