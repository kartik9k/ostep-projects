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
    path[1] = NULL;

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
	    while (NULL != cmds)
        {
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
        int cmds_proc_id[100];
        while (i <= parallel_cmds)
        {
            char *cmd_for_child = (char *)malloc(sizeof(cmds_to_exec[i]));
            strcpy (cmd_for_child, cmds_to_exec[i]);

        #if defined (DEBUG)
            printf ("Running command %d: %s\n", i, cmd_for_child);
        #endif

            /* Seperate commands and arguments */
            char *curr_cmd = strsep(&cmd_for_child, " ");
        #if defined (DEBUG)
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
        #endif
            /* Execute path cmd on parent branch */
            if (0 == strcmp(curr_cmd, "path"))
            {
                if (NULL != cmd_for_child)
                {
                #if defined(DEBUG)
                    printf ("Command is path, printing args : %s\n", cmd_for_child);
                #endif
                    /* Parsing args and adding to Path */
                    char *arg = strsep(&cmd_for_child, " ");
                    int itr = 0;
                    while (NULL != arg)
                    {
                        path[itr] = arg;
                        arg = strsep(&cmd_for_child, " ");
                        itr += 1;
                    }
                    path[itr] = NULL;

                    itr = 0;
                #if defined (DEBUG)
                    while (NULL != path[itr])
                    {
                        printf ("path[%d] = %s\n", itr, path[itr]);
                        itr += 1;
                    }
                #endif
                }

                /* Error handling here */
                else
                {
                    
                }
                
            }

            /* Execute cd on parent branch */
            else if (0 == strcmp(curr_cmd, "cd"))
            {
                if (NULL != cmd_for_child)
                {
                #if defined(DEBUG)
                    printf ("Command is cd, printing args : %s\n", cmd_for_child);
                #endif
                    /* Parsing args and adding to Path */
                    char *arg = strsep(&cmd_for_child, " ");
                    int itr = 0;

                    char *to_path;

                    while (NULL != arg)
                    {
                        to_path = arg;
                        arg = strsep(&cmd_for_child, " ");
                        itr += 1;
                    }

                    if (1 == itr)
                    {
                    #if defined (DEBUG)
                        printf ("Changing dir to : %s\n", to_path);
                    #endif
                        int ret_chdir = chdir((const char *)to_path);
                        if (0 == ret_chdir)
                        {
                        #if defined (DEBUG)
                            char cur_dir[1000];
                            getcwd(cur_dir, 1000);
                            printf ("Changed to dir : %s\n", cur_dir);
                        #endif
                        }

                        /* Error handling here */
                        else
                        {

                        }
                        
                    }

                    /* Error handling here */
                    else
                    {
                        
                    }
                    
                }

                else
                {
                    
                }
                
            }

            /* Execute exit on Parent branch */
            else if (0 == strcmp(curr_cmd, "exit"))
            {
                /* Wait for pids to complete */

                /* Call exit func */
            }

            /* Else fork and exec */
            else
            {
                int proc_id = fork();

                /* Child branch -- exec cmd[i] here */
                if (0 == proc_id)
                {
                    int itr = 0;
                    while (NULL != path[itr])
                    {
                        char *path_to_exec = malloc(sizeof(path[itr]) + sizeof(curr_cmd));
                        strcat(path_to_exec, path[itr]);
                        strcat(path_to_exec, curr_cmd);

                        if (0 == access(path_to_exec, X_OK))
                        {
                        #if defined (DEBUG)
                            printf ("Cmd : %s found in %s\n", curr_cmd, path_to_exec);
                        #endif
                            execv(path_to_exec, &cmds_to_exec[i]);
                        }

                        itr += 1;

                        free((void *)path_to_exec);
                    }
                #if defined (DEBUG)
                    printf ("Returning\n");
                #endif
                    return 0;
                }

                /* Parent branch */
                else
                {
                    /* Fork failed -- return from here */
                    if (-1 == proc_id)
                    {
                        printf ("Fork failed\n");
                        return 0;
                    }

                    printf ("In parent call\n");
                    cmds_proc_id[i] = proc_id;
                    /* Do nothing */
                }
            }
            i += 1;
        }

        /* Wait for PIDs to finish */
    }
}