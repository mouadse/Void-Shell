/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:55:50 by msennane          #+#    #+#             */
/*   Updated: 2024/11/07 02:09:00 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// we are using the same parser from xv6 kernel

// func prototypes
t_command			*parsecmd(char *line, int *exit_status);
t_command			*parsepipe(char **ps, char *es, int *ret);
t_command			*parseredirs(t_command *sub_cmd, char **ps, char *es);
t_command			*parseexec(char **ps, char *es, int *ret);

t_command	*parsepipe(char **ps, char *es, int *ret)
{
	t_command	*left;
	t_command	*right;

	left = parseexec(ps, es, ret);
	if (peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		right = parsepipe(ps, es, ret);
		left = create_pipecmd(left, right);
	}
	return (left);
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

static t_command	*handle_redirect(t_command *sub_cmd, char **ps, char *es,
		int tok, char *q, char *eq)
{
	t_command		*cmd;
	t_redir_data	redir;

	cmd = sub_cmd;
	if (tok == '<')
	{
		redir = create_redirdata(q, eq, 0, O_RDONLY);
		cmd = create_redircmd(parseredirs(sub_cmd, ps, es), redir, '<');
	}
	else if (tok == '>')
	{
		redir = create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_TRUNC);
		cmd = create_redircmd(parseredirs(sub_cmd, ps, es), redir, '>');
	}
	else if (tok == '+')
	{
		redir = create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_APPEND);
		cmd = create_redircmd(parseredirs(sub_cmd, ps, es), redir, '+');
	}
	else if (tok == '%')
	{
		redir = create_redirdata(q, eq, 0, O_RDONLY);
		cmd = create_redircmd(parseredirs(sub_cmd, ps, es), redir, '%');
	}
	return (cmd);
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
		cmd = handle_redirect(sub_cmd, ps, es, tok, q, eq);
	}
	return (cmd);
}
