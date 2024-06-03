/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:36:51 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 14:48:10 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	g_var = 0;

/******************************MAIN*******************************/

// prompt avec gestion de CTRL + D
// Et gestion de cas particulier (! : #)
char	*prompt(t_data *data)
{
	char	*line;

	line = readline(READLINE_MSG);
	if (line == NULL)
	{
		clean_exit(data);
	}
	else if (line[0] == '\0' || !ft_strcmp(line, ":")
		|| !ft_strcmp(line, "#") || !ft_strcmp(line, "!"))
	{
		if (!ft_strcmp(line, ":") || !ft_strcmp(line, "#"))
			g_var = 0;
		if (!ft_strcmp(line, "!"))
			g_var = 1;
		free(line);
		return (ft_strdup(""));
	}
	add_history(line);
	return (line);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = initialize_program(ac, av, env);
	run_program(data);
	free(data);
	return (0);
}
