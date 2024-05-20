/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:43:09 by apintus           #+#    #+#             */
/*   Updated: 2024/05/20 18:49:23 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_word(t_token **tokens)
{
	t_ast	*node;

	node = NULL;
	if (*tokens != NULL && ((*tokens)->type == WORD || (*tokens)->type == INFILE
			|| (*tokens)->type == OUTFILE || (*tokens)->type == CMD || (*tokens)->type == ARG))
	{
		node = create_ast(*tokens);
		if (node == NULL) // funcheck
			return (NULL);
		*tokens = (*tokens)->next;
	}
	return (node);
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*node;
	int		i;

	node = parse_word(tokens);
	if (node == NULL) // funcheck
		return (NULL);
	i = 1;
	while (*tokens != NULL &&  ((*tokens)->type == WORD || (*tokens)->type == INFILE
				|| (*tokens)->type == OUTFILE || (*tokens)->type == CMD || (*tokens)->type == ARG))
	{
		node->args[i] = (*tokens)->value; // Add the word to the args array of the node
		node->args[i + 1] = NULL; // Ensure the args array is NULL-terminated
		*tokens = (*tokens)->next;
		i++;
	}
	return (node);
}

t_ast	*parse_redirection(t_token **tokens)
{
	t_ast	*node;
	t_ast	*redir;
	t_ast	*last_redir;

	node = parse_command(tokens);
	// if (node == NULL) // funcheck CELUI QUI FOU LA MERDE
	// 	return (NULL);
	last_redir = NULL;
	while (*tokens != NULL && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == REDIR_APPEND
			|| (*tokens)->type == REDIR_HEREDOC))
	{
		redir = create_redir_node(tokens);
		if (redir == NULL)
			return NULL;
		node = add_redir_node_to_ast(node, redir, last_redir);
		last_redir = redir;
	}
	return (node);
}
// t_ast	*parse_redirection(t_token **tokens)
// {
// 	t_ast	*node;
// 	t_ast	*redir;
// 	t_ast	*last_redir;

// 	node = parse_command(tokens);
// 	last_redir = NULL;
// 	while (*tokens != NULL && ((*tokens)->type == REDIR_IN
// 			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == REDIR_APPEND
// 			|| (*tokens)->type == REDIR_HEREDOC))
// 	{
// 		redir = create_ast(*tokens);
// 		*tokens = (*tokens)->next;
// 		if (*tokens == NULL || ((*tokens)->type != INFILE && (*tokens)->type != OUTFILE))
// 		{
// 			fprintf(stderr, "Error: expected a filename after redirection\n");
// 			free_ast(redir);
// 			return (NULL);
// 		}
// 		redir->right = parse_command(tokens);
// 		if (last_redir == NULL)
// 		{
// 			redir->left = node;
// 			node = redir;
// 		}
// 		else
// 		{
// 			redir->left = last_redir->left;
// 			last_redir->left = redir;
// 		}
// 		last_redir = redir;
// 	}
// 	return (node);
// }


t_ast	*parse_pipe(t_token **tokens)
{
	t_ast *node;
	t_ast *pipe;

	node = parse_redirection(tokens);
	while (*tokens != NULL && (*tokens)->type == PIPE)
	{
		pipe = create_ast(*tokens);
		if (pipe == NULL) // funcheck
			return (NULL);
		pipe->left = node;
		*tokens = (*tokens)->next;
		pipe->right = parse_redirection(tokens);
		node = pipe;
	}
	return (node);
}

t_ast	*parse_tokens(t_token **tokens)
{
	if (*tokens == NULL)
		return (NULL);
	return (parse_pipe(tokens));
}

