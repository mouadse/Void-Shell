/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 21:06:28 by msennane          #+#    #+#             */
/*   Updated: 2024/09/29 21:22:43 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "environment.h"
# include "minishell.h"
# include "queue.h"

# define EXEC 1
# define REDIR 2
# define PIPE 3

# define MAX_ARGS 1024

typedef struct s_command
{
	int			type;
}				t_command;

typedef struct s_pipe
{
	int			type;
	t_command	*left;
	t_command	*right;
}				t_pipe;

typedef struct s_exec
{
	int			type;
	char		*argv[MAX_ARGS];
	char		*eargv[MAX_ARGS];
}				t_exec;

typedef struct s_redir
{
	int			type;
	char		*file;
	char *efile; // to store the expanded file
	int			fd;
	int mode;        // O_RDONLY, O_WRONLY, O_APPEND
	char redir_type; // '<', '>', '>>'
	t_command	*subcmd;
}				t_redir;

typedef struct s_redir_data
{
	char		*file;
	char		*efile;
	int			fd;
	int			mode;
}				t_redir_data;

typedef struct s_params
{
	char		*buf;
	char		**envp;
	t_env_var	*env_vars;
	t_command	*tree;
	t_queue		*queue;
}				t_params;

#endif
