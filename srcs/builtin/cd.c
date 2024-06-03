/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:02:35 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 14:50:26 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*********************************CD************************************/

int	cd_to_home(t_data *data)
{
	if (chdir(get_var_value(data, "HOME")) == -1)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		g_var = 1;
		return (1);
	}
	return (0);
}

int	cd_to_directory(char *dir)
{
	if (chdir(dir) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(dir, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		g_var = 1;
		return (1);
	}
	return (0);
}

void	update_pwd_oldpwd(t_data *data, char *cwd)
{
	t_env	*pwd;
	t_env	*oldpwd;

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
}

int	ft_cd(char **args, t_data *data)
{
	char	*cwd;

	if (!args[1])
	{
		if (cd_to_home(data) == 1)
			return (1);
	}
	else if (!args[2])
	{
		if (cd_to_directory(args[1]) == 1)
			return (1);
	}
	else
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		g_var = 1;
		return (1);
	}
	cwd = static_cwd(UPDATE);
	if (!cwd)
		return (1);
	update_pwd_oldpwd(data, cwd);
	return (0);
}
