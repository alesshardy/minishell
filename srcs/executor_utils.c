/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:07:31 by apintus           #+#    #+#             */
/*   Updated: 2024/05/30 13:07:38 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/********************ADJUST FILE AND CMD********************************/

int	contains_white_space(char *str, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

char	*copy_str(char *new_str, char *str, size_t *i, size_t end)
{
	int	j;

	j = 0;
	while (*i < end)
	{
		new_str[j++] = str[(*i)++];
	}
	new_str[j] = '\0';
	return (new_str);
}

char	*check_cmd_quotes(char *str)
{
	size_t	i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	if ((str[0] == '\'' || str[0] == '\"')
		&& str[ft_strlen(str) - 1] == str[0] && ft_strlen(str) != 2
		&& !contains_white_space(str + 1, ft_strlen(str) - 2))
	{
		while (i < ft_strlen(str) && str[++i] != str[0])
		{
			new_str[j++] = str[i];
		}
		new_str[j] = '\0';
	}
	else
	{
		new_str = copy_str(new_str, str, &i, ft_strlen(str));
	}
	free(str);
	return (new_str);
}

char	*remove_quotes_file(char *str)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(ft_strlen(str) + 1);
	if (new_str == NULL)
		return (NULL);
	if ((str[0] == '\'' || str[0] == '\"') && str[ft_strlen(str) - 1] == str[0])
	{
		while (str[++i] != str[0])
		{
			new_str[j++] = str[i];
		}
		new_str[j] = '\0';
	}
	else
	{
		while (str[i] != '\0')
			new_str[j++] = str[i++];
		new_str[j] = '\0';
	}
	free(str);
	return (new_str);
}

/*************************************EXEC**********************************/
void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}
