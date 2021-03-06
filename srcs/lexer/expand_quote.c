/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/29 19:39:50 by lfalkau           #+#    #+#             */
/*   Updated: 2020/04/10 17:35:40 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>

char	*remove_quotes(char *raw)
{
	char	len;
	char	*tmp;

	len = ft_strlen(raw);
	if (len > 1 && raw[len - 1] == *raw)
	{
		tmp = raw;
		if (!(raw = ft_strndup(tmp + 1, len - 2)))
			return (NULL);
		free(tmp);
		return (raw);
	}
	write(1, "minishell: closing quote is missing\n", 36);
	return (NULL);
}

int		expand_squotes(t_lxr *lst)
{
	if (!(lst->raw = remove_quotes(lst->raw)))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

char	*expand_quoted_dollar(char *raw, char *var, size_t vlen, t_env *env)
{
	size_t	len;
	char	*tmp_1;
	char	*tmp_2;
	char	*tmp_3;

	len = var - raw;
	if (!(tmp_1 = len > 0 ? ft_strndup(raw, len) : ft_strdup("")))
		return (NULL);
	tmp_2 = var[1] == '?' ?
		expand_exitcode(NULL) : expand_variable(ft_strndup(var, vlen), env);
	if (!tmp_2)
		return (NULL);
	if (!(tmp_3 = ft_strjoin(tmp_1, tmp_2)))
		return (NULL);
	free(tmp_1);
	free(tmp_2);
	var += (vlen);
	if (!(tmp_1 = ft_strlen(var) > 0 ? ft_strdup(var) : ft_strdup("")))
		return (NULL);
	if (!(tmp_2 = ft_strjoin(tmp_3, tmp_1)))
		return (NULL);
	free(tmp_3);
	free(tmp_1);
	free(raw);
	return (tmp_2);
}

int		expand_dquotes(t_lxr *lst, t_env *env)
{
	char	*s;
	size_t	vlen;

	if (!(lst->raw = remove_quotes(lst->raw)))
		return (EXIT_FAILURE);
	s = lst->raw;
	while (*s)
	{
		if (*s == '$' && (s[1] == '_' || ft_isalnum(s[1]) || *s == '$'))
		{
			vlen = 1;
			while (s[vlen] == '_' || ft_isalnum(s[vlen]))
				vlen++;
			vlen = *(s + 1) == '?' ? 2 : vlen;
			if (!(lst->raw = expand_quoted_dollar(lst->raw, s, vlen, env)))
				return (EXIT_FAILURE);
			s = lst->raw;
		}
		else
			s++;
	}
	return (EXIT_SUCCESS);
}
