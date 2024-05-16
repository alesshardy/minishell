# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kammi <kammi@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/15 12:43:39 by apintus           #+#    #+#              #
#    Updated: 2024/05/10 15:07:41 by kammi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#########################################################
## ARGUMENTS

NAME = minishell
ARCHIVE = minishell.a
HEADER = includes/minishell.h
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra -g
MAKE_LIB = ar -rcs
BONUS_NAME = pipex_bonus

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
RESET = \033[0m

##########################################################
## SOURCES

SRCS = srcs/main.c\
		srcs/minishell.c\
		srcs/check_input.c\
		srcs/token.c\
		srcs/token_utils.c\
		srcs/parser.c\
		srcs/parser_utils.c\
		srcs/executor.c\
		srcs/builtins.c\
		srcs/environment_init.c\
		srcs/signals.c\
		srcs/here_doc.c\
		srcs/adjust_ast.c\
		srcs/env_utils.c\
		srcs/expand_env_var.c\
		srcs/cleanning.c\

OBJS = $(SRCS:.c=.o)

# LIBFT
LIBFT_DIR = libft
LIBFT_MAKE_COMMAND = make -s -C $(LIBFT_DIR)
LIBFT_PATH = $(LIBFT_DIR)/libft.a

##########################################################
## RULES

all : $(NAME) $(LIBFT_PATH)

		@echo " "
		@echo " "
		@echo "                               ,*..       .   ..//\*"
		@echo "                                ,....           .    .   ,"
		@echo "                              .,,.,. ."
		@echo "                            *..... ...."
		@echo "                           ,.,,,.... .      .,.,,..             *"
		@echo "                         /,,,,,.....,.,*/////////***.. .        ."
		@echo "                         ,... .  ..,*//(((((((((/////*,.        ./"
		@echo "                         ....   .,*//(((((((((((((////*,         ."
		@echo "                         *..  ..*/((((((((((((((//////**,"
		@echo "                          #* ////*,,,,*/(/(((///,.,.,.,,,. ."
		@echo "                           /./(. .,,*, ,*/((//**,*,...,...,. ,./"
		@echo "                          #/. #**,*,../**  ,  .,*/..,.,,, ,*,..*,"
		@echo "                          (/*##((//////// /(/ ,*/////**** **,,*,*"
		@echo "                          #(#*(.//(//((/,////* *////////.**,.****"
		@echo "                            /./((((((((//(((/*,,//.. *//**, *.*"
		@echo "                             .,/((((((//**//*.,*////////*,../"
		@echo "                              ..,//,...    ..    ....***,..*"
		@echo "                              /...,..*/**,,,,,,,,**. .... ."
		@echo "                               ......,***,*,,,,,,,..     /"
		@echo "                                ,.....,*.,. ..,,.. .    ,"
		@echo "                                *(/..    .    .    .       .*#"
		@echo "                             .###((*.               .***"
		@echo "                        **&&%(###((((((*,      . .,***"
		@echo "                   %%%%%%%%%%%(#(((           ////"
		@echo " "
		@echo " "

$(NAME) : $(ARCHIVE) $(LIBFT_PATH)
	@echo "$(GREEN)Linking libraries and building $@...$(RESET)"
	@$(CC) $(ARCHIVE) $(LIBFT_PATH) -lreadline -o $(NAME)
	@echo "$(GREEN)Success \n$(RESET)"

$(ARCHIVE) : $(OBJS)
	@echo "$(GREEN)Creating archive...$(RESET)"
	@$(MAKE_LIB) $(ARCHIVE) $(OBJS)

%.o : %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_PATH) :
	@$(LIBFT_MAKE_COMMAND)

clean :
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) $(OBJS) $(ARCHIVE)
	@$(LIBFT_MAKE_COMMAND) clean

fclean : clean
	@echo "$(RED)Cleaning all files...$(RESET)"
	@$(RM) $(NAME)
	@$(LIBFT_MAKE_COMMAND) fclean

re : fclean all

.PHONY : all clean fclean re
