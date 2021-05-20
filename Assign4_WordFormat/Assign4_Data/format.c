/*Joel Sare*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int comp(const void * a, const void * b)
{
    int c;
    char * ac = *(char **) a;
    char * bc = *(char **) b;

    for (c = 0; c < strlen(ac) && c < strlen(bc); c++)
    {
        if (tolower(ac[c]) < tolower(bc[c]))
        {
            return -1;
        }
        else if (tolower(ac[c]) > tolower(bc[c]))
        {
            return 1;
        }
    }

    if (strlen(ac) < strlen(bc))
    {
        return -1;
    }
    else if (strlen(ac) > strlen(bc))
    {
        return 1;
    }

    for (c = 0; c < strlen(ac) && c < strlen(bc); c++)
    {
        if ((ac[c]) < (bc[c]))
        {
            return -1;
        }
        else if ((ac[c]) > (bc[c]))
        {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int limit = 0;
    int i;
    int numwords = 0;
    int count = 1;
    char * endptr;
    char filename[100];
    char currentline[200];
    char outputline[200];
    char currWord[50];
    char prevWord[50];
    char * fname;
    FILE * input;
    FILE * output;
    FILE * words;
    char * word;
    char ** wordlist = malloc(sizeof(char*));

    if (argc < 3)
    {
        printf("This program requires two input parameters\n");
        return 1;
    }

    limit = strtol(argv[1], &endptr, 10);
    if (*endptr != '\0')
    {
        printf("The first argument must be an integer\n");
        return 1;
    }

    if (limit < 25 || limit > 100)
    {
        printf("First arguement must be between 25 and 100\n");
        return 1;
    }

    input = fopen(argv[2], "r");
    if (input == NULL)
    {
        printf("\"%s\" file does not exist\n", argv[2]);
        return 1;
    }
    memset(filename, '\0', sizeof(filename));
    strcpy(filename, argv[2]);

    fname = strcat(filename, ".out");
    output = fopen(fname, "w");
    if (output == NULL)
    {
        printf("error opening .out file\n");
        return 1;
    }

    memset(filename, '\0', sizeof(filename));
    strcpy(filename, argv[2]);

    fname = strcat(filename, ".words");
    words = fopen(fname, "w");
    if (words == NULL)
    {
        printf("error opening .words file\n");
        return 1;
    }

    while (fgets(currentline, sizeof(currentline), input) != NULL)
    {
        word = strtok(currentline, " ");
        while (word != NULL)
        {
            word[strcspn(word, "\r\n" )] = '\0';
            if (word[0] != '\0') 
            {
                if (strlen(word) + strlen(outputline) < limit)
                {
                    if (strlen(outputline) != 0)
                        strcat(outputline, " ");
                    strcat(outputline, word);
                }
                else
                {
                    fputs(outputline, output);
                    fputs("\n", output);
                    memset(outputline, '\0', sizeof(outputline));
                    strcat(outputline, word);
                }
                wordlist= realloc(wordlist, (numwords + 1) * sizeof(char*));
                wordlist[numwords] = strdup(word);
                numwords++; 
            }
            word = strtok(NULL, " ");
        }
    }
    fputs(outputline, output);

    qsort(wordlist, numwords, sizeof(char*), comp);

    memset(prevWord, '\0', sizeof(prevWord));

    for (i = 1; i < numwords; i++)
    {
        memset(currWord, '\0', sizeof(prevWord));
        strcpy(currWord, wordlist[i]);

        memset(prevWord, '\0', sizeof(prevWord));
        strcpy(prevWord, wordlist[i-1]);
        currWord[strcspn(currWord, "\r\n" )] = '\0';

        if(currWord[0] != '\0')
        {    
            if (strcmp(prevWord, currWord) == 0)
            {
                count++;
            }
            else
            {
                fprintf(words, "%s - %d\n", prevWord, count);
                count = 1;
            }
        }
    }
    fprintf(words, "%s - %d\n", currWord, count);

    fclose(input);
    fclose(output);
    fclose(words);
    return 0;
}
