#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#define DEBUG

int main(int argc, char** argv)
{
    /* Infinite loop */
    bool loop_inf = true;
    
    /* Initialize Path variable */
    char **path[1000];
    path[0] = (char *)malloc(sizeof("/bin/"));
    strcpy(path[0], "/bin/");

    while (true == loop_inf)
    {
        /* Get user input */
		printf("wish> ");
		char *input_line = NULL;
		size_t bufsize = 0; 
  		getline(&input_line, &bufsize, stdin);

  		char *temp = input_line;
  		while ('\n' != *(input_line))
  			input_line += 1;

  		*input_line = '\0';
  		input_line = temp;

		
        /* Tokenize user input -- check if there are multiple cmds */
        char *cmds = strsep(&input_line, "&");
   		int i = 0;
        int parallel_cmds = 0;
        char *cmds_to_exec[100];
	    while (NULL != cmds){
            if (' ' == *((char *)cmds))
            {
                cmds += 1;
            }
	        cmds_to_exec[i] = cmds;
	        cmds = strsep(&input_line, "&");
	        i += 1;
	    }
	    cmds_to_exec[i] = NULL;

        /* Set number of parallel commands */
        parallel_cmds = i - 1;


    #if defined(DEBUG)
        i = 0;
        while (cmds_to_exec[i] != NULL)
        {
            printf ("Command %d : %s\n", i + 1, cmds_to_exec[i]);
            i += 1;
        }
    #endif

        i = 0;

        /* Store list of proc_ids */
        pid_t cmds_proc_id[100];
        while (i <= parallel_cmds)
        {
            char *cmd_for_child = (char *)malloc(sizeof(cmds_to_exec[i]));
            strcpy (cmd_for_child, cmds_to_exec[i]);

        #if defined (DEBUG)
            printf ("Running command %d: %s\n", i, cmd_for_child);
        #endif

            /* Seperate commands and arguments */
            char *curr_cmd = strsep(&cmd_for_child, " ");
            if (NULL != curr_cmd)
            {
                /* Cmd name - curr_cmd */
                printf ("Current cmd name : %s\n", curr_cmd);
                if (NULL != cmd_for_child)
                {
                    /* Cmd args - cmd_for_child */
                    printf ("Current args : %s\n", cmd_for_child);
                }
            }

            /* Execute path cmd on parent branch */
            if (0 == strcmp(curr_cmd, "path"))
            {

            }

            /* Else fork and exec */
            else
            {
                pid_t proc_id = fork();

                /* Child branch -- exec cmd[i] here */
                if (0 == proc_id)
                {
                    return 0;
                }

                /* Parent branch */
                else
                {
                    /* Fork failed -- return from here */
                    if (-1 == proc_id)
                    {
                        return 0;
                    }
                    cmds_proc_id[i] = proc_id;
                    /* Do nothing */
                }
            }
            i += 1;
        }

        /* Wait for PIDs to finish */
    }
}