/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:06:13 by msennane          #+#    #+#             */
/*   Updated: 2024/12/12 22:43:29 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*getvar_name(char *arg)
{
	int	i;
	int	n;

	i = 0;
	if (ft_isdigit(arg[i]))
	{
		n = arg[i] - '0';
		return (ft_itoa(n));
	}
	while (arg[i] && ((ft_isalnum(arg[i]) || arg[i] == '_')))
		i++;
	return (ft_substr(arg, 0, i));
}

static void	replace_var_in_line(char *line, int *i, t_shell_context *context,
		t_queue_char *q)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = getvar_name(line + *i);
	var_value = get_env_value(var_name, context->env_vars);
	if (var_value)
		enqueue_str(q, var_value);
	*i += ft_strlen(var_name);
}

static char	*process_line(t_shell_context *context, char *line,
		int *exit_status)
{
	int				i;
	t_queue_char	q;
	char			*exit_code;

	i = 0;
	init_queue_char(&q);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			exit_code = ft_itoa(*exit_status);
			enqueue_str(&q, exit_code);
			i += 2;
		}
		else if (line[i] == '$' && (is_whitespace(line[i + 1])))
			enqueue_char(&q, line[i++]);
		else if (line[i] == '$')
			replace_var_in_line(line, &i, context, &q);
		else
			enqueue_char(&q, line[i++]);
	}
	return (queue_char_str_convert(&q));
}

static char	*read_heredoc_input(char *del, t_shell_context *context,
		int *exit_status)
{
	char	*line;
	char	*heredoc_line;
	t_queue	queue;
	int		tty_fd;
	int		is_quoted;
	char	*clean_del;

	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		terminate_with_error(context, "open", 1);
	dup2(tty_fd, STDIN_FILENO);
	ft_close(context, tty_fd);
	init_queue(&queue);
	is_quoted = 0;
	clean_del = clean_delimiter(del, &is_quoted);
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, clean_del, ft_strlen(clean_del)) == 0
			&& line[ft_strlen(clean_del)] == '\n')
			break ;
		if(is_quoted)
			heredoc_line = ft_strdup(line);
		else
			heredoc_line = process_line(context, line, exit_status);
		enqueue(&queue, heredoc_line);
	}
	return (queue_str_convert(&queue));
}

static void	write_heredoc_file(t_shell_context *context, char *content)
{
	int	fd;

	if (!content)
		terminate_cleanly(context, 0);
	fd = open(SHELL_HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		terminate_with_error(context, "open", 1);
	if (write(fd, content, ft_strlen(content)) < 0)
		terminate_with_error(context, "write", 1);
	ft_close(context, fd);
}

void	execute_redirects_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	t_redir	*redir_cmd;
	char	*heredoc_content;

	redir_cmd = (t_redir *)cmd;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (redir_cmd->redir_type != '%')
	{
		if (is_ambiguous_redirect(redir_cmd->file))
		{
			ft_putstr_fd("Void-shell: ", STDERR_FILENO);
			ft_putstr_fd(redir_cmd->file, STDERR_FILENO);
			ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
			*exit_status = 1;
			terminate_cleanly(context, *exit_status);
			return ;
		}
		ft_close(context, redir_cmd->fd);
		if (open(redir_cmd->file, redir_cmd->mode, 0644) < 0)
			terminate_with_error(context, "open", 1);
	}
	else
	{
		heredoc_content = read_heredoc_input(redir_cmd->file, context,
				exit_status);
		write_heredoc_file(context, heredoc_content);
		ft_close(context, redir_cmd->fd);
		if (open(SHELL_HEREDOC_FILE, redir_cmd->mode, 0644) < 0)
			terminate_with_error(context, "open", 1);
	}
	execute_command(redir_cmd->sub_cmd, context, exit_status);
}
