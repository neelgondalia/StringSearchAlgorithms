/*
* 0940429
* Neel Gondalia
* CIS 3490- A3
* March 19, 2018
* P21.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ARRAYSIZE 44049
#define FILENAME "data.txt"
#define STR_MAX_LEN 90
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"

/*
 * prints the results of a search
 */
void printResults (int numOccurences, int patternShifts, char *pattern, double clock) 
{
	printf (RED "\n***************** BRUTE FORCE STRING SEARCH ALGORITHM RESULTS ****************************\n" RESET);
	printf("\n--> String searched: %s\n", pattern);
	printf(GREEN "--> Number of Occurences: %d\n" RESET, numOccurences);
	printf(GREEN "--> Number of Shifts: %d\n" RESET, patternShifts);
	printf(GREEN "--> Execution Time: %f seconds\n" RESET, clock);
	printf(GREEN "--> Performance Ratio: %f shifts/milliseconds\n\n" RESET, (double)(patternShifts/(clock*1000)));
	printf (RED "******************************************************************************************\n\n" RESET);
}

/*
 * Function that takes in a 2D array and on a line by line basis checks the number of times the pattern is found and returns the results by updating the results array
 */
void bruteForceSearch(char arr[ARRAYSIZE][STR_MAX_LEN], char *pattern, int patternLen, int results[])
{
    for(int i = 0; i<ARRAYSIZE; i++)
    {
        int lineLen = strlen(arr[i]);
        
        for(int j = 0; j<(lineLen-patternLen); j++)
        {
            int compareLen = 0;
            //checks the string length of pattern distance away from j index to see if they are equal
            for(int k = 0; k<patternLen;k++)			
            {
				//same character so keeps a counter while in the k for loop
                if(arr[i][j+k] == pattern[k])
                    compareLen++;		
            }
            
            //occurence found if length of the pattern is same as the count counted in the k loop
            if(compareLen == patternLen)
				results[0] += 1;		 //occurence

            results[1] += 1;			//shift
        }
    }
}


int main() 
{
	printf ("loading file to array...please wait...\n");
	FILE *fptr = fopen(FILENAME, "r");
	char arr[ARRAYSIZE][STR_MAX_LEN];
	int k = 0;
    
    while (fgets(arr[k],STR_MAX_LEN,fptr))
	    k++;
	
	fclose(fptr);
	k = 0;
    printf (GREEN "DONE!\n" RESET);
    
    int flag = 1;  
    while (flag == 1)
    {
		//getting pattern from user
		char *pattern = malloc(sizeof(char) * STR_MAX_LEN);
		printf ("\nEnter a pattern to search for: ");
		fgets(pattern,STR_MAX_LEN,stdin);
		pattern[strcspn(pattern,"\n")] = 0;
		int patternLen = strlen(pattern);
		
		//searches for pattern
		int results[2] = {0};
		clock_t startTime = clock();
		bruteForceSearch(arr,pattern,patternLen,results);
		printResults(results[0], results[1], pattern, ((double)(clock() - startTime) / CLOCKS_PER_SEC));
		
		free(pattern);
		
		char *cont = malloc(sizeof(char) * STR_MAX_LEN);
		printf ("Want to search another pattern? (yes/no)\n");
		fgets(cont,STR_MAX_LEN,stdin);
		cont[strcspn(cont,"\n")] = 0;
		
		//user wants to quit
		if (strcmp(cont,"no") == 0) flag = 0;
		free(cont);
	}
	return 0;
}
