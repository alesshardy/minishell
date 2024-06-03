/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:20:29 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 15:09:55 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/********************************EXPORT*************************************/

int	validate_and_get_env_var(char *arg, t_data *data, t_env **env)
{
	char	*tmp;

	tmp = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	if (!tmp || !is_valid_var_name(tmp))
	{
		free(tmp);
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_var = 1;
		return (1);
	}
	*env = get_env_var(data, tmp);
	free(tmp);
	return (0);
}

void	handle_export_with_equal(char *arg, t_data *data)
{
	t_env	*env;

	if (validate_and_get_env_var(arg, data, &env) == 1)
		return ;
	if (env)
	{
		free(env->value);
		env->value = ft_strdup(ft_strchr(arg, '=') + 1);
	}
	else
		add_env_var(data, arg);
}

void	handle_export_without_equal(char *arg, t_data *data)
{
	t_env	*env;

	if (!is_valid_var_name(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		g_var = 1;
		return ;
	}
	env = get_env_var(data, arg);
	if (env)
		env->value = NULL;
	else
	{
		if (add_env_var(data, arg) != 0)
		{
			return ;
		}
	}
}

int	ft_export(char **args, t_data *data)
{
	int	i;

	if (!args[1])
		return (ft_export_no_args(data));
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			handle_export_with_equal(args[i], data);
		else
			handle_export_without_equal(args[i], data);
		i++;
	}
	return (0);
}
