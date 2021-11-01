SRCS				=	philo.c \
						threads.c \
						utils.c \
						libft.c
OBJS				= $(SRCS:.c=.o)
DEPS				= $(SRCS:.c=.d)

NAME				= philo

CC 					= gcc
RM 					= rm -f
CFLAGS				= -Wall -Wextra -Werror -pthread #-g -fsanitize=thread
CPPFLAGS			= -MMD -I.

all:				$(NAME)

$(NAME):			$(OBJS) Makefile
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

-include			$(DEPS)

bonus:				all

clean:
					$(RM) $(OBJS) $(DEPS)

fclean:				clean
					$(RM) $(NAME)

re:					fclean all

sc:
					rm -rf *.o *.a */*.o */*.a */*.d *.d 'philo '* philo

.PHONY:				all clean fclean re bonus \
					sc

#.SILENT:
# leaks --atExit -- ./philo 5 800 200 200 7
