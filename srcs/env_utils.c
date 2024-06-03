/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:00:25 by kammi             #+#    #+#             */
/*   Updated: 2024/05/22 13:54:28 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_var_value(t_data *data, char *var)
{
	t_env	*env;

	env = get_env_var(data, var);
	if (env)
		return (env->value);
	return (NULL);
}

char	*ft_find_path(char **split, char *cmd)
{
	int		i;
	char	*path;
	char	*tmp;

	i = 0;
	while (split[i])
	{
		path = ft_strjoin(split[i], "/");
		tmp = path;
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK | X_OK) == 0)
		{
			free_array(split);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(split);
	return (NULL);
}

char	*get_cmd_path(char **envp, char *cmd, int i)
{
	char	*path;
	char	**split;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			split = ft_split(path, ':');
			if (!split)
				return (free(path), NULL);
			free(path);
			path = ft_find_path(split, cmd);
			if (path)
				return (path);
			break ;
		}
		i++;
	}
	return (ft_strdup(cmd));
}

char	**get_env_array(t_env *env, int i)
{
	char	**env_array;
	t_env	*head;
	char	*tmp;

	head = env;
	while (env)
	{
		i++;
		env = env->next;
	}
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	env = head;
	while (env)
	{
		tmp = ft_strjoin(env->name, "=");
		env_array[i] = ft_strjoin(tmp, env->value);
		free(tmp);
		i++;
		env = env->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
