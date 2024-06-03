/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 14:02:19 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 14:49:09 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/***********************HANDLE UNQUOTE DOLLAR TOKEN*************************/

bool	check_dollar(char *value, int i, bool in_sq, bool in_dq)
{
	if (value[i] == '$' && !in_sq)
	{
		if (value[i + 1] == ' ' || value[i + 1] == '\0'
			|| value[i + 1] == '$'
			|| (in_dq && (value[i + 1] == '"' || value[i + 1] == ' ')))
		{
			return (false);
		}
		else
		{
			return (true);
		}
	}
	return (false);
}

void	remove_cur_token(t_token **tokens, t_token **prev, t_token **current)
{
	if (*prev == NULL)
		*tokens = (*current)->next;
	else
	{
		(*prev)->next = (*current)->next;
		if ((*current)->next != NULL)
			(*current)->next->prev = *prev;
	}
	free((*current)->value);
	free(*current);
	if (*prev == NULL)
		*current = *tokens;
	else
		*current = (*prev)->next;
}

bool	check_dollar_in_token(char *value, bool *in_sq, bool *in_dq)
{
	int		i;
	bool	dollar_found;

	i = 0;
	dollar_found = false;
	while (value[i] != '\0')
	{
		check_quotes(value[i], in_sq, in_dq);
		if (check_dollar(value, i, *in_sq, *in_dq))
		{
			dollar_found = true;
			break ;
		}
		i++;
	}
	return (dollar_found);
}

void	process_cur_token(t_token **tokens, t_token **prev, t_token **current)
{
	bool	in_sq;
	bool	in_dq;
	bool	dollar_found;

	in_sq = false;
	in_dq = false;
	dollar_found = check_dollar_in_token((*current)->value, &in_sq, &in_dq);
	if (dollar_found)
		remove_cur_token(tokens, prev, current);
	else
	{
		*prev = *current;
		*current = (*current)->next;
	}
}

void	remove_unquoted_dollar_tokens(t_token **tokens)
{
	t_token	*prev;
	t_token	*current;

	prev = NULL;
	current = *tokens;
	while (current != NULL)
	{
		process_cur_token(tokens, &prev, &current);
	}
}
