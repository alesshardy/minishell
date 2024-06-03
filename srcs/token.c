/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:15:43 by apintus           #+#    #+#             */
/*   Updated: 2024/05/30 12:51:01 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*****************************tokenizer***********************************/

// Gère les redirections d'entrée (< et <<)
void	handle_input_redirection(char **input, t_token **tokens)
{
	t_token	*new;

	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			new = new_token("<<", REDIR_HEREDOC);
			if (new == NULL)
				return ;
			add_token(tokens, new);
			(*input)++;
		}
		else
		{
			new = new_token("<", REDIR_IN);
			if (new == NULL)
				return ;
			add_token(tokens, new);
		}
	}
	(*input)++;
}

// Gère les redirections de sortie (> et >>)
void	handle_output_redirection(char **input, t_token **tokens)
{
	t_token	*new;

	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			new = new_token(">>", REDIR_APPEND);
			if (new == NULL)
				return ;
			add_token(tokens, new);
			(*input)++;
		}
		else
		{
			new = new_token(">", REDIR_OUT);
			if (new == NULL)
				return ;
			add_token(tokens, new);
		}
	}
	(*input)++;
}

// Fonction originale modifiée pour appeler les nouvelles fonctions
void	handle_redirection(char **input, t_token **tokens)
{
	t_token	*new;

	if (**input == '<')
	{
		handle_input_redirection(input, tokens);
	}
	else if (**input == '>')
	{
		handle_output_redirection(input, tokens);
	}
	else if (**input == '|')
	{
		new = new_token("|", PIPE);
		if (new == NULL)
			return ;
		add_token(tokens, new);
		(*input)++;
	}
}

void	handle_word(char **input, t_token **tokens, bool expand)
{
	char	*start;
	int		in_quotes;
	char	quote;

	start = *input;
	in_quotes = 0;
	quote = '\0';
	while (**input)
	{
		if (in_quotes == 0 && (**input == '\'' || **input == '\"'))
		{
			in_quotes = 1;
			quote = **input;
		}
		else if (in_quotes == 1 && **input == quote)
			in_quotes = 0;
		else if (in_quotes == 0 && (**input == ' ' || **input == '\t'
				|| **input == '\n' || **input == '\v'
				|| **input == '\f' || **input == '\r'
				|| (ft_strchr("><|", **input) && expand)))
			break ;
		(*input)++;
	}
	add_word_token(&start, input, tokens);
}

//main tokenizer
t_token	*tokenizer(char *input, t_data *data, bool expand)
{
	t_token	*tokens;

	tokens = NULL;
	(void)data;
	if (check_input(input) == 1)
	{
		g_var = 2;
		return (NULL);
	}
	while (*input)
	{
		while ((*input) && (*input == ' ' || *input == '\t'
				|| *input == '\n' || *input == '\v'
				|| *input == '\f' || *input == '\r'))
				input++;
		if (*input == '\0')
			break ;
		if (ft_strchr("><|", *input) && expand)
			handle_redirection(&input, &tokens);
		else
			handle_word(&input, &tokens, expand);
	}
	post_process_tokens(&tokens, data, expand);
	return (tokens);
}
// VISUAL
/* t_token	*tokenizer(char *input, t_data *data, bool expand)
{
	t_token	*tokens;

	tokens = NULL;
	(void)data;
	if (check_input(input) == 1)
	{
		g_var = 2;
		return (NULL);
	}
	while(*input)
	{
		while ((*input) && (*input == ' ' || *input == '\t'
				|| *input == '\n' || *input == '\v'
				|| *input == '\f' || *input == '\r'))
				input++;
		if (*input == '\0')
			break;
		if (ft_strchr("><|", *input) && expand)
			handle_redirection(&input, &tokens);
		else
			handle_word(&input, &tokens, expand);
	}
	display_tokens(tokens); // VISUAL
	if (expand)
	{
		printf("redefine dollar\n"); // VISUAL
		redefine_dollar(&tokens, data);
	}
	printf("redefine dollar\n"); // VISUAL
	display_tokens(tokens); // VISUAL
	remove_unquoted_dollar_tokens(&tokens);
	printf("remove empty $\n"); // VISUAL
	display_tokens(tokens); // VISUAL
	redefine_word_token(tokens);
	redefine_cmd_token(tokens);
	remove_outer_quotes_from_tokens(tokens);
	printf("redefine dollar\n"); // VISUAL
	display_tokens(tokens); // VISUAL
	return (tokens);
} */
