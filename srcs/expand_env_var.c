/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:10:15 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 15:09:08 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	init_env_var(t_env_var *ev, char *line)
{
	ev->i = 0;
	ev->j = 0;
	ev->var_len = 0;
	ev->quote_char = 0;
	ev->len = ft_strlen(line);
	ev->new_line = malloc(ft_strlen(line) + 1);
	if (ev->new_line == NULL)
		return (1);
	return (0);
}

void	handle_quotes(t_env_var *ev, char *line)
{
	if (line[ev->i] == '\'' || line[ev->i] == '\"')
	{
		if (ev->quote_char == line[ev->i])
			ev->quote_char = 0;
		else if (ev->quote_char == 0)
			ev->quote_char = line[ev->i];
	}
}

int	handle_dollar(t_env_var *ev, char *line, t_data *data)
{
	ev->start_i = ev->i;
	ev->new_tokens = handle_dollar_expan(line, &ev->i, data, &ev->var_len);
	if (ev->new_tokens)
	{
		ev->len = ev->len - (ev->i - ev->start_i) + ev->var_len;
		ev->new_line = ft_realloc(ev->new_line, (ev->len + 1) - ev->var_len,
				ev->len + 1);
		if (ev->new_line == NULL)
		{
			free(ev->new_line);
			return (1);
		}
	}
	return (0);
}

void	handle_new_tokens(t_env_var *ev)
{
	if (ev->new_tokens)
	{
		ev->current_token = ev->new_tokens;
		while (ev->current_token != NULL)
		{
			ev->current_value = ev->current_token->value;
			while (*ev->current_value)
				ev->new_line[ev->j++] = *ev->current_value++;
			if (ev->current_token->next != NULL)
				ev->new_line[ev->j++] = ' ';
			ev->current_token = ev->current_token->next;
		}
		free_tokens(ev->new_tokens);
	}
}

t_token	*expand_env(char *line, t_data *data)
{
	t_env_var	ev;

	if (init_env_var(&ev, line))
		return (NULL);
	while (line[ev.i] != '\0')
	{
		handle_quotes(&ev, line);
		if (line[ev.i] == '$' && ev.quote_char != '\'')
		{
			if (line[ev.i + 1] == ' ' || line[ev.i + 1] == '\''
				|| line[ev.i + 1] == '\"' || line[ev.i + 1] == '\0')
			{
				ev.new_line[ev.j++] = line[ev.i++];
				continue ;
			}
			if (handle_dollar(&ev, line, data) == 1)
				return (NULL);
			handle_new_tokens(&ev);
		}
		else
			ev.new_line[ev.j++] = line[ev.i++];
	}
	ev.new_line[ev.j] = '\0';
	ev.new_tokens = tokenizer(ev.new_line, data, false);
	return (free(ev.new_line), ev.new_tokens);
}
