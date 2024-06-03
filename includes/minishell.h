/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:03:31 by apintus           #+#    #+#             */
/*   Updated: 2024/05/30 13:58:01 by apintus          ###   ########.fr       */
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
# include <stdbool.h>
# include <sys/stat.h>

extern int	g_var;

# define MAX_ARGS 1024
# define HERE_DOC_FILE "/tmp/.here_doc"

// Colors
# define RED "\1\033[0;31m\2"
# define GREEN "\1\033[0;32m\2"
# define YELLOW "\1\033[0;33m\2"
# define BLUE "\1\033[0;34m\2"
# define MAGENTA "\1\033[0;35m\2"
# define CYAN "\1\033[0;36m\2"
# define RESET "\1\033[0m\2"
# define READLINE_MSG	"\1\033[1;32m\2minishell$ \1\033[0m\2"

//signal
# define CTRL_C_EXIT 130
# define CTRL_D_EXIT 0
# define CTRL_BS_EXIT 131

typedef enum token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	ENV_VAR,
	CMD,
	ARG,
	INFILE,
	OUTFILE,
	LIMITER
}	t_token_type;

typedef enum e_cwd_action
{
	NOTHING,
	UPDATE,
	FREE,
}	t_cwd_action;

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
	char			**args;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				equal_sign;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

typedef struct s_env_var
{
	char	*new_line;
	size_t	i;
	size_t	j;
	char	quote_char;
	t_token	*new_tokens;
	size_t	len;
	size_t	var_len;
	size_t	start_i;
	t_token	*current_token;
	char	*current_value;
}	t_env_var;

typedef struct s_data
{
	t_env	*env;
	char	**env_array;
	char	*prompt;
	t_token	*tokens;
	t_token	*first_token;
	t_ast	*ast;
	t_ast	*first_redir_out;
	size_t	token_count;
	int		exit_status;
	int		here_doc_fd;
}	t_data;

/*main*/
char	*prompt(t_data *data);

/*minishell*/
void	run_program(t_data *data);
t_data	*initialize_program(int ac, char **av, char **env);
t_data	*initialize_data(void);

/*signal*/
void	signals_heredoc(int sig);
void	signals_heredoc_parents(int sig);
void	child_ctrl_c(int sig_num);
void	handle_sigquit(int sig);
void	ctrl_c_handler(int sig);

void	signals_handler(void);
void	child_signals_handler(void);

/*check_input*/
int		check_input(char *line);
int		unclosed_quotes(char *line);
int		logical_operator(char *line);
void	update_quote_count(char c, int *sq_count, int *dq_count);

int		misplace_operator(char *line);
void	initialize_var_mis_op(int *i, int *sq, int *dq, int *cmd_flag);
int		misplace_redirection(char *line);
void	initialize_var_mis_redir(int *i, int *sq_count, int *dq_count);

/*token*/
t_token	*tokenizer(char *input, t_data *data, bool expand);
void	handle_word(char **input, t_token **tokens, bool expand);
void	handle_redirection(char **input, t_token **tokens);
void	handle_output_redirection(char **input, t_token **tokens);
void	handle_input_redirection(char **input, t_token **tokens);

void	add_token(t_token **tokens, t_token *new_token);
t_token	*new_token(char *value, t_token_type type);
t_token	*get_last_token(t_token *tokens);
void	add_word_token(char **start, char **input, t_token **tokens);
char	*remove_outer_quotes(char *word);

void	post_process_tokens(t_token **tokens, t_data *data, bool expand);
void	redefine_word_token(t_token *tokens);
void	redefine_cmd_token(t_token *tokens);
void	remove_outer_quotes_from_tokens(t_token *tokens);
void	check_quotes(char c, bool *in_sq, bool *in_dq);

bool	check_dollar(char *value, int i, bool in_sq, bool in_dq);
void	remove_cur_token(t_token **tokens, t_token **prev, t_token **current);
bool	check_dollar_in_token(char *value, bool *in_sq, bool *in_dq);
void	process_cur_token(t_token **tokens, t_token **prev, t_token **current);
void	remove_unquoted_dollar_tokens(t_token **tokens);

