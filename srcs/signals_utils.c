/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kammi <kammi@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 15:32:15 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 15:09:43 by kammi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_signals_handler(void)
{
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
}

void	signals_handler(void)
{
	signal(SIGINT, ctrl_c_handler);
	signal(SIGQUIT, SIG_IGN);
}
