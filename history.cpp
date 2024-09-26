#include "shell.h"
#include "doubleLinkList.h"

#include <stdlib.h>






bool valid_flag ( std ::string & flag)
 { 

     for ( char   c : flag ) 
      {
            if ( !isdigit(c))
              return 0 ;  
              
      }
    return 1; 
 
 }

// first call here

void history_implementation(char** cmd_args, const int arg_len)
{
     if (arg_len > 2 )
         {
            printf("Too many arguments are passed , expected numnber of arguments are 2 . \n");
            return ; 
         }
     
     if ( arg_len == 2) 
      {   
         std:: string valid =  std::string( cmd_args[1]) ; 

        if (!valid_flag ( valid) )
            {
            printf( "Expected only numeric values \n" );
            return ;
            }

         long long   see =  atoi(cmd_args[1]);

         if (see <= 0 ) 
          {
            printf (" Expected positive value \n");
            return ;
          }
          
        else if (see  > 10 ) 
        see =10 ; 

        displayHistory(see);

          

      }
    else 
     {
       displayHistory(10);
     }
}