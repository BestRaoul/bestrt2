/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additiona.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 14:36:22 by pkondrac          #+#    #+#             */
/*   Updated: 2022/11/21 19:12:03 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADDITIONAL_H
# define ADDITIONAL_H

# include "libft.h"
# include <aio.h>
# include <stdarg.h>

int	eq(int x, int y);
int	less(int x, int y);
int	less_eq(int x, int y);
int	more(int x, int y);
int	more_eq(int x, int y);

int	ft_find(char c, const char *str);
int	find(char c, char *str);
int	find_noescape(char c, char *str);
int	finds_noescape(char *cs, char *str);
int	findf_noescape(int (*f)(char), char *str);

int	findf_ne_nq(int (*f)(char), char *s);
int	finds_ne_nq(char *cs, char *s);
int	find_ne_nqs(char c, char *s, char *quotes);
int	findf(int (*f)(char), char *str);
int	findf_nt(int (*f)(char c), char *str);

void	*free_arr(void **arr);
void	*frees(int count, ...);
void	*frees2(int count, ...);
double	ft_atod(const char *s);

int	get_intr(int (*compare)(int, int), int *r, int size);
int	find_intr(int x, int *r, int size);
int	count_intr(int (*f)(int), int *r, int size);
int	countx_intr(int x, int (*compare)(int, int), int *r, int size);

int	ft_size(const char **arr);
int len(const char *str);

char	**ft_split_sep(char const *s, char *sep);

int	in(const char c, const char *str);

#endif