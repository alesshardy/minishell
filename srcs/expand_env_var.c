/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:10:15 by apintus           #+#    #+#             */
/*   Updated: 2024/05/15 18:24:38 by apintus          ###   ########.fr       */
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

// char	*expand_env(char *line, t_env *env)
// {
// 	char	*new_line;
// 	char	*env_value;
// 	size_t	i;
// 	size_t	j;
// 	size_t	start;
// 	char	quote_char;

// 	i = 0;
// 	j = 0;
// 	quote_char = 0;
// 	new_line = malloc(ft_strlen(line) + 1);
// 	if (new_line == NULL)
// 		return (NULL);
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '\'' || line[i] == '\"')
// 		{
// 			if (quote_char == line[i])
// 				quote_char = 0;
// 			else if (quote_char == 0)
// 				quote_char = line[i];
// 		}
// 		if (line[i] == '$' && quote_char != '\'')
// 		{
// 			i++;
// 			start = i;
// 			while (line[i] != '\0' && (ft_isalnum(line[i]) || line[i] == '_'))
// 				i++;
// 			if (line[start] == '?') // si la variable est $?
// 			{
// 				env_value = ft_itoa(global_var);
// 				//printf("env_value = %s\n", env_value);
// 				i++; // on incrémente i pour ne pas prendre le ?
// 			}
// 			else
// 				env_value = get_env_value(ft_substr(line, start, i - start), env);
// 			if (env_value)
// 			{
// 				while (*env_value)
// 					new_line[j++] = *env_value++;
// 			}
// 		}
// 		else
// 		{
// 			new_line[j++] = line[i++];
// 		}
// 	}
// 	new_line[j] = '\0';
// 	return (new_line);
// }// cette fonction remplace les variables d'environnement par leur valeur

t_token	*handle_dollar_expansion(char *line, size_t *i, t_env *env, t_data *data, size_t *len)
{
	size_t	start;
	char	*env_value;
	t_token	*new_tokens;

	//*len = 0;
	(*i)++;
	start = *i;
	while (line[*i] != '\0' && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	if (line[start] == '?') // si la variable est $?
	{
		env_value = ft_itoa(global_var);
		(*i)++; // on incrémente i pour ne pas prendre le ?
	}
	else
		env_value = get_env_value(ft_substr(line, start, *i - start), env);
	if (env_value)
	{
		*len = *len + ft_strlen(env_value);
		new_tokens = tokenizer(env_value, data, false); //ture ou false ?
		free(env_value);
		return new_tokens;
	}
	return NULL;
}

t_token	*expand_env(char *line, t_env *env, t_data *data)
{
	char	*new_line;
	size_t	i;
	size_t	j;
	char	quote_char;
	t_token	*new_tokens;
	size_t	len;
	size_t	var_len;
	size_t	start_i;
	t_token	*current_token;
	char	*current_value;

	i = 0;
	j = 0;
	var_len = 0;
	quote_char = 0;
	len = ft_strlen(line);
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
			if (line[i + 1] == ' ' || line[i + 1] == '\'' || line[i + 1] == '\"' || line[i + 1] == '\0')
            {
                new_line[j++] = line[i++];
                continue;
            }
			start_i = i;
			new_tokens = handle_dollar_expansion(line, &i, env, data, &var_len);
			if (new_tokens)
			{
				len = len - (i - start_i) + var_len;
				new_line = ft_realloc(new_line, (len + 1) - var_len, len + 1);// a implementer
				//printf("len = %zu\n", len);
				if (new_line == NULL)
				{
					free(new_line);
					return NULL;
				}
				if (new_tokens)
				{
					current_token = new_tokens;
					while (current_token != NULL)
					{
						current_value = current_token->value;
						while (*current_value)
							new_line[j++] = *current_value++;
						if (current_token->next != NULL)
							new_line[j++] = ' '; // add a space after each token except the last one
						current_token = current_token->next;
					}
				free_tokens(new_tokens);
				}
			}
		}
		else
		{
			new_line[j++] = line[i++];
		}
	}
	new_line[j] = '\0';
	new_tokens = tokenizer(new_line, data, false);
	free(new_line);
	return new_tokens;
}

