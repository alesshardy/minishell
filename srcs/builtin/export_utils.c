/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:22:42 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 15:10:05 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************EXPORT UTILS*************************************/
// Les noms de variables ne peuvent pas commencer par un chiffre
// Les noms de variables ne peuvent contenir
//  que des caractères alphanumériques et des soulignements
int	is_valid_var_name(char *var)
{
	int	i;

	if (!var || !*var || ft_isdigit(*var))
		return (0);
	i = 0;
	while (var[i])
	{
		if (!ft_isalnum(var[i]) && var[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	add_env_var(t_data *data, char *var)
{
	t_env	*env;
	t_env	*tmp;
	char	*name;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(var, '=');
	name = ft_substr(var, 0, equal_sign - var);
	if (!name)
		return (1);
	if (equal_sign)
		value = ft_strdup(equal_sign + 1);
	else
		value = NULL;
	if (!value && equal_sign)
		return (free(name), 1);
	env = new_env_node(name, value, 1);
	if (!env)
		return (free(name), free(value), 1);
	tmp = data->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = env;
	env->prev = tmp;
	return (0);
}

char	**duplicate_env(t_data *data)
{
	t_env	*env;
	int		count;
	char	**names;
	int		i;

	count = 0;
	env = data->env;
	while (env)
	{
		count++;
		env = env->next;
	}
	names = malloc(sizeof(char *) * (count + 1));
	i = 0;
	env = data->env;
	while (env)
	{
		names[i++] = env->name;
		env = env->next;
	}
	names[i] = NULL;
	return (names);
}

void	sort_env(char **env)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
			{
				tmp = env[i];
				env[i] = env[j];
				env[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	ft_export_no_args(t_data *data)
{
	char	**names;
	int		i;
	t_env	*env;

	names = duplicate_env(data);
	sort_env(names);
	i = 0;
	while (names[i])
	{
		env = get_env_var(data, names[i]);
		printf("export -x %s", names[i]);
		if (env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		i++;
	}
	free(names);
	return (0);
}
