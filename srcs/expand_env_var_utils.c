/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:53:45 by apintus           #+#    #+#             */
/*   Updated: 2024/05/22 18:55:50 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_value(char *var, t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->name, var) == 0)
		{
			if (tmp->value)
				return (ft_strdup(tmp->value));
			else
				return (ft_strdup(""));
		}
		tmp = tmp->next;
	}
	return (NULL);
}

t_token	*handle_dollar_expan(char *line, size_t *i, t_data *data, size_t *len)
{
	size_t	start;
	char	*env_value;
	t_token	*new_tokens;

	(*i)++;
	start = *i;
	while (line[*i] != '\0' && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (line[start] == '?')
	{
		env_value = ft_itoa(g_var);
		(*i)++;
	}
	else
		env_value = get_env_value(ft_substr(line, start, *i - start),
				data->env);
	if (env_value)
	{
		*len = *len + ft_strlen(env_value);
		new_tokens = tokenizer(env_value, data, false);
		free(env_value);
		return (new_tokens);
	}
	return (NULL);
}
