/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:06:13 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 14:39:31 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"





static char	*read_heredoc_input(char *del, t_shell_context *context,
		int *exit_status)
{
	char	*line;
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
	if (!clean_del)
		clean_del = ft_strdup("");
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (clean_del[0] == '\0')
		{
			if (line[0] == '\n')
				break ;
		}
		else if (ft_strlen(line) >= ft_strlen(clean_del) && ft_strncmp(line,
				clean_del, ft_strlen(clean_del)) == 0
			&& line[ft_strlen(clean_del)] == '\n')
			break ;
		if (is_quoted)
			enqueue(&queue, ft_strdup(line));
		else
			enqueue(&queue, process_line_hd(context, line, exit_status));
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
	ignore_signals_heredoc();
	if (redir_cmd->redir_type != '%')
	{
		if (ft_strcmp(redir_cmd->file, "\x01\x01\x01") == 0)
		{
			*exit_status = 1;
			terminate_cleanly(context, *exit_status);
		}
		if (!redir_cmd->file)
		{
			ft_putstr_fd("minishell: No such file or directory\n",
				STDERR_FILENO);
			*exit_status = 1;
			terminate_cleanly(context, *exit_status);
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
