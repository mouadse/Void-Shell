#include "../include/minishell.h"
#include <fcntl.h>
#include <stdio.h>

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

static void	run_cmd_helper(t_shell_context *context, int *exit_status)
{
	int		status;
	t_exec	*exec;

	set_signal_handler(context->tree);
	if (ft_fork(context) == 0)
	{
		// printf("We are inside fork\n");
		store_subprocess_pid(getpid(), context);
		execute_command(context->tree, context, exit_status);
	}
	// this is the parent process
	waitpid(-1, &status, 0); // wait for the child process to finish
	retrieve_exit_status(context->tree, context, exit_status, status);
	exec = (t_exec *)context->tree;
	// first test is to check if the user demanded to exit the shell
	if ((exec->type == CMD_EXEC) && exec->argv[0] && ft_strcmp(exec->argv[0], "exit") == 0)
	{
		if (!exec->argv[1] || (exec->argv[1] && (!is_numeric(exec->argv[1])
					|| !exec->argv[2])))
		{
			unlink("/tmp/exit_status.tmp");
			unlink("/tmp/child_pid.tmp");
			terminate_cleanly(context, *exit_status);
		}
	}
	clean_shell(context);
}
static void	run_cmd(t_shell_context *context, int *exit_status)
{
	add_history(context->input);
	context->tree = parsecmd(context->input, exit_status);
	if (!context->tree)
	{
		ft_free(context->input);
		return ;
	}
	process_all_commands(context->tree, context, exit_status);
	if (is_built_in_command(context->tree))
	{
		run_built_in_command((t_exec *)context->tree, &context->env_vars,
			exit_status);
		printf("built-in command\n");
		ft_free(context->input);
		release_command_resources(context->tree); // this is to be reviewed
		free_queue(&context->queue);
	}
	else
	{
		run_cmd_helper(context, exit_status);
	}
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
		run_cmd(&context, &exit_status);
	}
	free_env(context.env_vars);
	rl_clear_history();
	return (EXIT_SUCCESS);
}
