#ifndef SHELL_H
#define SHELL_H


/// paths

extern char* myHome ;
extern char* prevDirPath; 

//track process status
extern int   exit_flag;
extern  int  track_foreground ;

// terminal and display 
char* get_username();
char* get_system_name();
char* get_current_directory();
void set_initial_directory();
void turn_on_raw_mode(struct termios *orig_termios);
void turn_off_raw_mode(struct termios *orig_termios);

void display_prompt();

//commands 
void cd_implementation( char** cmd_args, const int arg_len);
void echo_implementation(char** cmd_args, const int arg_len);
void pwd_implementation();
void ls_implementation(char **cmd_args, int arg_len);
void pinfo_implementation(char *command  ,char ** cmd_args  , int arg_len);
void search_implementation(char **cmd_args, int arg_len) ;
void history_implementation(char** cmd_args, const int arg_len);
void setup_signal_handlers();


// pipe 
void redirection_and_piping(char* cmd);

#endif // SHELL_H
