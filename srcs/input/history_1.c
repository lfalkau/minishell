/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/10 23:03:24 by lfalkau           #+#    #+#             */
/*   Updated: 2020/04/12 20:38:25 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void	hst_replace_last(t_hst **hst, t_dynstr *dstr)
{
	char	*cmd;

	if (!(cmd = ft_strdup((*hst)->cmd)))
		return ;
	while ((*hst)->next)
		*hst = (*hst)->next;
	free((*hst)->cmd);
	(*hst)->cmd = cmd;
	dstr->str = cmd;
}

void	hst_save(char *cmd)
{
	int fd;

	fd = open_reg_file(HISTORY_PATH, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd != -1)
	{
		write(fd, cmd, ft_strlen(cmd));
		write(fd, "\n", 1);
		close(fd);
	}
}
