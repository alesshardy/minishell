/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 13:53:26 by kammi             #+#    #+#             */
/*   Updated: 2024/05/20 18:28:08 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*last;

	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	last = *tokens;
	while (last->next)
		last = last->next;
	last->next = new_token;
	new_token->prev = last;
}


t_token	*new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	// new_token->value = value;
	new_token->value = ft_strdup(value); // ADDED
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = type;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);

}

t_token *get_last_token(t_token *tokens)
{
	t_token *tmp = tokens;

	// Traverse the list until the last token
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
	}

	return tmp;
}

void	add_word_token(char **start, char **input, t_token **tokens)
{
	char	*word;
	t_token	*new;

	if (*input > *start)
	{
		word = ft_substr(*start, 0, *input - *start);
		//printf("word: %s\n", word); // debug
		if (word)
		{
			new = new_token(word, WORD);
			if (new)
				add_token(tokens, new);
			else
				ft_putstr_fd("Error: malloc failed\n", 2);
			free(word);
		}
		else
		{
			ft_putstr_fd("Error: malloc failed\n", 2);
		}
	}
}

char	*remove_outer_quotes(char *word)
{
	int		length;
	char	*new_word;
	int		i = 0;
	int		j = 0;
	char	in_quotes = 0;

	length = ft_strlen(word);
	new_word = malloc(length + 1);
	if (new_word == NULL)
		return (NULL);
	while (i < length)
	{
		if ((word[i] == '\'' || word[i] == '\"') && !in_quotes)
			in_quotes = word[i];
		else if (word[i] == in_quotes)
			in_quotes = 0;
		else
			new_word[j++] = word[i];
		i++;
	}
	new_word[j] = '\0';
	//free(word);//free the old word
	return (new_word);
}

void remove_outer_quotes_from_tokens(t_token *tokens)
{
	t_token	*current;
	char	*new_word;

	current = tokens;
	while (current != NULL)
	{
		if (current->type == ARG || current->type == OUTFILE || current->type == INFILE)
		{
			new_word = remove_outer_quotes(current->value);
			free(current->value);
			current->value = new_word;
		}
		current = current->next;
	}
}

