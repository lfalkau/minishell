/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfalkau <lfalkau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/03 15:16:22 by lfalkau           #+#    #+#             */
/*   Updated: 2020/04/06 10:54:20 by lfalkau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Return a (t_env *) linked list based on (char **env) passed to minishell
t_env	*env_get(char **e)
{
	t_env	*env;
	t_env	*t;
	int 	i;

	i = 0;
	env = NULL;
	while (e[i])
	{
		if (!(t = malloc(sizeof(t_env))))
			return (env_free(env));
		t->next = env;
		env = t;
		t->value = ft_strdup(ft_strchr(e[i], '=') + 1);
		t->key = ft_strndup(e[i], ft_strlen(e[i]) - ft_strlen(t->value) - 1);
		i++;
	}
	return (env);
}

// Return the value of a given environment variable key, NULL if not set
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

// Computes the length of the given t_env linked list
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

// Turn back a (t_env *) to a (char **) array, useful to give argv to execve
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

// Push an element to the given (t_env *) linked list
int		env_push_back(t_env *env, char *key, char *value)
{
	t_env	*new;

	if (!(new = (t_env *)malloc(sizeof(t_env))))
	{
		free(key);
		free(value);
		return (EXIT_FAILURE);
	}
	new->key = key;
	new->value = value;
	while (env && env->next)
		env = env->next;
	env->next = new;
	new->next = NULL;
	return (EXIT_SUCCESS);
}

// Remove the first element by freeing
void	env_remove_first(t_env *env)
{
	t_env	*tmp;

	free(env->key);
	free(env->value);
	if ((tmp = env->next))
	{
		env->key = tmp->key;
		env->value = tmp->value;
		env->next = tmp->next;
		free(tmp);
	}
}

// Remove a specified item from (t_env *) linked list
void	env_remove_elm(t_env *env, t_env *elm)
{
	t_env	*tmp;

	while (env->next != elm)
		env = env->next;
	tmp = elm->next;
	free(elm->key);
	free(elm->value);
	free(elm);
	env->next = tmp;
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

// Fully free env
void	*env_free(t_env *env)
{
	t_env *tmp;

	while (env)
	{
		tmp = env;
		free(tmp->key);
		free(tmp->value);
		env = env->next;
		free(tmp);
	}
	return (NULL);
}