/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 00:03:09 by msennane          #+#    #+#             */
/*   Updated: 2024/12/19 00:07:01 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	context->envp = envp;
	context->env_vars = NULL;
	init_env_var(&context->env_vars, envp);
	init_queue(&context->queue);
	context->last_cmd = NULL;
	*exit_status = EXIT_SUCCESS;
}

static void	run_cmd_helper(t_shell_context *context, int *exit_status)
{
	int		status;
	t_exec	*exec;

	set_signal_handler(context->tree);
	if (ft_fork(context) == 0)
	{
		store_subprocess_pid(getpid(), context);
		execute_command(context->tree, context, exit_status);
	}
	waitpid(-1, &status, 0);
	retrieve_exit_status(context->tree, context, exit_status, status);
	exec = (t_exec *)context->tree;
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

// static void	run_cmd(t_shell_context *context, int *exit_status)
// {
// 	t_exec	*exec_cmd;

// 	context->tree = parsecmd(context->input, exit_status);
// 	if (!context->tree)
// 		return ;
// 	if (count_redirections(context->tree) > 16)
// 	{
// 		ft_putstr_fd("void-shell: maximum here-document count exceeded\n", 2);
// 		*exit_status = 2;
// 		ft_free(context->input);
// 		return ;
// 	}
// 	process_all_commands(context->tree, context, exit_status);
// 	if (context->tree && context->tree->type == CMD_EXEC)
// 	{
// 		exec_cmd = (t_exec *)context->tree;
// 		insert_env_var(&context->env_vars, create_env_var("_",
// 				exec_cmd->argv[0]));
// 	}
// 	if (!context->input)
// 	{
// 		if (context->tree && context->tree->type == CMD_EXEC)
// 		{
// 			exec_cmd = (t_exec *)context->tree;
// 			insert_env_var(&context->env_vars, create_env_var("_",
// 					exec_cmd->argv[0]));
// 		}
// 		else
// 		{
// 			insert_env_var(&context->env_vars, create_env_var("_",
// 					context->input));
// 		}
// 	}
// 	if (is_built_in_command(context->tree))
// 	{
// 		run_built_in_command((t_exec *)context->tree, &context->env_vars,
// 			exit_status);
// 		release_command_resources(context->tree);
// 		free_queue(&context->queue);
// 	}
// 	else
// 		run_cmd_helper(context, exit_status);
// 	ft_free(context->input);
// }

static void	check_redirect_limits(t_shell_context *context, int *exit_status)
{
	if (count_redirections(context->tree) > 16)
	{
		ft_putstr_fd("void-shell: maximum here-document count exceeded\n", 2);
		*exit_status = 2;
		ft_free(context->input);
	}
}

static void	update_env_underscore(t_shell_context *context)
{
	t_exec	*exec_cmd;

	if (context->tree && context->tree->type == CMD_EXEC)
	{
		exec_cmd = (t_exec *)context->tree;
		insert_env_var(&context->env_vars, create_env_var("_",
				exec_cmd->argv[0]));
	}
	else
		insert_env_var(&context->env_vars, create_env_var("_", context->input));
}

static void	execute_built_in(t_shell_context *context, int *exit_status)
{
	run_built_in_command((t_exec *)context->tree, &context->env_vars,
		exit_status);
	release_command_resources(context->tree);
	free_queue(&context->queue);
}

static void	run_cmd(t_shell_context *context, int *exit_status)
{
	context->tree = parsecmd(context->input, exit_status);
	if (!context->tree)
		return ;
	check_redirect_limits(context, exit_status);
	if (*exit_status == 2)
		return ;
	process_all_commands(context->tree, context, exit_status);
	if (!context->input)
		update_env_underscore(context);
	if (is_built_in_command(context->tree))
		execute_built_in(context, exit_status);
	else
		run_cmd_helper(context, exit_status);
	ft_free(context->input);
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
		context.input = readline("void-shell$ ");
		if (!context.input)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		add_history(context.input);
		if (!is_shell_input_valid(context.input, &exit_status))
		{
			ft_free(context.input);
			context.input = NULL;
			continue ;
		}
		run_cmd(&context, &exit_status);
	}
	rl_clear_history();
	gc_free_all();
	return (EXIT_SUCCESS);
}
