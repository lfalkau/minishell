/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/19 00:35:57 by lfalkau           #+#    #+#             */
/*   Updated: 2020/03/26 10:41:01 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "prompt.h"
#include "keys.h"

// Can either return the command or start the multiline command process
char	*cmd_return(t_cmd *cmd)
{
	char	*next_line;
	char	*command;

	if (cmd->len > 0 && cmd->raw[cmd->len - 1] == '\\')
	{
		write(1, NEW_LINE_PROMPT, 18);
		next_line = get_cmd();
		join_commands(cmd, next_line);
	}
	command = cmd->raw;
	free(cmd);
	return (command);
}

// Handle cursor movement, and history later
void	cmd_arrows(t_cmd *cmd, char *buf)
{
	if (*buf++ == '[')
	{
		if (*buf == ESC_KEY_RIGHT)
			if (can_move_cursor(cmd, right))
				write(1, CURSOR_RIGHT, 3);
		if (*buf == ESC_KEY_LEFT)
			if (can_move_cursor(cmd, left))
				write(1, CURSOR_LEFT, 3);
	}
}

// Delete the character just before the cursor, if there's some
void	cmd_backspace(t_cmd *cmd)
{
	int cpos_diff;

	if (pop(cmd))
	{
		write(1, CURSOR_LEFT, 3);
		write(1, cmd->raw + cmd->cpos, cmd->len - cmd->cpos + 1);
		write(1, " ", 1);
		cpos_diff = cmd->len - cmd->cpos + 1;
		move_cursor(left, cpos_diff);
	}
}

// Handle all printable characters, adding them at the good position in the cmd
void	cmd_character(t_cmd *cmd, char *buf)
{
	int		cpos_diff;

	while (ft_isprint(*buf) && push(*buf, cmd))
	{
		write(1, buf, 1);
		write(1, cmd->raw + cmd->cpos, cmd->len - cmd->cpos + 1);
		cpos_diff = cmd->len - cmd->cpos;
		move_cursor(left, cpos_diff);
		buf++;
	}
}

// Bash style Ctrl-D, exit shell if empty command, DEL's behaviour otherwise
t_bool	cmd_ctrld_shoould_exit(t_cmd *cmd)
{
	if (cmd->len == 0)
	{
		if (cmd->capacity < 5)
			return (false);
		ft_memcpy(cmd->raw, "exit\0", 5);
		write(1, "exit", 4);
		return (true);
	}
	if (cmd->cpos < cmd->len)
	{
		write(1, CURSOR_RIGHT, 3);
		cmd->cpos++;
		cmd_backspace(cmd);
	}
	return (false);
}

// Ctrl-U should erase the command
void	cmd_ctrlu(t_cmd *cmd)
{
	move_cursor(left, cmd->cpos);
	fill_with(' ', cmd->len);
	move_cursor(left, cmd->len);
	erase(cmd);
}

// Main loop of input, reads and processes input, only returns the raw of cmd
char	*get_cmd()
{
	t_cmd	*cmd;
	char	buf[5];

	cmd = new_cmd();
	while (true)
	{
		ft_memset(buf, 0, 5);
		read(0, &buf, 4);
		if (*buf == ESCAPE_KEY)
			cmd_arrows(cmd, buf + 1);
		else if (*buf == RETURN_KEY)
			break;
		else if (*buf == BACKSPACE_KEY)
			cmd_backspace(cmd);
		else if (*buf == CTRL_D_KEY && cmd_ctrld_shoould_exit(cmd))
			break;
		else if (*buf == CTRL_U_KEY)
			cmd_ctrlu(cmd);
		else
			cmd_character(cmd, buf);
	}
	return (cmd_return(cmd));
}
