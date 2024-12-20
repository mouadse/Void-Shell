/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:21:19 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 16:54:04 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <assert.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <limits.h>

// My own custom header files below

# include "../libft/libft.h"
# include "command.h"
# include "environment.h"
# include "exec_engine.h"
# include "garbage_collector.h"
# include "noise.h"
# include "parser.h"
# include "queue.h"
# include "tokenizer.h"
# include "utils.h"
# include "builtins.h"
#endif
