NMS_BASE	= isalpha isdigit isalnum isascii isprint strlen memset bzero memcpy memmove strlcpy strlcat toupper tolower strchr strrchr strncmp memchr memcmp strnstr atoi calloc strdup substr strjoin strtrim split itoa strmapi striteri putchar_fd putstr_fd putendl_fd putnbr_fd lstnew lstadd_front lstsize lstlast lstadd_back lstdelone lstclear lstiter lstmap
NMS_ADDITONAL =  comparison.c\
				free_arr.c\
				frees.c\
				finds.c\
				finds2.c\
				in.c\
				ft_atod.c\
				ft_intr.c\
				ft_size.c\
				ft_split_sep.c\

LIB_SRCS	+= $(foreach wrd,$(NMS_BASE), base/ft_$(wrd).c) 
LIB_SRCS	+= $(foreach wrd,$(NMS_ADDITONAL), additional/$(wrd)) 

LIB_OBJS	= ${LIB_SRCS:.c=.o}

NAME	= libft.a

CC		= gcc

RM		= rm -f

CFLAGS	= -Wall -Wextra -Werror -I.

.c.o:
			@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

${NAME}:	${LIB_OBJS} 
			@ar -rcs ${NAME} ${LIB_OBJS}

all:		${NAME}

clean:
			@${RM} ${LIB_OBJS}

fclean:		clean
			@${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re
