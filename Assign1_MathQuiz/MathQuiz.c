#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Parameter d represents difficulty [0, 4].
Returns number depending on d.
*/
int getnum(int d)
{
    if (d == 1)
    {
        return rand() % 11;
    }
    else if (d == 2)
    {
        return rand() % 51;
    }
    else if (d == 3)
    {
        return rand() % 101;
    }
    else
    {
        return rand() % 201 - 100;
    }
}

/*
Parameter d represents difficulty (1-4).
Parameter i represents question #.
Returns correct answer of the question.
*/
int generate_question(int d)
{
    static int i = 0;
    int x = getnum(d);
    int y = getnum(d);
    int o = rand() % 4;
    i++;

    if (o == 0)
    {
        printf("Question %d: %d + %d =\n", i, x, y);
        return x + y;
    }
    else if (o == 1)
    {
        printf("Question %d: %d - %d =\n", i, x, y);
        return x - y;
    }
    else if (o == 2)
    {
        printf("Question %d: %d * %d =\n", i, x, y);
        return x * y;
    }
    else
    {
        while(y == 0)
        {
            y = getnum(d);
        }
        printf("Question %d: %d / %d =\n", i, x, y);
        return x / y;
    }
}

/*
Parameter answer represents the correct answer.
Returns 1 if answer was correct, 0 otherwise.
*/
int answer_question(int answer)
{
    int response;
    printf("Enter Answer: ");
    scanf("%d", &response);

    return (answer == response)? 1 : 0;
}

/*
Parameter r is a random int between [0,2];
Prints random bad response.
*/
void printBadResponse(int r)
{
    if (r == 0)
    {
        printf("WRONG!\n");
    }
    else if (r == 1)
    {
        printf("Sorry!\n");
    }
    else
    {
        printf("THAT WAS WRONG!\n");
    }
}

/*
Parameter r is a random int between [0,2];
Prints random good response.
*/
void printGoodResponse(int r)
{
    if (r == 0)
    {
        printf("Good job!\n");
    }
    else if (r == 1)
    {
        printf("Way to go!\n");
    }
    else
    {
        printf("Yay!\n");
    }
}
/*
Parameter c = 1 if answer was correct, 0 if incorrect.
Randomly prints response depending on if answer was right or wrong.
*/
void print_response(int c)
{
    int r = rand() % 3;
    if (c == 1)
    {
        printGoodResponse(r);
    }
    else
    {
        printBadResponse(r);
    }
}

int main()
{
    int numQuestions;
    int difficulty;
    int i = 1;
    int numCorrect = 0;
    srand(time(NULL));

    do
    {
        printf("How many questions for this test (1 - 20)? ");
        scanf("%d", &numQuestions);
    } while (numQuestions >= 21 || numQuestions <= 0);

    do
    {
        printf("Select difficulty (1-4): ");
        scanf("%d", &difficulty);
    } while (difficulty >=5 || difficulty <= 0);
    
    while (i <= numQuestions)
    {
        int answer = generate_question(difficulty);
        int correct = answer_question(answer);
        print_response(correct);
        if (correct == 1)
        {
            numCorrect++;
        }
        else
        {
            printf("The correct answer was %d\n", answer);
        }
        i++;
    }
    printf("Your score was %d/%d\n", numCorrect, numQuestions);
    
    return 0;
}
