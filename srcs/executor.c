/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:19:23 by apintus           #+#    #+#             */
/*   Updated: 2024/05/30 13:45:08 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pipe_child_process(t_data *data, t_ast *ast, int fd[2], int end)
{
	dup2(fd[end], end);
	close(fd[0]);
	close(fd[1]);
	executor(data, ast);
	exit(g_var);
}

void	ft_pipeline(t_data *data, t_ast *ast)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid2;

	pipe(fd);
	pid = fork();
	child_signals_handler();
	if (pid == 0)
		pipe_child_process(data, ast->left, fd, 1);
	else
	{
		pid2 = fork();
		if (pid2 == 0)
			pipe_child_process(data, ast->right, fd, 0);
		else
		{
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &data->exit_status, 0);
			g_var = data->exit_status >> 8;
			waitpid(pid2, &data->exit_status, 0);
			g_var = data->exit_status >> 8;
		}
	}
	signals_handler();
}

void	ft_redir_in(t_data *data, t_ast *ast)
{
	int		fd;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	fd = open(ast->right->args[0], O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(ast->right->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		g_var = 1;
		close(saved_stdin);
		return ;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	executor(data, ast->left);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

void	handle_redirections(t_data *data, t_ast *ast)
{
	if (ast->type == REDIR_OUT || ast->type == REDIR_APPEND)
	{
		ft_redir_out(data, ast);
	}
	else if (ast->type == REDIR_IN || ast->type == REDIR_HEREDOC)
	{
		ft_redir_in(data, ast);
	}
	else
		executor(data, ast);
}

void	executor(t_data *data, t_ast *ast)
{
	if (ast == NULL)
		return ;
	if (ast->type == CMD)
	{
		if (ft_strncmp(ast->args[0], "echo", 4) == 0)
			ft_echo(ast->args);
		else if (ft_strncmp(ast->args[0], "cd", 2) == 0)
			ft_cd(ast->args, data);
		else if (ft_strncmp(ast->args[0], "pwd", 3) == 0)
			ft_pwd();
		else if (ft_strncmp(ast->args[0], "export", 6) == 0)
			ft_export(ast->args, data);
		else if (ft_strncmp(ast->args[0], "unset", 5) == 0)
			ft_unset(ast->args, data);
		else if (ft_strncmp(ast->args[0], "env", 3) == 0)
			ft_env(data);
		else if (ft_strncmp(ast->args[0], "exit", 4) == 0)
			ft_exit(ast->args, data);
		else
			ft_exec(data, ast->args);
	}
	else if (ast->type == PIPE)
		ft_pipeline(data, ast);
	else
		handle_redirections(data, ast);
}
