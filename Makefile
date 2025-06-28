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
			 	$(SRCDIR_SERVER)options_parser/options/display_eggs.c		\
			 	$(SRCDIR_SERVER)connection/server.c							\
			 	$(SRCDIR_SERVER)connection/client.c							\
			 	$(SRCDIR_SERVER)connection/message_receiver.c				\
			 	$(SRCDIR_SERVER)connection/message_sender.c					\
			 	$(SRCDIR_SERVER)connection/writing_buffer.c					\
			 	$(SRCDIR_SERVER)connection/team_join.c						\
			 	$(SRCDIR_SERVER)connection/team_join_ai.c					\
			 	$(SRCDIR_SERVER)connection/team_join_gui.c					\
			 	$(SRCDIR_SERVER)connection/socket.c							\
			 	$(SRCDIR_SERVER)connection/connection_handler.c				\
			 	$(SRCDIR_SERVER)connection/signal_handler.c					\
			 	$(SRCDIR_SERVER)map/map.c									\
			 	$(SRCDIR_SERVER)map/coordinates.c							\
			 	$(SRCDIR_SERVER)map/player_management.c						\
			 	$(SRCDIR_SERVER)map/egg_management.c						\
			 	$(SRCDIR_SERVER)map/resources.c								\
			 	$(SRCDIR_SERVER)map/resource_names.c						\
			 	$(SRCDIR_SERVER)map/tile.c									\
			 	$(SRCDIR_SERVER)map/tile_utils.c							\
			 	$(SRCDIR_SERVER)team/player/player.c						\
			 	$(SRCDIR_SERVER)team/player/movement.c						\
				$(SRCDIR_SERVER)team/player/look.c							\
				$(SRCDIR_SERVER)team/player/inventory.c						\
				$(SRCDIR_SERVER)team/player/ejection.c						\
				$(SRCDIR_SERVER)team/player/broadcast.c						\
				$(SRCDIR_SERVER)game/incantation/check_incantation.c		\
				$(SRCDIR_SERVER)game/incantation/incantation_allocation.c	\
				$(SRCDIR_SERVER)game/incantation/update_incantation.c		\
			 	$(SRCDIR_SERVER)team/egg/egg.c								\
				$(SRCDIR_SERVER)team/egg/spawn.c							\
				$(SRCDIR_SERVER)team/team_allocation.c						\
			 	$(SRCDIR_SERVER)team/team_egg.c								\
			 	$(SRCDIR_SERVER)team/team_player.c							\
				$(SRCDIR_SERVER)utils/string/string.c						\
				$(SRCDIR_SERVER)utils/string/repeat_string.c				\
				$(SRCDIR_SERVER)game/game.c									\
				$(SRCDIR_SERVER)game/teams_util.c							\
				$(SRCDIR_SERVER)game/tick.c									\
				$(SRCDIR_SERVER)game/update_end.c							\
				$(SRCDIR_SERVER)command_handler/command_parser.c			\
				$(SRCDIR_SERVER)command_handler/command_factory.c			\
				$(SRCDIR_SERVER)command_handler/command_tokenizer.c			\
				$(SRCDIR_SERVER)command_handler/command_validator.c			\
				$(SRCDIR_SERVER)command_handler/command_executor.c			\
				$(SRCDIR_SERVER)command_handler/command_buffer.c			\
				$(SRCDIR_SERVER)commands/ai/forward.c						\
				$(SRCDIR_SERVER)commands/ai/left.c							\
				$(SRCDIR_SERVER)commands/ai/right.c							\
				$(SRCDIR_SERVER)commands/ai/look_cmd.c						\
				$(SRCDIR_SERVER)commands/ai/inventory_cmd.c					\
				$(SRCDIR_SERVER)commands/ai/connect_nbr.c					\
				$(SRCDIR_SERVER)commands/ai/fork.c							\
				$(SRCDIR_SERVER)commands/ai/take.c							\
				$(SRCDIR_SERVER)commands/ai/set.c							\
				$(SRCDIR_SERVER)commands/ai/broadcast_cmd.c					\
				$(SRCDIR_SERVER)commands/ai/eject.c							\
				$(SRCDIR_SERVER)commands/ai/incantation_cmd.c				\
				$(SRCDIR_SERVER)commands/gui/msz.c							\
				$(SRCDIR_SERVER)commands/gui/ppo.c							\
				$(SRCDIR_SERVER)commands/gui/pnw.c							\
				$(SRCDIR_SERVER)commands/gui/bct.c							\
				$(SRCDIR_SERVER)commands/gui/mct.c							\
				$(SRCDIR_SERVER)commands/gui/plv.c							\
				$(SRCDIR_SERVER)commands/gui/tna.c							\
				$(SRCDIR_SERVER)commands/gui/pdi.c							\
				$(SRCDIR_SERVER)commands/gui/pin.c							\
				$(SRCDIR_SERVER)commands/gui/enw.c							\
				$(SRCDIR_SERVER)commands/gui/ebo.c							\
				$(SRCDIR_SERVER)commands/gui/pfk.c							\
				$(SRCDIR_SERVER)commands/gui/pdr.c							\
				$(SRCDIR_SERVER)commands/gui/pgt.c							\
				$(SRCDIR_SERVER)commands/gui/sgt.c							\
				$(SRCDIR_SERVER)commands/gui/sst.c							\
				$(SRCDIR_SERVER)commands/gui/edi.c							\
				$(SRCDIR_SERVER)commands/gui/pex.c							\
				$(SRCDIR_SERVER)commands/gui/pbc.c							\
				$(SRCDIR_SERVER)commands/gui/pic.c							\
				$(SRCDIR_SERVER)commands/gui/pie.c							\
				$(SRCDIR_SERVER)commands/gui/seg.c							\

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


