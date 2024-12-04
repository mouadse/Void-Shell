#include "../../include/minishell.h"

static t_env_var	*remove_env_var(t_env_var *node, char *key)
{
	t_env_var	*next_node;

	if (!node)
		return (NULL);
	if (ft_strcmp(node->key, key) == 0)
	{
		next_node = node->next;
		return (next_node);
	}
	node->next = remove_env_var(node->next, key);
	return (node);
}

void	unset_env(char **args, t_env_var **env_var_list, int *exit_status)
{
	int			i;
	t_env_var	*temp;

	i = 1;
	while (args[i])
	{
		if (!is_valid_var(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			*exit_status = 1;
			i++;
			continue ;
		}
		if (ft_strcmp((*env_var_list)->key, args[i]) == 0)
		{
			temp = *env_var_list;
			*env_var_list = (*env_var_list)->next;
		}
		else
			*env_var_list = remove_env_var(*env_var_list, args[i]);
		i++;
	}
}
