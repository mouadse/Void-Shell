/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/10/30 02:03:38 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *get_env_value(char *key, t_env_var *env) {
  t_env_var *head;

  head = env;
  while (head != NULL) {
    if (ft_strcmp(head->key, key) == 0)
      return (head->value);
    head = head->next;
  }
  return (NULL);
}

void free_env_node(t_env_var *env) {
  if (!env)
    return;
  if (env->key)
    free(env->key);
  if (env->value)
    free(env->value);
  free(env);
}

void free_env(t_env_var *env) {
  t_env_var *head;
  t_env_var *tmp;

  head = env;
  while (head != NULL) {
    tmp = head;
    head = head->next;
    free_env_node(tmp);
  }
}
