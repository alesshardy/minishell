/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:31:27 by apintus           #+#    #+#             */
/*   Updated: 2024/03/21 16:58:17 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	misplace_operator(char *line)
{
	int	i;

	i = 0;
	if (line[i] == '|' || line[i] == '&')
		return (1);
	while (line[i] != '\0')
	{
		//a completer
	}
	return (0);
}

int	logical_operator(char *line)
{
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '&' && line[i + 1] == '&')
			return (1);
		if (line[i] == '|' && line[i + 1] == '|')
			return (1);
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
	{
		ft_putstr_fd("Error: Unclosed quotes\n", 2);
		return (1);
	}
	if (logical_operator(line) == 1)
	{
		ft_putstr_fd("Error: Logical operators '&&' and '||' are not supported.\n", 2);
		return (1);
	}
	if (misplace_operator(line) == 1)
	{
		ft_putstr_fd("Error: Misplaced operator\n", 2);
		return (1);
	}
	return (0);
}
