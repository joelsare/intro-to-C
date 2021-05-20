#include <stdio.h>
#include <string.h>

int location[50][50] = {0};
int rows;
int columns;
char grid[50][50];

int checkUpRight(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i--;
            j = j + 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i++;
    j = j - 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i++;
        j = j - 2;
    }
    return 1;
}

int checkDownRight(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i++;
            j = j + 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i--;
    j = j - 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i--;
        j = j - 2;
    }
    return 1;
}

int checkDownLeft(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i++;
            j = j - 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i--;
    j = j + 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i--;
        j = j + 2;
    }
    return 1;
}

int checkUpLeft(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i--;
            j = j - 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i++;
    j = j + 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i++;
        j = j + 2;
    }
    return 1;
}

int checkLeft(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            j = j - 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    j = j + 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        j = j + 2;
    }
    return 1;
}

int checkRight(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(j >= columns)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            j = j + 2;
            continue;
        }
        else
        {
            return 0;
        }
    }
    j = j - 2;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        j = j - 2;
    }
    return 1;
}

int checkUp(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(i >= rows || i < 0 || j >= columns || j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i--;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i++;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i++;
    }
    return 1;
}

int checkDown(char word[], int length, int i, int j)
{
    int c;
    for (c = 0; c < length; c++)
    {
        if(i >= rows || i < 0 || j >= columns || j < 0)
        {
            return 0;
        }
        else if(grid[i][j] == word[c])
        {
            i++;
            continue;
        }
        else
        {
            return 0;
        }
    }
    i--;
    for(c = 0; c < length; c++)
    {
        location[i][j] = 1;
        i--;
    }
    return 1;
}

int main(int argc, char *args[])
{
    int i, j;
    int length;
    int limit = 50;
    char c;
    char word[50];
    char first;

    for (j = 0; j < 50; j++)
    {
        c = getchar();
        grid[0][j] = c;
        if (c == '\n')
        {
            limit = j + 1;
            break;
        }
    }

    rows = limit / 2;
    columns = limit;

    for (i = 1; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
        {
            c = getchar();
            grid[i][j] = c;
        }
    }

    while(scanf("%s", word) == 1)
    {
        length = strlen(word);
        first = word[0];
        for (i = 0; i < rows; i++)
        {
            for(j = 0; j < columns; j++)
            {
                if(first == grid[i][j])
                {
                    checkRight(word,length, i, j);
                    checkDownLeft(word,length, i, j);
                    checkUpRight(word,length, i, j);
                    checkDownRight(word,length, i, j);
                    checkUpLeft(word,length, i, j);
                    checkUp(word,length, i, j);
                    checkDown(word,length, i, j);
                    checkLeft(word,length, i, j);
                }
            }
        }
    }

    for (i = 0; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
        {
            if(location[i][j] == 0)
            {
                if(grid[i][j] != '\n')
                {
                    grid[i][j] = ' ';
                }
            }
        }

    }

    for (i = 0; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
        {
            printf("%c", grid[i][j]);
        }
    }

    return 0;
}
