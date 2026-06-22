# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 16:12:56 by lbonnet           #+#    #+#              #
#    Updated: 2026/06/20 13:45:59 by lbonnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME=codexion
CC=cc
C_FLAGS=-Wall -Wextra -Werror -pthread -g3
H_FILES=
C_FILES=src-test/main.c\
		src-test/parsing.c\
		src-test/utils.c\
		src-test/memory.c\
		src-test/errors.c\
		src-test/initialize.c\
		src-test/cleanup.c\
		src-test/debug.c\
		src-test/setter.c\
		src-test/getter.c\
		src-test/logs.c\
		src-test/simulation.c\
		src-test/heap.c\
		src-test/monitor.c\
		src-test/coders.c\
		src-test/dongles.c\
		src-test/time.c

all: $(NAME)

$(NAME):
	$(CC) $(C_FLAGS) $(C_FILES) -o $(NAME)

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME) clean
