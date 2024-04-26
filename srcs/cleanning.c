/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanning.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:48:37 by apintus           #+#    #+#             */
/*   Updated: 2024/04/26 17:44:26 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env != NULL)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	ft_free_data(t_data *data)
{
	if (data->first_token)
	{
		printf("free_tokens\n");
		free_tokens(data->first_token);
		data->first_token = NULL;
	}
	if (data->prompt)
	{
		printf("free_prompt\n");
		free(data->prompt);
		data->prompt = NULL;
	}
	if (data->ast)
	{
		printf("free_ast\n");
		free_ast(data->ast);
		data->ast = NULL;
	}
	//free_array(data->env_array);
}

int	clean_exit(t_data *data)
{
	ft_free_data(data);
	free_env(data->env);
	free(data);
	rl_clear_history();
	exit(global_var);
}
