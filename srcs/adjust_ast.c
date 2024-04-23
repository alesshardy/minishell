/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_ast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 16:20:17 by apintus           #+#    #+#             */
/*   Updated: 2024/04/23 15:46:41 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	move_cmd(t_ast *node)
{
	t_ast	*new_node;
	int		arg_count;
	int		i;

	if (node->right->args[1] != NULL) // Si il y a plus d'un argument à droite
	{
		new_node  = malloc(sizeof(t_ast)); // Créer un nouveau noeud
		new_node->type = CMD; // Le type du nouveau noeud est CMD

		// Créer un nouveau tableau pour les arguments
		arg_count = 0;
		while (node->right->args[arg_count] != NULL)
			arg_count++;
		new_node->args = malloc(sizeof(char*) * (arg_count - 1 + 1)); // -1 pour le premier argument, +1 pour NULL

		// Copier les arguments dans le nouveau tableau
		i = 1;
		while (i < arg_count)
		{
			new_node->args[i - 1] = node->right->args[i];
			i++;
		}
		new_node->args[arg_count - 1] = NULL;

		new_node->left = NULL; // Le nouveau noeud n'a pas de fils gauche
		new_node->right = NULL; // Le nouveau noeud n'a pas de fils droit

		node->left = new_node; // Le fils gauche du noeud actuel devient le nouveau noeud
		node->right->args[1] = NULL; // On supprime les arguments après le premier du noeud droit
	}
}

void	adjust_ast(t_ast *node)
{
	if (node == NULL)
		return;

	if (node->type == REDIR_IN || node->type == REDIR_OUT || node->type == REDIR_APPEND || node->type == REDIR_HEREDOC)
	{
		if (node->left == NULL && node->right != NULL && ((node->right->type == INFILE) || (node->right->type == OUTFILE)))
		{
			if (node->right->args != NULL)
				move_cmd(node);
		}
	}

	adjust_ast(node->left);
	adjust_ast(node->right);
}

void	move_arg(t_ast *node)
{
	int	arg_count_right;
	int	arg_count_left;
	int	i;

	if (node->right->args[1] != NULL) // Si il y a plus d'un argument à droite
	{
		// Compter le nombre d'arguments dans le noeud de droite
		arg_count_right = 0;
		while (node->right->args[arg_count_right] != NULL)
			arg_count_right++;

		// Compter le nombre d'arguments dans le noeud de gauche
		arg_count_left = 0;
		while (node->left->args[arg_count_left] != NULL)
			arg_count_left++;

		// Allouer un nouveau tableau pour les arguments du noeud de gauche
		node->left->args = realloc(node->left->args, sizeof(char*) * (arg_count_left + arg_count_right + 1));

		// Copier les arguments du noeud de droite vers le noeud de gauche
		i = 0;
		while (i < arg_count_right)
		{
			node->left->args[arg_count_left + i] = node->right->args[i + 1];
			i++;
		}
		node->left->args[arg_count_left + i] = NULL;

		// Supprimer les arguments après le premier du noeud de droite
		node->right->args[1] = NULL;
	}
}

void	adjust_ast_file(t_ast *node)
{
	if (node == NULL)
		return;
	if (node->type == REDIR_IN || node->type == REDIR_OUT || node->type == REDIR_APPEND || node->type == REDIR_HEREDOC)
	{
		if (node->left != NULL && node->right != NULL && ((node->right->type == INFILE) || (node->right->type == OUTFILE)))
		{
			if (node->right->args != NULL)
				move_arg(node);
		}
	}
	adjust_ast_file(node->left);
	adjust_ast_file(node->right);
}
