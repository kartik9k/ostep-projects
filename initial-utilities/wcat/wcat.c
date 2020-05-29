#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    int iter;
    for (iter = 1; iter < argc; iter++)
    {
        int in_fd = open(argv[iter],  O_RDONLY);
        if (in_fd >= 0)
        {
            int size_buf = 100;
            char buf[size_buf];
            
            int bytes_rd = read(in_fd, (void *)buf, size_buf);
            while (bytes_rd != 0)
            {
                printf ("%s", (char *)buf);
                bytes_rd = read(in_fd, (void *)buf, size_buf);
            }
        }
        else
        {
            /* Do nothing */
            printf ("wcat: cannot open file\n");
            return 1;
        }
        close(in_fd);

    }
    return 0;
}