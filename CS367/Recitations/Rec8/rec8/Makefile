# comments are always a great idea.

# we use this variable as $(CC) when building commands.
CC=gcc

# we keep using these flags in combination, so they're
# stuffed into a variable. Saves us from having to 
# re-type them repeatedly, and of course we can then
# change it in one place for all uses.
CFLAGS=-O0 -std=c99

FUNC_OS=or8.o to_goal.o overwrite.o rec_fib.o call_or8.o call_to_goal.o call_overwrite.o

# see below for the rec8 target.
all: rec8

# the command 'make clean' should remove all generated files.
clean:
	-rm -rf *~ *# *.o rec8.s rec8.o rec8

# target rec8 builds the overall executable, rec8.
# depends on all the individual functions' object files.
rec8: rec8.o  $(FUNC_OS)
	$(CC) $(CFLAGS) -o rec8 rec8.o $(FUNC_OS)

# compiles rec8.c into rec8.s; needs no other files.
rec8.s: rec8.c
	$(CC) $(CFLAGS) -S rec8.c

# compiles rec8.s into rec8.o; needs no other files.
rec8.o: rec8.s
	$(CC) $(CFLAGS) -c rec8.s

# each of the remaining targets also compiles each
# .s file into the corresponding .o file, with no
# other dependencies on other files.

or8.o: or8.s
	$(CC) $(CFLAGS) -c or8.s

to_goal.o: to_goal.s
	$(CC) $(CFLAGS) -c to_goal.s

overwrite.o: overwrite.s
	$(CC) $(CFLAGS) -c overwrite.s

call_or8.o: call_or8.s
	$(CC) $(CFLAGS) -c call_or8.s

call_to_goal.o: call_to_goal.s
	$(CC) $(CFLAGS) -c call_to_goal.s

call_overwrite.o: call_overwrite.s
	$(CC) $(CFLAGS) -c call_overwrite.s

rec_fib.o: rec_fib.s
	$(CC) $(CFLAGS) -c rec_fib.s
