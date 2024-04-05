/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:36:51 by apintus           #+#    #+#             */
/*   Updated: 2024/04/05 17:18:32 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// char *delete_exces_space(char *str)
// {
// 	char	*new_str;
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	j = 0;
// 	new_str = malloc(ft_strlen(str) + 1);
// 	if (new_str == NULL)
// 		return (NULL);
// 	while (str[i] != '\0')
// 	{
// 		if (str[i] == ' ')
// 		{
// 			new_str[j] = str[i];
// 			j++;
// 			while (str[i] == ' ')
// 				i++;
// 		}
// 		new_str[j] = str[i];
// 		i++;
// 		j++;
// 	}
// 	new_str[j] = '\0';
// 	return (new_str);
// }

const char	*get_token_type_name(t_token_type type)
{
	const char	*token_type_names[11];

	token_type_names[0] = "WORD";
	token_type_names[1] = "PIPE";
	token_type_names[2] = "REDIRECT_IN";
	token_type_names[3] = "REDIRECT_OUT";
	token_type_names[4] = "REDIRECT_APPEND";
	token_type_names[5] = "REDIRECT_HEREDOC";
	token_type_names[6] = "TOKEN_ENV_VAR";
	token_type_names[7] = "CMD";
	token_type_names[8] = "ARG";
	token_type_names[9] = "INFILE";
	token_type_names[10] = "OUTFILE";
	if (type >= 0 && type < 11)
		return (token_type_names[type]);
	return ("UNKNOWN");
}

void	display_tokens(t_token *tokens)
{
	t_token	*token;

	token = tokens;
	while (token)
	{
		printf("Token: \033[0;36m %-20s \033[0m |\t \
			Type: \033[0;35m %-18s \033[0m \n",
			token->value, get_token_type_name(token->type));
		printf("--------------------------------------------------\n");
		token = token->next;
	}
}

void print_ast(t_ast *node, int depth)
{
    if (node == NULL)
        return;

    // Print the current node with indentation
    for (int i = 0; i < depth; i++)
        printf("  ");

 // Print the type of the node
    switch(node->type) {
        case WORD:
            printf("WORD: ");
            for (int i = 0; node->args[i] != NULL; i++)
                printf("%s ", node->args[i]);
            printf("\n");
            break;
        case PIPE:
            printf("PIPE: |\n");
            break;
        case REDIR_OUT:
            printf("REDIR_OUT: >\n");
            break;
        case REDIR_IN:
            printf("REDIR_IN: <\n");
            break;
        case REDIR_APPEND:
            printf("REDIR_APPEND: >>\n");
            break;
        case REDIR_HEREDOC:
            printf("REDIR_HEREDOC: <<\n");
            break;
		case CMD:
			printf("CMD: ");
			for (int i = 0; node->args[i] != NULL; i++)
                printf("%s ", node->args[i]);
            printf("\n");
			break;
		case ARG:
			printf("ARG: ");
			for (int i = 0; node->args[i] != NULL; i++)
				printf("%s ", node->args[i]);
			printf("\n");
			break;
		case INFILE:
			printf("INFILE: ");
			for (int i = 0; node->args[i] != NULL; i++)
                printf("%s ", node->args[i]);
            printf("\n");
			break;
		case OUTFILE:
			printf("OUTFILE: ");
			for (int i = 0; node->args[i] != NULL; i++)
                printf("%s ", node->args[i]);
            printf("\n");
			break;
        default:
            printf("UNKNOWN TYPE\n");
            break;
    }

    if (node->left != NULL) {
        printf("%*s├─ Left: ", depth * 2, "");
        print_ast(node->left, depth + 1);
    }
    if (node->right != NULL) {
        printf("%*s└─ Right: ", depth * 2, "");
        print_ast(node->right, depth + 1);
    }
}

char	*prompt()
{
	char	*line;
	line = readline("minishell$> ");
	add_history(line);
	return (line);
}

t_data	*initialize_data()
{
	t_data *data = malloc(sizeof(t_data));
	if (data != NULL)
	{
		data->env = NULL;
		data->prompt = NULL;
		data->tokens = NULL;
		data->token_count = 0;
	}
	return data;
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	if (ac != 1)
	{
		printf("Error: too many arguments\n");
		return (1);
	}
	(void)av;
	(void)env;
	data = initialize_data();
	while (1)
	{
		//prompt
		data->prompt = prompt();
		printf("prompt : %s\n", data->prompt); //visualiser le prompt
		//tokenize
		data->tokens = tokenizer(data->prompt);
		display_tokens(data->tokens); //visualiser les tokens
		redefine_word_token(data->tokens);
		redefine_cmd_token(data->tokens);
		printf("			REDEFINE\n");
		display_tokens(data->tokens); //visualiser les tokens
		//parse
		data->ast = parse_tokens(&data->tokens);
		print_ast(data->ast, 0); //visualiser l'arbre
		//exit temporaire
		if (ft_strncmp(data->prompt, "exit", 4) == 0) //exit temporaire
		{
			free(data->prompt);
			break ;
		}
		free(data->prompt);
	}
	free(data);
	return (0);
}
