/* Lauren Sherman */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

/* Data type to store a word and its count */
struct StructWordFreq
{
	char *word; /* the word */ 
	int count; /* the word frequency */
}; 
typedef struct StructWordFreq WordFreq;

/* Function declarations */
int readFile(char* filename, WordFreq* list[], int* numWords);
void processWord(char* buffer, WordFreq* list[], int* numWords);
int search(char* buffer, WordFreq list[], int numWords);
int print(char* input, char* filename, WordFreq list[], int numWords);

/* Start of main */
int main(int argc, char* argv[])
{
	WordFreq* list = 0;
	int numWords = 0;
	int a;

	/*if there are not enough input arguments*/
	if(argc < 3) 
	{
		printf("%s: usage %s inputFile outFile \n", argv[0], argv[0]);
		return 2; 
	}

	/* Call the function to read the input file */
	readFile(argv[1], &list, &numWords);
	/* Call the function to print the output */
	print(argv[1], argv[2], list, numWords);

	for(a = 0; a < numWords; a++)
	{
		free(list[a].word);
	}
	free(list);
	
	return 0;
}

/* Function to read the input file */
int readFile(char* filename, WordFreq* list[], int* numWords)
{
	FILE* fptr;
	char buffer[MAX_BUFFER_SIZE];
	char ch; 
	int i = 0; /* Counter into buffer */
	

	fptr = fopen(filename, "r");
	
	if(!fptr) /* No file read, so return 1 */
	{
		return 1;
	}

	/* Read a character into ch as long as we have not reached the end of the file */
	while((ch = fgetc(fptr)) != EOF) 
	{
		if(isalpha(ch) && i < MAX_BUFFER_SIZE)
		{
			buffer[i] = tolower(ch);
			i++; 
		}
		else if(i > 0)
		{
			buffer[i] = '\0'; /* Null terminates the characters in buffer to make it a C string */
			
			/* Process this word */ 
			processWord(buffer, list, numWords);
			
			i = 0;
			buffer[0] = '\0'; /* Makes buffer a C string, even though empty */
		}
	}

	fclose(fptr);

	/* Successfully opened and read a file, so return 0 */
	return 0;
}

/* Function to process a word */
void processWord(char* buffer, WordFreq* list[], int *numWords)
{
	int loc;
	int a;
	WordFreq* tempList = 0;

	loc = search(buffer, *list, *numWords);
	if(loc != -1) /* Word was found in the list */
	{
		(*list)[loc].count++; /* Increase the count */
		
	}

	else /* Word was not found in the list */
	{
		/* Allocate space for tempList */
		tempList = (WordFreq *) malloc(sizeof(WordFreq)*(*numWords + 1));

		/* Copy elements from list to tempList */
		for(a = 0; a < *numWords; a++)
		{
			tempList[a] = (*list)[a];
		}
		
		/* Add new word to end of tempList */
		tempList[*numWords].word = (char *) malloc(sizeof(char)*strlen(buffer) + 1);
		strcpy(tempList[*numWords].word, buffer);
		tempList[*numWords].count = 1; 
		(*numWords) = (*numWords) + 1;
		
		/* Free list */
		if(*list != NULL)
		{
			free(*list);
		}
		
		/* Make list point to tempList */
		*list = tempList;
	}
	
}

/* Function to search if a word is already in the list */
int search(char* buffer, WordFreq list[], int numWords)
{
	int i = 0;
	int loc = -1; /* Location in the array, if it is there, or -1 if it is not */

	while(i < numWords && loc == -1) /* Continue as long as haven't reached end of the array and haven't found it */
	{
		if(strcmp(buffer, list[i].word) == 0)
		{
			loc = i;
		}
		i++;
	}
	return loc; 
}

/* Function to print the words and frequencies to a file */
int print(char* input, char* filename, WordFreq list[], int numWords)
{
	FILE* f; 
	int a; 
	int b;
	WordFreq temp;

	f = fopen(filename, "w");
	if (f == NULL)
	{
		printf("%s\n", "Error opening file");
		exit(5);
	}

	printf("%s has %d unique words\n", input, numWords);
	printf("------------------------------------------\n");
	printf("Creating %s... done!\n", filename);

	/* Sort list */
	for (b = 1; b < numWords; b++)
	{
		for (a = 0; a < numWords - 1; a++)
		{
			if(list[a].count < list[a+1].count)
			{
				temp = list[a];
				list[a] = list[a+1];
				list[a+1] = temp;
			}
		}
	}

	for(a = 0; a < numWords; a++)
	{
		fprintf(f, "%s %d\n", list[a].word, list[a].count);
	} 
	fclose(f);

	return 0;
} 
