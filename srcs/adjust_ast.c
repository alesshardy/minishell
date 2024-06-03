/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:20:17 by apintus           #+#    #+#             */
/*   Updated: 2024/05/21 17:50:25 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*create_new_node(t_ast *node, int *arg_count)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (new_node == NULL)
		return (NULL);
	new_node->type = CMD;
	*arg_count = 0;
	while (node->right->args[*arg_count] != NULL)
		(*arg_count)++;
	new_node->args = malloc(sizeof(char *) * (*arg_count));
	if (new_node->args == NULL)
		return (NULL);
	return (new_node);
}

void	populate_new_node_args(t_ast *new_node, t_ast *node, int arg_count)
{
	int	i;

	i = 1;
	while (i < arg_count)
	{
		new_node->args[i - 1] = node->right->args[i];
		i++;
	}
	new_node->args[arg_count - 1] = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
}

void	move_cmd(t_ast *node)
{
	t_ast	*new_node;
	int		arg_count;

	if (node->right->args[1] != NULL)
	{
		new_node = create_new_node(node, &arg_count);
		if (new_node == NULL)
			return ;
		populate_new_node_args(new_node, node, arg_count);
		node->left = new_node;
		node->right->args[1] = NULL;
	}
}

void	adjust_ast(t_ast *node)
{
	if (node == NULL)
		return ;
	if (node->type == REDIR_IN || node->type == REDIR_OUT
		|| node->type == REDIR_APPEND || node->type == REDIR_HEREDOC)
	{
		if (node->left == NULL && node->right != NULL
			&& ((node->right->type == INFILE)
				|| (node->right->type == OUTFILE)))
		{
			if (node->right->args != NULL)
				move_cmd(node);
		}
	}
	adjust_ast(node->left);
	adjust_ast(node->right);
}
