#include "../include/minishell.h"

static int	is_shell_input_valid(char *input, int *exit_status)
{
	if (!ft_strlen(input) || is_whitespace_string(input)
		|| !check_command_validity(input, exit_status))
		return (0);
	return (1);
}

static void	init_shell_env(t_shell_context *context, char **envp,
		int *exit_status)
{
	*exit_status = EXIT_SUCCESS;
	context->envp = envp;
	context->env_vars = NULL;
	init_queue(&context->queue);
	init_env_var(&context->env_vars, envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_context	context;
	int				exit_status;

	(void)argc;
	(void)argv;
	init_shell_env(&context, envp, &exit_status);
	while (1)
	{
		setup_signals();
		context.input = readline("void-shell$ "); // colors later
		if (!context.input)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (!is_shell_input_valid(context.input, &exit_status))
		{
			free(context.input);
			continue ;
		}
		add_history(context.input); // history for readline
									// run the shell command
	}
	free_env(context.env_vars);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
