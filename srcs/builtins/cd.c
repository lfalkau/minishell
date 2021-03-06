/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 16:44:15 by lfalkau           #+#    #+#             */
/*   Updated: 2020/05/14 12:15:26 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdlib.h>

static char	*set_path_with_home(char *arg, t_env *env, t_bool *allocated)
{
	char	*home;
	char	*tmp;

	if (!(home = get_env_var("HOME", env)))
		return (NULL);
	if (!arg)
		return (home);
	else
	{
		if ((tmp = ft_strjoin(home, arg + 1)))
		{
			*allocated = true;
			return (tmp);
		}
		return (NULL);
	}
}

static char	*set_path_with_oldpwd(t_env *env)
{
	char *oldpwd;

	if (!(oldpwd = get_env_var("OLDPWD", env)))
		return (NULL);
	return (oldpwd);
}

static void	save_olpwd(t_env *env)
{
	char	*assignment;
	char	*cwd;

	if ((cwd = getcwd(NULL, 0)))
	{
		if ((assignment = ft_strcjoin("OLDPWD", cwd, '=')))
		{
			make_assignment(env, assignment);
			free(assignment);
		}
		free(cwd);
	}
}

int			ms_cd(int ac, char **av, t_env *env)
{
	char	*path;
	t_bool	allocated;

	av[ac] = NULL;
	allocated = false;
	if (!(path = av[1]) || *path == '~')
		path = set_path_with_home(av[1], env, &allocated);
	if (path && ft_strcmp(path, "-") == 0)
		path = set_path_with_oldpwd(env);
	save_olpwd(env);
	if (path && chdir(path) == -1)
	{
		write(1, "cd: no such file or directory: ", 31);
		write(1, av[1], ft_strlen(av[1]));
		write(1, "\n", 1);
		return (EXIT_FAILURE);
	}
	else if (!path)
	{
		write(1, "cd: HOME env variable not found\n", 32);
		return (EXIT_FAILURE);
	}
	if (path && allocated)
		free(path);
	return (EXIT_SUCCESS);
}