NET_GUI	=	$(SRCDIR_GUI)network/
POSIX	=	$(NET_GUI)POSIX_wrapper/

COMMAND_GUI = $(SRCDIR_GUI)command/
COMMANDS_GUI = $(COMMAND_GUI)commands/

# Sources
SRC_GUI = $(SRCDIR_GUI)main.cpp	\
		$(SRCDIR_GUI)Parsing.cpp	\
		$(SRCDIR_GUI)ZappyApp.cpp	\
		$(SRCDIR_GUI)scenne/Scenne.cpp \
		$(SRCDIR_GUI)GameManager.cpp \
		$(SRCDIR_GUI)entity/Egg.cpp \
		$(SRCDIR_GUI)entity/Entity.cpp \
		$(SRCDIR_GUI)entity/Resources.cpp \
		$(SRCDIR_GUI)entity/Player.cpp \
		$(SRCDIR_GUI)entity/TileContentManager.cpp \
		$(SRCDIR_GUI)entity/TileDisplay.cpp	\
		$(SRCDIR_GUI)Team.cpp	\
		$(NET_GUI)NetworkWorker.cpp	\
		$(NET_GUI)NetworkManager.cpp	\
		$(NET_GUI)message/MessageProtocol.cpp	\
		$(POSIX)SocketTransport.cpp	\
		$(POSIX)connection/ReconnectionService.cpp	\
		$(POSIX)connection/ConnectionManager.cpp	\
		$(COMMAND_GUI)CommandHandler.cpp	\
		$(COMMANDS_GUI)BoardContentCommand.cpp	\
		$(COMMANDS_GUI)EggBornCommand.cpp	\
		$(COMMANDS_GUI)EggDieCommand.cpp	\
		$(COMMANDS_GUI)EggNewCommand.cpp	\
		$(COMMANDS_GUI)MapSizeCommand.cpp	\
		$(COMMANDS_GUI)PlayerBroadcastCommand.cpp	\
		$(COMMANDS_GUI)PlayerDieCommand.cpp	\
		$(COMMANDS_GUI)PlayerDropCommand.cpp	\
		$(COMMANDS_GUI)PlayerExpelledCommand.cpp	\
		$(COMMANDS_GUI)PlayerForkCommand.cpp	\
		$(COMMANDS_GUI)PlayerGetCommand.cpp	\
		$(COMMANDS_GUI)PlayerIncantationCommand.cpp	\
		$(COMMANDS_GUI)PlayerIncantationEndCommand.cpp	\
		$(COMMANDS_GUI)PlayerInventoryCommand.cpp	\
		$(COMMANDS_GUI)PlayerLevelCommand.cpp	\
		$(COMMANDS_GUI)PlayerNewCommand.cpp	\
		$(COMMANDS_GUI)PlayerPositionCommand.cpp	\
		$(COMMANDS_GUI)ServerBadParamCommand.cpp	\
		$(COMMANDS_GUI)ServerEndGameCommand.cpp	\
		$(COMMANDS_GUI)ServerGetTimeCommand.cpp	\
		$(COMMANDS_GUI)ServerMessageCommand.cpp	\
		$(COMMANDS_GUI)ServerSetTimeCommand.cpp	\
		$(COMMANDS_GUI)ServerUnknownCommand.cpp	\
		$(COMMANDS_GUI)TeamNamesCommand.cpp	\

