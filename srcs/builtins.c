/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 18:33:28 by kammi             #+#    #+#             */
/*   Updated: 2024/04/25 17:35:44 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/********************************************ECH0************************************/
// int	ft_echo(char **args)
// {
// 	int	i;

// 	i = 1;
// 	while (args[i])
// 	{
// 		ft_putstr_fd(args[i], 1);
// 		if (args[i + 1])
// 			ft_putstr_fd(" ", 1);
// 		i++;
// 	}
// 	ft_putstr_fd("\n", 1);
// 	return (0);
// }

static bool	only_n(char *str)
{
	int	i;

	i = 0;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(char **cmd)
{
	int		i;
	bool	flag;

	i = 1;
	flag = false;
	while (cmd[i] && cmd[i][0] == '-' && only_n(cmd[i] + 1))
	{
		flag = true;
		i++;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (!flag)
		printf("\n");
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

int	add_env_var(t_data *data, char *var)
{
	t_env	*env;
	t_env	*tmp;
	char	*name;
	char	*value;

	name = ft_substr(var, 0, ft_strchr(var, '=') - var);
	if (!name)
		return (1);
	value = ft_strdup(ft_strchr(var, '=') + 1);
	if (!value)
	{
		free(name);
		return (1);
	}
	env = new_env_node(name, value, 1);
	if (!env)
	{
		free(name);
		free(value);
		return (1);
	}
	tmp = data->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = env;
	env->prev = tmp;
	return (0);
}

static char	**duplicate_env(t_data *data)
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

static void	sort_env(char **env)
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
			if (strcmp(env[i], env[j]) > 0)
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

static int	ft_export_no_args(t_data *data)
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
int	ft_export(char **args, t_data *data)
{
	int		i;
	char	*tmp;
	t_env	*env;

	if (!args[1])
		return (ft_export_no_args(data));
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			tmp = ft_substr(args[i], 0, ft_strchr(args[i], '=') - args[i]);
			if (!tmp)
				return (1);
			env = get_env_var(data, tmp);
			if (env)
			{
				free(env->value);
				env->value = ft_strdup(ft_strchr(args[i], '=') + 1);
			}
			else
				add_env_var(data, args[i]);
			free(tmp);
		}
		else
		{
			env = get_env_var(data, args[i]);
			if (env)

				env->value = NULL;
			else
			{
				// Ajoutez une vérification ici pour vous assurer que add_env_var a réussi
				if (add_env_var(data, args[i]) != 0)
				{
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}

/******************************************UNSET************************************/

int	ft_unset(char **args, t_data *data)
{
	int		i;
	t_env	*env;
	t_env	*tmp;

	i = 1;
	while (args[i])
	{
		env = get_env_var(data, args[i]);
		if (env)
		{
			if (env->prev)
				env->prev->next = env->next;
			if (env->next)
				env->next->prev = env->prev;
			tmp = env->next;
			free(env->name);
			free(env->value);
			free(env);
			env = tmp;
		}
		i++;
	}
	return (0);
}

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
