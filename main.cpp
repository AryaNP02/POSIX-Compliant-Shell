#include "shell.h"
#include "doubleLinkList.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <termios.h> 
#include <dirent.h>
#include <sys/types.h>

using namespace std; 

#define Input_size 1024




void  find_in_directory (vector<string> & contents ,const string& path) ; // use to get list of all files/directory.


void get_command(string &input, vector<string> &commands) { 
    
    // parsing the input : handling case for  delimeter within quotes.
    string temp;
    bool prev_qoutes = false;
    
    for (char c : input) {
        if (c == '"') {
            prev_qoutes = !prev_qoutes;
            temp.push_back(c);

        } 
        else if (c == ';')
         {
            if (!temp.empty() && !prev_qoutes) 
            {
                commands.push_back(temp);
                temp.clear();
            }
        }
        else if (c == ' ' && prev_qoutes ) 
        {
            
            temp.append("/*");  
        } 
        else
         
            temp.push_back(c);
        
    }
    
    if (!temp.empty()) {
        commands.push_back(temp);
    }
}
/////////////////////////////////////////////// Reading in Raw mode ///////////////////////////////////////////
void read_input( vector<string> &commands) {
    struct termios orig_termios;
    turn_on_raw_mode(&orig_termios);  

    string input;  
    char c;
    
    int  prev_hist_pos  = 0 ; 
   

    while (true) {
        c = getchar();  
 
        if (c == 10)  
            break;  

        
        
     if (c == 127) 
        {  
            
            if (input.empty())
                 continue; 

                input.pop_back(); 
                printf("\b \b"); 
              
         }
         else if (c == 27)  
            {
                c = getchar();  
                c = getchar();  
                if (c == 'A')  
                {
                    
                   
                   string temp =   getprevious(prev_hist_pos);
                   prev_hist_pos++;

                   while (!input.empty())
                   {
                input.pop_back(); 
                printf("\b \b"); 
                   }

                   input = temp ;
                   for ( char c : temp) 
                    {
                        putchar(c);
                    }
                }
            /*
             else if ( c == 'B') 
               { prev_hist_pos--;
                 if (prev_hist_pos  < 0 ) 
                  prev_hist_pos = 0 ; 
                  
                string temp =   getprevious(prev_hist_pos);
                   

                   while (!input.empty())
                   {
                input.pop_back(); 
                printf("\b \b"); 
                   }

                   input = temp ;
                   for ( char c : temp) 
                    {
                        putchar(c);
                    }
               }
               */
               
                continue;
            }
          else if (c == '\t') 
          {
            int last_space = input.find_last_of(' ');
            
            string prefix ;

            if (last_space == string::npos) 
                   prefix = input ;
            else 
             
                 prefix = input.substr(last_space + 1);


            vector<string> list_all ;


            find_in_directory(  list_all,".");

            vector<string> all_match;


            // searching is  done O(n*k) , n : No. of  files , k = length of prefix .
            for ( auto file : list_all) {
                if (file.find(prefix) == 0) {  
                    all_match.push_back(file);
                }
            }
            //

            if (all_match.size() == 1) {
            
                string match = all_match.front();
                
              
                for ( int i= input.length()-1 ; i >= 0 ;  i--) 
                 {
                     if ( input[i] == ' ') 
                       {
                        break;
                       }
                       printf("\b \b");
                       input.pop_back();
                 }
            

                for ( char  c:  match) 
                    {
                        putchar(c);
                        input.push_back(c);
                    }
             

        
            } else if (all_match.size() > 1) {
       

                printf("\n");

                for ( auto match : all_match) {
                    printf("%s\n", match.c_str());
                }
                
                display_prompt();

              for ( char  c : input ) 
               putchar(c);
            
            }
            continue;

            
          } 
        else if ( c == 4) //ctrl+d
         {
               
               
                turn_off_raw_mode(&orig_termios); 
                printf("\n");
                 exit(1);
         }
        

        else if ( c >= 32 && c <= 126 ) 
        {
            input.push_back(c);
            printf("%c", c);  
          
        }
    }


    if (!input.empty()) {
        addCommand(input);  
        get_command(input, commands);
    }

    printf("\n");  
    turn_off_raw_mode(&orig_termios);  
}



// ======================================= main  ======================================================//
int main() {
   
    set_initial_directory();// to  get prompt credentials.
    loadFromFile();          // init   double linklist  - for history.
    setup_signal_handlers(); 


    while (true) 
    {
        vector<string> commands;  
        display_prompt();  
        
        read_input(commands);  

  

        for (auto cmd : commands) 
            {   char * commands_to_pass  = (char *) cmd.c_str(); 
                redirection_and_piping(commands_to_pass);   
                
            } 
     
        if (exit_flag)
         break;


    }

    saveToFile();  //saving content to a file.
    deletelist();  //  free allocated memory
    return 0;
}

///////////////////////////////////////// List all files ////////////////////////////////////////////


 void  find_in_directory (vector<string> & contents ,const string& path) {
 
    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        perror("opendir");
        return ;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
       
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            contents.push_back(entry->d_name);
        }
    }

    closedir(dir);
  
}