/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 13:38:30 by apintus           #+#    #+#             */
/*   Updated: 2024/05/23 13:55:59 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//main token post processing function
void	post_process_tokens(t_token **tokens, t_data *data, bool expand)
{
	if (expand)
		redefine_dollar(tokens, data);
	remove_unquoted_dollar_tokens(tokens);
	redefine_word_token(*tokens);
	redefine_cmd_token(*tokens);
	remove_outer_quotes_from_tokens(*tokens);
}

/******************************redefine_token*****************************/

void	redefine_word_token(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		if (token->type == WORD)
		{
			if (token->prev && token->prev->type == REDIR_IN)
				token->type = INFILE;
			else if (token->prev && token->prev->type == REDIR_OUT)
				token->type = OUTFILE;
			else if (token->prev && token->prev->type == REDIR_APPEND)
				token->type = OUTFILE;
			else if (token->prev && token->prev->type == REDIR_HEREDOC)
				token->type = LIMITER;
			else
				token->type = CMD;
		}
		token = token->next;
	}
}

void	redefine_cmd_token(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		if (token->type == CMD && token->prev && token->prev->type == CMD)
			token->type = ARG;
		else if (token->type == CMD && token->prev && token->prev->type == ARG)
			token->type = ARG;
		token = token->next;
	}
}

void	remove_outer_quotes_from_tokens(t_token *tokens)
{
	t_token	*current;
	char	*new_word;

	current = tokens;
	while (current != NULL)
	{
		if (current->type == ARG || current->type == OUTFILE
			|| current->type == INFILE)
		{
			new_word = remove_outer_quotes(current->value);
			free(current->value);
			current->value = new_word;
		}
		current = current->next;
	}
}

//token_utils3.c
void	check_quotes(char c, bool *in_sq, bool *in_dq)
{
	if (c == '\'')
		*in_sq = !(*in_sq);
	else if (c == '"')
		*in_dq = !(*in_dq);
}
