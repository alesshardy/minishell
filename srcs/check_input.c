/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:31:27 by apintus           #+#    #+#             */
/*   Updated: 2024/05/21 18:44:49 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_quote_count(char c, int *sq_count, int *dq_count)
{
	if (c == '\'')
		*sq_count += 1;
	if (c == '\"')
		*dq_count += 1;
}

int	logical_operator(char *line)
{
	int	i;
	int	sq_count;
	int	dq_count;

	i = 0;
	sq_count = 0;
	dq_count = 0;
	while (line[i] != '\0')
	{
		update_quote_count(line[i], &sq_count, &dq_count);
		if (sq_count % 2 == 0 && dq_count % 2 == 0)
		{
			if (line[i] == '&' && line[i + 1] == '&')
				return (1);
			if (line[i] == '|' && line[i + 1] == '|')
				return (1);
			if (line[i] == ';')
				return (1);
		}
		i++;
	}
	return (0);
}

int	unclosed_quotes(char *line)
{
	int	quote;
	int	i;

	quote = 0;
	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote == 0)
				quote = line[i];
			else if (quote == line[i])
				quote = 0;
		}
		i++;
	}
	if (quote != 0)
		return (1);
	return (0);
}

int	check_input(char *line)
{
	if (unclosed_quotes(line) == 1)
		return (ft_putstr_fd("Error: Unclosed quotes\n", 2), 1);
	if (logical_operator(line) == 1)
		return (ft_putstr_fd("Error: Logical operators '&&' and \
'||' and ';' are not supported.\n", 2), 1);
	if (misplace_operator(line) == 1)
		return (ft_putstr_fd("minishell: syntax error \
near unexpected token `|'\n", 2), 1);
	if (misplace_redirection(line) == 1)
		return (ft_putstr_fd("minishell: syntax error near \
unexpected token `newline'\n", 2), 1);
	return (0);
}
