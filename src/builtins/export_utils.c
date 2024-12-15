#include "../../include/minishell.h"

int	is_valid_var(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (0);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*extract_key(char *str, char *equal_pos)
{
	char	*key;

	if (!equal_pos)
		return (ft_strdup(str));
	key = ft_substr(str, 0, equal_pos - str);
	return (key);
}

// void	insert_exported_var(char *equal_sign, char *key,
// 		t_env_var **env_var_list)
// {
// 	t_env_var	*new_node;

// 	if (!equal_sign)
// 	{
// 		new_node = create_env_var(key, NULL);
// 		insert_env_var(env_var_list, new_node);
// 	}
// 	else
// 	{
// 		new_node = create_env_var(key, ft_strdup(equal_sign + 1));
// 		insert_env_var(env_var_list, new_node);
// 	}
// }

void	insert_exported_var(char *equal_sign, char *key,
		t_env_var **env_var_list)
{
	t_env_var	*new_node;
	char		*value;
	char		*existing_value;
	char		*new_value;

	if (!equal_sign)
	{
		new_node = create_env_var(key, NULL);
		insert_env_var(env_var_list, new_node);
	}
	else
	{
		value = ft_strdup(equal_sign + 1);
		if (equal_sign[-1] == '+') // Check for += operator
		{
			existing_value = get_env_value(key, *env_var_list);
			if (existing_value)
			{
				new_value = ft_strjoin(existing_value, value);
				value = new_value;
			}
		}
		new_node = create_env_var(key, value);
		insert_env_var(env_var_list, new_node);
	}
}

void	display_env_variables(t_env_var *env_var_list)
{
	t_env_var	*current;

	current = env_var_list;
	while (current)
	{
		if (current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
}

void	simple_export(char **args, t_env_var *env_var_list)
{
	if (!args[1])
	{
		display_env_variables(env_var_list);
		return ;
	}
}
