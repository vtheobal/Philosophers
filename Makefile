NAME		=	philo

SRCS		= 	main.c \
				utils.c \
				death.c \
				activity.c


OBJS		= 	$(patsubst %.c,%.o,$(SRCS))

CC			= 	gcc

CFLAGS		= 	-Wall -Wextra -Werror

INCLUDES	= 	philosophers.h

RM			= 	rm -rf

all: 			$(NAME)

$(OBJS): $(INCLUDES)

$(NAME): 		$(OBJS)
				@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)	

clean:
				@${RM} $(OBJS)

fclean:			
				@${RM} $(OBJS) $(NAME)

re:				fclean all

.PHONY:			all clean fclean re