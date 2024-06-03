/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 14:50:42 by apintus           #+#    #+#             */
/*   Updated: 2024/05/27 15:51:59 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*************************CREATE FILENAME*********************************/

char	*create_filename(void)
{
	char	*filename;

	filename = find_tmp_filename();
	if (filename == NULL)
		return (NULL);
	return (filename);
}

int	open_file(t_data *data, char *filename)
{
	data->here_doc_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (data->here_doc_fd == -1)
	{
		free(filename);
		return (-1);
	}
	return (0);
}

char	*handle_null_line(t_data *data, char *filename, char *limiter)
{
	if (g_var == CTRL_C_EXIT)
	{
		close(data->here_doc_fd);
		unlink(filename);
		return (NULL);
	}
	close(data->here_doc_fd);
	eof_display(limiter);
	return (filename);
}

void	write_line_to_file(t_data *data, char *line)
{
	int	len;

	len = ft_strlen(line);
	write(data->here_doc_fd, line, len);
	write(data->here_doc_fd, "\n", 1);
}

int	check_line_against_limiter(char *line, char *limiter)
{
	if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
		&& (ft_strlen(line)) == ft_strlen(limiter))
	{
		free(line);
		return (1);
	}
	return (0);
}
