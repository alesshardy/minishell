/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 15:22:37 by apintus           #+#    #+#             */
/*   Updated: 2024/05/22 15:26:16 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**********************HERE DOC UTILS**************************/

void	delete_tmp_files(void)
{
	int		i;
	char	*tmp_itoa;
	char	*filename;

	i = 1;
	while (1)
	{
		tmp_itoa = ft_itoa(i);
		filename = ft_strjoin(HERE_DOC_FILE, tmp_itoa);
		if (access(filename, F_OK) == -1)
		{
			free(tmp_itoa);
			free(filename);
			break ;
		}
		unlink(filename);
		free(tmp_itoa);
		free(filename);
		i++;
	}
}

char	*find_tmp_filename(void)
{
	char	*filename;
	int		i;
	char	*tmp_itoa;

	i = 1;
	while (1)
	{
		tmp_itoa = ft_itoa(i);
		filename = ft_strjoin(HERE_DOC_FILE, tmp_itoa);
		free(tmp_itoa);
		if (filename == NULL)
			return (NULL);
		if (access(filename, F_OK) == -1)
			return (filename);
		free(filename);
		i++;
	}
}

void	eof_display(char *limiter)
{
	ft_putstr_fd("minishell: warning: here-document.\
delimited by end-of-file (wanted `", 2);
	ft_putstr_fd(limiter, 2);
	ft_putstr_fd("')\n", 2);
}
