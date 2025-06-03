##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

################################### Global ###################################

# Source folder
SRCDIR = ./src/

# Headers folder
INCLUDES = ./include/

SRC_INCLUDE = ./src/

# GCC Flags
ERROR = -Werror -Wall -Wextra -Wshadow

all: zappy_server zappy_gui zappy_ai

DEP	=	$(SRC_SERVER:.c=.d)	\
		$(SRC_GUI:.cpp=.d)	\
		$(SRC_AI:.c=.d)		\

-include $(DEP)

clean:
	rm -f $(OBJ_SERVER)
	rm -f $(OBJ_GUI)
	rm -f $(OBJ_AI)
	rm -f $(TESTS_SRC:.c=.o)
	rm -f $(DEP)
	rm -f *.gcno
	rm -f *.gcda

fclean: clean
	rm -f $(NAME_SERVER)
	rm -f $(NAME_GUI)
	rm -f $(NAME_AI)
	rm -f $(TESTS_NAME)

re: fclean all

re_clean: fclean all clean

.PHONY: all clean \
	fclean re \
	tests_run unit_tests coverage

################################### Server ###################################

# Executable name
NAME_SERVER = zappy_server

# Folder name
SRCDIR_SERVER = ${SRCDIR}server/

# Sources
SRC_SERVER = $(SRCDIR_SERVER)main.c	\

# Objects
OBJ_SERVER = $(SRC_SERVER:.c=.o)
$(SRCDIR_SERVER)%.o: $(SRCDIR_SERVER)%.c
	$(CC_SERVER) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_SERVER)

# Compilation Flags
CFLAGS_SERVER += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g	\

# Pre Compilation
CC_SERVER := gcc

# Rule
zappy_server: $(OBJ_SERVER)
	$(CC_SERVER) -o $(NAME_SERVER) $(OBJ_SERVER) $(CFLAGS_SERVER)

#################################### GUI ####################################

# Executable name
NAME_GUI = zappy_gui

# Folder name
SRCDIR_GUI = ${SRCDIR}gui/

# Sources
SRC_GUI = $(SRCDIR_GUI)main.cpp	\

# Objects
OBJ_GUI = $(SRC_GUI:.cpp=.o)
$(SRCDIR_GUI)%.o: $(SRCDIR_GUI)%.cpp
	$(CC_GUI) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_GUI)

# Compilation Flags
CFLAGS_GUI += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g	\
			-std=c++23

# Pre Compilation
CC_GUI := g++

# Rule
zappy_gui: $(OBJ_GUI)
	$(CC_GUI) -o $(NAME_GUI) $(OBJ_GUI) $(CFLAGS_GUI)

##################################### AI #####################################

# Executable name
NAME_AI = zappy_ai

# Folder name
SRCDIR_AI = ${SRCDIR}ai/

# Sources
SRC_AI = $(SRCDIR_AI)main.c	\

# Objects
OBJ_AI = $(SRC_AI:.c=.o)
$(SRCDIR_AI)%.o: $(SRCDIR_AI)%.c
	$(CC_AI) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_AI)

# Compilation Flags
CFLAGS_AI += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g	\

# Pre Compilation
CC_AI := gcc

# Rule
zappy_ai: $(OBJ_AI)
	$(CC_AI) -o $(NAME_AI) $(OBJ_AI) $(CFLAGS_AI)

################################### Tests ###################################

TESTS_NAME = unit_tests.out

# Folders name
TESTS = ./tests/

# Sources
TESTS_SRC =	\

# Test Compilation Flags
UNIT_FLAGS = $(FLAGS) -lcriterion --coverage

# Compilation Flags
CFLAGS_TESTS += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g	\

# Pre Compilation
CC_TESTS := gcc

$(TESTS)%.o: $(TESTS)%.cpp
	$(CC_TESTS) -c $< -o $@ $(CFLAGS_TESTS)

unit_tests: $(TESTS_SRC:.cpp=.o)
	$(CC_TESTS) -o $(TESTS_NAME) $(TESTS_SRC:.cpp=.o)	\
		$(UNIT_FLAGS)

tests_run: unit_tests
	./$(TESTS_NAME) --verbose

coverage: tests_run
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch
