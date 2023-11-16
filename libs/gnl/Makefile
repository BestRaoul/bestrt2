SOURCES	=	gnl.c\
			gnl_utils.c

OBJECTS	= ${SOURCES:.c=.o}

NAME	= libgnl.a

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -I.

.c.o:
			@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${OBJECTS} 
			@ar -rcs ${NAME} ${OBJECTS}

all:		${NAME}

clean:
			@${RM} ${OBJECTS}

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
