/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:36:51 by apintus           #+#    #+#             */
/*   Updated: 2024/04/16 17:27:02 by apintus          ###   ########.fr       */
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

/*******************************************VISUAL*************************************************/

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
/*********************************************MAIN****************************************************/

char	*get_prompt(void)
{
	char	*pwd;
	char	*tmp;
	char	*prompt;

	pwd = get_pwd();
	if (!pwd)
		return (ft_strdup(READLINE_MSG));
	tmp = pwd;
	prompt = ft_strjoin(pwd, READLINE_MSG);
	free(pwd);
	return (prompt);
}
char	*prompt()
{
	char	*prompt;
	char	*line;
	prompt = get_prompt();
	line = readline(prompt);
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
		data->count_redir_in = 0;
		data->count_redir_out = 0;
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
	signals_handler();
	data = initialize_data();
	init_shell_env(data, env);
	while (1)
	{
		//prompt
		data->prompt = prompt();
		printf("prompt : %s\n", data->prompt); //visualiser le prompt
		//tokenize
		data->tokens = tokenizer(data->prompt);
		display_tokens(data->tokens); //visualiser les tokens
		//parse
		data->ast = parse_tokens(&data->tokens);
		print_ast(data->ast, 0); //visualiser l'arbre
		//execute
		count_redirection(data, &data->ast);
		printf("redir_in : %d\n", data->count_redir_in);
		printf("redir_out : %d\n", data->count_redir_out);
		if (data->count_redir_in > 1 || data->count_redir_out > 1)
		{
			//printf("first_redir_out : %s\n", data->first_redir_out->right->args[0]); //fou la merde
			handle_multi_redir(data, data->ast);
			executor(data, data->ast);
		}
		else
			executor(data, data->ast);
		//exit temporaire
		if (ft_strncmp(data->prompt, "exit", 4) == 0) //exit temporaire
		{
			free(data->prompt);
			break ;
		}
		free(data->prompt);
		reset_count_redirection(data);
	}
	free(data);
	return (0);
}

//MAIN DEBUG
/* int	main(int ac, char **av, char **env)
{
	t_data	*data;

	if (ac != 1)
	{
		printf("Error: too many arguments\n");
		return (1);
	}
	(void)av;
	signals_handler();
	data = initialize_data();
	init_shell_env(data, env);
	while (1)
	{
		//prompt
		data->prompt = "cat A < B < C";
		printf("prompt : %s\n", data->prompt); //visualiser le prompt
		//tokenize
		data->tokens = tokenizer(data->prompt);
		display_tokens(data->tokens); //visualiser les tokens
		//parse
		data->ast = parse_tokens(&data->tokens);
		print_ast(data->ast, 0); //visualiser l'arbre
		//execute
		count_redirection(data, &data->ast);
		printf("redir_in : %d\n", data->count_redir_in);
		printf("redir_out : %d\n", data->count_redir_out);
		if (data->count_redir_in > 1 || data->count_redir_out > 1)
		{
			//printf("first_redir_out : %s\n", data->first_redir_out->right->args[0]);
			handle_multi_redir(data, data->ast);
			executor(data, data->ast);
		}
		else
			executor(data, data->ast);
		//exit temporaire
		if (ft_strncmp(data->prompt, "exit", 4) == 0) //exit temporaire
		{
			free(data->prompt);
			break ;
		}
		free(data->prompt);
		reset_count_redirection(data);
	}
	free(data);
	return (0);
} */
