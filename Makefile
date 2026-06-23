# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 16:12:56 by lbonnet           #+#    #+#              #
#    Updated: 2026/06/23 10:30:25 by lbonnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME=codexion
CC=cc
C_FLAGS=-Wall -Wextra -Werror -pthread -g3
H_FILES=
C_FILES=src/main.c\
		src/parsing.c\
		src/utils.c\
		src/memory.c\
		src/errors.c\
		src/initialize.c\
		src/cleanup.c\
		src/setter.c\
		src/getter.c\
		src/logs.c\
		src/simulation.c\
		src/heap.c\
		src/monitor.c\
		src/coders.c\
		src/dongles.c

all: $(NAME)

$(NAME):
	$(CC) $(C_FLAGS) $(C_FILES) -o $(NAME)

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME) clean
