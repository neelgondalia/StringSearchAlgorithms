/*
* 0940429
* Neel Gondalia
* CIS 3490- A3
* March 19, 2018
* P23.c
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
#define DEBUGSEARCH true
#define DEBUGMAIN true
#define DEBUGSUFFIX true
#define DEBUG_MAX_ITER 5
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
#define RED "\x1b[31m"


/*
 * initializes bad and good tables
 */
void initializeTable(int table[], int sizeOfUserStr, int size) 
{
	for (int i = 0; i<size; i++) 
		table[i] = sizeOfUserStr;
}

/*
 * computes the bad character shift table
 */
void badCharacterShiftTable(int table[], char *userStr, int sizeOfUserStr) 
{	
	initializeTable(table,sizeOfUserStr,TABLESIZE);
	
	for (int i = 0; i < (sizeOfUserStr-1); i++) 
		table[(unsigned int)(userStr[i] - 32)] = (sizeOfUserStr - 1) - i; 
}

/*
 * gets all the suffixes for the suffix table based the pattern
 */
int getSuffix(char *suffix, char*userStr, int userStrLen, int index)
{
	int j = 0;
	for (int i = index; i < userStrLen; i++, j++)
		suffix[j] = userStr[i];
	return j;
}

/*
 * computes the good suffix shift table
 */
void goodSuffixShiftTable(int table[], char *userStr, int sizeOfUserStr)
{
    initializeTable(table,sizeOfUserStr,sizeOfUserStr);
	
	char suffix[sizeOfUserStr];
	
    int i,suffixIndex,j,checkIndex,suffixExaminedLen,matchLen,check,count;
    
    for(i = (sizeOfUserStr-1); i>=0; i--)
    {
		count = 1;
        suffixIndex = i;
        j=getSuffix(suffix,userStr,sizeOfUserStr,i);
        
        if (DEBUGSUFFIX)	printf("\nj is %d\n",j);
        
        if (DEBUGSUFFIX) 
			for (int dbug = 0; dbug < sizeOfUserStr; dbug++) printf ("GOOD SUFFIX TESTING -> SUFFIX TABLE: %c\n", suffix[dbug]);
		
		for (i = sizeOfUserStr - j - 1; i >= 0; i--, count++) 
		{
			checkIndex = 0; suffixExaminedLen = 0; matchLen = 0;
			for (check = i-j+1; check <= i; check++, checkIndex++)			//compares one iteration of one line
			{
				if (check >= 0)
				{
					suffixExaminedLen++;
					if (userStr[check] == suffix[checkIndex])
						matchLen++;
				}				
			}
			
			if (matchLen == suffixExaminedLen)			//if iterations for comparing two characters equal the length of pattern, it is an occurence
			{
				if (DEBUGSUFFIX && suffixIndex > 15)	printf (RED "i: %d 	suffixIndex:%d		count:%d	j:%d	matchLen:%d\n" RESET,i,suffixIndex, count, j, matchLen);
				
				table[suffixIndex] = count + j - matchLen;
				break;
			}
		}
    }
}

/*
 * returns the max value of two integers passed to the search function
 */
int max(int a, int b) 
{
	if (a < b)
		return b;
	return a;
}

/*
 * returns an appropriate integer based on the bad character rule to the search function
 */
int computeBTableVal(int toBeChecked) 
{
	if (toBeChecked > 0)
		return toBeChecked;
	return 1;
}

/*
 * searches for all occurences of a pattern entered by user with the help of good and bad tables
 */ 
