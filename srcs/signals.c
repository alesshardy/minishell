/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 11:29:14 by kammi             #+#    #+#             */
/*   Updated: 2024/05/29 14:48:43 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ctrl_c_handler(int sig)
{
	(void)sig;
	g_var = CTRL_C_EXIT;
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

void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		g_var = CTRL_C_EXIT;
	else
	{
		ft_putstr_fd("Quit (core dumped)", 1);
		g_var = 131;
	}
	write(1, "\n", 1);
}

void	signals_heredoc_parents(int sig)
{
	if (sig == SIGINT)
	{
		g_var = CTRL_C_EXIT;
		write(1, "\n", 1);
	}
	else if (sig == SIGQUIT)
	{
		ft_putstr_fd("Quit: 3\n", 1);
		g_var = 131;
	}
}

void	signals_heredoc(int sig)
{
	(void)sig;
	if (sig == SIGINT)
	{
		g_var = CTRL_C_EXIT;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		close(STDIN_FILENO);
		return ;
	}
}
