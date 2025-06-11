##
## EPITECH PROJECT, 2025
## Zappy_mirror
## File description:
## Makefile
##

# Library name
NAME	=	libvector.a

# Folders name
SRCDIR	=	./vector/
METHOD	=	$(SRCDIR)methods/

# Sources
SRC	=	\
		$(SRCDIR)vector.c					\
		$(METHOD)all.c						\
		$(METHOD)any.c						\
		$(METHOD)assign.c					\
		$(METHOD)back.c						\
		$(METHOD)capacity.c					\
		$(METHOD)clear.c					\
		$(METHOD)count.c					\
		$(METHOD)data.c						\
		$(METHOD)empty.c					\
		$(METHOD)erase.c					\
		$(METHOD)filter.c					\
		$(METHOD)find.c						\
		$(METHOD)for_each.c					\
		$(METHOD)insert.c					\
		$(METHOD)map.c						\
		$(METHOD)pop_back.c					\
		$(METHOD)push_back.c				\
		$(METHOD)reduce.c					\
		$(METHOD)reserve.c					\
		$(METHOD)resize.c					\
		$(METHOD)reverse.c					\
		$(METHOD)shrink_to_fit.c			\
		$(METHOD)size.c						\
		$(METHOD)slice.c					\
		$(METHOD)sort.c

# Headers folder
INCLUDES	=	./../../../include/

# GCC Flags
ERROR	=	-Werror -Wall -Wextra

# Compilation Flags
CFLAGS	+=	$(ERROR) -I$(INCLUDES) -g

# Pre Compilation
CC := gcc

OBJ	=	$(SRC:.c=.o)

DEP	=	$(SRC:.c=.d)

all: $(NAME)

$(NAME): $(OBJ)
		ar rc -o $(NAME) $(OBJ)

-include $(DEP)
%.o: %.cpp
		$(CC) -c $< -o $@ $(CFLAGS)

clean:
		rm -f $(OBJ)
		rm -f $(DEP)

fclean: clean
		rm -f $(NAME)

.PHONY:	all clean fclean
