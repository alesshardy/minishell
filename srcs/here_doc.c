/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:36:41 by apintus           #+#    #+#             */
/*   Updated: 2024/05/15 15:22:12 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	delete_tmp_files(void)
{
	int i;
	char *tmp_itoa;
	char *filename;

	i = 1;
	while (1)
	{
		tmp_itoa = ft_itoa(i);
		filename = ft_strjoin(HERE_DOC_FILE, tmp_itoa);
		if (access(filename, F_OK) == -1) // If the file does not exist
		{
			free(tmp_itoa);
			free(filename);
			break;
		}
		unlink(filename); // Delete the file
		free(tmp_itoa);
		free(filename);
		i++;
	}
}

char	*find_tmp_filename(void) // Trouve un nom de fichier temporaire
{
	char	*filename;
	int		i;
	char	*tmp_itoa;

	i = 1;
	while (1)
	{
		tmp_itoa = ft_itoa(i); // Convertit un int en string
		filename = ft_strjoin(HERE_DOC_FILE, tmp_itoa); // Crée un nom de fichier temporaire
		free(tmp_itoa);
		if (filename == NULL)
			return (NULL);
		if (access(filename, F_OK) == -1) // Si le fichier n'existe pas
			return (filename);
		free(filename);
		i++;
	}
}


void	eof_display(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putstr_fd("')\n", 2);
}

char	*create_tmp_file(t_data *data, char *limiter)
{
	char	*filename;
	char	*line;
	int		len;


	filename = find_tmp_filename();
	if (filename == NULL)
		return (NULL);
	data->here_doc_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644); // Ouvre le fichier temporaire
	if (data->here_doc_fd == -1)
	{
		free(filename);
		return (NULL);
	}
	signal(SIGINT, &signals_heredoc);
	while (1)
	{

		line = readline("> "); // Lit une ligne
		if (line == NULL)
		{
			if (global_var == CTRL_C_EXIT)
			{
				close(data->here_doc_fd);
				unlink(filename);
				return (NULL);
			}
			close(data->here_doc_fd);
			eof_display(limiter);
			return (filename);
		}
		len = ft_strlen(line);
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& (ft_strlen(line)) == ft_strlen(limiter)) // Si la ligne est égale au délimiteur
		{
			free(line);
			break ;
		}
		write(data->here_doc_fd, line, len); // Écrit la ligne dans le fichier temporaire
		write(data->here_doc_fd, "\n", 1); // Écrit un retour à la ligne
		free(line);
	}
	close(data->here_doc_fd); // Ferme le fichier temporaire
	return (filename);
}

// void	handle_here_doc(t_data *data, t_token **tokens)
// {
// 	t_token	*current;
// 	t_token	*limiter;
// 	char	*tmp_file;

// 	current = *tokens;
// 	while (current != NULL)
// 	{
// 		if (current->type == REDIR_HEREDOC)
// 		{
// 			limiter = current->next;
// 			if (limiter != NULL && limiter->type == LIMITER)
// 			{
// 				tmp_file = create_tmp_file(data, limiter->value);
// 				if (tmp_file != NULL)
// 				{
// 					free(limiter->value);
// 					limiter->value = tmp_file;
// 					limiter->type = INFILE;
// 				}
// 			}
// 		}
// 		current = current->next;
// 	}
// }

void	handle_here_doc(t_data *data, t_token **tokens)
{
	t_token	*current;
	t_token	*limiter;
	char	*tmp_file;
	pid_t	pid;

	current = *tokens;
	while (current != NULL)
	{
		if (current->type == REDIR_HEREDOC)
		{
			limiter = current->next;
			if (limiter != NULL && limiter->type == LIMITER)
			{
				pid = fork();
				if (pid < 0)
				{
					perror("fork failed");
					exit(EXIT_FAILURE);
				}
				if (pid == 0) // This is the child process
				{
					signal(SIGINT, child_ctrl_c);
					signal(SIGQUIT, child_ctrl_c);
					tmp_file = create_tmp_file(data, limiter->value);
					if (tmp_file != NULL)
					{
						free(limiter->value);
						limiter->value = tmp_file;
						limiter->type = INFILE;
					}
					exit(EXIT_SUCCESS);
				}
				else // This is the parent process
				{
					signal(SIGINT, &signals_heredoc_parents);
					wait(NULL); // Wait for the child process to finish
				}
				signal(SIGINT, ctrl_c_handler);  // Restore signal handlers
				signal(SIGQUIT, handle_sigquit);  // Restore signal handlers
			}
		}
		current = current->next;
	}
}
