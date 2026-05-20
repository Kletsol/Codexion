# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/05 16:12:56 by lbonnet           #+#    #+#              #
#    Updated: 2026/05/20 11:19:02 by lbonnet          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=codexion
C=cc
C_FLAGS=-Wall -Wextra -Werror -pthread
H_FILES=
C_FILES=

all: $(NAME)

$(NAME): $(C_FILES) $(LIBFT_A)
	$(CC) $(C_FLAGS) $(C_FILES) $(LIBFT_A) -o $(NAME)

$(LIBFT_O_FILES): $(LIBFT_A)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

clean:
	make -C $(LIBFT_DIR) clean
	rm -rf $(O_FILES)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(BONUS_NAME)
	rm -rf $(LIBFT_A)
	rm -rf $(TESTER_DIR) $(TESTER_FILES) $(CHECKER_FILE)

re: fclean $(NAME) clean


.PHONY: all clean fclean re test
