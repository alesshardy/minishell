/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:11:32 by apintus           #+#    #+#             */
/*   Updated: 2024/05/30 13:52:13 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*************************************EXEC**********************************/

void	handle_directory(char **args, char *cmd)
{
	if ((ft_strncmp(cmd, "/", 1) != 0 && ft_strncmp(cmd, ".", 1) != 0)
		|| args[0][0] == '\0')
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
}

void	handle_permissions(char **args, char *cmd, struct stat st)
{
	if ((st.st_mode & S_IXUSR) == 0 && ft_strchr(cmd, '/') == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	else if ((st.st_mode & S_IXUSR) == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
}

void	exec_child_process(char **args, char *cmd, char **env_array)
{
	struct stat	st;

	close_all_fds();
	if (stat(cmd, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			handle_directory(args, cmd);
		else
			handle_permissions(args, cmd, st);
	}
	if (execve(cmd, args, env_array) == -1)
	{
		handle_exec_error(args, cmd);
	}
}

void	handle_fork(t_data *data, char **args, char *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (ft_strcmp(cmd, "./minishell") != 0)
		child_signals_handler();
	if (pid == 0)
		exec_child_process(args, cmd, data->env_array);
	else
	{
		free_array(data->env_array);
		free(cmd);
		free(args[0]);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_var = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_var = 128 + WTERMSIG(status);
	}
	signals_handler();
}

void	ft_exec(t_data *data, char **args)
{
	char	*cmd;
	int		i;

	i = 0;
	args[0] = remove_outer_quotes(args[0]);
	if (args[0] == NULL)
		return ;
	args[0] = check_cmd_quotes(args[0]);
	data->env_array = get_env_array(data->env, i);
	if (data->env_array == NULL)
	{
		free(args[0]);
		return ;
	}
	cmd = get_cmd_path(data->env_array, args[0], i);
	if (cmd == NULL)
	{
		free_array(data->env_array);
		free(cmd);
		return ;
	}
	handle_fork(data, args, cmd);
}
