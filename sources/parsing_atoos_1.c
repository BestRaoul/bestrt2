/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upate_item.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 10:46:01 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 20:41:00 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

vec3	atov3(const char *str, vec3 __default)
{
	char	**av;
	int		ac;

	if (str == NULL || ft_strncmp(str, "-", 2) == 0)
		return (__default);
	av = _gc_add_2d(ft_split(str, ','));
	ac = ft_size((const char **)av);
	if (ac == 1)
		return (v_3(atof(av[0])));
	else if (ac == 3)
		return (v3(atof(av[0]), atof(av[1]), atof(av[2])));
	return (dprintf(2, "Parse error: expected v3 got %s\n", str), __default);
}

PFPN	atod(const char *str, PFPN __default)
{
	if (str == NULL || ft_strncmp(str, "-", 2) == 0)
		return (__default);
	return (atof(str));
}
