/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:29:14 by kammi             #+#    #+#             */
/*   Updated: 2024/05/15 15:33:21 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	signals_handler(void)
{
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ctrl_c_handler(int sig)
{
	(void)sig;
	global_var = CTRL_C_EXIT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(1, "Quit: 3\n", 8);
}
// ctrl + '\' cette co
// void	signals_heredoc(int sig)
// {
// 	(void)sig;
// 	global_var = CTRL_C_EXIT;
// 	write(1, "\n", 1);

// }
void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		global_var = CTRL_C_EXIT;
	else
	{
		ft_putstr_fd("Quit (core dumped)", 1);
		global_var = 131;
	}
	write(1, "\n", 1);
}
void	signals_heredoc_parents(int sig)
{
	if (sig == SIGINT)
		ft_printf("\n");
}

void	signals_heredoc(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		global_var = CTRL_C_EXIT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		close(STDIN_FILENO);
		return ;
	}
}
