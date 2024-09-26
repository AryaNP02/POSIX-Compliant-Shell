#include "shell.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h> 

#include <iostream>


#define Max_execute_path 1024

using namespace std;

struct ProcessHandler 
{    
    pid_t pid;

    string pid_str;
    string path_for_status, path_for_exec;
    string mem ;

    string process_status;

};



bool isForeground(pid_t pid) {

    return (tcgetpgrp(STDIN_FILENO) ==  getpgid(pid));
}


void pinfo_implementation(char *command  ,char ** cmd_args  , int arg_len) 
{

    if (arg_len > 2) 
    {
        printf("Error: Too many arguments passed \n");
        return;
    }

    ProcessHandler process ; 

    
    long long  execFile_len;
   

   // cout <<arg_len;

    if (arg_len == 1) 
    {
        process.pid = getpid(); 
        process.pid_str = to_string(process.pid);

    } 
   else 
   {
     
        string arg(cmd_args[1]);

        for (char ch : arg) 
        {
            if (!isdigit(ch)) 
            {
                printf("Error: invalid pid \n");
                return;
            }
        }
       process.pid = stoi(arg);

        if (process.pid < 0) 
        {
            printf("Error: pid cannot be negative \n");
            return;
        }
        process.pid_str = arg;
    }

    
    process.path_for_status = "/proc/" + process.pid_str + "/status";
    process.path_for_exec = "/proc/" + process.pid_str + "/exe";


    ifstream status_file(process.path_for_status);

    if (!status_file) 
    {
        perror("Error :fail to open status file \n");
        return;
    }

   
    string line;

    while (getline(status_file, line)) 
    {
    
        if (line.substr(0, 6) == "State:") {
        
            int colon_pos = line.find(':');
            if (colon_pos != (int)string::npos) {
                process.process_status.push_back( line[colon_pos + 2]);
                if (isForeground(process.pid))
                  {
                    process.process_status.push_back('+');
                  } 
            }
        }
        
        else if (line.substr(0, 7) == "VmSize:") {
           
            long start = line.find(':');
            if (start != (long)string::npos) 
            {
               
            
                process.mem = line.substr(start + 1 );
                for (int i=0 ; i <3 ; i++) 
                 process.mem.pop_back();
            
             }
        }
    }
    status_file.close();

    

    printf("pid -- %20s\n", process.pid_str.c_str());
    printf("Process Status -- %5s\n", process.process_status.c_str());
    if (process.mem != "")
    { process.mem = process.mem + "{Virtual}";
        printf("memory -- %s\n", process.mem.c_str());
    }

 
    char exec_path[Max_execute_path + 1];
    bool can_access =1; 
    execFile_len = readlink(process.path_for_exec.c_str(), exec_path, Max_execute_path);

   if (execFile_len < 0) {
        perror("Error :fail to open exec file \n");
        can_access =0 ; 
    } 
    
    if (can_access)
   { exec_path[execFile_len] = '\0';

    
    printf("Executable Path -- %s\n", exec_path);}

}

