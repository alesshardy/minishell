/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:03:31 by apintus           #+#    #+#             */
/*   Updated: 2024/04/05 17:09:31 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

# define MAX_ARGS 1024

typedef enum token_type
{
	WORD,
	PIPE, //|
	REDIR_IN, //<
	REDIR_OUT, //>
	REDIR_APPEND, //>>
	REDIR_HEREDOC, //<<
	ENV_VAR, //$
	CMD,
	ARG,
	INFILE,
	OUTFILE
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_ast
{
	t_token_type	type;
	int				file;
	char			**args;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_data
{
	char	**env;
	char	*prompt;
	t_token	*tokens;
	t_ast	*ast;
	size_t	token_count;
}	t_data;

/*main.c*/
char	*prompt(void);

/*check_input.c*/
int		check_input(char *line);
int		unclosed_quotes(char *line);
int		logical_operator(char *line);
int		misplace_operator(char *line);
int		misplace_redirection(char *line);

/*token.c*/
void	free_tokens(t_token *tokens);
void	add_token(t_token **tokens, t_token *new_token);
t_token	*new_token(char *value, t_token_type type);
void	handle_redirection(char **input, t_token **tokens);
void	handle_word(char **input, t_token **tokens);
t_token	*tokenizer(char *input);

void	redefine_word_token(t_token *tokens);
void	redefine_cmd_token(t_token *tokens);


/*parser.c*/
t_ast	*create_ast(t_token *tokens);
t_ast	*parse_word(t_token **tokens);
t_ast	*parse_command(t_token **tokens);
t_ast	*parse_redirection(t_token **tokens);
t_ast	*parse_pipe(t_token **tokens);
t_ast	*parse_tokens(t_token **tokens);

#endif
