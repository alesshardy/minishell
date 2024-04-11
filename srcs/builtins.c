/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:33:28 by kammi             #+#    #+#             */
/*   Updated: 2024/04/11 12:32:08 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/********************************************ECH0************************************/
int	ft_echo(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	ft_putstr_fd("\n", 1);
	return (0);
}

/********************************************CD************************************/

int	ft_cd(char **args, t_data *data)
{
	char	*cwd;
	char	*tmp;

	printf("args[1] = %s\n", args[1]);
	printf("args[2] = %s\n", args[2]);

	if (!args[1])
	{
		if (chdir(get_var_value(data, "HOME")) == -1)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}

	else if (!args[2])
	{
		if (chdir(args[1]) == -1)
		{
			ft_putstr_fd("minishell: cd: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
			ft_putstr_fd("\n", 2);
			return (1);
		}
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	cwd = static_cwd(UPDATE);
	if (!cwd)
		return (1);
	tmp = data->env[get_var_index(data, "PWD")];
	data->env[get_var_index(data, "PWD")] = ft_strjoin("PWD=", cwd);
	free(tmp);
	tmp = data->env[get_var_index(data, "OLDPWD")];
	data->env[get_var_index(data, "OLDPWD")] = ft_strjoin("OLDPWD=", tmp);
	free(tmp);
	return (0);
}

/********************************************PWD************************************/

char	*static_cwd(int action)
{
	static char	*cwd;

	if (action == FREE)
	{
		free(cwd);
		cwd = NULL;
	}
	else if (!cwd || action == UPDATE)
	{
		free(cwd);
		cwd = NULL;
		cwd = getcwd(cwd, 0);
	}
	return (cwd);
}

int	ft_pwd(void)
{
	char	*cwd;

	cwd = static_cwd(NOTHING);
	if (!cwd)
		return (1);
	printf("%s\n", cwd);
	return (0);
}

char	*get_pwd(void)
{
	char	*cwd;
	char	*tmp;

	cwd = static_cwd(NOTHING);
	if (!cwd)
		return (NULL);
	tmp = cwd;
	cwd = ft_strjoin("\001\033[34m\002", cwd);
	if (!cwd)
		return (NULL);
	tmp = cwd;
	cwd = ft_strjoin(cwd, "\001\033[0m\002\n");
	free(tmp);
	return (cwd);
}

/***************************************EXPORT**************************************/

/******************************************UNSET************************************/

/********************************************ENV************************************/

int	ft_env(t_data *data)
{
	int	i;

	i = 0;
	while (data->env[i])
	{
		ft_putstr_fd(data->env[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

/******************************************EXIT**************************************/
