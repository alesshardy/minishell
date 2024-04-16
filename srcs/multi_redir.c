/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 15:24:14 by apintus           #+#    #+#             */
/*   Updated: 2024/04/16 17:01:13 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	reset_count_redirection(t_data *data)
{
	data->count_redir_in = 0;
	data->count_redir_out = 0;
}

void	count_redirection(t_data *data, t_ast **ast)
{
	t_ast	*tmp;

	tmp = *ast;
	while (tmp)
	{
		if (tmp->type == REDIR_IN || tmp->type == REDIR_HEREDOC)
			data->count_redir_in++;
		if (tmp->type == REDIR_OUT || tmp->type == REDIR_APPEND)
		{
			if (data->count_redir_out == 0)
				data->first_redir_out = tmp;
			data->count_redir_out++;
		}
		count_redirection(data, &tmp->left);
		count_redirection(data, &tmp->right);
		break;
	}
}

void handle_multi_redir(t_data *data, t_ast *ast)
{
	(void)data;
	(void)ast;
	printf("handle_multi_redir\n");
}