# Objects
OBJ_GUI = $(SRC_GUI:.cpp=.o)
$(SRCDIR_GUI)%.o: $(SRCDIR_GUI)%.cpp
	$(CC_GUI) -c $< -o $@ -MMD -MF $(@:.o=.d) -MT $@ $(CFLAGS_GUI)

# OGRE specific flags
OGRE_INCLUDE = -I/usr/local/include/OGRE -I/usr/local/include/OGRE/Bites 	\
-I/usr/local/include/OGRE/RTShaderSystem
OGRE_LIBS = -L/usr/local/lib -lOgreMain -lOgreBites -lOgreRTShaderSystem

# Compilation Flags
CFLAGS_GUI += $(ERROR) -I$(INCLUDES) -I$(SRC_INCLUDE) -g \
			-std=c++17 $(OGRE_INCLUDE)

# Pre Compilation
CC_GUI := g++

# Rule
zappy_gui: $(OBJ_GUI)
	$(CC_GUI) -o $(NAME_GUI) $(OBJ_GUI) $(CFLAGS_GUI) $(OGRE_LIBS)

##################################### AI #####################################

# Executable name
NAME_AI = zappy_ai

# Folder name
SRCDIR_AI = ${SRCDIR}ai/refactored/

# Headers folder
INCLUDES_AI = ${INCLUDES}ai/

# Sources
SRC_AI = $(SRCDIR_AI)__main__.py	\
		$(SRCDIR_AI)client.py	\
		$(SRCDIR_AI)network.py	\
		$(SRCDIR_AI)utils.py	\
		$(SRCDIR_AI)vision.py

# Objects
OBJ_AI =

# Compilation Flags
CFLAGS_AI +=

# Pre Compilation
CC_AI := python3

