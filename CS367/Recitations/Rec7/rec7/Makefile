# comments are always a great idea.

# we use this variable as $(CC) when building commands.
CC=gcc

# we keep using these flags in combination, so they're
# stuffed into a variable. Saves us from having to 
# re-type them repeatedly, and of course we can then
# change it in one place for all uses.
CFLAGS=-O0 -std=c99

FUNC_OS=clamp.o bitLength.o sumOdds.o fib.o collatzLength.o 

# see below for the rec7 and switch targets.
all: rec7 switch

# the command 'make clean' should remove all generated files.
clean:
	-rm -rf *~ *# *.o rec7.s rec7.o rec7 switch.s switch.o switch

# target rec7 builds the overall executable, rec7.
# depends on all the individual functions' object files.
rec7: rec7.o  $(FUNC_OS)
	$(CC) $(CFLAGS) -o rec7 rec7.o $(FUNC_OS)

# compiles rec7.c into rec7.s; needs no other files.
rec7.s: rec7.c
	$(CC) $(CFLAGS) -S rec7.c

# compiles rec7.s into rec7.o; needs no other files.
rec7.o: rec7.s
	$(CC) $(CFLAGS) -c rec7.s

# each of the remaining targets also compiles each
# .s file into the corresponding .o file, with no
# other dependencies on other files.

clamp.o: clamp.s
	$(CC) $(CFLAGS) -c clamp.s

bitLength.o: bitLength.s
	$(CC) $(CFLAGS) -c bitLength.s

sumOdds.o: sumOdds.s
	$(CC) $(CFLAGS) -c sumOdds.s

fib.o: fib.s
	$(CC) $(CFLAGS) -c fib.s

collatzLength.o: collatzLength.s
	$(CC) $(CFLAGS) -c collatzLength.s


# target switch builds the switch executable.
switch: switch.o
	$(CC) $(CFLAGS) -o switch switch.o 

# compiles switch.c into switch.s; needs no other files.
switch.s: switch.c
	$(CC) $(CFLAGS) -S switch.c

# compiles switch.s into switch.o; needs no other files.
switch.o: switch.s
	$(CC) $(CFLAGS) -c switch.s





