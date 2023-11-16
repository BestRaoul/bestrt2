/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finds2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkondrac <marvin@42lausanne.ch>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 09:20:59 by pkondrac          #+#    #+#             */
/*   Updated: 2022/10/28 15:14:27 by pkondrac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "additional.h"

//returns next MATCH-ing UN-\ UN-'-"
//else LEN
int	findf_ne_nq(int (*f)(char), char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			i += find_noescape(s[i], &(s[i + 1])) + 1;
			if (i == len(s))
				break ;
		}
		if (f(s[i]))
			if (i == 0 || s[i - 1] != '\\')
				break ;
		i++;
	}
	return (i);
}

//returns next char in cs UN-\ UN-'-"
//else LEN
int	finds_ne_nq(char *cs, char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '\"')
		{
			i += find_noescape(s[i], &(s[i + 1])) + 1;
			if (i == len(s))
				break ;
		}
		if (in(s[i], cs))
			if (i == 0 || s[i - 1] != '\\')
				break ;
		i++;
	}
	return (i);
}

//returns next C of UN-\ and UN-QUOTES
//else LEN
int	find_ne_nqs(char c, char *s, char *quotes)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (in(s[i], quotes))
		{
			i += find_noescape(s[i], &(s[i + 1])) + 1;
			if (i == len(s))
				break ;
		}
		if (s[i] == c)
			if (i == 0 || s[i - 1] != '\\')
				break ;
		i++;
	}
	return (i);
}

/*
 * Returns index of first matching char, else LEN
 */
int	findf(int (*f)(char), char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (f(str[i]))
			return (i);
		i++;
	}
	return (i);
}

/*
 * Returns index of first NON-matching char, else LEN
 */
int	findf_nt(int (*f)(char c), char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (f(str[i]) == 0)
			return (i);
		i++;
	}
	return (i);
}
