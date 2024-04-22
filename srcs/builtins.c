/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:33:28 by kammi             #+#    #+#             */
/*   Updated: 2024/04/22 18:06:40 by apintus          ###   ########.fr       */
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

int		ft_cd(char **args, t_data *data)
{
	char	*cwd;
	t_env	*pwd;
	t_env	*oldpwd;

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
	pwd = get_env_var(data, "PWD");
	if (pwd)
	{
		free(pwd->value);
		pwd->value = ft_strjoin("PWD=", cwd);
	}
	oldpwd = get_env_var(data, "OLDPWD");
	if (oldpwd)
	{
		free(oldpwd->value);
		oldpwd->value = ft_strjoin("OLDPWD=", cwd);
	}
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
	t_env	*env;

	env = data->env;
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd(env->name, 1);
			ft_putstr_fd("=", 1);
			ft_putstr_fd(env->value, 1);
			ft_putstr_fd("\n", 1);
		}
		env = env->next;
	}
	return (0);
}

/******************************************EXIT**************************************/

int	ft_exit(char **args, t_data *data)
{
	int	i;
	int	exit_code;

	i = 0;
	if (args[1])
	{
		if (args[1][0] == '-')
			i = 1;
		while (args[1][i])
		{
			if (!ft_isdigit(args[1][i]))
			{
				ft_putstr_fd("minishell: exit: ", 2);
				ft_putstr_fd(args[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				exit(2);
			}
			i++;
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		exit_code = ft_atoi(args[1]);
		if (exit_code < 0)
			exit_code = 256 + exit_code;
		else if (exit_code > 255)
			exit_code = exit_code % 256;
		exit(exit_code);
	}
	exit(data->exit_status);
}
