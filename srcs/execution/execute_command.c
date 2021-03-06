/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/16 12:25:01 by lfalkau           #+#    #+#             */
/*   Updated: 2020/05/14 15:45:02 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

static void		*get_builtin_func(char *exename)
{
	int exelen;

	exelen = ft_strlen(exename) + 1;
	if (ft_strncmp("exit", exename, exelen) == 0)
		return (&ms_exit);
	if (ft_strncmp("cd", exename, exelen) == 0)
		return (&ms_cd);
	if (ft_strncmp("echo", exename, exelen) == 0)
		return (&ms_echo);
	if (ft_strncmp("env", exename, exelen) == 0)
		return (&ms_env);
	if (ft_strncmp("unset", exename, exelen) == 0)
		return (&ms_unset);
	if (ft_strncmp("export", exename, exelen) == 0)
		return (&ms_export);
	if (ft_strncmp("pwd", exename, exelen) == 0)
		return (&ms_pwd);
	if (ft_strncmp("alias", exename, exelen) == 0)
		return (&ms_alias);
	if (ft_strncmp("unalias", exename, exelen) == 0)
		return (&ms_unalias);
	return (NULL);
}

static int		try_each_path(char **pathes, char **av, char **env)
{
	int		i;
	char	*exepath;

	i = 0;
	while (pathes[i])
	{
		if (!(exepath = ft_strcjoin(pathes[i], av[0], '/')))
		{
			ft_free_array(pathes);
			ft_free_array(env);
			return (EXIT_FAILURE);
		}
		execve(exepath, av, env);
		free(exepath);
		i++;
	}
	ft_free_array(pathes);
	return (EXIT_SUCCESS);
}

static void		execute_binary(char **av, t_env *env)
{
	char		**pathes;
	char		*pathvar;
	char		**e;
	extern int	errno;

	if (!(e = env_to_arr(env)))
		exit(EXIT_FAILURE);
	if ((pathvar = get_env_var("PATH", env)) && ft_isalnum(*av[0]))
	{
		if (!(pathes = ft_split(pathvar, ':')))
		{
			ft_free_array(e);
			exit(EXIT_FAILURE);
		}
		if ((try_each_path(pathes, av, e)) == EXIT_FAILURE)
			exit(EXIT_FAILURE);
	}
	execve(*av, av, e);
	ft_free_array(e);
	errno == ENOENT ? write(1, "minishell: command not found: ", 30) :
		write(1, "minishell: permission denied: ", 30);
	write(1, av[0], ft_strlen(av[0]));
	write(1, "\n", 1);
	exit(errno == ENOENT ? 127 : 126);
}

void			adjust_exit_status(int *status)
{
	if (WIFEXITED(*status))
		*status = *status ? WEXITSTATUS(*status) : EXIT_SUCCESS;
	else if (WIFSIGNALED(*status))
		*status = 128 + WTERMSIG(*status);
}

unsigned char	execute_command(t_ast *ast, t_env *env)
{
	int		pid;
	int		status;
	char	**av;
	int		(*f)(int ac, char **av, t_env *env);

	if (!(av = ast->cmd))
		return (EXIT_FAILURE);
	status = 0;
	if ((f = get_builtin_func(av[0])))
		status = f(arglen(av), av, env);
	else
	{
		if ((pid = fork()) < 0)
			return (EXIT_FAILURE);
		else if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			execute_binary(av, env);
		}
		waitpid(pid, &status, 0);
		adjust_exit_status(&status);
	}
	return ((unsigned char)status);
}
