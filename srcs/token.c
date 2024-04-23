/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 16:15:43 by apintus           #+#    #+#             */
/*   Updated: 2024/04/23 16:33:04 by apintus          ###   ########.fr       */
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
	new_token->value = value;
	new_token->type = type;
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);

}

// t_token	*create_token(char *value, t_token_type type)
// {
// 	t_token	*token;

// 	token = malloc(sizeof(t_token));
// 	if (token == NULL)
// 		return (NULL);
// 	token->value = ft_strdup(value);
// 	token->type = type;
// 	token->next = NULL;
// 	return (token);
// }


/***************************************redefine_token*********************************************/

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
			/*else if (token->prev && token->prev->type == ENV_VAR)
				token->type = ARG;*/
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

/***************************************tokenizer**************************************************/

//handle (|, <, << , >, >>)
void	handle_redirection(char **input, t_token **tokens)
{
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			add_token(tokens, new_token( "<<", REDIR_HEREDOC));//handle heredoc
			(*input)++;
		}
		else
			add_token(tokens, new_token( "<", REDIR_IN));//handle redirection in
	}
	else if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			add_token(tokens, new_token( ">>", REDIR_APPEND));//handle redirection append
			(*input)++;
		}
		else
			add_token(tokens, new_token( ">", REDIR_OUT));//handle redirection out
	}
	else if (**input == '|')
		add_token(tokens, new_token( "|", PIPE));//handle pipe
	(*input)++;
}

//handle word

void	add_word_token(char **start, char **input, t_token **tokens)
{
	char	*word;

	if (*input > *start)
	{
		word = ft_substr(*start, 0, *input - *start);
		//printf("word: %s\n", word); // debug
		if (word)
		{
			add_token(tokens, new_token(strdup(word), WORD));
			free(word);
		}
		else
			ft_putstr_fd("Error: malloc failed\n", 2);
	}
}

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
	add_word_token(&start, input, tokens);
}

/*************************************HANDLE QUOTES********************************************************/

//remove empty quote from tokens
/* char	*remove_empty_quotes(char *word)
{
	int		length;
	char	*new_word;
	int		i;
	int		j;
	int		in_quotes;

	i = 0;
	j = 0;
	in_quotes = 0;
	length = ft_strlen(word);
	new_word = malloc(length + 1);
	while (i < length)
	{
		if ((word[i] == '\'' && word[i + 1] != '\'') || (word[i] == '\"' && word[i + 1] != '\"'))
		{
			if (in_quotes == word[i])
				in_quotes = 0;
			else if (!in_quotes)
				in_quotes = word[i];
		}
		if ((word[i] == '\'' || word[i] == '\"') && i + 1 < length && word[i + 1] == word[i] && !in_quotes)
			i += 2;
		else
			new_word[j++] = word[i++];
	}
	new_word[j] = '\0';
	return (new_word);
}

void	remove_empty_quotes_from_tokens(t_token *tokens)
{
	t_token	*current;
	char	*new_word;

	current = tokens;
	while (current != NULL)
	{
		new_word = remove_empty_quotes(current->value);
		free(current->value);
		current->value = new_word;
		current = current->next;
	}
} */
//remove outer quotes from tokens
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

//main tokenizer
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
	display_tokens(tokens);
	printf("			REDEFINE\n");
	redefine_dollar(&tokens);
	//remove_empty_quotes_from_tokens(tokens); // inuliser pour le moment wtf
	redefine_word_token(tokens);
	redefine_cmd_token(tokens);
	remove_outer_quotes_from_tokens(tokens);
	return (tokens);
}
