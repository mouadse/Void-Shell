/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 14:50:51 by mouad             #+#    #+#             */
/*   Updated: 2024/12/09 23:49:06 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <assert.h>

// messy code here
size_t ft_strlen(const char *str) {
  size_t i;
//   assert(str && "str cannot be null");
  i = 0;
  while (str[i])
    i++;
  return (i);
}
