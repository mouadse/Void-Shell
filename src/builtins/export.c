#include "../../include/minishell.h"

static void	ft_put_error(const char *arg)
{
	ft_putstr_fd("minishell: export: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

// Check if character is valid in identifier
static int	is_valid_char(char c)
{
	return (isalpha(c) || isdigit(c) || c == '_');
}

// Validate identifier and extract operator
static int	is_valid_identifier(const char *str, char *op)
{
	size_t	i;

	i = 0;
	// Validate each character before '=' or '+'
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		if (!is_valid_char(str[i]))
		{
			ft_put_error(str);
			return (0);
		}
		i++;
	}
	// Check for empty identifier, starts with digit, or invalid '+'
	if (i == 0 || isdigit((unsigned char)str[0]) || (str[i] == '+' && str[i
			+ 1] != '='))
	{
		ft_put_error(str);
		return (0);
	}
	// Extract operator if present
	if (str[i] == '=')
	{
		*op = '=';
	}
	else if (str[i] == '+')
	{
		*op = '+';
	}
	else
	{
		*op = '\0'; // No operator present
	}
	return (1);
}

// Check if variable exists in the environment variables list
static t_env_var	*find_env_var(t_env_var *env_vars, const char *str)
{
	size_t	key_length;

	key_length = 0;
	// Determine the length of the key in the input string
	while (str[key_length] && str[key_length] != '=' && str[key_length] != '+')
	{
		key_length++;
	}
	// Traverse the environment variables list to find a match
	while (env_vars)
	{
		if (ft_strncmp(str, env_vars->key, key_length) == 0
			&& env_vars->key[key_length] == '\0')
		{
			return (env_vars);
		}
		env_vars = env_vars->next;
	}
	return (NULL);
}

// Handle exporting of variables
static void	export_variables(t_env_var **env_vars, char **args)
{
	size_t		i;
	char		operator_char;
	t_env_var	*existing_var;
	t_env_var	*updated_node;
	char		*value_start;
	char		*current_value;
	char		*new_value;

	i = 1;
	while (args[i])
	{
		if (is_valid_identifier(args[i], &operator_char))
		{
			existing_var = find_env_var(*env_vars, args[i]);
			if (!existing_var)
			{
				// Variable does not exist; add it to the list
				extract_and_push(env_vars, args[i]);
			}
			else
			{
				value_start = NULL;
				if ((value_start = strchr(args[i], '=')) != NULL
					&& operator_char == '=')
				{
					// Handle simple assignment
					value_start++; // Move past '='
					updated_node = create_env_var(existing_var->key,
							ft_strdup(value_start));
					update_env_var(*env_vars, updated_node);
				}
				else if ((value_start = strchr(args[i], '=')) != NULL
					&& operator_char == '+')
				{
					// Handle appending to existing value
					current_value = get_env_value(existing_var->key, *env_vars);
					new_value = ft_strjoin(current_value, value_start + 1);
					if (!new_value)
					{
						perror("ft_strjoin failed");
						exit(EXIT_FAILURE);
					}
					updated_node = create_env_var(existing_var->key, new_value);
					update_env_var(*env_vars, updated_node);
				}
			}
		}
		i++;
	}
}

// Main export function
void	ft_export(t_env_var **env_vars, char **args)
{
	t_env_var	*current;

	if (args[1])
	{
		export_variables(env_vars, args);
	}
	else
	{
		// If no arguments are provided, display all environment variables
		current = *env_vars;
		while (current)
		{
			if (!current->value)
			{
				printf("declare -x %s\n", current->key);
			}
			else
			{
				printf("declare -x %s=\"%s\"\n", current->key, current->value);
			}
			current = current->next;
		}
	}
}
