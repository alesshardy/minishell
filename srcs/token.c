/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:15:43 by apintus           #+#    #+#             */
/*   Updated: 2024/03/29 12:17:32 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/***************************************utiles token **************************************************/

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens != NULL)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*tmp;

	if (*tokens == NULL)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (token == NULL)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

/***************************************tokenizer**************************************************/

//handle (|, <, << , >, >>)
void	handle_redirection(char **input, t_token **tokens)
{
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			add_token(tokens, create_token( "<<", REDIR_HEREDOC));//handle heredoc
			(*input)++;
		}
		else
			add_token(tokens, create_token( "<", REDIR_IN));//handle redirection in
	}
	else if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			add_token(tokens, create_token( ">>", REDIR_APPEND));//handle redirection append
			(*input)++;
		}
		else
			add_token(tokens, create_token( ">", REDIR_OUT));//handle redirection out
	}
	else if (**input == '|')
		add_token(tokens, create_token( "|", PIPE));//handle pipe
	(*input)++;
}

//handle word

void	handle_word(char **input, t_token **tokens)
{
	char	*start;
	int		in_quotes;
	char	quote;

	start = *input;
	in_quotes = 0;
	quote = '\0';
	while(**input)
	{
		if (in_quotes == 0 && (**input == '\'' || **input == '\"'))
		{
			in_quotes = 1;
			quote = **input;
		}
		else if (in_quotes == 1 && **input == quote)
			in_quotes = 0;
		else if (in_quotes == 0 && (**input == ' ' || **input == '\t'
					|| **input == '\n' || **input == '\v' || **input == '\f' || **input == '\r'
					|| ft_strchr("><|", **input)))
			break;
		(*input)++;
	}
	add_token(tokens, create_token(ft_substr(start, 0, *input - start), WORD));
}

t_token	*tokenizer(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	//check input
	if (check_input(input) == 1)
		return (NULL);
	while(*input)
	{
		while ((*input) && (*input == ' ' || *input == '\t'
					|| *input == '\n' || *input == '\v' || *input == '\f' || *input == '\r'))
				input++;
		if (ft_strchr("><|", *input))
			handle_redirection(&input, &tokens);
		else
			handle_word(&input, &tokens);
	}
	return (tokens);
}
