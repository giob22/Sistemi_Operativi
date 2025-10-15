#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h> // for isspace();

int is_background(char * s){
    int i = strlen(s);
    i--;
    while (i >= 0 && (char)s[i] == ' '){
        i--;
    }
    return (((char)s[i] == '&') ? i : -1);
}


int main(){

    pid_t pid;
    int st;
    const int NMAX = 50;
    const int CMAX = 100;
    const int PMAX = 10;
    char* argv[NMAX];
    char* p_argv[PMAX];
    char comLine[CMAX + 1];
    char * token; // temp
    char endShell[5] = "quit";

    int n;

    printf("--SHELL OPENED--\n");
    

    while(1){
        printf("my_shell>> ");

        // read

        if(fgets(comLine, CMAX + 1, stdin) == NULL){
            fprintf(stderr, "errore in lettura");
            return -1;
        }

        // put \0 at the end of comLine
        char* p;
        if((p = strrchr(comLine, '\n')) != NULL)
            *p = '\0';

        if (strcmp(comLine, endShell) == 0){
            printf("--SHELL CLOSED--\n");
            return 0;
        }
        
        // press enter
        if(strlen(comLine) == 0){
            continue;
        }
        
        
        pid = fork();
        if(pid < 0){
            fprintf(stderr,"error during the creation of the process\n");
            return -1;
        }else if(pid == 0){
            //code for the new process
            
            // slip the comLine
            
            //if & (background)
            n = 0;
            token = strtok(comLine, "|");
            while (token != NULL){
                p_argv[n] = token;
                n++;
                token = strtok(NULL, "|");
            }
            int p_n = n;
            int pos = is_background(p_argv[p_n - 1]);
            if(pos >= 0){
                p_argv[p_n - 1][pos] = ' ';
                pid_t pid_other = fork();
				if(pid_other < 0){
					fprintf(stderr, "generazione del processo in backgound fallita.\n");
					_exit(1);
                }else if (pid_other > 0){
                    // I terminate it in order to do a backgound execution
                    _exit(0);
                }else if (pid_other == 0){
                    int fd = open("/dev/null", O_RDONLY);

                    dup2(fd,0) // redirect stdin to /dev/null so the process can't interrupt the shell
                    close(fd);
                }
            }
            //

            //pipe matrix coteining file directors;
            int pipes[p_n - 1][2];
            // saving the current stdin
            int stdin_backup = dup(STDIN_FILENO);
            for(int i = 0; i < p_n - 1; i++){
                if (pipe(pipes[i]) == -1){
                    fprintf(stderr,"error during the pipe connection\n");
                    return -1;
                }
            }
            pipes[0][0] = stdin_backup; // in case the process is a background process
            int i = 0;
            while(i > p_n){
                dup2()
                
                
                
                
                
                i++;
            }

            
            
        
                        
            return 0;            
    
        
    }


    return 0;
}