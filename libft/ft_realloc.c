/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apintus <apintus@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 17:48:44 by apintus           #+#    #+#             */
/*   Updated: 2024/05/23 17:48:51 by apintus          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	min_size;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	if (old_size < new_size)
		min_size = old_size;
	else
		min_size = new_size;
	ft_memcpy(new_ptr, ptr, min_size);
	free(ptr);
	return (new_ptr);
}

/* void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
	{
		free(ptr);
		return (NULL);
	}
	ft_memcpy(new_ptr, ptr, old_size < new_size ? old_size : new_size);
	free(ptr);
	return (new_ptr);
} */
// void	*ft_realloc(void *ptr, size_t size)
// {
// 	void	*new_ptr;

// 	if (!ptr)
// 		return (malloc(size));
// 	if (size == 0)
// 	{
// 		free(ptr);
// 		return (NULL);
// 	}
// 	new_ptr = malloc(size);
// 	if (!new_ptr)
// 	{
// 		free(ptr);
// 		return (NULL);
// 	}
// 	ft_memcpy(new_ptr, ptr, size);
// 	free(ptr);
// 	return (new_ptr);
// }
