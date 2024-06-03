/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_ast_files.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 15:10:09 by apintus           #+#    #+#             */
/*   Updated: 2024/05/21 17:50:02 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*cleaner_quotes_arg(char *arg)
{
	int		length;
	char	*new_arg;
	int		i;
	int		j;
	char	in_quotes;

	i = 0;
	j = 0;
	in_quotes = 0;
	length = ft_strlen(arg);
	new_arg = malloc(length + 1);
	if (new_arg == NULL)
		return (NULL);
	while (i < length)
	{
		if ((arg[i] == '\'' || arg[i] == '\"') && !in_quotes)
			in_quotes = arg[i];
		else if (arg[i] == in_quotes)
			in_quotes = 0;
		else
			new_arg[j++] = arg[i];
		i++;
	}
	new_arg[j] = '\0';
	return (new_arg);
}

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL)
		count++;
	return (count);
}

void	move_arg(t_ast *node)
{
	int		arg_count_right;
	int		arg_count_left;
	int		i;
	char	*new_arg;

	if (node->right->args[1] != NULL)
	{
		arg_count_right = count_args(node->right->args);
		arg_count_left = count_args(node->left->args);
		node->left->args = ft_realloc(node->left->args, sizeof(char *)
				* arg_count_left, sizeof(char*)
				* (arg_count_left + arg_count_right + 1));
		if (node->left->args == NULL)
			return ;
		arg_count_right--;
		i = 0;
		while (i < arg_count_right)
		{
			new_arg = cleaner_quotes_arg(node->right->args[i + 1]);
			node->left->args[arg_count_left + i] = new_arg;
			i++;
		}
		node->left->args[arg_count_left + i] = NULL;
		node->right->args[1] = NULL;
	}
}

void	adjust_ast_file(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->type == REDIR_IN || node->type == REDIR_OUT
		|| node->type == REDIR_APPEND || node->type == REDIR_HEREDOC)
	{
		if (node->left != NULL && node->right != NULL
			&& ((node->right->type == INFILE)
				|| (node->right->type == OUTFILE)))
		{
			if (node->right->args != NULL)
				move_arg(node);
		}
	}
	adjust_ast_file(node->left);
	adjust_ast_file(node->right);
}
