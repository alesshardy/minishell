/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:31:27 by apintus           #+#    #+#             */
/*   Updated: 2024/04/02 13:59:59 by apintus          ###   ########.fr       */
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

int	misplace_redirection(char *line)
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
		if ((line[i] == '<' || line[i] == '>') && sq_count % 2 == 0 && dq_count % 2 == 0)
		{
			i++;
			if (line[i - 1] == line[i])
				i++;
			while ((line[i] != '\0') && (line[i] == ' ' || line[i] == '\t'
					|| line[i] == '\n' || line[i] == '\v' || line[i] == '\f' || line[i] == '\r'))
				i++;
			if (line[i] == '\0' || line[i] == '|' || line[i] == '&' || line[i] == '<' || line[i] == '>')
				return (1);
		}
		i++;
	}
	return (0);
}

int	misplace_operator(char *line)
{
	int	i;
	int	sq_count;
	int	dq_count;
	int	cmd_flag;

	i = 0;
	sq_count = 0;
	dq_count = 0;
	cmd_flag = 0;
	if (line[i] == '|' || line[i] == '&')
		return (1);
	while (line[i] != '\0')
	{
		//printf("line[%d] = %c\t cmd_flag = %d\n", i, line[i], cmd_flag);
		update_quote_count(line[i], &sq_count, &dq_count);
		if ((line[i] == '|' || line[i] == '&') && sq_count % 2 == 0 && dq_count % 2 == 0)
		{
			if (cmd_flag == 1) // si pas de cmd apres pipe
				return (1);
			cmd_flag = 1;
		}
		else if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\v' && line[i] != '\f' && line[i] != '\r')
			cmd_flag = 0;
		i++;
	}
	if (cmd_flag == 1)
		return (1);
	return (0);
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
		return(ft_putstr_fd("Error: Unclosed quotes\n", 2), 1);
	if (logical_operator(line) == 1)
		return (ft_putstr_fd("Error: Logical operators '&&' and '||' and ';' are not supported.\n", 2), 1);
	if (misplace_operator(line) == 1)
		return (ft_putstr_fd("Error: Misplaced operator\n", 2), 1);
	if (misplace_redirection(line) == 1)
		return (ft_putstr_fd("Error: Misplaced redirection\n", 2), 1);
	return (0);
}
