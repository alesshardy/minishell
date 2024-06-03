/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:23:36 by kammi             #+#    #+#             */
/*   Updated: 2024/05/22 13:47:50 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/****************************ENV LISTE CHAINEE****************************/

t_env	*get_env_var(t_data *data, char *var)
{
	t_env	*env;
	int		i;

	i = 0;
	env = data->env;
	while (env)
	{
		i++;
		if (ft_strcmp(env->name, var) == 0)
			return (env);
		env = env->next;
	}
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

void	handle_shlvl(t_data *data)
{
	t_env	*shlvl_node;
	int		shlvl_value;

	shlvl_node = get_env_var(data, "SHLVL");
	if (shlvl_node && shlvl_node->value)
	{
		shlvl_value = ft_atoi(shlvl_node->value);
		free(shlvl_node->value);
		shlvl_node->value = ft_itoa(shlvl_value + 1);
	}
	else
		add_env_node(&(data->env),
			new_env_node(ft_strdup("SHLVL"), ft_strdup("1"), 1));
}

// i initialiser dans minishell.c c'est moche mais bon
t_env	*init_env(t_data *data, char **env, int i)
{
	char	*equal_pos;
	t_env	*new_node;

	while (env[i])
	{
		equal_pos = ft_strchr(env[i], '=');
		if (equal_pos)
		{
			new_node = new_env_node(ft_substr(env[i], 0, equal_pos - env[i]),
					ft_strdup(equal_pos + 1), 1);
			if (!new_node)
				return (NULL);
		}
		else
		{
			new_node = new_env_node(ft_strdup(env[i]), NULL, 0);
			if (!new_node)
				return (NULL);
		}
		add_env_node(&(data->env), new_node);
		i++;
	}
	handle_shlvl(data);
	return (data->env);
}
