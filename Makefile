PROJECT=compilator
FILES=main.o lexiA.o symtable.o test_lexical.o buffer.o token_buffer.o parser.o semantic.o param_buffer.o initialization.o exp.o stack_for_exp.o symtable_stack.o adjust_constant.o stack_of_int.o builtin_functions.o
CFLAGS=-g
CC=gcc

# Make all command
all: clean $(PROJECT)

combo: clean $(PROJECT) run

# Make project
$(PROJECT): $(FILES)
	$(CC) $(CFLAGS) -o $(PROJECT) $(FILES) -lm

# Run project
run:
	./$(PROJECT)

# Clean project
clean:
	rm -f *.o *.out $(PROJECT)