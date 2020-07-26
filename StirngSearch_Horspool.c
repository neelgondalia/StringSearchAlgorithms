/*
* 0940429
* Neel Gondalia
* CIS 3490- A3
* March 19, 2018
* P22.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ARRAYSIZE 44049
#define FILENAME "data.txt"
#define STR_MAX_LEN 90
#define TABLESIZE 500
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"

/*
 * fills the shift table based on the pattern
 */
void shiftTable (int table[], char *userStr, int sizeOfUserStr) 
{
	for (int i = 0; i<TABLESIZE; i++) 
		table[i] = sizeOfUserStr;
	
	for (int i = 0; i < (sizeOfUserStr-1); i++) 
		table[(int)(userStr[i] - 32)] = (sizeOfUserStr - 1) - i; 		//uses ascii values
}

/*
 * searches for occurences of a pattern using the shift table and passes results of number of occurences and shifts back to the main by updating the results array
 */
void horspoolSearch(char arr[][STR_MAX_LEN], int table[], char *userStr, int userStrLen, int results[]) 
{
	bool flag = false;	
	int k, i = 0;
	for(i=0; i<ARRAYSIZE; i++)
    {   
		int strLen = strlen(arr[i]);
		int j = 0;
        for (j=0; j<(strLen-userStrLen);j += table[(int)((unsigned char)arr[i][j+k] - 32)]) 		//for loop that runs until one line is scanned by using the shift table as a basis of incrementation
        {				
			int indexShift = 0, countIndex = 0;
            for(k = userStrLen - 1; k>=0; k--)
            {
                if(arr[i][j+k] == userStr[k])			//same character
                {
					if(!flag)
                        indexShift++;
                    countIndex++;
                }
                else
                    flag = true;
            }
            k = userStrLen - 1 - indexShift;
                
			if(countIndex == userStrLen)			//same length, therefore an occurence of pattern found
                results[0] += 1;                
            results[1] += 1;
        }
    }
}

/*
 * prints the results of a search
 */
void printResults(int results[], char *pattern, double clock) 
{
	printf (RED "\n***************** HORSPOOL ALGORITHM RESULTS ****************************\n" RESET);
	printf("\n--> String searched: %s\n", pattern);
	printf(GREEN "--> Number of Occurences: %d\n" RESET, results[0]);
    printf(GREEN "--> Number of Shifts: %d\n" RESET, results[1]);
	printf(GREEN "--> Execution Time: %f seconds\n" RESET, clock);
	printf(GREEN "--> Performance Ratio: %f shifts/milliseconds\n\n" RESET, (double)(results[1]/(clock*1000)));
	printf (RED "****************************************************************************\n\n" RESET);
}

int main () 
{	
	FILE *fptr = fopen(FILENAME, "r");
	char arr[ARRAYSIZE][STR_MAX_LEN];
	int i = 0;
	int flag = 1;
	
	while (fgets(arr[i],STR_MAX_LEN,fptr)) {
	    i++;
	}
	fclose(fptr);
	
	while (flag==1) 
	{
		char *pattern = malloc (sizeof(char)*STR_MAX_LEN);
		printf("\nEnter a pattern to search for: ");
		fgets(pattern,STR_MAX_LEN,stdin);
		pattern[strcspn(pattern,"\n")] = 0;
		
		int sTable[TABLESIZE] = {0};
		shiftTable(sTable, pattern, strlen(pattern)); 
		
		int results[2] = {0};
		clock_t startTime = clock();
		horspoolSearch(arr,sTable,pattern,strlen(pattern),results);
		printResults(results,pattern,((double)(clock() - startTime) / CLOCKS_PER_SEC));
		
		free(pattern);
		
		char *cont = malloc(sizeof(char) * STR_MAX_LEN);
		printf ("Want to search another pattern? (yes/no)\n");
		fgets(cont,STR_MAX_LEN,stdin);
		cont[strcspn(cont,"\n")] = 0;
		
		if (strcmp(cont,"no") == 0) flag = 0;
		free(cont);
	}
	return 0;
}


