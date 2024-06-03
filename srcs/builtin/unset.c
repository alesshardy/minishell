/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:07:37 by apintus           #+#    #+#             */
/*   Updated: 2024/05/21 18:07:59 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/********************************UNSET************************************/

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
