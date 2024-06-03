# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apintus <apintus@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/03/15 12:43:39 by apintus           #+#    #+#              #
#    Updated: 2024/05/30 13:58:10 by apintus          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#########################################################
## ARGUMENTS

NAME = minishell
HEADER = includes/minishell.h
CC = cc
RM = rm -f
CFLAGS = -Wall -Werror -Wextra

# Colors
RED = \033[0;31m
GREEN = \033[0;32m
RESET = \033[0m


##########################################################
## SOURCES

SRCS = srcs/main.c\
		srcs/minishell.c\
		srcs/check_input.c\
		srcs/check_input_utils.c\
		srcs/token.c\
		srcs/token_utils.c\
		srcs/token_utils2.c\
		srcs/token_utils3.c\
		srcs/token_utils4.c\
		srcs/parser.c\
		srcs/parser_utils.c\
		srcs/executor.c\
		srcs/executor_utils.c\
		srcs/executor_utils2.c\
		srcs/executor_utils3.c\
		srcs/builtin/cd.c\
		srcs/builtin/echo.c\
		srcs/builtin/env.c\
		srcs/builtin/export.c\
		srcs/builtin/export_utils.c\
		srcs/builtin/unset.c\
		srcs/builtin/pwd.c\
		srcs/builtin/exit.c\
		srcs/environment_init.c\
		srcs/signals.c\
		srcs/signals_utils.c\
		srcs/here_doc.c\
		srcs/here_doc_utils.c\
		srcs/here_doc_utils2.c\
		srcs/adjust_ast.c\
		srcs/adjust_ast_files.c\
		srcs/env_utils.c\
		srcs/expand_env_var.c\
		srcs/expand_env_var_utils.c\
		srcs/cleanning.c\

OBJS = $(SRCS:.c=.o)

# LIBFT
LIBFT_DIR = libft
LIBFT_MAKE_COMMAND = make -s -C $(LIBFT_DIR)
LIBFT_PATH = $(LIBFT_DIR)/libft.a

##########################################################
## RULES

all : $(NAME) $(LIBFT_PATH)

	@echo "              ,%#/#((/.#                               (/**,,"
	@echo "           # %,//......,,,,,                     /..          .,*/"
	@echo "          %*.  .         ..,,,               *.....         .      .,"
	@echo "       /     .,********,,....,**           .,...."
	@echo "     .   ,,***///////////////..,*        *,....  ...        ."
	@echo "       ,******/////////////((##,/       ,.,,,....       .,,,,..            ,"
	@echo "  &. .,,******/*///////////(((#&**    *,..,,. ..,.,*/////////**,,.         ."
	@echo "  #   .,,*****/////(//////(((##%%(    ,....   ..*//(/((((((((///*,,.       .*"
	@echo "  /   .,,*,.....,,*////*,,,**/%&%%    .... . .,//(((((((((((//////*."
	@echo "  %  ,*,,,,******,,*////,***//(%@%     .....*///((((((((((((//////*,"
	@echo "  & .*,,,... .*,,,,,*/(/*,,..,(&@#      * ((/*.....,//(((/*,......,..  .  ."
	@echo " *..,,***,*****,,****//#(/**//(#%%     ((,. ,,/////*.*///*,,.,///*. ,..* .,,"
	@echo " %, ,,,,*************//(%(((///#&&*    (/. #/***,./** *// ,***,*,,.,* *,.,**"
	@echo "  ,..,,,******//***,,*//%(#//((%%%(    (//#,((////((/ ///..////////*,**,.,,*"
	@echo "   ....,,*****//*,,..,*,,*///((%#(*    (((,((.//((/.///(/**.///////***,.**//"
	@echo "   #. ..,,***,,.........,,,**/((/(        ,*(((((((/((((/***///////**,,."
	@echo "    . ...........,,,,,,*,,,.,,*,*#        *.,/((/*,,...,.  .,.,*//**.../"
	@echo "          . ...,,******//(/*.,*,*          *..,,...,,,***,*,,..  ,,.. *"
	@echo "     .      ...,,,.....,,**,.,,*%           ...,.**//////*****....   /"
	@echo "               ..,,,,,,*,*,,,,*%             ......**,.  .,,,. .    ("
	@echo "               .........,,.,,,                /    . .     .      .,"
	@echo "                   ........,*                  (.. .             ,"
	@echo "                      ....                                   ..*"
	@echo "$(GREEN) .S_SsS_S.    .S   .S_sSSs     .S    sSSs   .S    S.     sSSs  S.      S."
	@echo ".SS~S*S~SS.  .SS  .SS~YS%%b   .SS   d%%SP  .SS    SS.   d%%SP  SS.     SS."
	@echo "S%S 'Y' S%S  S%S  S%S   'S%b  S%S  d%S'    S%S    S%S  d%S'    S%S     S%S"
	@echo "S%S     S%S  S%S  S%S    S%S  S%S  S%|     S%S    S%S  S%S     S%S     S%S"
	@echo "S%S     S%S  S&S  S%S    S&S  S&S  S&S     S%S SSSS%S  S&S     S&S     S&S"
	@echo "S&S     S&S  S&S  S&S    S&S  S&S  Y&Ss    S&S  SSS&S  S&S_Ss  S&S     S&S"
	@echo "S&S     S&S  S&S  S&S    S&S  S&S  'S&&S   S&S    S&S  S&S~SP  S&S     S&S"
	@echo "S&S     S&S  S&S  S&S    S&S  S&S    'S*S  S&S    S&S  S&S     S&S     S&S"
	@echo "S*S     S*S  S*S  S*S    S*S  S*S     l*S  S*S    S*S  S*b     S*b     S*b"
	@echo "S*S     S*S  S*S  S*S    S*S  S*S    .S*P  S*S    S*S  S*S.    S*S.    S*S."
	@echo "S*S     S*S  S*S  S*S    S*S  S*S  sSS*S   S*S    S*S   SSSbs   SSSbs   SSSbs"
	@echo "SSS     S*S  S*S  S*S    SSS  S*S  YSS'    SSS    S*S    YSSP    YSSP    YSSP"
	@echo "        SP   SP   SP          SP                  SP"
	@echo "        Y    Y    Y           Y                   Y$(RESET)"

$(NAME) : $(OBJS) $(LIBFT_PATH)
	@echo "$(GREEN)Linking libraries and building $@...$(RESET)"
	@$(CC) $(OBJS) $(LIBFT_PATH) -lreadline -o $(NAME)
	@echo "$(GREEN)Success \n$(RESET)"

%.o : %.c $(HEADER)
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT_PATH) :
	@$(LIBFT_MAKE_COMMAND)

clean :
	@echo "$(RED)Cleaning object files...🧹$(RESET)"
	@$(RM) $(OBJS) $(ARCHIVE)
	@$(LIBFT_MAKE_COMMAND) clean

fclean : clean
	@echo "$(RED)Cleaning all files...🧹$(RESET)"
	@$(RM) $(NAME)
	@$(LIBFT_MAKE_COMMAND) fclean

re : fclean all

.PHONY : all clean fclean re