void boyersMooreSearch(char arr[][STR_MAX_LEN], int bTable[], int gTable[], char *userStr, int userStrLen, int results[]) 
{	
	int i = 0, temp = 0, indexShift = 0, countIndex = 0, k = 0, j = 0, strLen = 0;
	
	for(i=0; i<ARRAYSIZE; i++)
    {   
		strLen = strlen(arr[i]);
        for (j=0; j<(strLen-userStrLen);j += max(gTable[userStrLen-indexShift], temp)) 		//iterates based on max between good table shits and bad table shifts
        {				
			bool flag = false; 
			indexShift = 0; countIndex = 0;          
            for(k = userStrLen - 1; k>=0; k--)
            {
				if (DEBUGSEARCH && i < DEBUG_MAX_ITER) printf ("user string index: %d,	user string char: %c\nline number: %d, line array index: %d, line array char: %c\n", k, userStr[k], i, j+k, arr[i][j+k]);
                
                if(arr[i][j+k] == userStr[k])
                {
					countIndex++;
					if(!flag)
                        indexShift++;
                }
                else
                    flag = true;
            }
            k = userStrLen - 1 - indexShift;
             
			if(countIndex == userStrLen)
                results[0] += 1;
            
            results[1] += 1;           
            temp = computeBTableVal((bTable[((unsigned char)arr[i][j+k] - 32)]- indexShift));		//computes bad character table shift
            
            if (DEBUGSEARCH && i < DEBUG_MAX_ITER) printf ("i:%d	j:%d	k:%d	gTableIndex:%d		gTableVal:%d	bValIndex:%d	bValatIndex-indexShift:%d		bVal:%d	 MAX: %d\n",i,j,k,userStrLen-indexShift,gTable[userStrLen-indexShift],((unsigned char)arr[i][j+k] - 32),bTable[((unsigned char)arr[i][j+k] - 32)]-indexShift,temp,max(gTable[userStrLen-indexShift], temp));
        }
    }
}

/*
 * prints the results of an individual search
 */
void printResults(int results[], char *pattern, double clock) 
{
	printf (RED "\n***************** BOYER-MOORE ALGORITHM RESULTS ****************************\n" RESET);
	printf("\n--> String searched: %s", pattern);
	printf(GREEN "--> Number of Occurences: %d\n" RESET, results[0]);
    printf(GREEN "--> Number of Shifts: %d\n" RESET, results[1]);
	printf(GREEN "--> Execution Time: %f seconds\n" RESET, clock);
	printf(GREEN "--> Performance Ratio: %f shifts/milliseconds\n\n" RESET, (double)(results[1]/(clock*1000)));
	printf (RED "****************************************************************************\n\n" RESET);
}

int main() 
{
	FILE *fptr = fopen(FILENAME, "r");
	char arr[ARRAYSIZE][STR_MAX_LEN];
	int flag = 1, k = 0;

	while (fgets(arr[k],STR_MAX_LEN,fptr)) {
	    k++;
	}
	fclose(fptr);
	k=0;
	
	while (flag == 1)
	{
		char *pattern = malloc (sizeof(char)*STR_MAX_LEN);
		printf("\nEnter a pattern to search for: ");
		fgets(pattern,STR_MAX_LEN,stdin);
		int patternLen = strlen(pattern);
			
		//getting good suffix table
		int gsst[TABLESIZE] = {0};
		goodSuffixShiftTable(gsst,pattern,patternLen-1);
		
		if(DEBUGMAIN) printf("GOT HERE : goodSuffixTable\n");
		
		//getting bad character table
		int bcst[TABLESIZE] = {0};
		badCharacterShiftTable(bcst,pattern,patternLen-1);
		
		if(DEBUGMAIN) printf("GOT HERE : badCharacterTable\n");
		
		if (DEBUGMAIN) 
		{
			printf ("\nGOOD SUFFIX MODIFIED TABLE\n\n");
			for (int i = 0; i < patternLen; i++)
				printf ("k: %d		distance: %d\n", i, gsst[i]);
		}
		if (DEBUGMAIN)
		{
			printf ("\nBAD CHARACTER MODIFIED TABLE\n\n");
			for (int i = 0; i < TABLESIZE-400; i++)
				printf ("k: %d		distance: %d\n", i, bcst[i]);
		}
		
		int results[2] = {0};
	
		if (DEBUGMAIN) printf ("BEFORE pattern: %s		patternLen: %d		results[0] = %d		results[1] = %d\n", pattern, patternLen, results[0], results[1]);
		
		clock_t startTime = clock();
		boyersMooreSearch(arr,bcst,gsst,pattern,patternLen-1,results);						//searching for occurences of the pattern entered by user
		printResults(results,pattern,((double)(clock() - startTime) / CLOCKS_PER_SEC));
		
		if (DEBUGMAIN) printf ("AFTER pattern: %s		patternLen: %d		results[0] = %d		results[1] = %d\n", pattern, patternLen, results[0], results[1]);
		
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
