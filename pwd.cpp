#include "shell.h"

#include <stdio.h>
#include <unistd.h>



#define  max_path_length 1024

void pwd_implementation() {
    
    char cwd[max_path_length];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}
