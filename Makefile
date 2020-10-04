# Makefile
# Used to compile and build the project
# run "make help" for command help

# project name (url friendly)
PROJ = chell

# compiler type to be used. gcc for C and g++ for CPP
CC = gcc

# list of all .c/.cpp files to be compiled (space separated, without extension)
FILES = chell chellin

#--------(DO NOT EDIT BELOW)--------#

# Generated variables
ifeq ($(CC), gcc)
	EXT = c
else
	EXT = cpp
endif
SOURCE = $(foreach var, $(FILES), $(var).$(EXT) )
COMPILED_SOURCE = $(foreach var, $(FILES), compiled/$(var).o )

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
	@./$(PROJ)

build: $(PROJ)

compiled/%.o: %.$(EXT)
	@if [ ! -d "compiled" ]; then mkdir compiled; fi
	@echo Compiling $< ...
	@$(CC) -c $< -o $@
	@echo done compiling $<
	
$(PROJ): $(COMPILED_SOURCE)
	@echo Building $@ ...
	@$(CC) $(COMPILED_SOURCE) -o $(PROJ)
	@echo Build completed successfully
	@echo execute ./$(PROJ) to run the program

clean:
	@for file in $(COMPILED_SOURCE) $(PROJ); do \
		if [ -f $$file ]; then \
			echo deleting $$file; \
			rm $$file; \
		fi \
	done
	@echo Done