/*parser*/
t_ast	*parse_tokens(t_token **tokens);
t_ast	*parse_pipe(t_token **tokens);
t_ast	*parse_redirection(t_token **tokens);
t_ast	*parse_command(t_token **tokens);
t_ast	*parse_word(t_token **tokens);

t_ast	*create_ast(t_token *token);
t_ast	*create_redir_node(t_token **tokens);
t_ast	*add_redir_node_to_ast(t_ast *node, t_ast *redir, t_ast *last_redir);

/*adjust_ast*/
void	adjust_ast(t_ast *node);
void	move_cmd(t_ast *node);
void	populate_new_node_args(t_ast *new_node, t_ast *node, int arg_count);
t_ast	*create_new_node(t_ast *node, int *arg_count);

void	adjust_ast_file(t_ast *node);
void	move_arg(t_ast *node);
int		count_args(char **args);
char	*cleaner_quotes_arg(char *arg);

/*builtin*/
int		ft_cd(char **args, t_data *data);
int		ft_echo(char **cmd);
int		ft_env(t_data *data);
int		ft_exit(char **args, t_data *data);
char	*get_pwd(void);
int		ft_pwd(void);
char	*static_cwd(int action);
int		ft_unset(char **args, t_data *data);

int		ft_export(char **args, t_data *data);
void	handle_export_without_equal(char *arg, t_data *data);
void	handle_export_with_equal(char *arg, t_data *data);
int		validate_and_get_env_var(char *arg, t_data *data, t_env **env);
int		ft_export_no_args(t_data *data);
void	sort_env(char **env);
char	**duplicate_env(t_data *data);
int		add_env_var(t_data *data, char *var);
int		is_valid_var_name(char *var);

/*executor*/
void	executor(t_data *data, t_ast *ast);
void	handle_redirections(t_data *data, t_ast *ast);
void	ft_redir_in(t_data *data, t_ast *ast);
void	ft_redir_out(t_data *data, t_ast *ast);
void	ft_pipeline(t_data *data, t_ast *ast);
void	pipe_child_process(t_data *data, t_ast *ast, int fd[2], int end);
void	ft_exec(t_data *data, char **args);
void	exec_child_process(char **args, char *cmd, char **env_array);
void	close_all_fds(void);
char	*remove_quotes_file(char *str);
char	*check_cmd_quotes(char *str);
int		contains_white_space(char *str, int len);
void	handle_exec_error(char **args, char *cmd);

/*cleanning*/
int		clean_exit(t_data *data);
void	ft_free_data(t_data *data);
void	free_tokens(t_token *tokens);
void	free_ast(t_ast *ast);
void	free_env(t_env *env);

void	handle_fork_error(void);

/*env*/
t_env	*init_env(t_data *data, char **env, int i);
void	handle_shlvl(t_data *data);
void	add_env_node(t_env **env, t_env *new_node);
t_env	*new_env_node(char *name, char *value, int equal_sign);
t_env	*get_env_var(t_data *data, char *var);

char	*get_var_value(t_data *data, char *var);
void	free_array(char **array);
char	**get_env_array(t_env *env, int i);
char	*get_cmd_path(char **envp, char *cmd, int i);
char	*ft_find_path(char **split, char *cmd);

/*expand env*/
void	redefine_dollar(t_token **tokens, t_data *data);
int		has_only_dollar(char *line);
int		has_dollar(char *line);
t_token	*expand_env(char *line, t_data *data);
t_token	*handle_dollar_expan(char *line, size_t *i, t_data *data, size_t *len);
char	*get_env_value(char *var, t_env *env);

/*here_doc*/
void	handle_here_doc(t_data *data, t_token **tokens);
void	process_current_token(t_data *data, t_token *current);
void	handle_parent_process_heredoc(void);
void	handle_child_process(t_data *data, t_token *limiter);
char	*create_tmp_file(t_data *data, char *limiter);

int		check_line_against_limiter(char *line, char *limiter);
void	write_line_to_file(t_data *data, char *line);
char	*handle_null_line(t_data *data, char *filename, char *limiter);
int		open_file(t_data *data, char *filename);
char	*create_filename(void);

void	eof_display(char *limiter);
char	*find_tmp_filename(void);
void	delete_tmp_files(void);

#endif
