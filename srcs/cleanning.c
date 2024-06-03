/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanning.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:48:37 by apintus           #+#    #+#             */
/*   Updated: 2024/05/23 15:15:53 by apintus          ###   ########.fr       */
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

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast->args);
	free(ast);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens != NULL)
	{
		tmp = tokens;
		tokens = tokens->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	ft_free_data(t_data *data)
{
	if (data->first_token)
	{
		free_tokens(data->first_token);
		data->first_token = NULL;
	}
	if (data->prompt)
	{
		free(data->prompt);
		data->prompt = NULL;
	}
	if (data->ast)
	{
		free_ast(data->ast);
		data->ast = NULL;
	}
}

int	clean_exit(t_data *data)
{
	ft_free_data(data);
	static_cwd(FREE);
	free_env(data->env);
	free(data);
	rl_clear_history();
	ft_putstr_fd("exit\n", 1);
	exit(g_var);
}
