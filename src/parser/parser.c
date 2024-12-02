/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:55:50 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 20:22:47 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// we are using the same parser from xv6 kernel

// func prototypes

t_command	*parsepipe(char **ps, char *es, int *ret)
{
	t_command	*cmd;

	cmd = parseexec(ps, es, ret);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = create_pipecmd(cmd, parsepipe(ps, es, ret));
	}
	return (cmd);
}

t_command	*parsecmd(char *line, int *exit_status)
{
	int			ret;
	char		*ps;
	char		*es;
	t_command	*cmd;

	ret = 0;
	ps = line;
	es = ps + ft_strlen(line);
	cmd = parsepipe(&ps, es, &ret);
	peek(&ps, es, "");
	if (ps != es || ret != 0)
	{
		if (ret == 1)
			ft_putstr_fd("Void-shell: too many arguments\n", 2);
		else
			ft_putstr_fd("Void-shell: syntax error\n", 2);
		*exit_status = 258;
		release_command_resources(cmd);
		return (NULL);
	}
	cmd_null_terminate(cmd);
	return (cmd);
}

static void	parse_next_redir(t_command **cmd, t_exec **ecmd, char **ps,
		char *es)
{
	t_command	*tmp;

	if ((*cmd) != (t_command *)(*ecmd))
	{
		tmp = (*cmd);
		while (((t_redir *)tmp)->sub_cmd->type == CMD_REDIR)
			tmp = ((t_redir *)tmp)->sub_cmd;
		((t_redir *)tmp)->sub_cmd = parseredirs(((t_command *)(*ecmd)), ps, es);
	}
	else
		(*cmd) = parseredirs((*cmd), ps, es);
}

t_command	*parseredirs(t_command *sub_cmd, char **ps, char *es)
{
	char		*q;
	char		*eq;
	int			tok;
	t_command	*cmd;

	cmd = sub_cmd;
	if (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		gettoken(ps, es, &q, &eq);
		if (tok == '<')
			cmd = create_redircmd(parseredirs(sub_cmd, ps, es),
					create_redirdata(q, eq, 0, O_RDONLY), '<');
		else if (tok == '>')
			cmd = create_redircmd(parseredirs(sub_cmd, ps, es),
					create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_TRUNC),
					'>');
		else if (tok == '+')
			cmd = create_redircmd(parseredirs(sub_cmd, ps, es),
					create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_APPEND),
					'+');
		else if (tok == '%')
			cmd = create_redircmd(parseredirs(sub_cmd, ps, es),
					create_redirdata(q, eq, 0, O_RDONLY), '%');
	}
	return (cmd);
}

static int	collect_exec_argument(t_exec **exec_cmd, char **ps, char *es,
		int *arg_count)
{
	int		token;
	char	*start;
	char	*end;

	token = gettoken(ps, es, &start, &end);
	if (token == '\0')
		return (0);
	(*exec_cmd)->argv[*arg_count] = start;
	(*exec_cmd)->eargv[*arg_count] = end;
	(*arg_count)++;
	return (1);
}

t_command	*parseexec(char **ps, char *es, int *status)
{
	t_exec		*exec_cmd;
	t_command	*cmd;
	int			arg_count;

	arg_count = 0;
	cmd = create_execcmd();
	exec_cmd = (t_exec *)cmd;
	cmd = parseredirs(cmd, ps, es);
	while (!peek(ps, es, "|"))
	{
		if (!collect_exec_argument(&exec_cmd, ps, es, &arg_count))
			break ;
		if (arg_count >= MAX_ARGS)
		{
			*status = 1;
			release_command_resources(cmd);
			return (NULL);
		}
		parse_next_redir(&cmd, &exec_cmd, ps, es);
	}
	exec_cmd->argv[arg_count] = NULL;
	exec_cmd->eargv[arg_count] = NULL;
	return (cmd);
}
