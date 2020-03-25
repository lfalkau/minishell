#include "minishell.h"
#include "tokens.h"
#include "libft.h"

static int	get_cmd_length(t_lex_lst *head)
{
	int	i;

	i = 0;
	while (head && head->token == WORD)
	{
		i++;
		head = head->next;
	}
	return (i);
}

static char	**create_simple_cmd(t_lex_lst **head)
{
	int	i;
	char	**cmd;

	if (!(*head))
		return (NULL);
	i = get_cmd_length(*head);
	while (*head && (*head)->token == WORD)
	{
		cmd[i++] = (*head)->raw;
		*head = (*head)->next;
	}
	cmd[i] = 0;
	return (cmd);
}
		

static t_rdct	*create_redirect_cmd(void *left, t_lex_lst **cur)
{
	t_rdct	*lst_head;

	if (!(lst_head = (t_rdct *)malloc(sizeof(t_rdct))))
		return (NULL);
	lst_head->left = left;
	if (!(*cur) || (*cur)->token != REDIRECT)
		lst_head->type = 0;
	else if (!ft_strncmp((*cur)->raw, "||", 3))
		lst_head->type = PIPE;
	else if (!ft_strncmp((*cur)->raw,"<", 2))
		lst_head->type = LESS;
	else if (!ft_strncmp((*cur)->raw, "<<", 3))
		lst_head->type = DLESS;
	else if (!ft_strncmp((*cur)->raw, ">", 2))
		lst_head->type = GREAT;
	else if (!ft_strncmp((*cur)->raw, ">>", 3))
		lst_head->type = DGREAT;
	if (*cur)
		*cur = (*cur)->next;
	lst_head->right = create_simple_cmd(cur);
	return (lst_head);
}

t_rdct		*parser(t_lex_lst *cur)
{
	void		*prs_head;
	char		**cmd;

	prs_head = NULL;
	cmd = create_simple_cmd(&cur);
	if (!cur)
		return (create_redirect_cmd(cmd, NULL));
	while (cur && cur->token == REDIRECT)
	{
		if (!prs_head)
			prs_head = create_redirect_cmd(cmd, &cur);
		else
			prs_head = create_redirect_cmd(prs_head, &cur);
	}
	return (prs_head);
}	