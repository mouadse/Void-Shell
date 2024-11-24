#include "../include/minishell.h"

static int	is_shell_input_valid(char *input, int *exit_status)
{
	if (!ft_strlen(input) || is_whitespace_string(input)
		|| !check_command_validity(input, exit_status))
		return (0);
	return (1);
}

int	main(void)
{
	t_shell_context	context;
	int				exit_status;

	
	return (EXIT_SUCCESS);
}
