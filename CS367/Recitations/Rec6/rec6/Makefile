# comments are always a great idea.

# we use this variable as $(CC) when building commands.
CC=gcc

# we keep using these flags in combination, so they're
# stuffed into a variable. Saves us from having to 
# re-type them repeatedly, and of course we can then
# change it in one place for all uses.
CFLAGS=-O0 -std=c99

FUNCS=mul_24.o average.o diff_sq.o set_bit.o set_bit_param.o set_bit_val.o func1.o func2.o func3.o 

# see below for the rec6 target.
all: rec6

# the command 'make clean' should remove all generated files.
clean:
	-rm -rf *~ *# *.o rec6.s rec6.o rec6

# target rec6 builds the overall executable, rec6.
# depends on all the individual functions' object files.
rec6: rec6.o  $(FUNCS)
	$(CC) $(CFLAGS) -o rec6 rec6.o $(FUNCS)

# compiles rec6.c into rec6.s; needs no other files.
rec6.s: rec6.c
	$(CC) $(CFLAGS) -S rec6.c

# compiles rec6.s into rec6.o; needs no other files.
rec6.o: rec6.s
	$(CC) $(CFLAGS) -c rec6.s

# each of the remaining targets also compiles each
# .s file into the corresponding .o file, with no
# other dependencies on other files.

mul_24.o: mul_24.s
	$(CC) $(CFLAGS) -c mul_24.s

average.o: average.s
	$(CC) $(CFLAGS) -c average.s

diff_sq.o: diff_sq.s
	$(CC) $(CFLAGS) -c diff_sq.s

set_bit.o: set_bit.s
	$(CC) $(CFLAGS) -c set_bit.s

set_bit_param.o: set_bit_param.s
	$(CC) $(CFLAGS) -c set_bit_param.s

set_bit_val.o: set_bit_val.s
	$(CC) $(CFLAGS) -c set_bit_val.s


# our functions from recitation 5 are also available!

func1.o: func1.s
	$(CC) $(CFLAGS) -c func1.s

func2.o: func2.s
	$(CC) $(CFLAGS) -c func2.s

func3.o: func3.s
	$(CC) $(CFLAGS) -c func3.s




