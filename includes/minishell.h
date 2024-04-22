/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:03:31 by apintus           #+#    #+#             */
/*   Updated: 2024/04/22 17:36:22 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h> // pid_t
# include <stdlib.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

# define MAX_ARGS 1024
# define HERE_DOC_FILE "/tmp/.here_doc"

// Colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"
# define READLINE_MSG	"\033[1;32mminishell\033[32m$ \033[0m"

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
	OUTFILE,
	LIMITER
}	t_token_type;

// get_cwd action parameter
enum					e_cwd_action //a norme ?
{
	NOTHING,
	UPDATE,
	FREE,
};

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
	int				file; // pe etre a supp
	char			**args;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				equal_sign; // a voir si utile
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_data
{
	t_env	*env;
	char	**env_array;
	char	*prompt;
	t_token	*tokens;
	t_ast	*ast;
	t_ast	*first_redir_out;
	size_t	token_count;
	int		count_redir_in;
	int		count_redir_out;
	int		exit_status;
	int		here_doc_fd;
}	t_data;

/*main.c*/
char	*prompt(void);

/*environment_init.c*/
//ancienne version
// char	**dup_env(char **env);
// void	init_shell_env_struct(t_data *data, char **env);
// int		get_var_index(t_data *data, char *var);
// char	*get_var_value(t_data *data, char *var);
// void	init_shell_env(t_data *data, char **env);

t_env	*get_env_var(t_data *data, char *var);
char	*get_var_value(t_data *data, char *var);
t_env	*new_env_node(char *name, char *value, int equal_sign);
void	add_env_node(t_env **env, t_env *new_node);
t_env	*init_env(t_data *data, char **env);

char	**get_env_array(t_env *env);
void	free_array(char **array);
char	*get_cmd_path(char **envp, char *cmd);
char	*ft_find_path(char **split, char *cmd);

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

/*adjust*/
int		is_white_space(char *str);
char	*check_cmd_quotes(char *str);
char	*remove_quotes_file(char *str);

/*execute.c*/
void	ft_exec(t_data *data, char **args);
void	ft_pipe(t_data *data, t_ast *ast);
void	ft_redir_out(t_data *data, t_ast *ast);
void	executor(t_data *data, t_ast *ast);

void	count_redirection(t_data *data, t_ast **ast);
void	reset_count_redirection(t_data *data);
void	handle_multi_redir(t_data *data, t_ast *ast);

/*builtins.c*/
int		ft_echo(char **args);
int		ft_cd(char **args, t_data *data);
char	*static_cwd(int action);
int		ft_pwd(void);
char	*get_pwd(void);
int		ft_env(t_data *data);
int		ft_exit(char **args, t_data *data);

/*signals*/
void	signals_handler(void);
void	ctrl_c_handler(int sig);

/*redefine*/
// void	redefine_word_token(t_token *tokens);
// void	redefine_cmd_token(t_token *tokens);
// void	remove_empty_quotes_from_tokens(t_token *tokens);
// char	*remove_empty_quotes(char *word);
// char	*remove_outer_quotes(char *word);
// void	remove_outer_quotes_from_tokens(t_token *tokens);
// void	redefine_tokens(t_token **tokens);

/*display*/
void	display_tokens(t_token *tokens);
void	print_ast(t_ast *ast, int level);

/*adjust_ast*/
void	move_cmd(t_ast *node);
void	adjust_ast(t_ast *node);

/*here_doc*/
void	handle_here_doc(t_data *data, t_token **tokens);
char	*create_tmp_file(t_data *data, char *limiter);
void	delete_tmp_files(void);

/*dollar handler*/
void	redefine_dollar(t_token **tokens);

#endif
