# Makefile
# Used to compile and build the project
# run "make help" for command help

# project name (url friendly)
PROJ = chell

# compiler type to be used. gcc for C and g++ for CPP
CC = gcc

# list of all .c/.cpp files to be compiled (space separated, without extension)
FILES = chell chad-readline commands chad-history


#--------(DO NOT EDIT BELOW)--------#

# Generated variables
ifeq ($(CC), gcc)
	EXT = c
else
	EXT = cpp
endif
SOURCE = $(foreach var, $(FILES), src/$(var).$(EXT) )
COMPILED_SOURCE = $(foreach var, $(FILES), obj/$(var).o )


all: $(PROJ)

# Defined functions
help:
	@echo Usage: make [OPTION]
	@echo 
	@echo -e 'Option \t\t Meaning'
	@echo
	@echo -e 'help \t\t Opens this help menu'
	@echo -e 'run \t\t Builds the complete project and runs it'
	@echo -e 'build \t\t Compiles all the files and builds the project'
	@echo -e '$(PROJ) \t\t Same as build'
	@echo -e '[filename].o \t Compiles .$(EXT) source for that file'
	@echo -e 'clean \t\t cleans up all the compiled files and final project build'

run: $(PROJ)
	@echo ----- running $(PROJ) -----
	@./bin/$(PROJ)


build: $(PROJ)

obj/%.o: src/%.$(EXT)
	@if [ ! -d "obj" ]; then mkdir obj; fi
	@echo Compiling $< ...
	@$(CC) -c $< -o $@
	@echo done compiling $<
	
$(PROJ): $(COMPILED_SOURCE)
	@if [ ! -d "bin" ]; then mkdir bin; fi
	@echo Building $@ ...
	@$(CC) $(COMPILED_SOURCE) -o bin/$(PROJ)
	@echo Build completed successfully
	@echo execute ./bin/$(PROJ) or make run to run the program

clean:
	@for file in $(COMPILED_SOURCE) $(PROJ); do \
		if [ -f $$file ]; then \
			echo deleting $$file; \
			rm $$file; \
		fi \
	done; \
        [ -d obj ] && rm -d obj; \
        [ -d bin ] && rm -r bin;
	@echo Done
