/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 12:01:46 by kammi             #+#    #+#             */
/*   Updated: 2024/05/14 17:28:36 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*add_redir_node_to_ast(t_ast *node, t_ast *redir, t_ast *last_redir)
{
	if (last_redir == NULL)
	{
		redir->left = node;
		node = redir;
	}
	else
	{
		redir->left = last_redir->left;
		last_redir->left = redir;
	}
	return node;
}

t_ast	*create_redir_node(t_token **tokens)
{
	t_ast	*redir;

	redir = create_ast(*tokens);
	*tokens = (*tokens)->next;
	if (*tokens == NULL || ((*tokens)->type != INFILE && (*tokens)->type != OUTFILE))
	{
		//fprintf(stderr, "Error: expected a filename after redirection\n"); //VOIrsi UTILE
		free_ast(redir);
		return (NULL);
	}
	redir->right = parse_command(tokens);
	return redir;
}

t_ast	*create_ast(t_token *token)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->type = token->type;
	node->args = malloc(sizeof(char *) * MAX_ARGS); // Allocate memory for args
	node->args[0] = token->value; // The first argument is the command itself
	node->args[1] = NULL; // Initialize the rest of the args array to NULL
	node->left = NULL;
	node->right = NULL;
	return (node);
}
