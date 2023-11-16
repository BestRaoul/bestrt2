/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finds.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

//else -1
int	ft_find(char c, const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

/* 
 * Returns index of first occurence of C, else LEN
 */
int	find(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (i);
}

/* 
 * Returns index of first occurence of UNSECAPED-char, else -1
 */
int	find_noescape(char c, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == c)
		{
			if (i == 0 || str[i - 1] != '\\')
				return (i);
		}
		i++;
	}
	return (i);
}

/* 
 * Returns index of first occurence of UNSECAPED-char in cs, else LEN
 */
int	finds_noescape(char *cs, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (in(str[i], cs))
		{
			if (i == 0 || str[i - 1] != '\\')
				return (i);
		}
		i++;
	}
	return (i);
}

/* 
 * returns next match of UN-\ in STR
 else LEN
 */
int	findf_noescape(int (*f)(char), char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (f(str[i]))
		{
			if (i == 0 || str[i - 1] != '\\')
				return (i);
		}
		i++;
	}
	return (i);
}
