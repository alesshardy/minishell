/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 15:10:25 by kammi             #+#    #+#             */
/*   Updated: 2024/05/17 13:46:36 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_data	*initialize_data()
{
	t_data *data = malloc(sizeof(t_data));
	if (data != NULL)
	{
		data->env = NULL;
		data->env_array = NULL;
		data->prompt = NULL;
		data->tokens = NULL;
		data->first_token = NULL;
		data->token_count = 0;
		data->ast = NULL;
		data->first_redir_out = NULL;
		data->token_count = 0;
		data->here_doc_fd = 0;
		data->exit_status = 0;

	}
	return (data);
}

t_data	*initialize_program(int ac, char **av, char **env)
{
	t_data	*data;

	if (ac != 1)
	{
		printf("Error: too many arguments\n");
		exit(1);
	}
	(void)av;
	signals_handler();
	data = initialize_data();
	data->env = init_env(data, env);
	return data;
}

void	run_program(t_data *data)
{
	while (1)
	{
		data->prompt = prompt(data);
		data->tokens = tokenizer(data->prompt, data, true);
		handle_here_doc(data, &data->tokens);
		data->first_token = data->tokens; // pointeur sur le premier token
		data->ast = parse_tokens(&data->tokens);
		adjust_ast(data->ast);
		adjust_ast_file(data->ast);
		//print_ast(data->ast, 0);
		executor(data, data->ast);
		//free(data->prompt);
		ft_free_data(data);
		delete_tmp_files();
	}
}
