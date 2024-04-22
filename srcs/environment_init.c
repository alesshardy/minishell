/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:23:36 by kammi             #+#    #+#             */
/*   Updated: 2024/04/22 18:14:48 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/***********************************ANCIEN ENV TABLEAU ** ***************************/

// fonction dup_env :
/* char	**dup_env(char **env)
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

} */

/*************************************ENV LISTE CHAINEE*******************************/


t_env	*get_env_var(t_data *data, char *var)
{
	t_env	*env;
	int i = 0;

	env = data->env;
	while (env)
	{
		i++;
		if (ft_strcmp(env->name, var) == 0)
			return (env);
		env = env->next;
	}
	//dprintf(2, "%d\n", i);//fdebug
	return (NULL);
}

char	*get_var_value(t_data *data, char *var)
{
	t_env	*env;

	env = get_env_var(data, var);
	if (env)
		return (env->value);
	return (NULL);
}

t_env	*new_env_node(char *name, char *value, int equal_sign)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->name = name;
	new_node->value = value;
	new_node->equal_sign = equal_sign;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	add_env_node(t_env **env, t_env *new_node)
{
	t_env	*last;

	if (*env == NULL)
	{
		*env = new_node;
		return ;
	}
	last = *env;
	while (last->next)
		last = last->next;
	last->next = new_node;
	new_node->prev = last;
}

t_env	*init_env(t_data *data, char **env)
{
	int		i;
	char	*equal_pos;
	t_env	*new_node;

	i = 0;
	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos)
		{
			new_node = new_env_node(ft_substr(env[i], 0, equal_pos - env[i]),
										ft_strdup(equal_pos + 1), 1);
		}
		else
		{
			new_node = new_env_node(ft_strdup(env[i]), NULL, 0);
		}
		add_env_node(&(data->env), new_node);
		i++;
	}
	//imprimer env
	// while (data->env)
	// {
	// 	printf("name: %s\n", data->env->name);
	// 	printf("value: %s\n", data->env->value);
	// 	printf("equal_sign: %d\n", data->env->equal_sign);
	// 	data->env = data->env->next;
	// }
	return (data->env);
}
