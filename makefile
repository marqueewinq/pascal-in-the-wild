# Makefile

OBJS	= bison.o lex.o main.o
NAME    = pas
CC	= g++
CFLAGS = -w -O2
#CFLAGS	= -Wall -Wextra -g
#FLEX_FLAGS = -d

all: clean compile run

vlg: $(NAME)
	valgrind ./$(NAME) input
	
run: $(NAME)
	./$(NAME) input

compile:		$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lfl
		@echo "[OK]"

lex.o:		lex.c
		$(CC) $(CFLAGS) -c lex.c -o lex.o

lex.c:		$(NAME).lex 
		flex $(FLEX_FLAGS) $(NAME).lex
		cp lex.yy.c lex.c

bison.o:	bison.c
		$(CC) $(CFLAGS) -c bison.c -o bison.o

bison.c:	$(NAME).y
		bison -d -v $(NAME).y
		cp $(NAME).tab.c bison.c
		cmp -s $(NAME).tab.h tok.h || cp $(NAME).tab.h tok.h

main.o:		$(NAME).cpp
		$(CC) $(CFLAGS) -c $(NAME).cpp -o main.o

lex.o yac.o main.o	: heading.h
lex.o main.o		: tok.h

clean:
	@rm -f *.o *~ lex.c lex.yy.c bison.c tok.h $(NAME).tab.c $(NAME).tab.h $(NAME).output

