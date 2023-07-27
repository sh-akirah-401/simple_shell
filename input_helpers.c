#include "shell.h"

char *get_args(char *line, int *exe_ret);
int call_args(char **args, char **front, int *exe_ret);
int run_args(char **args, char **front, int *exe_ret);
int handle_args(int *exe_ret);
int check_args(char **args);

/**
 * get_args - Gets a command from standard input.
 * @line: A buffer to store the command.
 * @exe_ret: The return value of the command last executed.
 * Return: If an error occurs - NULL.
 *         OR - a pointer to the command stored.
 */
char *get_args(char *line, int *exe_ret)
{
	size_t o = 0;
	ssize_t read;
	char *prompt = "$ ";

	if (line)
		free(line);

	read = _getline(&line, &o, STDIN_FILENO);
	if (read == -1)
		return (NULL);
	if (read == 1)
	{
		hist++;
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, prompt, 2);
		return (get_args(line, exe_ret));
	}

	line[read - 1] = '\0';
	variable_replacement(&line, exe_ret);
	handle_line(&line, read);

	return (line);
}

/**
 * call_args - Partitions operators from commands.
 * @args: An array of args.
 * @front: A double pointer at the beginning of args.
 * @exe_ret: The return value of the parent process command executed last.
 * Return: The return value of the command last executed.
 */
int call_args(char **args, char **front, int *exe_ret)
{
	int ret, index;

	if (!args[0])
		return (*exe_ret);
	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], "||", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret != 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
		else if (_strncmp(args[index], "&&", 2) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			args = replace_aliases(args);
			ret = run_args(args, front, exe_ret);
			if (*exe_ret == 0)
			{
				args = &args[++index];
				index = 0;
			}
			else
			{
				for (index++; args[index]; index++)
					free(args[index]);
				return (ret);
			}
		}
	}
	args = replace_aliases(args);
	ret = run_args(args, front, exe_ret);
	return (ret);
}

/**
 * run_args - Calls the execution of a command.
 * @args: An array of args.
 * @front: A double pointer at the beginning of args.
 * @exe_ret: The return value of the parent process command last executed.
 * Return: The return value of the command executed last.
 */
int run_args(char **args, char **front, int *exe_ret)
{
	int ret, j;
	int (*builtin)(char **args, char **front);

	builtin = get_builtin(args[0]);

	if (builtin)
	{
		ret = builtin(args + 1, front);
		if (ret != EXIT)
			*exe_ret = ret;
	}
	else
	{
		*exe_ret = execute(args, front);
		ret = *exe_ret;
	}

	hist++;

	for (j = 0; args[j]; j++)
		free(args[j]);

	return (ret);
}

/**
 * handle_args - Calls, and runs the execution of a command.
 * @exe_ret: The return value of the parent command last executed.
 * Return: If an end-of-file is read - END_OF_FILE (-2).
 *         If the input cant be tokenized - -1.
 *         O/w - The exit value of the command executed last.
 */
int handle_args(int *exe_ret)
{
	int ret = 0, index;
	char **args, *line = NULL, **front;

	line = get_args(line, exe_ret);
	if (!line)
		return (END_OF_FILE);

	args = _strtok(line, " ");
	free(line);
	if (!args)
		return (ret);
	if (check_args(args) != 0)
	{
		*exe_ret = 2;
		free_args(args, args);
		return (*exe_ret);
	}
	front = args;

	for (index = 0; args[index]; index++)
	{
		if (_strncmp(args[index], ";", 1) == 0)
		{
			free(args[index]);
			args[index] = NULL;
			ret = call_args(args, front, exe_ret);
			args = &args[++index];
			index = 0;
		}
	}
	if (args)
		ret = call_args(args, front, exe_ret);

	free(front);
	return (ret);
}

/**
 * check_args - Checks if there are any leading ';', ';;', '&&', or '||'.
 * @args: 2D pointer to tokenized commands and args.
 * Return: If a ';', '&&', or '||' is placed at an invalid position - 2.
 * OR - 0.
 */
int check_args(char **args)
{
	size_t j;
	char *cur, *nex;

	for (j = 0; args[j]; j++)
	{
		cur = args[j];
		if (cur[0] == ';' || cur[0] == '&' || cur[0] == '|')
		{
			if (j == 0 || cur[1] == ';')
				return (create_error(&args[j], 2));
			nex = args[j + 1];
			if (nex && (nex[0] == ';' || nex[0] == '&' || nex[0] == '|'))
				return (create_error(&args[j + 1], 2));
		}
	}
	return (0);
}
