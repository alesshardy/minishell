/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:19:23 by apintus           #+#    #+#             */
/*   Updated: 2024/05/17 18:27:16 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*********************************ADJUST FILE AND CMD********************************/

/* int	is_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n'
			&& str[i] != '\v' && str[i] != '\f' && str[i] != '\r')
			return (0);
		i++;
	}
	return (1);

}

char	*check_cmd_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	if ((str[0] == '\'' || str[0] == '\"') && str[ft_strlen(str) - 1] == str[0] && !is_white_space(str + 1))
	{
		while (str[++i] != str[0])
		{
			new_str[j++] = str[i];
		}
		new_str[j] = '\0';
	}
	else
	{
		while (str[i] != '\0')
		{
			new_str[j++] = str[i++];
		}
		new_str[j] = '\0';
	}
	printf("new_str = %s\n", new_str); //fdebug
	return (new_str);
} */

int	contains_white_space(char *str, int len)
{
	int i;

	i = 0;
	while (i < len)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return 1;
		i++;
	}
	return (0);
}

char	*check_cmd_quotes(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	if ((str[0] == '\'' || str[0] == '\"') && str[ft_strlen(str) - 1] == str[0]
			&& ft_strlen(str) != 2 && !contains_white_space(str + 1, ft_strlen(str) - 2))
	{
		while (str[++i] != str[0])
		{
			new_str[j++] = str[i];
		}
		new_str[j] = '\0';
	}
	else
	{
		while (str[i] != '\0')
		{
			new_str[j++] = str[i++];
		}
		new_str[j] = '\0';
	}
	free(str); //ajout pour free str
	//printf("new_str = %s\n", new_str); //fdebug
	return (new_str);
}

char	*remove_quotes_file(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	if ((str[0] == '\'' || str[0] == '\"') && str[ft_strlen(str) - 1] == str[0])
	{
		while (str[++i] != str[0])
		{
			new_str[j++] = str[i];
		}
		new_str[j] = '\0';
	}
	else
	{
		while (str[i] != '\0')
			new_str[j++] = str[i++];
		new_str[j] = '\0';
	}
	free(str);
	return (new_str);
}

/*************************************EXEC*************************************/
void	close_all_fds()
{
	int i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}
// void	exec_child_process(char **args, char *cmd, char **env_array)
// {
// 	close_all_fds();
// 	if (execve(cmd, args, env_array) == -1)
// 	{
// 		ft_putstr_fd("minishell:", 2);
// 		ft_putstr_fd(args[0], 2);
// 		ft_putstr_fd(": ", 2);
// 		if (errno ==ENOENT)
// 		{
// 			ft_putstr_fd("Command not found", 2);
// 			exit(127);
// 		}
// 		else if (errno == EACCES)
// 		{
// 			ft_putstr_fd("Permission denied\n", 2);
// 			exit(126);
// 		}
// 		else
// 			ft_putstr_fd(strerror(errno), 2);
// 		ft_putstr_fd("\n", 2);
// 		exit(1);
// 	}
// }

void	exec_child_process(char **args, char *cmd, char **env_array)
{
	struct stat st;

	close_all_fds();
	if (stat(cmd, &st) == 0)
	{
		// if (ft_strncmp(cmd, "/", 1) != 0 || ft_strncmp(cmd, ".", 1) != 0)
		// {
		// 	ft_putstr_fd("minishell: ", 2);
		// 	ft_putstr_fd(args[0], 2);
		// 	ft_putstr_fd(": command not found\n", 2);
		// 	exit(127);
		// }
		if (S_ISDIR(st.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			exit(126);
		}
		else if ((st.st_mode & S_IXUSR) == 0) //
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": Permission denied\n", 2);
			exit(126);
		}
	}
	if (execve(cmd, args, env_array) == -1)
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
}

void	ft_exec(t_data *data, char **args)
{
	pid_t	pid;
	char	*cmd;
	int		status;

	args[0] = remove_outer_quotes(args[0]);
	args[0] = check_cmd_quotes(args[0]);
	data->env_array = get_env_array(data->env);
	cmd = get_cmd_path(data->env_array, args[0]);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (pid == 0)
		exec_child_process(args, cmd, data->env_array);
	else
	{
		free_array(data->env_array); //ajout pour free env_array
		free(cmd); //ajout pour free cmd
		free(args[0]); //ajout pour free args[0]
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			global_var = WEXITSTATUS(status);
			//printf("global_varE = %d\n", global_var); //fdebug
		}
		else if (WIFSIGNALED(status))
		{
			global_var = 128 + WTERMSIG(status);
			//printf("global_varS = %d\n", global_var); //fdebug
		}
	}
	signal(SIGINT, ctrl_c_handler);  // Restore signal handlers
	signal(SIGQUIT, SIG_IGN);  // Restore signal handlers
}
// void	ft_exec(t_data *data, char **args)
// {
// 	pid_t	pid;
// 	char	*cmd;
// 	int		status;

