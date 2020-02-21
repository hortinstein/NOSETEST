SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include
DIRS   = $(SRCDIR) $(OBJDIR) $(BINDIR) $(INDDIR)


GOOGLE_TEST_LIB = gtest
GOOGLE_TEST_INCLUDE = /usr/local/include

# name of executable
EXEC = ll
TEST_EXEC = tests

# all the cource code pregenerated as a string and not just the string `*.c`
SRC = $(wildcard $(SRCDIR)/*.c*)
FILES = $(notdir $(SRC))
# loses the `.c` suffix
tmp = $(basename $(FILES))
# all the object files we will need
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(tmp)))

# gnu c compiler
CC = g++

# `Wall`    - warns about questionable things
# `Werror`  - makes all warnings errors
# `Wextra`  - enables some extra warning flags that `all` doesn't set
# `Wunused` - complains about any variable, function, label, etc. not being used
CFLAGS = -Wall -Werror -Wextra -Wunused -Wno-pointer-arith 
# `g`           - generate source code debug info
# `std=`        - sets the language standard, in this case c99
# `_GNU_SOURCE` - is a macro that tells the compiler to use rsome gnu functions
# `pthred`      - adds support for multithreading with the pthreads lib (for preprocessor
#                 and linker)
# `O3`          - the level of optimization
CFLAGS += -g -D_GNU_SOURCE -pthread -O3 -I $(GOOGLE_TEST_INCLUDE)
# `-I` - adds directory to the system search path (for include files)
CFLAGS += -I"$(INCDIR)" 

# designates which rules aren't actually targets
.PHONY: all o exec test clean clean_obj clean_ll clean_very

all: $(OBJ) $(EXEC) $(TEST_EXEC)

# for only compiling object voce
o: $(OBJ)

# for only compiling executable binary
exec: $(EXEC)

# for only compiling executable binary
test_exec: $(TEST_EXEC)

# combiles the object files necessary for linking
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	@echo building object files...
	$(CC) $(CFLAGS) -o $@ -c $<

# combiles the object files necessary for linking
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(OBJDIR)
	@echo building object files...
	$(CC) $(CFLAGS) -o $@ -c $<

# makes sure bin/ is created and the builds the proper binary name
$(EXEC): %: $(BINDIR) $(BINDIR)/%

# had to do this so it wouldn't recompile each time
$(BINDIR)/%: $(SRCDIR)/%.c
	@echo building binary...
	$(CC) $(CFLAGS) -DLL -o $@ $<

###############
### TESTS
###############

# makes sure bin/ is created and the builds the proper binary name
$(TEST_EXEC): %: $(BINDIR) $(BINDIR)/%

# had to do this so it wouldn't recompile each time
$(BINDIR)/%: $(SRCDIR)/%.cpp
	@echo building test binary...
	$(CC) $(CFLAGS) -o $@ $< -L /usr/local/lib -l $(GOOGLE_TEST_LIB) 

test: $(EXEC)
	@echo running tests...
	@$(BINDIR)/$(EXEC)

###############
### CLEANS
###############

$(DIRS):
	@mkdir -p $@

# cleans everything up when done
clean: clean_obj clean_ll
# *.dSYM directories made by clang on darwin
	@rm -rf $(BINDIR)/*.dSYM
# removes the object files. useful at the end of all
clean_obj:
	@echo removing object files...
	@rm -rf $(OBJ)
clean_ll:
	@echo removing binary...
	@rm -f $(BINDIR)/$(EXEC)
clean_very:
	@echo removing binary directory...
	@rm -rf $(BINDIR)
	@echo removing object directory...
	@rm -rf $(OBJDIR)
