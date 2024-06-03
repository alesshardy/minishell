/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:01:06 by apintus           #+#    #+#             */
/*   Updated: 2024/05/21 18:04:06 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************ECH0************************************/

static bool	only_n(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 1;
	flag = false;
	while (cmd[i] && cmd[i][0] == '-' && only_n(cmd[i] + 1))
	{
		flag = true;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
	g_var = 0;
	return (0);
}
