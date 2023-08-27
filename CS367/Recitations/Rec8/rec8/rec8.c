#include <stdio.h>   /* printf  */
#include <string.h>  /* strncmp, strdup */
#include <stdlib.h>  /* atoi, strtol    */

extern long     call_or8 ();
extern long    	call_to_goal ();
extern long     call_overwrite ();
extern long     rec_fib (long);

/*
receives command line arguments for:
 #1 function to be tested
 #2 first argument
 (#3+: more arguments, or array values for the last argument)
*/
int main(int argc, char ** argv){
  /* check for minimum number of arguments */
  if (argc<2){
    printf("error - not enough arguments.\n\n\tusage: ./rec5 funcname (arg)\n\n");
    return 1;
  }
  
  /* dispatch to the correct function */
  char* funcName = argv[1];
  
  if ( ! strncmp("call_or8",funcName,10)){
    printf("%ld\n",call_or8());
  }
  
  else if ( ! strncmp("call_to_goal",funcName,10)){
    printf("%ld\n",call_to_goal());
  }
 
  else if ( ! strncmp("call_overwrite",funcName,10)){
    printf("%ld\n",call_overwrite());
  }
  
  else if ( ! strncmp("rec_fib",funcName,10)){
    long x;
    sscanf(argv[2],"%ld",&x);
    printf("%ld\n",rec_fib(x)); 
  }
 
  /* give a helpful message when the function is misspelled. */
  else {
    printf("error - unrecognized command '%s'.\n",argv[1]);
    return 2;
  }
}
