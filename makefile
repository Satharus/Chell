CFLAGS   = -Wall
LDFLAGS  =
PROG = chell
CXX = gcc

FILES = chell.c chad-readline.c chad-history.c commands.c
SOURCE_FILES = $(foreach file, $(FILES), src/$(file))

HFILES = defs.h chad-history.h  chad-readline.h  chell.h  commands.h
HEADER_FILES = $(foreach file, $(HFILES), src/$(file))


# top-level rule to create the program.
all: $(PROG)

# compile chell  
$(PROG): $(SOURCE_FILES) $(HEADER_FILES)
	@if [ ! -d "bin" ]; then mkdir bin; fi
	$(CXX) $(CFLAGS) $(SOURCE_FILES) -o bin/$(PROG) $(LDFLAGS)

# cleaning everything that can be automatically recreated with "make"
clean:
	rm -r obj bin

run: 
	@echo ----- running $(PROG) -----
	@./bin/$(PROG)
