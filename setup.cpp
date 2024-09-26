#include "shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <stdio.h>
#include <termios.h> 

using namespace std;

///  init my global variabls //// 
char * myHome;
char * prevDirPath;
int exit_flag;




void set_exit_flag()
{
    exit_flag =0 ;
}


void set_initial_directory() {
    set_exit_flag();
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
          myHome = strdup(cwd);
          prevDirPath = strdup(cwd);

    } else {
        perror("getcwd() error");
        exit(1);
    }
}

char* get_username() {
    uid_t uid = getuid();
    struct passwd *pw = getpwuid(uid);

    return strdup(pw->pw_name);
}

char* get_system_name() {
    char hostname[1024];
    gethostname(hostname, sizeof(hostname));
    return strdup(hostname);
}

char* get_current_directory() {
    char cwd[1024];

    getcwd(cwd, sizeof(cwd));
    return strdup(cwd);
}


///  handle   raw mode ///
void turn_on_raw_mode(struct termios *orig_termios) 
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);  

    *orig_termios = raw;          

    raw.c_lflag &= ~(ECHO | ICANON);  
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);  
}


void turn_off_raw_mode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}
