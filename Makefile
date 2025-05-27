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
		$(SRC_CLIENT:.cpp=.d)	\
		$(SRC_AI:.c=.d)		\

-include $(DEP)

clean:
	rm -f $(OBJ_SERVER)
	rm -f $(OBJ_CLIENT)
	rm -f $(OBJ_AI)
	rm -f $(TESTS_SRC:.c=.o)
	rm -f $(DEP)
	rm -f *.gcno
	rm -f *.gcda

fclean: clean
	rm -f $(NAME_SERVER)
	rm -f $(NAME_CLIENT)
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

################################### Client ###################################

# Executable name
NAME_CLIENT = zappy_gui

# Folder name
SRCDIR_CLIENT = ${SRCDIR}client/

# Sources
SRC_CLIENT = $(SRCDIR_CLIENT)main.cpp	\

# Objects
OBJ_CLIENT = $(SRC_CLIENT:.cpp=.o)
$(SRCDIR_CLIENT)%.o: $(SRCDIR_CLIENT)%.cpp
	$(CC_CLIENT) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_CLIENT)

# Compilation Flags
CFLAGS_CLIENT += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g	\
			-std=c++23

# Pre Compilation
CC_CLIENT := g++

# Rule
zappy_gui: $(OBJ_CLIENT)
	$(CC_CLIENT) -o $(NAME_CLIENT) $(OBJ_CLIENT) $(CFLAGS_CLIENT)

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

unit_tests: $(OBJ_SERVER) $(TESTS_SRC:.cpp=.o)
	$(CC_TESTS) -o $(TESTS_NAME) $(OBJ_SERVER) $(TESTS_SRC:.cpp=.o)	\
		$(UNIT_FLAGS)

tests_run: unit_tests
	./$(TESTS_NAME) --verbose

coverage: tests_run
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch
