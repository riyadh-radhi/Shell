#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <pwd.h>


char **get_input(char *input) {

	char **command = malloc(8 * sizeof(char *)); 
	char *input_array;
	int i = 0;

	input_array = strtok(input, " "); 
	while (input_array != NULL) {
		command[i] = input_array;
		i++;
		input_array = strtok(NULL, " ");
	}
	command[i] = NULL;
	if (command == NULL) exit(1);
	return command;
}
int cd (char *path){
	return chdir(path);
}

int main() {
	char **command;
	char *input;
	int stat_loc;
	char *input_array;
	pid_t cpid;
	int background = 0;

	while (1) {
		background = 0;
		if (getuid() == 0) {
			printf("root");
			input = readline("# ");
		}
		if (getuid() != 0) {
			struct passwd *pws;
			pws = getpwuid(getuid());
			printf("%s", pws->pw_name);
			input = readline("$");
		}	

		command = get_input(input);

		if (!command[0]) {
			free(input);
			free(command);
			continue;
		}
		if (strcmp (command[0], "cd") == 0 ){
			if (cd(command[1]) <0) perror(command[1]);
			continue;
		}

		if (strcmp(command[0], "exit") == 0) break;


	        int i;
		int j = sizeof(command)/sizeof(char);
                for (i = 0; i < j; i++)
		{
                 //  printf ("Element[%d] =  %s\n", i, command[i]);
		   if (command[i] != NULL)
	           {
	            // printf ("strcmp magic\n");
	             if (strcmp(command[i], "&") == 0)
		     {
		       //printf ("\nfound %s as %d element\n", command[i], i);
                       command[i] = NULL;
		       background = 1;
	             }
		  }
	        }
                //printf ("\nnow forking\n\n");	
		cpid = fork();
	
		if (cpid < 0) {
			perror("There is something wrong\n"); 
			exit(1);
		}

		if (cpid == 0) { //child
		  {
                       /* int i;
			int j = sizeof(command)/sizeof(char);
                        for (i = 0; i < j; i++)
			{
                           printf ("\nElement[%d] =  %s", i, command[i]);

			}*/
			if (execvp(command[0], command) < 0) {
				perror(command[0]);
			        exit(1);
			}			
		}
		}
	       else
		 { //parent

			 if (background != 1 ) {
			    waitpid(cpid, &stat_loc, WUNTRACED);
			 }
		free(input); 
		free(command);


	         }
	}
	return 0;
}
