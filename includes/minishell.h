/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 15:03:31 by apintus           #+#    #+#             */
/*   Updated: 2024/03/22 15:12:21 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>

typedef enum token_type
{
	WORD,
	PIPE, //|
	REDIR_IN, //<
	REDIR_OUT, //>
	REDIR_APPEND, //>>
	REDIR_HEREDOC, //<<
	ENV_VAR //$
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				index;
	struct s_token	*next;
}	t_token;

typedef struct s_data
{
	char	**env;
	char	*prompt;
	t_token	**tokens;
	size_t	token_count;
}	t_data;

/*main.c*/
char	*prompt(void);

/*check_input.c*/
int		check_input(char *line);
int		unclosed_quotes(char *line);
int		logical_operator(char *line);
int		misplace_operator(char *line);
int		misplace_redirection(char *line);

#endif
