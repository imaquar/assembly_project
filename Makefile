CC = gcc # Definition of the compiler for C
ASM = nasm # Definition of the assembler for the assembler
CFLAGS = -m32 -Wall -Wextra -std=c99 -no-pie -O3 -ffast-math # Flags for the compiler C
ASMFLAGS = -f elf32 -F dwarf -g # Flags for the assembler

all: integral # Default target when calling make without parameters

integral: main.o funcs.o # Rule for building the main program
	$(CC) $(CFLAGS) -o $@ $^ -lm

main.o: main.c # Rule for compiling main.c
	$(CC) $(CFLAGS) -c $< -o $@

funcs.o: funcs.asm # Rule for compiling funcs.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean: # Target for cleaning the project
	rm -f *.o integral

test: integral # Target for running tests
	./integral --test-root 1:2:1.8:2.2:0.0001:1.95615
	./integral --test-root 2:3:0.0:1.0:0.0001:0.18741
	./integral --test-root 1:3:-0.5:0.0:0.0001:-0.20333
	./integral --test-integral 1:-0.20333:1.95615:0.0001:4.92110
	./integral --test-integral 2:0.18741:1.95615:0.0001:2.18618
	./integral --test-integral 3:-0.20333:0.18741:0.0001:0.39633
	./integral --test-root 11:22:2.5:3.0:0.0001:2.70000
	./integral --test-root 22:33:1.5:2.0:0.0001:1.87939
	./integral --test-root 11:33:1.8:2.3:0.0001:2.10287
	./integral --test-integral 11:2.10287:2.70000:0.0001:5.49331
	./integral --test-integral 22:1.87939:2.70000:0.0001:6.45745
	./integral --test-integral 33:1.87939:2.10287:0.0001:1.76971

# Declare phony targets (targets that don't represent actual files)
.PHONY: all clean test 