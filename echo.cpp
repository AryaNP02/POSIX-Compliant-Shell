#include "shell.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>



using namespace std; 



string  removeQuotes( string  s)
{
    string  after_removing ; 
     for ( int  i  =0 ;i <  s.size() ; i++ )
      {
       if (i < s.size() - 1 && s[i] == '/' && s[i + 1] == '*')
       {
        after_removing.push_back(' ');
        i++;
       }
       else if ( s[i] != '"')
          after_removing.push_back(s[i]);
      }

    return after_removing;
}


    //////   start here      /////


void echo_implementation(char** cmd_args, const int arg_len)
 { 

    for (int i = 1; i < arg_len; ++i) {
      
        //std :: cout <<  cmd_args[i] <<"\n" ;
        string show_text =  removeQuotes( string (cmd_args[i]));
        printf("%s", show_text.c_str());

        if ( i <  arg_len-1) 
          printf(" ");
    }
    printf("\n");
}