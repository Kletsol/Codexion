# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 16:12:56 by lbonnet           #+#    #+#              #
#    Updated: 2026/05/20 16:11:51 by lbonnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=codexion
C=cc
C_FLAGS=-Wall -Wextra -Werror -pthread
H_FILES=
C_FILES=test2.c

all: $(NAME)

$(NAME):
	$(CC) $(C_FLAGS) $(C_FILES) -o $(NAME)

$(LIBFT_O_FILES): $(LIBFT_A)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(O_FILES)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME) clean


.PHONY: all clean fclean re test
