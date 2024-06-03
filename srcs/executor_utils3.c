/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:28:22 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 14:46:44 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*****************************EXEC*********************************/

void	handle_exec_error(char **args, char *cmd)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(args[0], 2);
	ft_putstr_fd(": ", 2);
	if (errno == ENOENT)
	{
		if (ft_strncmp(cmd, "/", 1) == 0 || ft_strncmp(cmd, ".", 1) == 0)
			ft_putstr_fd("No such file or directory\n", 2);
		else
			ft_putstr_fd("command not found\n", 2);
		exit(127);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("Permission denied\n", 2);
		exit(126);
	}
	else
	{
		ft_putstr_fd(strerror(errno), 2);
		exit(1);
	}
}

/******************************REDIR OUT***********************************/

void	handle_open_error(char *arg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	g_var = 1;
}

void	handle_fork_error(void)
{
	perror("fork failed");
	exit(EXIT_FAILURE);
}

void	exec_child_process_redir_out(t_data *data, t_ast *ast, int fd)
{
	dup2(fd, STDOUT_FILENO);
	close(fd);
	executor(data, ast->left);
	exit(g_var);
}

void	ft_redir_out(t_data *data, t_ast *ast)
{
	int		fd;
	pid_t	pid;
	int		status;

	if (ast->type == REDIR_OUT)
		fd = open(ast->right->args[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else
		fd = open(ast->right->args[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		handle_open_error(ast->right->args[0]);
		return ;
	}
	pid = fork();
	if (pid == 0)
		exec_child_process_redir_out(data, ast, fd);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		g_var = WEXITSTATUS(status);
		close(fd);
	}
	else
		handle_fork_error();
}
