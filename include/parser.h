/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:46:40 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 16:20:50 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "command.h"

typedef struct s_quote_params {
	char			*arg;
	int				*i;
	int				*exit_status;
	t_queue_char	*q;
	t_shell_context	*context;
	int				*was_quoted;
}	t_quote_params;


t_command	*cmd_null_terminate(t_command *cmd);
void		release_command_resources(t_command *cmd);

int			check_redirection_syntax(char **ps, char *es, int *exit_status);
int			validate_pipe_syntax(char **ps, char *es, int *exit_status);
int			check_pipe_usage(char **ps, char *es, int *exit_status);
int			check_command_validity(char *line, int *exit_status);

int			has_special_characters(char *arg);

void		handle_single_quotes(char *str, int *index, t_queue_char *queue);
void		handle_double_quotes(char *str, int *values[2], t_queue_char *queue,
				t_shell_context *context);
void		handle_dollar_sign(char *str, int *values[2], t_queue_char *queue,
				t_shell_context *context);
char		*extract_variable_name(char *arg);
void		process_variable(char *str, int *values[2], t_queue_char *queue,
				t_shell_context *context);

void		process_all_commands(t_command *cmd, t_shell_context *context,
				int *exit_status);

// parser functions prototypes

t_command	*parsecmd(char *line, int *exit_status);
t_command	*parsepipe(char **ps, char *es, int *ret);
t_command	*parseredirs(t_command *sub_cmd, char **ps, char *es);
t_command	*parseexec(char **ps, char *es, int *ret);
char		**ft_split_beta(const char *s, char sep);
int			count_redirections(t_command *cmd);

// filename cleaner
char		*clean_argument2(char *arg, t_shell_context *context,
				int *exit_status);
int			is_ambiguous_redirect(const char *filename);

// HEREDOC PART
char	*clean_delimiter(char *str, int *is_quoted);
char	*replace_quotes_with_x(char *str);
char	*remove_quotes(const char *str);

// EXPANDER PART

char	*remove_quotes(const char *str);
void	clean_nulls_from_argv(char **argv, int size);
void	clean_execution_command_args(t_command *cmd, t_shell_context *context,
		int *exit_status);
char	*clean_argument(char *arg, t_shell_context *context,
		int *exit_status);
void	restore_quotes(char **vector);
void	process_argument(char *arg, t_queue_char *queue, int *exit_status,
		t_shell_context *context);
void	handle_double_quotes_filename(char *arg, int *values[2], t_queue_char *q,
		t_shell_context *context);

#endif
