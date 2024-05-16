/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 14:00:25 by kammi             #+#    #+#             */
/*   Updated: 2024/04/29 16:14:20 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	*get_cmd_path(char **envp, char *cmd)
{
	int		i;
	char	*path;
	char	**split;

	i = 0;
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
			free(path);
			path = ft_find_path(split, cmd);
			if (path)
				return (path);
			break ;
		}
		i++;
	}
	return (ft_strdup(cmd)); //modif pour retourne la commande mauvaise si rien de trouver
}

char	**get_env_array(t_env *env)
{
	char	**env_array;
	int		i;
	t_env	*head;
	char	*tmp;

	head = env;
	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	//printf("i = %d\n", i);//fdebug
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

	if (!array) //ajout
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]); // delete array[i]
		i++;
	}
	free(array); // delete array
}