// 	//signal(SIGINT, SIG_DFL);  // Reset to default behavior
// 	//signal(SIGQUIT, SIG_DFL);  // Reset to default behavior

// 	args[0] = remove_outer_quotes(args[0]);
// 	args[0] = check_cmd_quotes(args[0]);

// 	data->env_array = get_env_array(data->env);

// 	cmd = get_cmd_path(data->env_array, args[0]);

// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (execve(cmd, args, data->env_array) == -1)
// 		{
// 			ft_putstr_fd("minishell:", 2);
// 			ft_putstr_fd(args[0], 2);
// 			ft_putstr_fd(": ", 2);
// 			if (errno ==ENOENT)
// 			{
// 				ft_putstr_fd("Command not found", 2);
// 				exit(127);
// 			}
// 			else if (errno == EACCES)
// 			{
// 				ft_putstr_fd("Permission denied\n", 2);
// 				exit(126);
// 			}
// 			else
// 				ft_putstr_fd(strerror(errno), 2);
// 			ft_putstr_fd("\n", 2);
// 			exit(1);
// 		}
// 	}
// 	else
// 	{
// 		free_array(data->env_array); //ajout pour free env_array
// 		free(cmd); //ajout pour free cmd
// 		free(args[0]); //ajout pour free args[0]
// 		waitpid(pid, &status, 0);
// 		global_var = status >> 8;
// 	}

// 	signal(SIGINT, ctrl_c_handler);  // Restore signal handlers
// 	signal(SIGQUIT, handle_sigquit);  // Restore signal handlers
// }

void	pipe_child_process(t_data *data, t_ast *ast, int fd[2], int end)
{
	dup2(fd[end], end);
	close(fd[0]);
	close(fd[1]);
	executor(data, ast);
	exit(global_var); // fdpppppppppp
}

void	ft_pipeline(t_data *data, t_ast *ast)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;

	pipe(fd);
	pid = fork();
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
			waitpid(pid, &status, 0);
			global_var = status >> 8;
			//printf("global_var1 = %d\n", global_var); //fdebug
			waitpid(pid2, &status, 0);
			global_var = status >> 8;
			//printf("global_var2 = %d\n", global_var); //fdebug
		}
	}
}

/* void	ft_pipeline(t_data *data, t_ast *ast)
{
    int		fd[2];
    pid_t	pid;
    pid_t	pid2;
    int		status;

    pipe(fd);
    pid = fork();
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
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                global_var = WEXITSTATUS(status);
                printf("global_var1 = %d\n", global_var); //fdebug
            }
            else if (WIFSIGNALED(status))
                global_var = 128 + WTERMSIG(status);
            waitpid(pid2, &status, 0);
            if (WIFEXITED(status))
            {
                global_var = WEXITSTATUS(status);
                printf("global_var2 = %d\n", global_var); //fdebug
            }
            else if (WIFSIGNALED(status))
                global_var = 128 + WTERMSIG(status);
        }
    }
} */

// void	ft_pipeline(t_data *data, t_ast *ast)
// {
// 	int		fd[2];
// 	pid_t	pid;
// 	pid_t	pid2;
// 	int		status;

// 	pipe(fd);
// 	pid = fork();
// 	if (pid == 0) //child
// 		{
// 			dup2(fd[1], 1);
// 			close(fd[0]);
// 			close(fd[1]);
// 			executor(data, ast->left);
// 			exit(0);
// 		}
// 	else //parent
// 	{
// 		pid2 = fork();
// 		if (pid2 == 0) //child
// 		{
// 			dup2(fd[0], 0);
// 			close(fd[0]);
// 			close(fd[1]);
// 			executor(data, ast->right);
// 			exit(0);
// 		}
// 		else //parent
// 		{
// 			close(fd[0]);
// 			close(fd[1]);
// 			waitpid(pid, &status, 0);
// 			global_var = status >> 8;
// 			waitpid(pid2, &status, 0);
// 			global_var = status >> 8;
// 		}
// 	}
// }

