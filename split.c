#include "shell.h"

int token_len(char *str, char *delim);
int count_tokens(char *str, char *delim);
char **_strtok(char *line, char *delim);

/**
 * token_len - Locates the delimiter index.
 *             of the first token contained within a string.
 * @str: String.
 * @delim: The delimiter character.
 * Return: The delimiter index showing the end of
 *         the intitial token pointed to be str.
 */
int token_len(char *str, char *delim)
{
	int index = 0, len = 0;

	while (*(str + index) && *(str + index) != *delim)
	{
		len++;
		index++;
	}

	return (len);
}

/**
 * count_tokens - Counts the num of delimited
 *                words contained in a string.
 * @str: string
 * @delim: The delimiter character.
 * Return: The number of words within str.
 */
int count_tokens(char *str, char *delim)
{
	int index, tokens = 0, len = 0;

	for (index = 0; *(str + index); index++)
		len++;

	for (index = 0; index < len; index++)
	{
		if (*(str + index) != *delim)
		{
			tokens++;
			index += token_len(str + index, delim);
		}
	}

	return (tokens);
}

/**
 * _strtok - Tokenizes a string.
 * @line: string.
 * @delim: The delimiter character used to tokenize the string.
 * Return: A pointer to an array containing tokenized words.
 */
char **_strtok(char *line, char *delim)
{
	char **ptr;
	int index = 0, tokens, o, letters, j;

	tokens = count_tokens(line, delim);
	if (tokens == 0)
		return (NULL);

	ptr = malloc(sizeof(char *) * (tokens + 2));
	if (!ptr)
		return (NULL);

	for (o = 0; o < tokens; o++)
	{
		while (line[index] == *delim)
			index++;

		letters = token_len(line + index, delim);

		ptr[o] = malloc(sizeof(char) * (letters + 1));
		if (!ptr[o])
		{
			for (index -= 1; index >= 0; index--)
				free(ptr[index]);
			free(ptr);
			return (NULL);
		}

		for (j = 0; j < letters; j++)
		{
			ptr[o][j] = line[index];
			index++;
		}

		ptr[o][j] = '\0';
	}
	ptr[o] = NULL;
	ptr[o + 1] = NULL;

	return (ptr);
}
