/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:36:41 by apintus           #+#    #+#             */
/*   Updated: 2024/04/22 12:14:56 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	delete_tmp_files(void)
{
	int i;
	char *filename;

	i = 1;
	while (1)
	{
		filename = ft_strjoin(HERE_DOC_FILE, ft_itoa(i));
		if (access(filename, F_OK) == -1) // If the file does not exist
			break;
		unlink(filename); // Delete the file
		free(filename);
		i++;
	}
}

char	*find_tmp_filename(void) // Trouve un nom de fichier temporaire
{
	char	*filename;
	int		i;

	i = 1;
	while (1)
	{
		filename = ft_strjoin(HERE_DOC_FILE, ft_itoa(i)); // Crée un nom de fichier temporaire
		if (filename == NULL)
			return (NULL);
		if (access(filename, F_OK) == -1) // Si le fichier n'existe pas
			return (filename);
		free(filename);
		i++;
	}
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
	while (1)
	{
		line = readline("> "); // Lit une ligne
		if (line == NULL)
			break ;
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

void	handle_here_doc(t_data *data, t_token **tokens)
{
	t_token	*current;
	t_token	*limiter;
	char	*tmp_file;

	current = *tokens;
	while (current != NULL)
	{
		if (current->type == REDIR_HEREDOC)
		{
			limiter = current->next;
			if (limiter != NULL && limiter->type == LIMITER)
			{
				tmp_file = create_tmp_file(data, limiter->value);
				if (tmp_file != NULL)
				{
					free(limiter->value);
					limiter->value = tmp_file;
					limiter->type = INFILE;
				}
			}
		}
		current = current->next;
	}
}
