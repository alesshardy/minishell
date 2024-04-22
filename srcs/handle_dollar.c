/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:10:15 by apintus           #+#    #+#             */
/*   Updated: 2024/04/22 18:08:42 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_env(char *line)
{
	char	*new_line;
	char	*env;
	size_t	i;
	size_t	j;
	size_t	start;
	char	quote_char;

	i = 0;
	j = 0;
	quote_char = 0;
	new_line = malloc(ft_strlen(line) + 1);
	if (new_line == NULL)
		return (NULL);
	while (line[i] != '\0')
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote_char == line[i])
				quote_char = 0;
			else if (quote_char == 0)
				quote_char = line[i];
		}
		if (line[i] == '$' && quote_char != '\'')
		{
			i++;
			start = i;
			while (line[i] != '\0' && (ft_isalnum(line[i]) || line[i] == '_'))
				i++;
			env = getenv(ft_substr(line, start, i - start));
			if (env)
			{
				while (*env)
					new_line[j++] = *env++;
			}
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}
	new_line[j] = '\0';
	return (new_line);
}

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
} // cette fonction vérifie si la ligne contient un dollar

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
} // cette fonction vérifie si la ligne contient uniquement un dollar

void	redefine_dollar(t_token **tokens)
{
	t_token	*tmp;
	char	*new_value;

	tmp = *tokens;
	while (tmp != NULL)
	{
		if (has_dollar(tmp->value) && !has_only_dollar(tmp->value))
		{
			new_value = expand_env(tmp->value);
			free(tmp->value);
			tmp->value = new_value;
		}
		tmp = tmp->next;
	}
} // cette fonction remplace les variables d'environnement par leur valeur
