#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

#define MAX_MISSPELLED 1000
#undef calculate
#undef getrusage
#define expected_size 118819

#define DICTIONARY "wordlist.txt"

int main(int argc, char** argv)
{
	hashmap_t hashtable[HASH_SIZE];
	char* misspelled[MAX_MISSPELLED];

	FILE * fp = fopen(argv[1], "r");

	char * dictionary = argv[2];
	load_dictionary(dictionary, hashtable);
	int num_wrong = check_words(fp, hashtable, misspelled);
	for (int i = 0; i < num_wrong; i++)
	{
		printf("%s\n", misspelled[i]);
	}
}
