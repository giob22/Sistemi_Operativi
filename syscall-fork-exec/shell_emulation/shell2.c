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

                    dup2(fd,0); // redirect stdin to /dev/null so the process can't interrupt the shell
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
            int pipes_cnt = p_n - 1; // numero di pipe
            
            //inverto l'ordine dei file descriptor
            //for (int i = 0; i < (p_n -1); ++i){
            //    int temp = pipes[i][0];
            //    pipes[i][0] = pipes[i][1];
            //    pipes[i][1] = temp;
            //}
            
            
            
            
            int i = 0;
            while(i < p_n){
                int pid = fork();
                if (pid == 0){

                    if (i > 0) {                    // non è il primo: legge dalla pipe precedente
                        if (dup2(pipes[i-1][0], STDIN_FILENO) == -1) { perror("dup2 in"); _exit(1); }
                    }
                    if (i < p_n - 1) {              // non è l'ultimo: scrive sulla pipe corrente
                        if (dup2(pipes[i][1], STDOUT_FILENO) == -1) { perror("dup2 out"); _exit(1); }
                    }
                    // chiudi TUTTE le estremità di tutte le pipe
                    for (int k = 0; k < pipes_cnt; ++k) {
                        close(pipes[k][0]);
                        close(pipes[k][1]);
                    }
                    
                    n = 0;
                    token = strtok(p_argv[i]," ");
                    while(token != NULL){
                        argv[n] = token;
                        n++;
                        token = strtok(NULL, " ");
                    }
                    argv[n] = (char *)0;
                    
                    
                    
                    execvp(argv[0], argv);
                    printf("\nERROR\n");
                    _exit(1);
                }   
                i++;
            }

            // parent: chiudi tutte le pipe
            // perché in questo processo rimangono aperte. essendo che i processi che creo non sono altro che copie di questo.
            for (int i = 0; i < pipes_cnt; ++i) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
            // se foreground:
            for (int i = 0; i < p_n; ++i) wait(NULL);
            
        
                        
            _exit(0);            
    
        
    }

    wait(&st);

	if(WIFEXITED(st) && WEXITSTATUS(st) != 0)
		printf("processo terminato con stato negativo (%d)\n",st);
}
return 0;
}
