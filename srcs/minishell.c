/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 17:37:37 by lfalkau           #+#    #+#             */
/*   Updated: 2020/03/24 23:33:58 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "keys.h"

struct termios	g_ogterm;

// static void lex_printlst(t_lex_lst *lst)
// {
// 	while (lst)
// 	{
// 		printf("\033[0;93mT: \033[0;00m%i, \033[0;93mR: \033[0;00m%s\n", lst->token, lst->raw);
// 		lst = lst->next;
// 	}
// }

// printf("\n\033[0;95mcmd: \033[0;00m%s\n", cmd);
// lex_printlst(lst);

int	minishell(char **env)
{
	char		*cmd;
	t_lex_lst	*lst;

	while (true)
	{
		prompt(env);
		cmd = get_cmd();
		write(1, "\n", 1);
		save_cmd(cmd, HISTORY_PATH);
		lst = lexer(cmd);
		if (fork() == 0)
			get_simple_cmd(lst, env);
		else
			wait(0);
		free(cmd);
	}
	return (0);
}
