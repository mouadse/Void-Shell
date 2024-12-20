/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:06:13 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 16:25:57 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int	is_delimiter(char *line, char *clean_del)
{
	if (clean_del[0] == '\0')
		return (line[0] == '\n');
	if (ft_strlen(line) < ft_strlen(clean_del))
		return (0);
	if (ft_strncmp(line, clean_del, ft_strlen(clean_del)) != 0)
		return (0);
	return (line[ft_strlen(clean_del)] == '\n');
}

static void	init_heredoc(t_shell_context *context, t_queue *queue,
			char **clean_del, char *del)
{
	int	tty_fd;
	int	is_quoted;

	is_quoted = 0;
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd < 0)
		terminate_with_error(context, "open", 1);
	dup2(tty_fd, STDIN_FILENO);
	ft_close(context, tty_fd);
	init_queue(queue);
	*clean_del = clean_delimiter(del, &is_quoted);
	if (!*clean_del)
		*clean_del = ft_strdup("");
}

char	*read_heredoc_input(char *del, t_shell_context *context,
			int *exit_status)
{
	char	*line;
	t_queue	queue;
	int		is_quoted;
	char	*clean_del;

	if(ft_strchr(context->input, '\'') || ft_strchr(context->input, '\"'))
		is_quoted = 1;
	else
		is_quoted = 0;
	init_heredoc(context, &queue, &clean_del, del);
	while (1)
	{
		ft_putstr_fd("> ", STDERR_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line || is_delimiter(line, clean_del))
			break ;
		if (is_quoted)
			enqueue(&queue, ft_strdup(line));
		else
			enqueue(&queue, process_line_hd(context, line, exit_status));
	}
	return (queue_str_convert(&queue));
}
