/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/10 16:03:46 by lfalkau           #+#    #+#             */
/*   Updated: 2020/04/13 21:45:44 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "tokens.h"
#include <stdlib.h>

char			**lex_to_args(t_lxr *lst)
{
	char	**av;
	int		i;

	i = 0;
	if (!(av = malloc(sizeof(char *) * (lxr_cmdsize(lst) + 1))))
		return (NULL);
	while (lst && lst->token == WORD)
	{
		av[i] = lst->raw;
		lst = lst->next;
		i++;
	}
	av[i] = NULL;
	return (av);
}

static char		**create_simple_cmd(t_lxr **head)
{
	char	**cmd;

	cmd = lex_to_args(*head);
	while (*head && (*head)->token == WORD)
		*head = (*head)->next;
	return (cmd);
}

static t_psr	*create_redirect_cmd(void *left, t_lxr **cur)
{
	t_psr	*lst_head;

	if (!(lst_head = (t_psr *)malloc(sizeof(t_psr))))
		return (NULL);
	lst_head->left = left;
	if (!cur)
	{
		lst_head->type = 0;
		lst_head->right = NULL;
		return (lst_head);
	}
	if (!(*cur) || (*cur)->token != REDIRECT)
		lst_head->type = 0;
	else if (!ft_strcmp((*cur)->raw, "|"))
		lst_head->type = PIPE;
	else if (!ft_strcmp((*cur)->raw, "<") || !ft_strcmp((*cur)->raw, "<<"))
		lst_head->type = LESS;
	else if (!ft_strcmp((*cur)->raw, ">"))
		lst_head->type = GREAT;
	else if (!ft_strcmp((*cur)->raw, ">>"))
		lst_head->type = DGREAT;
	if (*cur)
		*cur = (*cur)->next;
	lst_head->right = create_simple_cmd(cur);
	return (lst_head);
}

t_psr			*parser(t_lxr *cur)
{
	void		*prs_head;
	char		**cmd;

	prs_head = NULL;
	cmd = create_simple_cmd(&cur);
	prs_head = create_redirect_cmd(cmd, NULL);
	if (!cur)
		return (prs_head);
	while (cur && cur->token == REDIRECT)
		prs_head = create_redirect_cmd(prs_head, &cur);
	return (prs_head);
}
