# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 16:12:56 by lbonnet           #+#    #+#              #
#    Updated: 2026/05/28 14:44:33 by lbonnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all clean fclean re

NAME=codexion
CC=cc
C_FLAGS=-Wall -Wextra -Werror -pthread
H_FILES=
C_FILES=main.c parsing.c utils.c memory.c initialize.c cleanup.c debug.c

all: $(NAME)

$(NAME):
	$(CC) $(C_FLAGS) $(C_FILES) -o $(NAME)

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME) clean
