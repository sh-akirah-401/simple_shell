#include "shell.h"

char *_strchr(char *s, char c);
int _strspn(char *s, char *accept);
int _strcmp(char *s1, char *s2);
int _strncmp(const char *s1, const char *s2, size_t n);

/**
 * _strchr - Locates a char in a string.
 * @s: The string to search.
 * @c: The char to locate.
 * Return: If c is found - a pointer to first occurence.
 *         If c is not found - NULL.
 */
char *_strchr(char *s, char c)
{
	int index;

	for (index = 0; s[index]; index++)
	{
		if (s[index] == c)
			return (s + index);
	}

	return (NULL);
}

/**
 * _strspn - Gets the prefix substrings length.
 * @s: The string to search.
 * @accept: The measured prefix.
 * Return: The number of bytes in s which
 *         consist only of bytes from @accept.
 */
int _strspn(char *s, char *accept)
{
	int bytes = 0;
	int index;

	while (*s)
	{
		for (index = 0; accept[index]; index++)
		{
			if (*s == accept[index])
			{
				bytes++;
				break;
			}
		}
		s++;
	}
	return (bytes);
}

/**
 * _strcmp - Compares two strings.
 * @s1: The first string compared.
 * @s2: The second string compared.
 * Return: Positive byte difference if s1 > s2
 *         0 if s1 = s2
 *         Negative byte difference if s1 < s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 != *s2)
		return (*s1 - *s2);

	return (0);
}

/**
 * _strncmp - Compares two strings.
 * @s1: Pointer of string.
 * @s2: Pointer of string.
 * @n: The first bytes of the strings to compare.
 * Return: Less than 0 if s1 is shorter than s2.
 *         0 if s1 and s2 are a match.
 *         Greater than 0 if s1 is longer than s2.
 */
int _strncmp(const char *s1, const char *s2, size_t n)
{
	size_t j;

	for (j = 0; s1[j] && s2[j] && j < n; j++)
	{
		if (s1[j] > s2[j])
			return (s1[j] - s2[j]);
		else if (s1[j] < s2[j])
			return (s1[j] - s2[j]);
	}
	if (j == n)
		return (0);
	else
		return (-15);
}
