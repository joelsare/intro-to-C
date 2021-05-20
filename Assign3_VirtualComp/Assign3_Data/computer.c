/* Joel Sare */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int words[100] = {0};
int accum;
int insCount = 0;
int insReg = 0;
int opCode = 0;
int operand = 0;

void halt()
{
    int i = 0;
    printf("\nREGISTERS:\n");
    printf("%-19s %+05d\n", "accumulator", accum);
    printf("%-19s %02d\n", "instructionCounter", insCount);
    printf("%-19s %04d\n", "instructionRegister", insReg);
    printf("%-19s %02d\n", "operationCode", opCode);
    printf("%-19s %02d\n\n", "operand", operand);
    printf("MEMORY:\n");
    printf("%8d%6d%6d%6d%6d%6d%6d%6d%6d%6d\n", 0,1,2,3,4,5,6,7,8,9);
    printf(" 0 ");
    for(i = 0; i < 100; i++)
    {
        if(i % 10 == 0 && i != 0)
        {
            printf("\n%d ", i);
        }
        printf("+%04d ", words[i]);
    }
    printf("\n");
}

int operationToNum(char * op)
{
    if(strcmp(op, "READ") == 0)
        return 10;
    if(strcmp(op, "WRIT") == 0)
        return 11;
    if(strcmp(op, "PRNT") == 0)
        return 12;
    if(strcmp(op, "LOAD") == 0)
        return 20;
    if(strcmp(op, "STOR") == 0)
        return 21;
    if(strcmp(op, "SET") == 0)
        return 22;
    if(strcmp(op, "ADD") == 0)
        return 30;
    if(strcmp(op, "SUB") == 0)
        return 31;
    if(strcmp(op, "DIV") == 0)
        return 32;
    if(strcmp(op, "MULT") == 0)
        return 33;
    if(strcmp(op, "MOD") == 0)
        return 34;
    if(strcmp(op, "BRAN") == 0)
        return 40;
    if(strcmp(op, "BRNG") == 0)
        return 41;
    if(strcmp(op, "BRZR") == 0)
        return 42;
    if(strcmp(op, "HALT") == 0)
        return 99;
    return -100;
}

int concat(int a, int b)
{
    char s1[50];
    char s2[50];
    sprintf(s1, "%02d", a);
    sprintf(s2, "%02d", b);

    strcat(s1,s2);
    return atoi(s1);
}

int compile()
{
    const int bufsize = 50;
    char buffer[50];
    char line[50];
    char com[50];
    int loc;
    int operation;
    int dest;
    int val;
    int hasHalt = 0;
    while(!feof(stdin))
    {
        fgets(buffer, bufsize, stdin);
        if (strcmp(buffer, "") == 0)
        {
            break;
        }
        strcpy(line, buffer);
        loc = atoi(strtok(buffer, " "));
        strcpy(com, strtok(NULL, " "));
        operation = operationToNum(com);
        dest = atoi(strtok(NULL, " "));
        if (dest > 9999 || dest < -999)
        {
            printf("Error: word overflow\nValue larger than 4 digits: %d\n", dest);
            exit(1);
        }
        if(operation == -100)
        {
            printf("Unable to compile because of unknown command: %s\n", line);
            exit(1);
        }
        if (operation == 99)
        {
            hasHalt = 1;
        }
        val = concat(operation, dest);
        if (operation == 22)
        {
            words[loc] = dest;
        }
        else
        {
            words[loc] = val;
        }
        memset(buffer,0,bufsize);
    }
    if (hasHalt == 0)
    {
        printf("ERROR, program does not contain HALT\n");
        return 1;
    }
    else
    {
        return 0;
    }
    
}

int execute()
{
    while(1)
    {
        insReg = words[insCount];
        opCode = insReg / 100;
        operand = insReg % 100;
        insCount++;

        if (insCount >= 100)
        {
            printf("Instruction counter went out of bounds\n");
            halt();
            return 1;
        }
        switch(opCode)
        {
            case 10:/*READ*/
            {
                int val;
                scanf("%d", &val);
                if(val > 9999 || val < -999)
                {
                    printf("Value larger than 4 digits: %d\n", val);
                    halt();
                    return 1;
                }
                words[operand] = val;
                break;
            }
            case 11:/*WRIT*/
            {
                printf("%04d\n", words[operand]);
                break;
            }
            case 12:/*PRNT*/
            {
                int i = operand;
                while(1)
                {
                    char c = (words[i] / 100);
                    char nc = (words[i] % 100);
                    if (c == 0)
                    {
                        break;
                    }
                    else if((c == '\n') || ((c >= 'A') && (c <= 'Z')))
                    {
                        printf("%c", c);
                    }
                    else
                    {
                        printf("Unknown character: %c\n", c);
                        break;
                    }
                    if (nc == 0)
                    {
                        break;
                    }
                    else if((nc == '\n') || ((nc >= 'A') && (nc <= 'Z')))
                    {
                        printf("%c", nc);  
                    }
                    else
                    {
                        printf("Unknown character: %c\n", nc);
                        break;
                    }
                    i++;
                }
                break;
            }
            case 20:/*LOAD*/
            {
                accum = words[operand];
                break;
            }
            case 21:/*STOR*/
            {
                words[operand] = accum;
                break;
            }
            case 30:/*ADD*/
            {
                accum = accum + words[operand];
                if(accum > 9999 || accum < -999)
                {
                    printf("Accumulator overflowed, value is: %d\n", accum);
                    halt();
                    return 1;
                }
                break;
            }
            case 31:/*SUB*/
            {
                accum = accum - words[operand];
                if(accum > 9999 || accum < -999)
                {
                    printf("Accumulator overflowed, value is: %d\n", accum);
                    halt();
                    return 1;
                }
                break;
            }
            case 32:/*DIV*/
            {
                if (words[operand] != 0)
                {
                    accum = accum / words[operand];
                    if(accum > 9999 || accum < -999)
                    {
                        printf("Accumulator overflowed, value is: %d\n", accum);
                        halt();
                        return 1;
                    }
                }
                else
                {
                    printf("Cannot divide by zero!\n");
                    halt();
                    return 1;
                }
                break;
            }
            case 33:/*MULT*/
            {
                accum = accum * words[operand];
                if(accum > 9999 || accum < -999)
                    {
                        printf("Accumulator overflowed, value is: %d\n", accum);
                        halt();
                        return 1;
                    }
                break;
            }
            case 34:/*MOD*/
            {
                accum = accum % words[operand];
                if(accum > 9999 || accum < -999)
                    {
                        printf("Accumulator overflowed, value is: %d\n", accum);
                        halt();
                        return 1;
                    }
                break;
            }
            case 40:/*BRAN*/
            {
                insCount = operand;
                break;
            }
            case 41:/*BRNG*/
            {
                if (accum < 0)
                {
                    insCount = operand;
                }
                break;
            }
            case 42:/*BRZR*/
            {
                if (accum == 0)
                {
                    insCount = operand;
                }
                break;
            }
            case 99:/*HALT*/
            {
                halt();
                return 1;
            }
            default:
            {
                printf("Unknown command: %d\n", opCode);
                halt();
                exit(1);
                break;
            }
        }
    }
}

int main()
{
    int success;
    printf("Compiling...\n");
    success = compile();
    if (success == 0)
    {
        stdin = fopen("/dev/tty", "r");
        printf("Executing...\n");
        execute();
    }
    return 0;
}
