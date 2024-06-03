/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/10 13:32:53 by apintus           #+#    #+#             */
/*   Updated: 2024/05/23 17:47:37 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// int	ft_strcmp(const char *s1, const char *s2)
// {
// 	while (*s1 && *s2 && *s1 == *s2)
// 	{
// 		s1++;
// 		s2++;
// 	}
// 	return ((unsigned char)*s1 - (unsigned char)*s2);
// }

//secure version
int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (0);
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}
