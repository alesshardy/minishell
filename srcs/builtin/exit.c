/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:10:33 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 15:13:56 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/****************************EXIT***************************************/

int	check_numeric_argument(char *arg, t_data *data)
{
	int	i;

	i = 0;
	if (arg[0] == '-' || arg[0] == '+')
		i = 1;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arg, 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			g_var = 2;
			return (clean_exit(data));
		}
		i++;
	}
	return (0);
}

int	check_multiple_arguments(char **args, t_data *data)
{
	(void)data;
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		g_var = 1;
		return (1);
	}
	return (0);
}

int	calculate_exit_code(char *arg, t_data *data)
{
	int	exit_code;

	exit_code = ft_atoi(arg);
	if (exit_code < 0)
		exit_code = 256 + exit_code;
	else if (exit_code > 255)
		exit_code = exit_code % 256;
	g_var = exit_code;
	return (clean_exit(data));
}

int	ft_exit(char **args, t_data *data)
{
	if (args[1])
	{
		if (check_numeric_argument(args[1], data) != 0)
			return (1);
		if (check_multiple_arguments(args, data) != 0)
			return (1);
		return (calculate_exit_code(args[1], data));
	}
	return (clean_exit(data));
}
