# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rsoo <rsoo@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/01 10:39:28 by rsoo              #+#    #+#              #
#    Updated: 2023/06/07 10:46:48 by rsoo             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = gcc
# CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3
CFLAGS = -fsanitize=address -g3
RM = rm -rf

SRCS_DIR = srcs/
SRCS_FILES = main parsing_utils piping freeing_utils here_doc_utils
C_FILES = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRCS_FILES)))
OBJ = $(C_FILES:.c=.o)
OUT = outfile

LIBFT_DIR = libft
LIBFT = libft.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBFT_DIR)/$(LIBFT)

clean:
	$(RM) $(OBJ)
	make clean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME) $(OUT)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
