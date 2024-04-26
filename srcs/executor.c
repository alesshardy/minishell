/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:19:23 by apintus           #+#    #+#             */
/*   Updated: 2024/04/26 18:10:14 by apintus          ###   ########.fr       */
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

void	ft_exec(t_data *data, char **args)
{
	pid_t	pid;
	char	*cmd;
	char	**env_array;
	int		status;

	signal(SIGINT, SIG_DFL);  // Reset to default behavior
	signal(SIGQUIT, SIG_DFL);  // Reset to default behavior

	args[0] = remove_outer_quotes(args[0]);
	args[0] = check_cmd_quotes(args[0]);

	env_array = get_env_array(data->env);

	cmd = get_cmd_path(env_array, args[0]);

	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd, args, env_array) == -1)
		{
			ft_putstr_fd("minishell:", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": ", 2);
			if (errno ==ENOENT)
			{
				ft_putstr_fd("Command not found", 2);
				exit(127);
			}
			else if (errno == EACCES)
			{
				ft_putstr_fd("Permission denied\n", 2);
				exit(126);
			}
			else
				ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
	else
	{
		free_array(env_array); //ajout pour free env_array
		free(cmd); //ajout pour free cmd
		//free_array(args);//ajout pour free args
		waitpid(pid, &status, 0);
		global_var = status >> 8;
	}

	signal(SIGINT, ctrl_c_handler);  // Restore signal handlers
	signal(SIGQUIT, handle_sigquit);  // Restore signal handlers
}

void	ft_pipeline(t_data *data, t_ast *ast)
{
	int		fd[2];
	pid_t	pid;
	pid_t	pid2;
	int		status;

	pipe(fd);
	pid = fork();
	if (pid == 0) //child
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		executor(data, ast->left);
		exit(0);
	}
	else //parent
	{
		pid2 = fork();
		if (pid2 == 0) //child
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			executor(data, ast->right);
			exit(0);
		}
		else //parent
		{
			close(fd[0]);
			close(fd[1]);
			waitpid(pid, &status, 0);
			global_var = status >> 8;
			waitpid(pid2, &status, 0);
			global_var = status >> 8;
		}
	}
}

//ancienne version
/* void	ft_exec(t_data *data, char **args)
{
	pid_t	pid;
	int		status;
	char	*cmd;
	char	**env_array;
	int		i; //a suprrimer si aps laissezla boucle


	signal(SIGINT, SIG_DFL);  // Reset to default behavior
	signal(SIGQUIT, SIG_DFL);  // Reset to default behavior
	i = 0;
	printf("PRECLEAN\n");
	printf("args[0] = %s\n", args[0]);
	args[0] = remove_outer_quotes(args[0]);
	printf("args[0] = %s\n", args[0]);
	args[0] = check_cmd_quotes(args[0]);
	// while(args[i])
	// {
	// 	//ft_putendl_fd(args[i], 2); //fdebug
	// 	args[i] = check_cmd_quotes(args[i]); //faut je verif si on est dans un ARG ou CMD
	// 	printf("args[%d] = %s\n", i, args[i]); //fdebug
	// 	i++;
	// }

	env_array = get_env_array(data->env);

	cmd = get_cmd_path(env_array, args[0]);
	//ft_putendl_fd("je suis dans ft_exec", 2);//fdebug
	//printf("cmd = %s\n", cmd);//fdebug
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd, args, env_array) == -1)
		{
			ft_putstr_fd("minishell:", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": ", 2);
			if (errno ==ENOENT)
				ft_putstr_fd("command not found", 2);
			else
				ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
	else
		waitpid(pid, &status, 0);

	signal(SIGINT, ctrl_c_handler);  // Restore signal handlers
	signal(SIGQUIT, handle_sigquit);  // Restore signal handlers

}

void	ft_pipeline(t_data *data, t_ast *ast)
{
	int		fd[2];
	pid_t	pid;
	int		status;
	int		saved_stdin;

	saved_stdin = dup(STDIN_FILENO);  // Sauvegarder l'entrée standard
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(fd[1], 1);
		close(fd[0]);
		close(fd[1]);
		executor(data, ast->left);
		exit(0);
	}
	else
	{
		dup2(fd[0], 0);
		close(fd[0]);
		close(fd[1]);
		waitpid(pid, &status, 0);
		executor(data, ast->right);
	}
	dup2(saved_stdin, STDIN_FILENO);  // Restaurer l'entrée standard
	close(saved_stdin);
} */

void	ft_redir_out(t_data *data, t_ast *ast)
{
	int		fd;
	//int		status;
	int		saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder la sortie standard

	//ast->right->args[0] = remove_quotes_file(ast->right->args[0]); // remove quotes from file name
	if (ast->type == REDIR_OUT)
		fd = open(ast->right->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(ast->right->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(ast->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return;
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	executor(data, ast->left);

	dup2(saved_stdout, STDOUT_FILENO);  // Restaurer la sortie standard
	close(saved_stdout);
}

// V2
//void	ft_redir_out(t_data *data, t_ast *ast)
// {
//     int		fd;
//     int		saved_stdout;
//     t_ast	*current;

//     saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder la sortie standard

//     current = ast;
//     while (current != NULL && (current->type == REDIR_OUT || current->type == REDIR_APPEND))
//     {
//         current->left->args[0] = remove_quotes_file(current->left->args[0]); // remove quotes from file name
//         if (current->type == REDIR_OUT)
//             fd = open(current->left->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         else
//             fd = open(current->left->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (fd < 0)
//         {
//             ft_putstr_fd("minishell: ", 2);
//             ft_putstr_fd(current->args[0], 2);
//             ft_putstr_fd(": ", 2);
//             ft_putstr_fd(strerror(errno), 2);
//             ft_putstr_fd("\n", 2);
//             return;
//         }
//         dup2(fd, STDOUT_FILENO);
//         close(fd);
//         current = current->right;
//     }

//     if (current != NULL && current->type == CMD)
//         executor(data, current);

//     dup2(saved_stdout, STDOUT_FILENO);  // Restaurer la sortie standard
//     close(saved_stdout);
// }

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
		ft_putstr_fd(ast->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		return;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	executor(data, ast->left);

	dup2(saved_stdin, STDIN_FILENO);  // Restaurer l'entrée standard
	close(saved_stdin);
}

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
	{
		printf("Error: ast is NULL\n");
		//data->last_exit_status = 0;
		return;
	}
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
		else if (ft_strncmp(ast->args[0], "siu", 3) == 0)
			printf("%d\n", global_var);
		else
			ft_exec(data, ast->args);
	}
	else if (ast->type == PIPE)
		ft_pipeline(data, ast);
	else
		handle_redirections(data, ast);
}
