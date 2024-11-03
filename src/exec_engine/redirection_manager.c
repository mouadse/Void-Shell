/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_manager.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:32 by msennane          #+#    #+#             */
/*   Updated: 2024/11/03 18:10:09 by msennane         ###   ########.fr       */
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

void	replace_var_in_line(char *line, int *i, t_shell_context *context,
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
	if (var_value)
		free(var_value);
}

char	*process_line(t_shell_context *context, char *line, int *exit_status)
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
			free(exit_code);
			i += 2;
		}
		else if (line[i] == '$' && (is_whitespace(line[i + 1])))
			enqueue_char(&q, line[i++]);
		else if (line[i] == '$')
			replace_var_in_line(line, &i, context, &q);
		else
			enqueue_char(&q, line[i++]);
	}
	if (line)
		free(line);
	return (queue_char_str_convert(&q));
}

char	*read_heredoc_input(char *del, t_shell_context *context,
		int *exit_status)
{
	char	*line;
	char	*heredoc_line;
	t_queue	queue;
	int		tty_fd;

	tty_fd = open("/dev/tty", O_RDWR);
	if (tty_fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE);
		// Replace with custom error handling and memory cleanup
	}
	dup2(tty_fd, STDIN_FILENO);
	close(tty_fd);
	init_queue(&queue);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			break ;
		}
		if (ft_strlen(line) == ft_strlen(del) + 1 && ft_strncmp(line, del,
				ft_strlen(line) - 1) == 0)
		{
			free(line);
			break ;
		}
		heredoc_line = process_line(context, line, exit_status);
		enqueue(&queue, heredoc_line);
	}
	return (queue_str_convert(&queue));
}

void	write_heredoc_file(t_shell_context *context, char *content)
{
	int	fd;

	fd = open(SHELL_HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE); // Replace with custom error handling and memory
							// cleanup
	}
	if (write(fd, content, ft_strlen(content)) < 0)
	{
		perror("write");
		exit(EXIT_FAILURE); // Replace with custom error handling and memory
							// cleanup
	}
	close(fd);
}

