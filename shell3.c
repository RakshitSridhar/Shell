#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#define MAX_INPUT_SIZE 1024

/*
Name: Rakshit Sridhar
Subject: Operating Systems: LAB 2


References->
https://geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
https://www.geeksforgeeks.org/size_t-data-type-c-language/
https://linuxhint.com/asprintf-3-c-function/
https://www.youtube.com/watch?v=Vx3GyTVFcMA
https://iq.opengenus.org/chdir-fchdir-getcwd-in-c/
https://www.youtube.com/watch?v=nrO_pXGZc3Y
https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
https://www.geeksforgeeks.org/access-command-in-linux-with-examples/
https://www.geeksforgeeks.org/pipe-system-call/
https://gist.github.com/tam5/be8e818d4c77dc480451
https://www.youtube.com/watch?v=ubt-UjcQUYg&list=PLbtzT1TYeoMhF4hcpEiCsOeN13zqrzBJq
https://stackoverflow.com/questions/26086600/warning-multi-character-character-constant-wmultichar
https://web.stanford.edu/class/cs111/spring22/proj_shell/
https://doc.lagout.org/operating%20system%20/linux/Learning%20the%20bash%20Shell%20-%20Unix%20Shell%20Programming.pdf
https://stackoverflow.com/questions/33606778/signal-handler-for-sigint
Tutors suggestions helped me tokenize the command and handling multiple arguments cases 8-16, also helped me differentiate > and >>.
*/

void handler(int sig)
{}


char* print_prompt() 
{
    char cwd[MAX_INPUT_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
        char *prompt;
        asprintf(&prompt, "[nyush %s]$", basename(cwd));
        return prompt;
    } 
    else 
    {
        printf("error");
        exit(1);
    }
}

int main() 
{

	char* buffer;
	size_t bufsize=1000;
	buffer=(char*)malloc(bufsize * sizeof(char));	
    while (1) 
    {
        
        char *prompt = print_prompt();
        printf("%s ", prompt);
	      fflush(stdout);
        free(prompt);

      

	      size_t input= getline(&buffer,&bufsize,stdin);

        if(input==-1)
        {
           break;
        }

        if(input<=1)
        {
          continue;
        }

        buffer[input-1]= '\0';
 
        if (strncmp(buffer, "exit", 4)==0)
       {

            char* cmd = buffer+5;
            int count=0;
            char *ptr = buffer;
            while((ptr = strchr(ptr, ' ')) != NULL)
            {
                count++;
                ptr++;
            }
            while(*cmd == ' ')
            {
              cmd++;
            }

            if (count>=1)
            {
              fprintf(stderr,"Error: invalid command\n");
              continue;
            }
              free(buffer);
              exit(0);
        }

        

            if(strncmp(buffer,"cd",2)==0)
            {
              char* cmd = buffer+3;

              int count=0;
              char *ptr = buffer;
              while((ptr = strchr(ptr, ' ')) != NULL) 
              {
                  count++;
                  ptr++;
              }

              if(count==0 || count > 2)
              {
                fprintf(stderr, "Error: invalid command\n");
                continue;
              }

              if(chdir(cmd)!=0)
              {
                fprintf(stderr, "Error: invalid directory\n");
              }

              continue;
            }
  
	pid_t pid = fork();
  if (pid < 0) 
  {
    printf("Fork Error");
  } 
  else if (pid == 0) 
  { 
 
	 	char* command = strtok(buffer, " ");
    char* args[MAX_INPUT_SIZE];
    args[0]=command;

	  int acount=1;
		char* arg = strtok(NULL, " ");
		while(arg !=NULL)
		{
      if(*arg == '<')
      {
        char *ifl = strtok(NULL, " ");
        int inp = open(ifl, O_RDONLY);
        if(inp == -1)
        {
          fprintf(stderr,"Error: invalid file\n");
        }
        dup2(inp,0);
        close(inp);
        
      }
      else if(strcmp(arg,">>")==0)
      {
        char *ofl = strtok(NULL, " ");
        int out = open(ofl, O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(out, 1);
        close(out);
      }
      else if(*arg == '>')
      {
        char *ofl = strtok(NULL, " ");
        int out = open(ofl, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        dup2(out, 1);
        close(out);
      }
      
      else{
        args[acount]=arg;
        acount++;
      }
			
			arg = strtok(NULL, " ");
			
		}

		args[acount]=NULL;
                  
    execvp(command, args);
		fprintf(stderr, "Error: invalid program\n");           
	  exit(1);  
  } 
  else 
  {
    int status;
    waitpid(-1, &status, 0);

  }

  }    
  free(buffer);   
    return 0;
}

