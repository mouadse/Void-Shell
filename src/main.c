#include "../include/minishell.h"
#include <complex.h>
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
	context->envp = envp;
	context->env_vars = NULL;
	init_env_var(&context->env_vars, envp);
	init_queue(&context->queue);
	context->last_cmd = ft_strdup("");
	*exit_status = EXIT_SUCCESS;
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
	if ((exec->type == CMD_EXEC) && exec->argv[0] && ft_strcmp(exec->argv[0],
			"exit") == 0)
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
		// ft_free(context->input);
		return ;
	}
	process_all_commands(context->tree, context, exit_status);
	// printf("The cmd u trying to execute is %s\n",
	//        ((t_exec *)context->tree)->argv[0]);
	context->last_cmd = ((t_exec *)context->tree)->argv[0];
	if (is_built_in_command(context->tree))
	{
		run_built_in_command((t_exec *)context->tree, &context->env_vars,
			exit_status);
		printf("built-in command\n");
		// ft_free(context->input);
		release_command_resources(context->tree); // this is to be reviewed
		free_queue(&context->queue);
	}
	else
	{
		run_cmd_helper(context, exit_status);
	}
	insert_env_var(&context->env_vars, create_env_var(ft_strdup("_"),
			context->last_cmd));
	ft_free(context->input);
}

void	fatal_signal_handler(int signum)
{
	// Set the signal flags
	g_gc.signal_received = 1;
	g_gc.signal_number = signum;
}

// Function to set up signal actions
static void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = flags;
	sigaction(signum, &act, NULL);
}

// Initialize signal handlers for fatal signals
void	setup_fatal_signals(void)
{
	setup_signal_action(SIGABRT, fatal_signal_handler, 0);
	setup_signal_action(SIGSEGV, fatal_signal_handler, 0);
	setup_signal_action(SIGBUS, fatal_signal_handler, 0);
	setup_signal_action(SIGTERM, fatal_signal_handler, 0);
}

void	handle_fatal_signals(void)
{
	if (g_gc.signal_received)
	{
		// Perform garbage collector clean-up
		gc_free_all();
		// Optionally print a message based on the received signal
		if (g_gc.signal_number == SIGABRT)
			ft_putstr_fd("Shell terminated by SIGABRT\n", STDERR_FILENO);
		else if (g_gc.signal_number == SIGSEGV)
			ft_putstr_fd("Shell terminated by SIGSEGV\n", STDERR_FILENO);
		else if (g_gc.signal_number == SIGBUS)
			ft_putstr_fd("Shell terminated by SIGBUS\n", STDERR_FILENO);
		else if (g_gc.signal_number == SIGTERM)
			ft_putstr_fd("Shell terminated by SIGTERM\n", STDERR_FILENO);
		else
			ft_putstr_fd("Shell terminated by signal\n", STDERR_FILENO);
		// Exit with a status code based on the signal
		exit(128 + g_gc.signal_number);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int				exit_status;
	t_shell_context	context;

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
			ft_free(context.input);
			context.input = NULL;
			continue ;
		}
		handle_fatal_signals();
		// this is a function that handles the signal exit
		run_cmd(&context, &exit_status);
	}
	free_env(context.env_vars);
	rl_clear_history();
	gc_free_all();
	return (EXIT_SUCCESS);
}
