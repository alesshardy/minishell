/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:36:51 by apintus           #+#    #+#             */
/*   Updated: 2024/03/19 17:24:47 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*prompt()
{
	char	*line;
	line = readline("minishell$> ");
	add_history(line);
	return (line);
}

t_data	*initialize_data()
{
	t_data *data = malloc(sizeof(t_data));
	if (data != NULL)
	{
		data->env = NULL;
		data->prompt = NULL;
		data->tokens = NULL;
		data->token_count = 0;
	}
	return data;
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	(void)ac;
	(void)av;
	data = initialize_data();
	while (1)
	{
		data->prompt = prompt();
		printf("prompt : %s\n", data->prompt);
		if (ft_strncmp(data->prompt, "exit", 4) == 0)
		{
			free(data->prompt);
			break ;
		}
		free(data->prompt);
	}
	free(data);
	return (0);
}
