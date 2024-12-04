#include "../../include/minishell.h"

void	ft_puterror(char *arg)
{
	ft_putstr_fd("minishell: cd: '", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': No such file or directory\n", 2);
}

void	ft_puterror2(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": not set\n", 2);
}

void	ft_puterror3(char *arg, char *arg2)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(arg2, 2);
	ft_putstr_fd("\n", 2);
}

void	set_env_var(const char *key, t_env_var *env_vars)
{
	char		*cwd;
	t_env_var	*node;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	node = create_env_var(ft_strdup(key), ft_strdup(cwd));
	free(cwd);
	update_env_var(env_vars, node);
}

int	check_home(t_env_var *env_vars)
{
	t_env_var	*tmp;

	tmp = env_vars;
	while (tmp)
	{
		if (!strncmp("HOME", tmp->key, 4))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

void	ft_chdir(t_env_var *env_vars, char *path)
{
	int	i;

	set_env_var("OLDPWD", env_vars);
	i = chdir(path);
	if (i)
	{
		ft_puterror(path);
		return ;
	}
	printf("%s\n", path);
	set_env_var("PWD", env_vars);
}

void	cd_oldpwd(t_env_var *env_vars)
{
	char	*path;
	char	*oldpwd;

	path = NULL;
	oldpwd = get_env_value("OLDPWD", env_vars);
	if (oldpwd)
		path = ft_strdup(oldpwd);
	if (!path)
		ft_puterror("OLDPWD");
	else
		ft_chdir(env_vars, path);
}

void	cd_home(t_env_var *env_vars)
{
	char	*path;
	char	*home;
	int		i;

	path = NULL;
	if (!check_home(env_vars))
		ft_puterror("HOME");
	else
	{
		set_env_var("OLDPWD", env_vars);
		home = get_env_value("HOME", env_vars);
		if (home)
			path = ft_strdup(home);
		printf("path = %s \n", path);
		i = chdir(path);
		if (i)
		{
			ft_puterror(path);
			return ;
		}
		set_env_var("PWD", env_vars);
	}
}

void	cd(char **args, int *exit_status, t_env_var **env_vars)
{
	char	*home;
	char	*cwd;
	int		i;

	home = getcwd(NULL, 0);
	cwd = ft_strdup(home);
	free(home);
	if (args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		*exit_status = 1;
	}
	else if (!args[1])
		cd_home(*env_vars);
	else if (!ft_strcmp(args[1], "-"))
		cd_oldpwd(*env_vars);
	else if (!ft_strcmp(args[1], ""))
	{
		printf("We are here\n");
		cd_oldpwd(*env_vars); // Handle empty string as cd -
	}
	else if (ft_strcmp(args[1], "."))
	{
		set_env_var("OLDPWD", *env_vars);
		i = chdir(args[1]);
		if (i == -1)
		{
			ft_puterror3(args[1], strerror(errno));
			*exit_status = 1;
			return ;
		}
		set_env_var("PWD", *env_vars);
	}
	else if (!cwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd:\
		cannot access parent directories: No such file or directory\n",
						2);
		*exit_status = 1;
	}
}
