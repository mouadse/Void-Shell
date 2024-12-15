#include "../../include/minishell.h"

// void	ft_export(char **args, t_env_var **env_var_list, int *exit_status)
// {
// 	int		i;
// 	char	*key;
// 	char	*equal_pos;

// 	if (!args[1])
// 	{
// 		display_env_variables(*env_var_list);
// 		return ;
// 	}
// 	i = 0;
// 	while (args[++i])
// 	{
// 		equal_pos = ft_strchr(args[i], '=');
// 		key = extract_key(args[i], equal_pos);
// 		if (!is_valid_var(key))
// 		{
// 			ft_putstr_fd("minishell: export: `", 2);
// 			ft_putstr_fd(key, 2);
// 			ft_putstr_fd("': not a valid identifier\n", 2);
// 			*exit_status = 1;
// 			i++;
// 			continue ;
// 		}
// 		insert_exported_var(equal_pos, key, env_var_list);
// 	}
// }
void ft_export(char **args, t_env_var **env_var_list, int *exit_status) {
  int i;
  char *key;
  char *equal_pos;
  int is_append;

  if (!args[1]) {
    display_env_variables(*env_var_list);
    return;
  }
  i = 0;
  while (args[++i]) {
    is_append = 0;
    equal_pos = ft_strchr(args[i], '=');
    if (equal_pos && equal_pos > args[i] && equal_pos[-1] == '+') {
      is_append = 1;
      equal_pos[-1] = '\0'; // Temporarily remove '+' for key extraction
    }
    key = extract_key(args[i], equal_pos);
    if (is_append && equal_pos)
      equal_pos[-1] = '+'; // Restore '+' if it was removed

    if (!is_valid_var(key)) {
      ft_putstr_fd("minishell: export: `", 2);
      ft_putstr_fd(key, 2);
      ft_putstr_fd("': not a valid identifier\n", 2);
      *exit_status = 1;
      continue;
    }
    insert_exported_var(equal_pos, key, env_var_list);
  }
}