void	ft_redir_out(t_data *data, t_ast *ast)
{
	int		fd;
	pid_t	pid;

	if (ast->type == REDIR_OUT)
		fd = open(ast->right->args[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else
		fd = open(ast->right->args[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(ast->right->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		global_var = 1;
		//ft_putstr_fd("SIUUUUU\n", 2);
		return;
	}

	pid = fork();
	if (pid == 0)
	{
		// This is the child process. Redirect the standard output and execute the command.
		dup2(fd, STDOUT_FILENO);
		close(fd);
		executor(data, ast->left);
		//dprintf(2, "%d\n", global_var);
		exit(global_var);
	}
	else if (pid > 0)
	{
		// This is the parent process. Wait for the child to finish.
		int status;
		waitpid(pid, &status, 0);
		global_var = WEXITSTATUS(status);
		close(fd);
	}
	else
	{
		// Fork failed.
		perror("fork");
		exit(EXIT_FAILURE);
	}
}

// OG FONCTIONNEL MAIS PASSE PAS LE TESTEUR A CAUSE CONFLIT SORTIS STANDARD
//  void	ft_redir_out(t_data *data, t_ast *ast)
// {
// 	int		fd;
// 	//int		status;
// 	int		saved_stdout;

// 	saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder la sortie standard

// 	//ast->right->args[0] = remove_quotes_file(ast->right->args[0]); // remove quotes from file name
// 	if (ast->type == REDIR_OUT)
// 		fd = open(ast->right->args[0], O_CREAT | O_TRUNC | O_WRONLY, 0644);
// 	else
// 		fd = open(ast->right->args[0], O_CREAT | O_APPEND | O_WRONLY, 0644);
// 	if (fd < 0)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(ast->right->args[0], 2);
// 		ft_putstr_fd(": ", 2);
// 		ft_putstr_fd(strerror(errno), 2);
// 		ft_putstr_fd("\n", 2);
// 		global_var = 1;
// 		ft_putstr_fd("SIUUUUU\n", 2);
// 		close(saved_stdout);
// 		return;
// 	}
// 	dup2(fd, STDOUT_FILENO);
// 	close(fd);
// 	executor(data, ast->left);
// 	dup2(saved_stdout, STDOUT_FILENO);  // Restaurer la sortie standard
// 	close(saved_stdout);
// }

/* void	ft_redir_out(t_data *data, t_ast *ast)
{
	int		fd;
	int		saved_stdout;
	char	*error_message = NULL;

	saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder la sortie standard

	if (ast->type == REDIR_OUT)
		fd = open(ast->right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(ast->right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		error_message = ft_strjoin("minishell: ", ast->right->args[0]);
		error_message = ft_strjoin(error_message, ": ");
		error_message = ft_strjoin(error_message, strerror(errno));
		error_message = ft_strjoin(error_message, "\n");
		global_var = 1;
	}
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		executor(data, ast->left);
		dup2(saved_stdout, STDOUT_FILENO);  // Restaurer la sortie standard
	}
	close(saved_stdout);

	if (error_message != NULL)
	{
		ft_putstr_fd(error_message, 2);
		free(error_message);
	}
} */

void	ft_redir_in(t_data *data, t_ast *ast)
{
	int		fd;
	//int		status;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);  // Sauvegarder l'entrée standard

	//ast->right->args[0] = remove_quotes_file(ast->right->args[0]); // remove quotes from file name
	fd = open(ast->right->args[0], O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(ast->right->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		global_var = 1;
		close(saved_stdin);
		return;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	executor(data, ast->left);

	dup2(saved_stdin, STDIN_FILENO);  // Restaurer l'entrée standard
	close(saved_stdin);
}

/* void	ft_redir_in(t_data *data, t_ast *ast)
{
	int		fd;
	int		saved_stdin;
	char	*error_message = NULL;

	saved_stdin = dup(STDIN_FILENO);  // Sauvegarder l'entrée standard

	fd = open(ast->right->args[0], O_RDONLY);
	if (fd < 0)
	{
		error_message = ft_strjoin("minishell: ", ast->right->args[0]);
		error_message = ft_strjoin(error_message, ": ");
		error_message = ft_strjoin(error_message, strerror(errno));
		error_message = ft_strjoin(error_message, "\n");
		global_var = 1;
	}
	else
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		executor(data, ast->left);
		dup2(saved_stdin, STDIN_FILENO);  // Restaurer l'entrée standard
	}
	close(saved_stdin);

	if (error_message != NULL)
	{
		ft_putstr_fd(error_message, 2);
		free(error_message);
	}
} */

// fonction qui gere les actions a effectuer en fonction du type de token

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
		return;
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
