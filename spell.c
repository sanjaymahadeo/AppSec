#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
bool check_word(const char* word, hashmap_t hashtable[])
{
	char lower_word[LENGTH + 1];
	int min;
	if (strlen(word) < LENGTH)
	{
		min = strlen(word);
	}
	else
	{
		min = LENGTH;
	}
	//word[min] = '\0';
	for (int j = 0; j < min; j++)
	{
		lower_word[j] = tolower(word[j]);
	}

	lower_word[min] = '\0';

	int bucket = hash_function(lower_word);
	hashmap_t cursor = hashtable[bucket];
	while (cursor != NULL)
	{
		if (strcmp(cursor->word, lower_word) == 0)
		{
			return true;
		}
		else
		{
			cursor = cursor->next;
		}
	}
	return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[])
{
	for (int k = 0; k < HASH_SIZE; k++)
	{
		hashtable[k] = NULL;
	}
	FILE *fptr;
	fptr = fopen(dictionary_file, "r");

	if (fptr == NULL)
	{
		return false;
	}
	size_t size = 0;
	char * line = NULL;

	while (getline(&line, &size, fptr) != -1)
	{
		char *word;
		word = strtok(line, "\n");
		hashmap_t new_node;
		new_node = malloc(sizeof(node)); //sizeof(node)
		new_node->next = NULL;
		int len;
		if (strlen(word) < LENGTH)
		{
			len = strlen(word);
		}
		else
		{
			len = LENGTH;
		}
		for (int i = 0;i<len;i++)
		{
			new_node->word[i] = tolower(word[i]);
		}
		new_node->word[len] = '\0';
		int bucket = hash_function(new_node->word);
		if (hashtable[bucket] == NULL)
		{
			hashtable[bucket] = new_node;
		}
		else
		{
			new_node->next = hashtable[bucket];
			hashtable[bucket] = new_node;
		}
	}
	fclose(fptr);
	return true;
}



int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[])
{
	int num_misspelled = 0;
	size_t size = 0;
	char * line = NULL;

	while (getline(&line, &size, fp) != -1)
	{
		char * word; // = malloc(sizeof(char)) * (LENGTH+1))
		word = strtok(line, " ");
		while (word != NULL)
		{

			int len = strlen(word);
			while (ispunct(word[0]))
			{
				for (int j = 0; j < strlen(word) - 1; j++)
				{
					word[j] = word[j + 1];
				}
				word[len - 1] = '\0';
				len = strlen(word);
			}
			while (ispunct(word[len - 1]) || word[len - 1] == '\n')
			{
				word[len - 1] = '\0';
				len = strlen(word);
			}

			for (int j = 0;j<strlen(word);j++)
			{
				word[j] = tolower(word[j]);
			}
			if (!check_word(word, hashtable))
			{
				misspelled[num_misspelled] = malloc(strlen(word) + 1);//strlen(word));
				int l = 0;
				for (;l < strlen(word);l++)
				{
					misspelled[num_misspelled][l] = word[l];
				}
				misspelled[num_misspelled][l] = '\0';
				num_misspelled++;
			}
			word = strtok(NULL, " ");
		}
	}
	return num_misspelled;
}
