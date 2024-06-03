/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:36:41 by apintus           #+#    #+#             */
/*   Updated: 2024/06/03 11:52:07 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	read_and_write_lines(t_data *data, char **filename,
	char *limiter, int saved_stdin)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (line == NULL)
		{
			*filename = handle_null_line(data, *filename, limiter);
			close(data->here_doc_fd);
			if (g_var == CTRL_C_EXIT)
			{
				dup2(saved_stdin, STDIN_FILENO);
				close_all_fds();
				return ;
			}
			return ;
		}
		if (line && check_line_against_limiter(line, limiter))
			break ;
		write_line_to_file(data, line);
		free(line);
	}
}

char	*create_tmp_file(t_data *data, char *limiter)
{
	char	*filename;
	int		saved_stdin;

	filename = create_filename();
	if (filename == NULL)
		return (NULL);
	if (open_file(data, filename) == -1)
		return (NULL);
	signal(SIGINT, &signals_heredoc);
	saved_stdin = dup(STDIN_FILENO);
	read_and_write_lines(data, &filename, limiter, saved_stdin);
	if (filename == NULL)
		return (NULL);
	close(data->here_doc_fd);
	close(saved_stdin);
	return (filename);
}

void	handle_child_process(t_data *data, t_token *limiter)
{
	char	*tmp_file;

	tmp_file = create_tmp_file(data, limiter->value);
	if (tmp_file != NULL)
	{
		free(limiter->value);
		limiter->value = tmp_file;
		limiter->type = INFILE;
	}
}

void	process_current_token(t_data *data, t_token *current)
{
	t_token	*limiter;

	if (current->type == REDIR_HEREDOC)
	{
		limiter = current->next;
		if (limiter != NULL && limiter->type == LIMITER)
		{
			handle_child_process(data, limiter);
		}
	}
}

void	handle_here_doc(t_data *data, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	if (g_var == CTRL_C_EXIT)
		g_var = 0;
	while (current != NULL)
	{
		if (g_var == CTRL_C_EXIT)
			break ;
		process_current_token(data, current);
		current = current->next;
	}
}
