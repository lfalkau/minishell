/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 15:16:22 by lfalkau           #+#    #+#             */
/*   Updated: 2020/04/03 16:27:37 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_get(char **e)
{
	t_env	*env;
	t_env	*tmp;
	int 	i;

	i = 0;
	env = NULL;
	while (e[i])
	{
		if (!(tmp = malloc(sizeof(t_env))))
			return (env_free(env));
		tmp->next = env;
		env = tmp;
		tmp->value = ft_strdup(ft_strchr(e[i], '=') + 1);
		tmp->key = ft_strndup(e[i], ft_strlen(e[i]) - ft_strlen(tmp->value) - 1);
		i++;
	}
	return (env);
}

// Return the content of a given environment variable name
char	*get_env_var(char *var_name, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(var_name, env->key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	env_size(t_env *env)
{
	int i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

char **env_to_arr(t_env *e)
{
	int		i;
	int		j;
	char	**a;
	char	*v;
	char	*t;

	i = env_size(e);
	if (!(a = malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	a[i] = NULL;
	j = 0;
	while (j < i)
	{
		if (!(v = ft_strjoin(e->key, "=")) || !(t = ft_strjoin(v, e->value)))
		{
			a[i] = NULL;
			return (a);
		}
		a[j] = t;
		free(v);
		j++;
		e = e->next;
	}
	return (a);
}

// Temporary function, DON'T FORGET TO REMOVE before submitting project
void	env_print(t_env *env)
{
	while (env)
	{
		printf("KEY: |%s|, VALUE: |%s|\n", env->key, env->value);
		env = env->next;
	}
}

void	*env_free(t_env *env)
{
	env = NULL;
	return (env);
}