# Rule
zappy_ai:
	@echo "#!/bin/bash" > $(NAME_AI)
	@echo "$(CC_AI) $(SRCDIR_AI)__main__.py \$$@" >> $(NAME_AI)
	@chmod +x $(NAME_AI)

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
			$(SRCDIR_SERVER)map/resource_names.c							\
			$(SRCDIR_SERVER)map/tile.c										\
			$(SRCDIR_SERVER)map/tile_utils.c								\
			$(SRCDIR_SERVER)team/player/player.c							\
			$(SRCDIR_SERVER)team/player/movement.c							\
			$(SRCDIR_SERVER)team/player/look.c								\
			$(SRCDIR_SERVER)team/player/inventory.c							\
			$(SRCDIR_SERVER)team/player/ejection.c							\
			$(SRCDIR_SERVER)team/player/broadcast.c							\
			$(SRCDIR_SERVER)game/incantation/check_incantation.c			\
			$(SRCDIR_SERVER)game/incantation/incantation_allocation.c		\
			$(SRCDIR_SERVER)game/incantation/update_incantation.c			\
			$(SRCDIR_SERVER)team/egg/egg.c									\
			$(SRCDIR_SERVER)team/egg/spawn.c								\
			$(SRCDIR_SERVER)team/team_allocation.c							\
			$(SRCDIR_SERVER)team/team_egg.c									\
			$(SRCDIR_SERVER)team/team_player.c								\
			$(SRCDIR_SERVER)constants.c										\
			$(SRCDIR_SERVER)connection/server.c								\
			$(SRCDIR_SERVER)connection/client.c								\
			$(SRCDIR_SERVER)connection/message_receiver.c					\
			$(SRCDIR_SERVER)connection/message_sender.c						\
			$(SRCDIR_SERVER)connection/writing_buffer.c						\
			$(SRCDIR_SERVER)connection/socket.c								\
			$(SRCDIR_SERVER)connection/connection_handler.c					\
			$(SRCDIR_SERVER)connection/signal_handler.c						\
			$(SRCDIR_SERVER)connection/team_join.c							\
			$(SRCDIR_SERVER)connection/team_join_ai.c						\
			$(SRCDIR_SERVER)connection/team_join_gui.c						\
			$(SRCDIR_SERVER)options_parser/options.c						\
			$(SRCDIR_SERVER)options_parser/parser.c							\
			$(SRCDIR_SERVER)options_parser/processor.c						\
			$(SRCDIR_SERVER)options_parser/options/port.c					\
			$(SRCDIR_SERVER)options_parser/options/width.c					\
			$(SRCDIR_SERVER)options_parser/options/height.c					\
			$(SRCDIR_SERVER)options_parser/options/teams.c					\
			$(SRCDIR_SERVER)options_parser/options/clients.c				\
			$(SRCDIR_SERVER)options_parser/options/frequency.c				\
			$(SRCDIR_SERVER)options_parser/options/help.c					\
			$(SRCDIR_SERVER)options_parser/options/debug.c					\
			$(SRCDIR_SERVER)options_parser/options/display_eggs.c			\
			$(SRCDIR_SERVER)command_handler/command_executor.c				\
			$(SRCDIR_SERVER)game/game.c										\
			$(SRCDIR_SERVER)game/tick.c										\
			$(SRCDIR_SERVER)game/teams_util.c								\
			$(SRCDIR_SERVER)game/update_end.c								\
			$(SRCDIR_SERVER)commands/ai/forward.c							\
			$(SRCDIR_SERVER)commands/ai/left.c								\
			$(SRCDIR_SERVER)commands/ai/right.c								\
			$(SRCDIR_SERVER)commands/ai/look_cmd.c							\
			$(SRCDIR_SERVER)commands/ai/inventory_cmd.c						\
			$(SRCDIR_SERVER)commands/ai/connect_nbr.c						\
			$(SRCDIR_SERVER)commands/ai/fork.c								\
			$(SRCDIR_SERVER)commands/ai/take.c								\
			$(SRCDIR_SERVER)commands/ai/set.c								\
			$(SRCDIR_SERVER)commands/ai/broadcast_cmd.c						\
			$(SRCDIR_SERVER)commands/ai/eject.c								\
			$(SRCDIR_SERVER)commands/ai/incantation_cmd.c					\
			$(SRCDIR_SERVER)commands/gui/msz.c								\
			$(SRCDIR_SERVER)commands/gui/ppo.c								\
			$(SRCDIR_SERVER)commands/gui/pnw.c								\
			$(SRCDIR_SERVER)commands/gui/bct.c								\
			$(SRCDIR_SERVER)commands/gui/mct.c								\
			$(SRCDIR_SERVER)commands/gui/plv.c								\
			$(SRCDIR_SERVER)commands/gui/tna.c								\
			$(SRCDIR_SERVER)commands/gui/pdi.c								\
			$(SRCDIR_SERVER)commands/gui/pin.c								\
			$(SRCDIR_SERVER)commands/gui/enw.c								\
			$(SRCDIR_SERVER)commands/gui/ebo.c								\
			$(SRCDIR_SERVER)commands/gui/pfk.c								\
			$(SRCDIR_SERVER)commands/gui/pdr.c								\
			$(SRCDIR_SERVER)commands/gui/pgt.c								\
			$(SRCDIR_SERVER)commands/gui/sgt.c								\
			$(SRCDIR_SERVER)commands/gui/sst.c								\
			$(SRCDIR_SERVER)commands/gui/edi.c								\
			$(SRCDIR_SERVER)commands/gui/pex.c								\
			$(SRCDIR_SERVER)commands/gui/pbc.c								\
			$(SRCDIR_SERVER)commands/gui/pic.c								\
			$(SRCDIR_SERVER)commands/gui/pie.c								\
			$(SRCDIR_SERVER)commands/gui/seg.c								\
			${TESTS}player_tests.c											\
			${TESTS}resources_tests.c										\
			${TESTS}map_tests.c												\
			${TESTS}egg_tests.c												\
			${TESTS}team_tests.c											\
			${TESTS}look_tests.c											\
			$(SRCDIR_SERVER)utils/string/string.c							\
			$(SRCDIR_SERVER)utils/string/repeat_string.c					\
			$(SRCDIR_SERVER)command_handler/command_parser.c				\
			$(SRCDIR_SERVER)command_handler/command_factory.c				\
			$(SRCDIR_SERVER)command_handler/command_tokenizer.c				\
			$(SRCDIR_SERVER)command_handler/command_validator.c				\
			$(SRCDIR_SERVER)command_handler/command_buffer.c				\
			${TESTS}command_parser_tests.c									\
            ${TESTS}command_tokenizer_tests.c								\
            ${TESTS}command_validator_tests.c								\
            ${TESTS}command_factory_tests.c									\
            ${TESTS}command_buffer_tests.c									\
            ${TESTS}command_executor_tests.c								\
            ${TESTS}command_parser_edge_tests.c								\
            ${TESTS}command_tokenizer_edge_tests.c							\
			${TESTS}debug_tests.c											\
			${TESTS}string_tests.c											\
			${TESTS}client_tests.c											\
			${TESTS}options_parser/integration_tests.c						\
			${TESTS}options_parser/parser_tests.c							\
			${TESTS}options_parser/processor_tests.c						\
			${TESTS}options_parser/port_option_tests.c						\
			${TESTS}options_parser/width_option_tests.c						\
			${TESTS}options_parser/height_option_tests.c					\
			${TESTS}options_parser/frequency_option_tests.c					\
			${TESTS}options_parser/clients_option_tests.c					\
			${TESTS}options_parser/teams_option_tests.c						\
			${TESTS}options_parser/help_option_tests.c						\
			${TESTS}options_parser/debug_option_tests.c						\
			${TESTS}options_parser/display_eggs_option_tests.c				\
			${TESTS}check_incantation_tests.c								\
			${TESTS}incantation_allocation_tests.c							\
			${TESTS}update_incantation_tests.c								\
			${TESTS}broadcast_tests.c										\
			${TESTS}tick_tests.c											\
			${TESTS}commands/ai/forward_tests.c								\
			${TESTS}commands/ai/left_tests.c								\
			${TESTS}commands/ai/right_tests.c								\
			${TESTS}commands/ai/inventory_cmd_tests.c						\
			${TESTS}commands/ai/look_cmd_tests.c							\
			${TESTS}commands/ai/take_tests.c								\
			${TESTS}commands/ai/set_tests.c									\
			${TESTS}commands/ai/eject_tests.c								\
			${TESTS}commands/ai/broadcast_cmd_tests.c						\
			${TESTS}commands/ai/connect_nbr_tests.c							\
			${TESTS}commands/ai/fork_tests.c								\
			${TESTS}commands/ai/incantation_cmd_tests.c						\
			${TESTS}commands/gui/edi_tests.c								\
			${TESTS}commands/gui/bct_tests.c								\
			${TESTS}commands/gui/ebo_tests.c								\
			${TESTS}commands/gui/msz_tests.c								\
			${TESTS}commands/gui/pdi_tests.c								\
			${TESTS}commands/gui/mct_tests.c								\
			${TESTS}commands/gui/pbc_tests.c								\
			${TESTS}commands/gui/pdr_tests.c								\
			${TESTS}commands/gui/pex_tests.c								\
			${TESTS}commands/gui/pfk_tests.c								\
			${TESTS}commands/gui/pgt_tests.c								\
			${TESTS}commands/gui/pic_tests.c								\
			${TESTS}commands/gui/pie_tests.c								\
			${TESTS}commands/gui/pin_tests.c								\
			${TESTS}commands/gui/plv_tests.c								\
			${TESTS}commands/gui/ppo_tests.c								\
			${TESTS}commands/gui/seg_tests.c								\
			${TESTS}commands/gui/sgt_tests.c								\
			${TESTS}commands/gui/sst_tests.c								\
			${TESTS}commands/gui/tna_tests.c								\
			${TESTS}commands/gui/pnw_tests.c								\

# Test Compilation Flags
UNIT_FLAGS = $(CFLAGS_SERVER) -L$(LIB_SERVER)	\
			-lvector -lcriterion --coverage -g

# Pre Compilation
CC_TESTS := gcc

unit_tests: $(LIB_SERVER)libvector.a
	$(CC_TESTS) -o $(TESTS_NAME) $(TESTS_SRC) $(UNIT_FLAGS)

tests_run: unit_tests
	./$(TESTS_NAME)

coverage: tests_run
	gcovr --exclude tests/
	gcovr --exclude tests/ --txt-metric branch

cs:	clean
	@coding-style . .
	@cat coding-style-reports.log
	@rm -f coding-style-reports.log
