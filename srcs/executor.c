/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:19:23 by apintus           #+#    #+#             */
/*   Updated: 2024/04/19 15:49:22 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*********************************ADJUST FILE AND CMD********************************/

int	is_white_space(char *str)
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
        return NULL;
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
        {
            new_str[j++] = str[i++];
        }
        new_str[j] = '\0';
    }
    free(str);
    return new_str;
}

/*************************************EXEC*************************************/

void	ft_exec(t_data *data, char **args)
{
	pid_t	pid;
	int		status;
	char	*cmd;
	int		i;

	i = 0;
	while(args[i])
	{
		args[i] = check_cmd_quotes(args[i]);
		//printf("args[%d] = %s\n", i, args[i]); //fdebug
		i++;
	}
	cmd = ft_strjoin(ft_strncmp(data->env[0], "PATH=", 5) == 0 ? data->env[0] + 5 : "/bin/", args[0]);
	//printf("cmd = %s\n", cmd); //fdebug
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd, args, data->env) == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(args[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			exit(1);
		}
	}
	else
		waitpid(pid, &status, 0);

}

void	ft_pipe(t_data *data, t_ast *ast)
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
}

void	ft_redir_out(t_data *data, t_ast *ast)
{
	int		fd;
	//int		status;
	int		saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);  // Sauvegarder la sortie standard

	ast->right->args[0] = remove_quotes_file(ast->right->args[0]); // remove quotes from file name
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

	ast->right->args[0] = remove_quotes_file(ast->right->args[0]); // remove quotes from file name
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
    if (ast == NULL) {
        printf("Error: ast is NULL\n");
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
			return ;
		else if (ft_strncmp(ast->args[0], "unset", 5) == 0)
			return ;
		else if (ft_strncmp(ast->args[0], "env", 3) == 0)
			ft_env(data);
		else
			ft_exec(data, ast->args);
	}
	else if (ast->type == PIPE)
		ft_pipe(data, ast);
	// else if (ast->type == REDIR_OUT || ast->type == REDIR_APPEND)
	// 	ft_redir_out(data, ast);
	// else if (ast->type == REDIR_IN)
	// 	ft_redir_in(data, ast);
	else
		handle_redirections(data, ast);
	// else
	// {
	// 	printf("Error: unknown ast type\n");
	// 	return;
	// }
}

