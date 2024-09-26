#include "shell.h"
#include <stdio.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <iostream>



# define pathSize 2048






void display_prompt() 
{

    char *username = getenv("USER");

    struct utsname sys_info;
    uname(&sys_info);


     char cwd[pathSize] ;
      if (getcwd(cwd,pathSize) == NULL) {
        perror("getcwd");
        printf("%s@%s:~> ", username, sys_info.nodename);
        return;
    }

    char *initial_dir = myHome; // current directory

    if (initial_dir == NULL) {
        fprintf(stderr, "Initial directory environment variable not set\n");
        exit(1);
    }
   
      //std ::  cout << "My current directory :"<< cwd  <<" \n" <<"my pseudeo home" <<initial_dir <<"\n";
  
    if (strncmp(cwd, initial_dir, strlen(initial_dir)) == 0) {
        if (strlen(cwd) == strlen(initial_dir)) {
            printf("%s@%s:~> ", username, sys_info.nodename);
        } else {
            printf("%s@%s:~%s> ", username, sys_info.nodename, cwd + strlen(initial_dir));
        }
    } else {
        printf("%s@%s:%s> ", username, sys_info.nodename, cwd);
    }

}


///// handling signals ///

void handle_ctrl_z (int sig) {
   
    if (track_foreground == -1) 
        {
         
             return;
        }

    kill(track_foreground, SIGTSTP); 
    printf("\nCProcess %d suspended and moved to background\n", track_foreground);
    track_foreground = -1; 
    display_prompt();
    
}

void handle_ctrl_C (int sig) {
   
    if (track_foreground == -1) 
     return; 

    kill(track_foreground, SIGINT);  
    printf("\n  Ctrl+C  recived ,  the  Process: %d is been interrupted\n", track_foreground);
    track_foreground = -1;
    display_prompt();
    
}



// setup

void setup_signal_handlers()
{
 signal(SIGINT, handle_ctrl_C);
 signal(SIGTSTP, handle_ctrl_z);

}
