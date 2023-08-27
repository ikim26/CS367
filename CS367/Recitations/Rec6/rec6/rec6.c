#include <stdio.h>   /* printf  */
#include <string.h>  /* strncmp, strdup */
#include <stdlib.h>  /* atoi, strtol    */
#include <malloc.h>  /* malloc, free */

extern long        mul_24 (long);
extern long       average (long, long);
extern long       diff_sq (long, long);
extern long       set_bit (long, long);
extern long      set_bit_param (long*, long);
extern long     set_bit_val (long*, long, long);

extern long         func1 (long);
extern long         func2 (long,long,long,long);
extern long         func3 (long*, long*, long);

static int check_func_params(const char* prog, const char* funcname, int argc, int req);

/*
 A helper function which makes sure we have enough input args.
 prog is the program's name
 funcname is the name of the function;
 argc is the program's argc;
 req is the required number of function inputs;
 If the number of args is ok, then returns true, otherwise prints an error message and quits.
*/
static int check_func_params(const char* prog, const char* funcname, int argc, int req) {
    if (argc == req + 2) return 1;
    fprintf(stderr, "error - wrong number of arguments.\n\n");
    fprintf(stderr, "\tusage: %s %s", prog, funcname);
    for (int i = 1; i <= req;  i++) { fprintf(stderr, " arg%d", i); }
    fprintf(stderr, "\n\n");
    exit(1);
}

/*
receives command line arguments for:
 #1 function to be tested
 #2 first argument
 (#3+: more arguments, or array values for the last argument)
*/
int main(int argc, char ** argv){
  /* check for minimum number of arguments */
  if (argc<3){
    fprintf(stderr, "error - not enough arguments.\n\n\tusage: %s funcname arg1 [arg2 arg3 ...]\n\n", argv[0]);
    return 1;
  }

  /* convert the inputs to numbers, wherever possible. */
  long *inputs = calloc(argc, sizeof(long));  // create an array to store the numbers
  long *args = inputs + 1;                    // the args themselves start at offset 2, so arg[1] would be arg1.
  if (!inputs) {
    fprintf(stderr, "error - memory allocation failed.\n\n");
    return 1;
  }
  memset(inputs, 0, argc * sizeof(long));     // default to all zeros
  for (int i = 2;  i < argc;  i++) {          // loop and load the chars
    inputs[i] = strtol(argv[i], NULL, 0);     // convert string to long (accepts both hex and decimal, zero on err)
  }
  
  
  /* dispatch to the correct function */
  char* progName = argv[0];
  char* funcName = argv[1];
  
  
  if ( ! strncmp("mul_24", funcName,10)){ 
    check_func_params(progName, funcName, argc, 1);
    long x = args[1];
    printf("%ld\n", mul_24(x)); 
  }
  
  
  else if ( ! strncmp("average",funcName,10)){
    check_func_params(progName, funcName, argc, 2);
    long a = args[1], b = args[2];
    printf("%ld\n", average(a,b));
  }
  
  else if ( ! strncmp("diff_sq",funcName,10)){
    check_func_params(progName, funcName, argc, 2);
    long x = args[1], y = args[2];
    printf("%ld\n", diff_sq(x, y));
  }
  
  else if ( ! strncmp("set_bit",  funcName,10)){
    check_func_params(progName, funcName, argc, 2);
    long flags = args[1], n = args[2];
    long result = set_bit(flags, n);
    printf("%ld (0x%lX)\n", result, result);
  }
  
  
  else if ( ! strncmp("set_bit_param",funcName,15)){
    check_func_params(progName, funcName, argc, 2);
    long flags = args[1], n = args[2];
    long result = set_bit_param(&flags, n);
    printf("%ld [%ld (0x%lX)]\n", result, flags, flags);
  }
  
  
  else if ( ! strncmp("set_bit_val",funcName,15)){
    check_func_params(progName, funcName, argc, 3);
    long flags = args[1], n = args[2], b = args[3];
    long result = set_bit_val(&flags, n, b);
    printf("%ld [%ld (0x%lX)]\n", result, flags, flags);
  }
  
  else if ( ! strncmp("func1", funcName,10)){
    check_func_params(progName, funcName, argc, 1);
    long n = args[1];
    printf("%ld\n", func1(n));
  }

  else if ( ! strncmp("func2",funcName,10)){
    check_func_params(progName, funcName, argc, 4);
    long a = args[1], b = args[2], c = args[3], d = args[4];
    printf("%ld\n", func2(a,b,c,d));
  }


 else if ( ! strncmp("func3",funcName,10)){
    check_func_params(progName, funcName, argc, 3);
    long x = args[1], y = args[2], z = args[3];
    printf("%ld\n", func3(&x,&y,z));
  }


  /* give a helpful message when the function is misspelled. */
  else {
    fprintf(stderr, "error - unrecognized command '%s'.\n",argv[1]);
    free(inputs);
    return 2;
  }

  free(inputs);
  return 0;
}
