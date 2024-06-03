/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:03:39 by apintus           #+#    #+#             */
/*   Updated: 2024/05/29 16:09:33 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*********************************PWD************************************/

char	*static_cwd(int action)
{
	static char	*cwd;

	if (action == FREE)
	{
		free(cwd);
		cwd = NULL;
	}
	else if (!cwd || action == UPDATE)
	{
		if (cwd)
			free(cwd);
		cwd = NULL;
		cwd = getcwd(cwd, 0);
	}
	return (cwd);
}

int	ft_pwd(void)
{
	char		*cwd;
	struct stat	st;

	cwd = static_cwd(NOTHING);
	if (!cwd)
		return (1);
	if (stat(cwd, &st) != 0)
	{
		ft_putstr_fd("minishell: pwd: ", 2);
		ft_putstr_fd("error retrieving current directory: ", 2);
		ft_putstr_fd("getcwd: cannot access parent directories: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		g_var = 1;
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

char	*get_pwd(void)
{
	char	*cwd;
	char	*tmp;

	cwd = static_cwd(NOTHING);
	if (!cwd)
		return (NULL);
	tmp = cwd;
	cwd = ft_strjoin("\001\033[34m\002", cwd);
	if (!cwd)
		return (NULL);
	tmp = cwd;
	cwd = ft_strjoin(cwd, "\001\033[0m\002\n");
	free(tmp);
	return (cwd);
}
