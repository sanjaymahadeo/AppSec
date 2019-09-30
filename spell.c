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

	for (int j = 0; j < min; j++)	// was LENGTH + 1
	{
		lower_word[j] = tolower(word[j]);
	}
	lower_word[min] = '\0'; //strlen(word)?LENGTH
	int bucket = hash_function(word);
	hashmap_t cursor = hashtable[bucket];
	while (cursor != NULL)
	{
		if (strcmp(cursor->word, word) == 0)
		{
			return true;
		}
		else
		{
			cursor = cursor->next;
		}
	}
		bucket = hash_function(lower_word);
		cursor = hashtable[bucket];
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
		char * word;
		word = strtok(line, " ");
		while (word != NULL)
		{
			hashmap_t new_node;
			new_node = malloc(sizeof(node)); //sizeof(node)
			new_node->next = NULL;
			strcpy(new_node->word, word);
			//for (int j = 0; j < LENGTH; j++)
			//{
			//	new_node->word[j] = word[j];
			//}
			//new_node->word[min] = '\0';
			int bucket = hash_function(word);
			if (hashtable[bucket] == NULL)
			{
				hashtable[bucket] = new_node;
			}
			else
			{
				new_node->next = hashtable[bucket];
				hashtable[bucket] = new_node;
			}
			word = strtok(NULL, " ");
		}
		free(word);
		word = NULL;
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
			if (ispunct(word[0]) != 0)
			{
				for (int j = 0; j < strlen(word) - 1; j++)
				{
					word[j] = word[j + 1];
				}
			}

			for (int i = 0; i < strlen(word); i++)
			{
				if ((ispunct(word[i]) != 0))
				{
					for (int j = i; j <= strlen(word); j++)
					{
						if (word[j + 1] == '\0')// || word[j + 1] == NULL)
						{
							word[j] = '\0';
						}
					}
				}
			}
			if (!check_word(word, hashtable))
			{

				misspelled[num_misspelled] = malloc(strlen(word));
				strcpy(misspelled[num_misspelled], word);
				num_misspelled++;
			}
			word = strtok(NULL, " ");
		}
		free(word);
		word = NULL;
	}
	return num_misspelled;
}
