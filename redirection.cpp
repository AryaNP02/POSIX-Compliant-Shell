#include "shell.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define Input_size 1024


/////////  handle my file //////
struct Redirect
 {
     int input_redirect ; 
     int output_redirect ; 
     int  append_redirect ;

     char * input_file ; 
     char * output_file;
 };


///// track current running process 
int track_foreground  = -1; 



////===========================  custom defined commmand ====================//
bool  isCustom_builtin( char *command  ,char ** cmd_args  , int arg_len) 
  {  
     if (strcmp(cmd_args[0], "cd") == 0) {
        cd_implementation(cmd_args, arg_len);
           return true ;  
          }

       
         else if (strcmp(cmd_args[0], "echo") == 0) {
        echo_implementation(cmd_args, arg_len);
          return   true ;

          }
         else if (strcmp(cmd_args[0], "pwd") == 0) {
        pwd_implementation();
        return true; 
        }


        else if (strcmp(cmd_args[0], "ls") == 0) 
        {
        ls_implementation(cmd_args, arg_len);
        return true; 
        }
        else if (strcmp(cmd_args[0], "pinfo") == 0)
        {
            pinfo_implementation(command, cmd_args, arg_len);
            return true; 
        }
        else if ( (strcmp(cmd_args[0], "search") == 0) )
        {
            search_implementation(cmd_args, arg_len);
            return true; 
        }

         else if ( (strcmp(cmd_args[0], "history") == 0) )
        {
            history_implementation(cmd_args, arg_len);
            return true; 
        }

     return false ;  // if no match found 


  }

  //=========================== main handler ======================== //

void execute_command(char* command) {
    
      Redirect  redir = {0,0,0, NULL , NULL } ;

     char* token = strtok(command, " ");
     char* cmd_args[Input_size];
     int arg_len = 0;
     int background = 0;

//////////  parsing commands   /////
    while (token != NULL) {

     if (strcmp(token, ">") == 0) 
       {
            redir.output_redirect = 1;
            redir.output_file = strtok(NULL, " ");
        }
         else if (strcmp(token, ">>") == 0) 
         {
            redir.append_redirect = 1;
            redir.output_file = strtok(NULL, " ");
        }
           else if (strcmp(token, "<") == 0) 
        {
            redir.input_redirect = 1;
            redir.input_file = strtok(NULL, " ");
        } 
        else if ( strcmp(token  , "&") == 0 ) 
           {
             background = 1 ;
           } 
     else 
            cmd_args[arg_len++] = token;
        

    token = strtok(NULL, " ");
    }
    cmd_args[arg_len] = NULL;// as execvp expect null ending 

     
     

    if( strcmp(cmd_args[0], "exit") ==0 ) // exit 
      {
        exit_flag =1 ; 
        return;
      }
     
     if (strcmp(cmd_args[0], "cd") == 0) {  // as cd doesn't produce any result ..exiting
        cd_implementation( cmd_args, arg_len);
        return ; 
     }


    pid_t pid = fork();

    if (pid == 0) { //child

        /// assumme   >  , >>  not occur together ..

        if (redir.input_redirect) {
            int fd_in = open(redir.input_file, O_RDONLY);
            if (fd_in < 0) {
                perror("open");
                exit(1);
            }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (redir.output_redirect) {
            int fd_out = open(redir.output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd_out < 0) {
                perror("open");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if (redir.append_redirect) {
            int fd_out = open(redir.output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd_out < 0) {
                perror("open");
                exit(1);
            }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

 
       if (!isCustom_builtin(command , cmd_args, arg_len))
       { 
          
          execvp(cmd_args[0], cmd_args);
          perror("execvp");
        
       } 
          exit(1);
    } else if (pid > 0) {//parent 
           
          
           if (!background) {  //  handling bg 

            track_foreground = pid;
            waitpid(pid, NULL, 0);
            track_foreground = -1;
           } 
          else
          {  
             printf(" PID: %d\n", pid);
         }
           
          
        
    }
    
}
/////////////////////////////////////////////////


void handle_pipe(char** commands, int num_commands)
 {
    int pipe_fd[2];       
    int prev_fd = -1;  //track prev pipe read  fd
    pid_t pid;

    for (int i = 0; i < num_commands; ++i) {
        if (i < num_commands - 1) {  
            if (pipe(pipe_fd) < 0) {
                perror("pipe ,exiting \n");
                exit(1);
            }
        }

        pid = fork();
        if (pid <  0 ) 
          {
            perror("Pid , error occur during fork \n");
            exit(1);
          }
    if (pid == 0) { 
            if (prev_fd != -1) {  
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < num_commands - 1) { 
                close(pipe_fd[0]);       
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }

            execute_command(commands[i]);  
            exit(0);  
        } 

   
        if (prev_fd != -1) {
            close(prev_fd);  
        }

        if (i < num_commands - 1) {
            close(pipe_fd[1]); 
            prev_fd = pipe_fd[0]; 
        }

        waitpid(pid, NULL, 0);    
        
        }
}


///====================== start from here ============//
void redirection_and_piping(char* cmd) {
    
   if (strchr(cmd, '|') == NULL) {
  
        execute_command(cmd); //if no pipe
    } else {

        char* pipe_cmds[Input_size];
        int num_pipes = 0;
        char* token = strtok(cmd, "|");
        while (token != NULL) {
            pipe_cmds[num_pipes++] = token;
            token = strtok(NULL, "|");
        }
        handle_pipe(pipe_cmds, num_pipes);
    }
}
