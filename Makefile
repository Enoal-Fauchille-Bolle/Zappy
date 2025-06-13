##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Makefile
##

################################### Global ###################################

# Source folder
SRCDIR = ./src/

SRC_INCLUDE = ./src/

# Headers folder
INCLUDES = ./include/

# GCC Flags
ERROR = -Werror -Wall -Wextra -Wshadow

all: zappy_server zappy_gui zappy_ai

DEP	=	$(SRC_SERVER:.c=.d)	\
		$(SRC_GUI:.cpp=.d)	\
		$(SRC_AI:.c=.d)		\

-include $(DEP)

clean:
	find . -name "*.gcda" -delete
	find . -name "*.gcno" -delete
	find . -name "*.a" -delete
	find . -name "*.o" -delete
	find . -name "*.d" -delete

fclean: clean
	rm -f $(NAME_SERVER)
	rm -f $(NAME_GUI)
	rm -f $(NAME_AI)
	rm -f $(TESTS_NAME)

re: fclean all

re_clean: fclean all clean

.PHONY: all clean \
	fclean re \
	tests_run unit_tests coverage cs

################################### Server ###################################

# Executable name
NAME_SERVER = zappy_server

# Folder name
SRCDIR_SERVER = $(SRCDIR)server/
LIB_SERVER	=	$(SRCDIR_SERVER)utils/

# Headers folder
INCLUDES_SERVER = $(INCLUDES)server/

# Sources
SRC_SERVER =	$(SRCDIR_SERVER)main.c										\
			 	$(SRCDIR_SERVER)constants.c									\
			 	$(SRCDIR_SERVER)options_parser/options.c					\
			 	$(SRCDIR_SERVER)options_parser/parser.c						\
			 	$(SRCDIR_SERVER)options_parser/processor.c					\
			 	$(SRCDIR_SERVER)options_parser/options/port.c				\
			 	$(SRCDIR_SERVER)options_parser/options/width.c				\
			 	$(SRCDIR_SERVER)options_parser/options/height.c				\
			 	$(SRCDIR_SERVER)options_parser/options/teams.c				\
			 	$(SRCDIR_SERVER)options_parser/options/clients.c			\
			 	$(SRCDIR_SERVER)options_parser/options/frequency.c			\
			 	$(SRCDIR_SERVER)options_parser/options/help.c				\
			 	$(SRCDIR_SERVER)options_parser/options/debug.c				\
			 	$(SRCDIR_SERVER)connection/server.c							\
			 	$(SRCDIR_SERVER)connection/client.c							\
			 	$(SRCDIR_SERVER)connection/socket.c							\
			 	$(SRCDIR_SERVER)connection/connection_handler.c				\
			 	$(SRCDIR_SERVER)map/map.c									\
			 	$(SRCDIR_SERVER)map/coordinates.c							\
			 	$(SRCDIR_SERVER)map/player_management.c						\
			 	$(SRCDIR_SERVER)map/egg_management.c						\
			 	$(SRCDIR_SERVER)map/resources.c								\
			 	$(SRCDIR_SERVER)map/tile.c									\
			 	$(SRCDIR_SERVER)team/player/player.c						\
			 	$(SRCDIR_SERVER)team/player/movement.c						\
			 	$(SRCDIR_SERVER)team/egg/egg.c								\
				$(SRCDIR_SERVER)team/egg/spawn.c							\
				$(SRCDIR_SERVER)team/team_allocation.c						\
			 	$(SRCDIR_SERVER)team/team_egg.c								\
			 	$(SRCDIR_SERVER)team/team_player.c							\

# Objects
OBJ_SERVER = $(SRC_SERVER:.c=.o)
$(SRCDIR_SERVER)%.o: $(SRCDIR_SERVER)%.c
	$(CC_SERVER) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_SERVER)

# Compilation Flags
CFLAGS_SERVER += $(ERROR) -I$(INCLUDES_SERVER) -I$(SRC_INCLUDE) -g	\

# Pre Compilation
CC_SERVER := gcc

# Rule

$(LIB_SERVER)libvector.a:
		make -C $(LIB_SERVER)

libclean: fclean
		make -C $(LIB_SERVER) fclean

zappy_server: $(OBJ_SERVER) $(LIB_SERVER)libvector.a
	$(CC_SERVER) -o $(NAME_SERVER) $(OBJ_SERVER) $(CFLAGS_SERVER)	\
	-L$(LIB_SERVER) -lvector

#################################### GUI ####################################

# Executable name
NAME_GUI = zappy_gui

# Folder name
SRCDIR_GUI = ${SRCDIR}gui/

# Headers folder
INCLUDES_CLIENT = ${INCLUDES}client/

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

# Headers folder
INCLUDES_AI = ${INCLUDES}ai/

# Sources
SRC_AI = $(SRCDIR_AI)main.c	\

# Objects
OBJ_AI = $(SRC_AI:.c=.o)
$(SRCDIR_AI)%.o: $(SRCDIR_AI)%.c
	$(CC_AI) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_AI)

# Compilation Flags
CFLAGS_AI += $(ERROR) -I$(INCLUDES_AI) -I$(SRC_INCLUDE) -g	\

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
TESTS_SRC =	$(SRCDIR_SERVER)map/map.c										\
			$(SRCDIR_SERVER)map/coordinates.c								\
			$(SRCDIR_SERVER)map/player_management.c							\
			$(SRCDIR_SERVER)map/egg_management.c							\
			$(SRCDIR_SERVER)map/resources.c									\
			$(SRCDIR_SERVER)map/tile.c										\
			$(SRCDIR_SERVER)team/player/player.c							\
			$(SRCDIR_SERVER)team/player/movement.c							\
			$(SRCDIR_SERVER)team/egg/egg.c									\
			$(SRCDIR_SERVER)team/egg/spawn.c								\
			$(SRCDIR_SERVER)team/team_allocation.c							\
			$(SRCDIR_SERVER)team/team_egg.c									\
			$(SRCDIR_SERVER)team/team_player.c								\
			${TESTS}player_tests.c											\
			${TESTS}resources_tests.c										\
			${TESTS}map_tests.c												\
			${TESTS}egg_tests.c												\
			${TESTS}team_tests.c											\

# Test Compilation Flags
UNIT_FLAGS = $(CFLAGS_SERVER) -L$(LIB_SERVER)	\
			-lvector -lcriterion --coverage -g

# Pre Compilation
CC_TESTS := gcc

unit_tests: $(LIB_SERVER)libvector.a
	$(CC_TESTS) -o $(TESTS_NAME) $(TESTS_SRC) $(UNIT_FLAGS)

tests_run: unit_tests
	./$(TESTS_NAME) --verbose

coverage: tests_run
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch

cs:	clean
	@coding-style . .
	@cat coding-style-reports.log
	@rm -f coding-style-reports.log
