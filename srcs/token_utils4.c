/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 18:50:45 by apintus           #+#    #+#             */
/*   Updated: 2024/05/22 19:03:40 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_dollar(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

int	has_only_dollar(char *line)
{
	size_t	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] != '$')
			return (0);
		i++;
	}
	return (1);
}

void	update_token_links(t_token *tmp, t_token *new_tokens, t_token **tokens)
{
	t_token	*last_new_token;

	last_new_token = get_last_token(new_tokens);
	last_new_token->next = tmp->next;
	if (tmp->prev)
		tmp->prev->next = new_tokens;
	else
		*tokens = new_tokens;
	new_tokens->prev = tmp->prev;
	if (tmp->next)
		tmp->next->prev = last_new_token;
}

void	redefine_dollar(t_token **tokens, t_data *data)
{
	t_token	*tmp;
	t_token	*new_tokens;
	t_token	*next_tmp;

	tmp = *tokens;
	while (tmp != NULL)
	{
		if (has_dollar(tmp->value) && !has_only_dollar(tmp->value))
		{
			new_tokens = expand_env(tmp->value, data);
			if (new_tokens)
			{
				update_token_links(tmp, new_tokens, tokens);
				next_tmp = tmp->next;
				free(tmp->value);
				free(tmp);
				tmp = next_tmp;
				continue ;
			}
		}
		tmp = tmp->next;
	}
}
