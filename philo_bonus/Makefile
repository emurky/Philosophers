# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: emurky <emurky@student.21-school.ru>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/07 12:38:19 by emurky            #+#    #+#              #
#    Updated: 2022/03/07 12:38:26 by emurky           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS				= philo_bonus.c processes.c utils.c libft.c
OBJS				= $(SRCS:.c=.o)
DEPS				= $(SRCS:.c=.d)

NAME				= philo_bonus

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
					rm -rf *.o *.a */*.o */*.a */*.d *.d 'philo_bonus '* philo_bonus *.txt

.PHONY:				all clean fclean re bonus \
					sc

#.SILENT:
# leaks --atExit -- ./philo_bonus 5 800 200 200 7
