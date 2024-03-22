/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:36:51 by apintus           #+#    #+#             */
/*   Updated: 2024/03/22 12:02:29 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *delete_exces_space(char *str)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == ' ')
		{
			new_str[j] = str[i];
			j++;
			while (str[i] == ' ')
				i++;
		}
		new_str[j] = str[i];
		i++;
		j++;
	}
	new_str[j] = '\0';
	return (new_str);
}

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

	if (ac != 1)
	{
		printf("Error: too many arguments\n");
		return (1);
	}
	(void)av;
	(void)env;
	data = initialize_data();
	while (1)
	{
		data->prompt = prompt();
		printf("prompt : %s\n", data->prompt);
		data->prompt = delete_exces_space(data->prompt);
		printf("prompt : %s\n", data->prompt);
		check_input(data->prompt);
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
