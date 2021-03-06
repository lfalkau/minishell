/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/14 20:13:35 by lfalkau           #+#    #+#             */
/*   Updated: 2020/05/14 15:55:34 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void		pipe_init(int *fd, int *pid, int *status)
{
	pid[0] = 0;
	pid[1] = 0;
	status[0] = EXIT_SUCCESS;
	status[1] = EXIT_SUCCESS;
	pipe(fd);
}

static void		set_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

unsigned char	pipeline(t_ast *ast, t_env *env)
{
	int				fd[2];
	int				pid[2];
	int				status[2];
	int				fexit;

	pipe_init(fd, pid, status);
	if ((pid[0] = fork()) == 0)
	{
		set_signals();
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		exit(execute(ast->left, env));
	}
	if ((pid[1] = fork()) == 0)
	{
		set_signals();
		dup2(fd[0], STDIN_FILENO);
		close(fd[1]);
		exit(execute(ast->right, env));
	}
	fexit = waitpid(-1, &status[0], 0);
	close(fexit == pid[1] ? fd[0] : fd[1]);
	waitpid(fexit == pid[1] ? pid[0] : pid[1], &status[1], 0);
	adjust_exit_status((fexit == pid[1] ? &status[0] : &status[1]));
	return ((unsigned char)(fexit == pid[1] ? status[0] : status[1]));
}
