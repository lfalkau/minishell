/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/24 16:45:43 by lfalkau           #+#    #+#             */
/*   Updated: 2020/03/25 12:12:11 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ms_pwd(int ac, char **av, char **env)
{
	av[ac] = NULL;
	write(1, get_env_var("PWD=", env), ft_strlen(get_env_var("PWD=", env)));
	write(1, "\n", 1);
	return (0);
}