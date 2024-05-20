/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/09 12:18:07 by apintus           #+#    #+#             */
/*   Updated: 2024/05/20 17:15:43 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* char	*ft_strdup(const char *s)
{
	char	*dest;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s);
	dest = malloc(sizeof(char) * len + 1);
	if (dest == NULL)
		return (0);
	while (s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
} */

char	*ft_strdup(const char *s)
{
	size_t		i;
	char		*str;

	str = (char *)malloc(sizeof(*s) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = s[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

/*#include <stdio.h>

int	main(void)
{
	char	str[] = "Abricots";
	printf("%s", ft_strdup(str));
	return (0);
}*/
