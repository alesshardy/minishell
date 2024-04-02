/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:43:09 by apintus           #+#    #+#             */
/*   Updated: 2024/04/02 12:30:26 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

t_ast	*parse_word(t_token **tokens)
{
	t_ast	*node;

	node = NULL;
	if (*tokens != NULL && (*tokens)->type == WORD)
	{
		node = create_ast(*tokens);
		*tokens = (*tokens)->next;
	}
	return (node);
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*node;
	int		i;

	node = parse_word(tokens);
	i = 1;
	while (*tokens != NULL && (*tokens)->type == WORD)
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

	node = parse_command(tokens);
	while (*tokens != NULL && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == REDIR_APPEND
			|| (*tokens)->type == REDIR_HEREDOC))
	{
		redir = create_ast(*tokens);
		redir->left = node;
		*tokens = (*tokens)->next;
		redir->right = parse_command(tokens);
		node = redir;
	}
	return (node);
}

t_ast	*parse_pipe(t_token **tokens)
{
	t_ast *node;
	t_ast *pipe;

	node = parse_redirection(tokens);
	while (*tokens != NULL && (*tokens)->type == PIPE)
	{
		pipe = create_ast(*tokens);
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

void	free_ast(t_ast *ast)
{
	if (ast == NULL)
		return ;
	free_ast(ast->left);
	free_ast(ast->right);
	free(ast->args); // Free the args array
	free(ast);
}
