/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:38:24 by apintus           #+#    #+#             */
/*   Updated: 2024/05/22 12:35:45 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	initialize_var_mis_redir(int *i, int *sq_count, int *dq_count)
{
	*i = 0;
	*sq_count = 0;
	*dq_count = 0;
}

int	misplace_redirection(char *line)
{
	int	i;
	int	sq_count;
	int	dq_count;

	initialize_var_mis_redir(&i, &sq_count, &dq_count);
	while (line[i] != '\0')
	{
		update_quote_count(line[i], &sq_count, &dq_count);
		if ((line[i] == '<' || line[i] == '>') && sq_count % 2 == 0
			&& dq_count % 2 == 0)
		{
			i++;
			if (line[i - 1] == line[i])
				i++;
			while ((line[i] != '\0') && (line[i] == ' ' || line[i] == '\t'
					|| line[i] == '\n' || line[i] == '\v'
					|| line[i] == '\f' || line[i] == '\r'))
				i++;
			if (line[i] == '\0' || line[i] == '|' || line[i] == '&'
				|| line[i] == '<' || line[i] == '>')
				return (1);
		}
		i++;
	}
	return (0);
}

void	initialize_var_mis_op(int *i, int *sq, int *dq, int *cmd_flag)
{
	*i = 0;
	*sq = 0;
	*dq = 0;
	*cmd_flag = 0;
}

int	misplace_operator(char *line)
{
	int	i;
	int	sq;
	int	dq;
	int	cmd_flag;

	initialize_var_mis_op(&i, &sq, &dq, &cmd_flag);
	if (line[i] == '|' || line[i] == '&')
		return (1);
	while (line[i] != '\0')
	{
		update_quote_count(line[i], &sq, &dq);
		if ((line[i] == '|' || line[i] == '&') && sq % 2 == 0 && dq % 2 == 0)
		{
			if (cmd_flag == 1)
				return (1);
			cmd_flag = 1;
		}
		else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i]
			!= '\v' && line[i] != '\f' && line[i] != '\r')
			cmd_flag = 0;
		i++;
	}
	if (cmd_flag == 1)
		return (1);
	return (0);
}
