#include "shell.h"

#include <sys/types.h>
#include <dirent.h>
#include <queue>
#include <string>

using namespace std ;




int  bfs_search( string & target) {
    
    queue<string> all_Dir;
    all_Dir.push(".");

    while (!all_Dir.empty()) 
    {
        string dir = all_Dir.front();
        all_Dir.pop();
        
        DIR* d = opendir(dir.c_str());
        if (d == NULL) {
            perror("opendir \n");
        
            return -1; 
        }

        struct dirent* entry;

        while ((entry = readdir(d)) != NULL) 
        {       
           

            string Dname = entry->d_name;
               
            if (Dname == target) {
                //cout<< "Find in"  <<dir.c_str();
                closedir(d);
                return 1;
            }
            
          
             if (Dname == "." || Dname == ".." )
              continue; 

            if (entry->d_type == DT_DIR ) {
                //cout<<  dir+"/"+Dname 
                all_Dir.push(dir +  "/" + Dname);
            }
        }
        closedir(d);
    }

    return 0;
}

//////////////////////////// start from here //////////////////////////////
void search_implementation(char **cmd_args, int arg_len) 
{
   if (arg_len < 2 || arg_len > 2 )
    {  
        printf("Incorrect number of argument , required 2 arguments \n");
        return;
    }

    string target = string(cmd_args[1]);
    int  result = bfs_search(target);
    
    if (result == -1) 
     {
        printf("Error occured , could not complete the task \n");
     }
    else if ( result == 1 ) 
     {
         printf("True \n");
     }
     else 
     printf("False \n");

     
 
}