// t_token	*expand_env(char *line, t_env *env, t_data *data)
// {
// 	char	*new_line;
// 	size_t	i;
// 	size_t	j;
// 	char	quote_char;
// 	t_token	*new_tokens;

// 	i = 0;
// 	j = 0;
// 	quote_char = 0;
// 	new_line = malloc(sizeof(char) * (ft_strlen(line) + 1)); // double the size for potential expansion
// 	if (new_line == NULL)
// 		return (NULL);
// 	while (line[i] != '\0')
// 	{
// 		if (line[i] == '\'' || line[i] == '\"')
// 		{
// 			if (quote_char == line[i])
// 				quote_char = 0;
// 			else if (quote_char == 0)
// 				quote_char = line[i];
// 		}
// 		if (line[i] == '$' && quote_char != '\'')
// 		{
// 			new_tokens = handle_dollar_expansion(line, &i, env, data);
// 			if (new_tokens)
// 			{
// 				while (*new_tokens->value)
// 					new_line[j++] = *new_tokens->value++;
// 				free(new_tokens);
// 			}
// 		}
// 		else
// 			new_line[j++] = line[i++];
// 	}
// 	new_line[j] = '\0';
// 	new_tokens = tokenizer(new_line, data);
// 	free(new_line);
// 	return new_tokens;
// }

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

// void	redefine_dollar(t_token **tokens, t_env *env)
// {
// 	t_token	*tmp;
// 	char	*new_value;

// 	tmp = *tokens;
// 	while (tmp != NULL)
// 	{
// 		if (has_dollar(tmp->value) && !has_only_dollar(tmp->value))
// 		{
// 			new_value = expand_env(tmp->value, env);
// 			printf("new_value = %s\n", new_value);
// 			free(tmp->value);
// 			tmp->value = new_value;
// 		}
// 		tmp = tmp->next;
// 	}
// } // cette fonction remplace les variables d'environnement par leur valeur

// void	redefine_dollar(t_token **tokens, t_env *env, t_data *data)
// {
// 	t_token	*tmp;
// 	t_token	*new_tokens;
// 	t_token	*last_new_token;

// 	tmp = *tokens;
// 	while (tmp != NULL)
// 	{
// 		if (has_dollar(tmp->value) && !has_only_dollar(tmp->value))
// 		{
// 			new_tokens = expand_env(tmp->value, env, data);
// 			if (new_tokens)
// 			{
// 				last_new_token = get_last_token(new_tokens);
// 				last_new_token->next = tmp->next;
// 				if (tmp->prev)
// 					tmp->prev->next = new_tokens;
// 				else
// 					*tokens = new_tokens;
// 				new_tokens->prev = tmp->prev;
// 				if (tmp->next)
// 					tmp->next->prev = last_new_token;
// 				free(tmp->value);
// 				free(tmp);
// 			}
// 		}
// 		tmp = tmp->next;
// 	}
// }

void	redefine_dollar(t_token **tokens, t_env *env, t_data *data)
{
	t_token	*tmp;
	t_token	*new_tokens;
	t_token	*last_new_token;
	t_token *next_tmp; // ajout d'une variable pour sauvegarder le pointeur vers tmp->next

	tmp = *tokens;
	while (tmp != NULL)
	{
		if (has_dollar(tmp->value) && !has_only_dollar(tmp->value))
		{
			new_tokens = expand_env(tmp->value, env, data);
			if (new_tokens)
			{
				last_new_token = get_last_token(new_tokens);
				last_new_token->next = tmp->next;
				if (tmp->prev)
					tmp->prev->next = new_tokens;
				else
					*tokens = new_tokens;
				new_tokens->prev = tmp->prev;
				if (tmp->next)
					tmp->next->prev = last_new_token;
				next_tmp = tmp->next; // sauvegarder le pointeur vers tmp->next avant de libérer tmp
				free(tmp->value);
				free(tmp);
				tmp = next_tmp; // utiliser le pointeur sauvegardé
				continue;
			}
		}
		tmp = tmp->next;
	}
}


