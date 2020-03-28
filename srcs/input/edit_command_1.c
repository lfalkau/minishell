/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_command_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/18 18:31:02 by lfalkau           #+#    #+#             */
/*   Updated: 2020/03/28 14:59:37 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The t_cms structure is a dynamic array, including a 'char * raw' that wrap
// the command itself, a 'size_t len' that contains the cmd length,
// a 'size_t capacity' that indicates how many bytes have been allocated,
// that is also the max length cmd can be and a 'size_t cpos' showing us
// the cursor position is the raw, vefry useful for command editing.

// Return an initialized command struct
t_cmd *new_cmd()
{
	t_cmd *cmd;

	if (!(cmd = malloc(sizeof(t_cmd))))
		return (NULL);
	if (!(cmd->raw = malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (NULL);
	ft_memset(cmd->raw, 0, BUFFER_SIZE + 1);
	cmd->capacity = BUFFER_SIZE;
	cmd->len = 0;
	cmd->cpos = 0;
	return (cmd);
}

// Realloc the command struct with two times more space
t_bool stretch(t_cmd *cmd)
{
	char	*new_raw;

	cmd->capacity *= 2;
	if (!(new_raw = malloc(sizeof(char) * (cmd->capacity + 1))))
		return (false);
	ft_memset(new_raw, 0, cmd->capacity + 1);
	ft_memcpy(new_raw, cmd->raw, cmd->len);
	free(cmd->raw);
	cmd->raw = new_raw;
	return (true);
}

// Concatenate c2 to c1->raw and update the struct
t_bool join_last_commands(t_hst **hst)
{
	t_cmd	*c1;
	t_cmd	*c2;
	char	*new_cmd;
	size_t	len;

	c1 = (*hst)->cmd;
	c2 = (*hst)->prev->cmd;
	pop(c2);
	if (!(new_cmd = ft_strjoin(c2->raw, c1->raw)))
		return (false);
	len = ft_strlen(new_cmd);
	c2->raw = new_cmd;
	c2->len = len;
	c2->capacity = len;
	c2->cpos = len;
	(*hst) = (*hst)->prev;
	free_cmd((*hst)->next->cmd);
	(*hst)->next = NULL;
	return (true);
}

// Delete the command's content, without freeing it as we can reuse it later
void erase(t_cmd *cmd)
{
	ft_memset(cmd->raw, 0, cmd->capacity);
	cmd->len = 0;
	cmd->cpos = 0;
}

void	free_cmd(t_cmd *cmd)
{
	free(cmd->raw);
	free(cmd);
}
