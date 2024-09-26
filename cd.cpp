#include "shell.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define max_path_length 4096

char cwd[max_path_length];


void set_Cwd()
{
       if (getcwd(cwd, max_path_length) == NULL ) 
         {
             perror("Error getting  directory");
             return ;
         }

}
void update_previous() 
{       if (prevDirPath != NULL) 
          free(prevDirPath);   
        prevDirPath = strdup(cwd); 
}


int chdir_to_parent() {
  
    char* last_slash = strrchr(cwd, '/');
    
    if (last_slash == myHome || last_slash == NULL) {
        
        if( chdir(myHome) !=  0 )
         {
            perror("cd");
             return -1; 
         }
         return 0 ; 

    }

    *last_slash = '\0';

    if (chdir(cwd) != 0) {
         perror("cd");
        return -1;
    }

    return 0;
}


 
void cd_implementation( char** cmd_args, const int arg_len) {
//std :: cout << " this call is from cd \n " ; 

    set_Cwd(); 
    if (arg_len == 1) {
        if (prevDirPath != NULL) 
          free(prevDirPath);   
        prevDirPath = strdup(cwd);  
        chdir(myHome);
    

        return ;
    }
    if ( arg_len > 2 ) 
      {
         printf("cd : Too many argument");
        return  ; 
      }


     if (cmd_args[1] == " ")
       {}
 
   
    char* new_dir = cmd_args[1];


       if (strcmp(new_dir, ".") == 0) {
        return ;
    } else if (strcmp(new_dir, "..") == 0) {
         update_previous();
        if (chdir_to_parent() == -1) {
            printf("cd: Error moving to parent\n");
            return ;
        }
       
        return ;
    } else if (strcmp(new_dir, "-") == 0) {
        if (prevDirPath != NULL) {
            if (chdir(prevDirPath) != 0) {
                perror("cd");
                return ;
            }
            return ;
        } else {
            printf("cd: Previous path not set\n");
            return ;
        }
    } else if (strcmp(new_dir, "~") == 0) {
        if (chdir(myHome) != 0) {
            perror("Error changing directory to home");
            return ;
        }
        update_previous();
        return ;
    }

    if (chdir(new_dir) != 0) {
        perror("cd");
        return ;
    }

    update_previous();
    return ;

}