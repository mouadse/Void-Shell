#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_env {
  char *name;
  char *value;
  struct s_env *next;
}t_env;

t_env *env = NULL;

// code substr func
char *substr(char *str, int start, int end) {
  char *substr = malloc(end - start + 1);
  for (int i = start; i < end; i++) {
	substr[i - start] = str[i];
  }
  substr[end - start] = '\0';
  return substr;
}

int main(void) {
  // code goes here
  t_env *new_env = malloc(sizeof(t_env));
  char *demo_env = "PATH=/usr/bin:/bin:/usr/sbin:/sbin";
  char *equal = strchr(demo_env, '=');
  new_env->name = substr(demo_env, 0, equal - demo_env);
  new_env->value = substr(demo_env, equal - demo_env + 1, strlen(demo_env));

  printf("the env name is: %s\n", new_env->name);
  printf("the env value is: %s\n", new_env->value);
  free(new_env->name);
  free(new_env->value);
  free(new_env);
  return (EXIT_SUCCESS);
}
