# Cibles particulières
.PHONY: clean, mrproper

LEX=flex
LEX_FILE=lexer.l
LEX_CFILE=lex.yy.c
LEX_OFILE=lex.yy.o

SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o) $(LEX_OFILE)

CC=gcc
CFLAGS=-W -Wall -Wextra -Wuninitialized -Wwrite-strings -Wno-missing-braces -ansi -pedantic
CLIBS=

LDFLAGS=
LIBS=

EXEC=compil

all: CFLAGS += -O3
debug: CFLAGS += -g

# all
all : $(EXEC)
debug : $(EXEC)

$(EXEC) : $(OBJECTS)
	$(CC) $^ -o $(EXEC) $(LDFLAGS) $(LIBS)

$(LEX_OFILE) : $(LEX_CFILE)
	$(CC) -c $<

%.o : %.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(CLIBS)

$(LEX_CFILE) : $(LEX_FILE)
	$(LEX) $<

# clean 
clean: 
	rm -rf *.o

# mrproper
mrproper: clean
	rm -rf $(EXEC)
