/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:23:36 by kammi             #+#    #+#             */
/*   Updated: 2024/04/10 13:15:59 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// fonction dup_env :
char	**dup_env(char **env)
{
	char	**new_env;
	int		i;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	init_shell_env_struct(t_data *data, char **env)
{
	int	i;
	int	i_var;
	int	i_value;

	data->env = dup_env(env);
	i = 0;
	while (env[i])
		i++;

	data->parsed_env = malloc(sizeof(char ***) * (i + 1));
	if (!data->parsed_env)
		return ;
	i_var = 0;
	while (i_var < i)
	{
		data->parsed_env[i_var] = malloc(sizeof(char **) * 2);
		if (!data->parsed_env[i_var])
			return ;
		i_value = 0;
		while (env[i_var][i_value] != '=')
			i_value++;
		data->parsed_env[i_var][0] = ft_substr(env[i_var], 0, i_value);
		data->parsed_env[i_var][1] = ft_substr(env[i_var], i_value + 1, ft_strlen(env[i_var]) - i_value);
		i_var++;
	}
	data->parsed_env[i_var] = NULL;
}

int	get_var_index(t_data *data, char *var)
{
	int	i;

	i = 0;
	while (data->parsed_env[i])
	{
		if (ft_strcmp(data->parsed_env[i][0], var) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	*get_var_value(t_data *data, char *var)
{
	int	i;

	i = get_var_index(data, var);
	if (i >= 0)
		return (data->parsed_env[i][1]);
	return (NULL);
}

void	init_shell_env(t_data *data, char **env)
{
	int	i;
	int index_var;

	printf("init_shell_env\n");
	if (env == NULL)
		return ;
	init_shell_env_struct(data, env);
	printf("init_shell_env 1\n");
	index_var = get_var_index(data, "SHLVL");
	printf("init_shell_env 2\n");
	i = 0;
	if (index_var >= 0)
	{
		data->parsed_env[index_var][1] = ft_itoa(ft_atoi(data->parsed_env[index_var][1]) + 1);
	}
	else
	{
		while (data->parsed_env[i])
			i++;
		data->parsed_env[i] = malloc(sizeof(char **) * 2);
		data->parsed_env[i][0] = ft_strdup("SHLVL");
		data->parsed_env[i][1] = ft_strdup("1");
		data->parsed_env[i + 1] = NULL;
	}

}
