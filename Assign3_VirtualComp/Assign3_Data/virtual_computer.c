#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"

void init_vm(VM *pvm)
{
	pvm->acc = 0;
	pvm->ic = 0;
	pvm->opcode = 0;
	pvm->operand = 0;

    memset(pvm->ir, 0x20, sizeof(pvm->ir));
	memset(pvm->memory, 0x30, sizeof(pvm->memory));
}

void print_word(const char value[], int size)
{
	int i;

	for (i=0; i<size; i++)
		printf("%c", value[i]);
}

void print_vm(const VM *pvm)
{
	int row,col;

    printf("\n");
	printf("REGISTERS:\n");
	printf("accumulator         %+05d\n", pvm->acc);
	printf("instructionCounter  %02d\n", pvm->ic);
	printf("instructionRegister %s\n", pvm->ir);
	printf("operationCode       %02d\n", pvm->opcode);
	printf("operand             %02d\n", pvm->operand);
	printf("\n");
	printf("MEMORY:\n");
	printf("       0     1     2     3     4     5     6     7     8     9\n");

	for (row=0; row<10; row++)
	{
		printf("%2d ", row*10);

		for (col=0; col<10; col++)
		{
			printf("+");
			print_word(pvm->memory[row*10+col], 4);
			printf(" ");

		}

		printf("\n");
	}
}

void print_compile_error(int line_number, const char *line, const char * error)
{
	printf("Error: %s\n", error);
	printf("Line %d: %s\n", line_number, line);
}

long int compile_int(const char * input)
{
	long int result;
	char *endptr;

	if (input == NULL)
		return -1;

	result = strtol(input, &endptr, 10);

	if (input == endptr)
		return -1;

	if (*endptr != '\0')
		return -1;

	return result;
}

static int has_halt = 0;

const char * compile_opcode(const char * input)
{
	if (strcmp("READ", input) == 0)
		return "10";

	if (strcmp("WRIT", input) == 0)
		return "11";

	if (strcmp("PRNT", input) == 0)
		return "12";

	if (strcmp("LOAD", input) == 0)
		return "20";

	if (strcmp("STOR", input) == 0)
		return "21";

	if (strcmp("SET", input) == 0)
		return "22";

	if (strcmp("ADD", input) == 0)
		return "30";

	if (strcmp("SUB", input) == 0)
		return "31";

	if (strcmp("DIV", input) == 0)
		return "32";

	if (strcmp("MULT", input) == 0)
		return "33";

	if (strcmp("MOD", input) == 0)
		return "34";

	if (strcmp("BRAN", input) == 0)
		return "40";

	if (strcmp("BRNG", input) == 0)
		return "41";

	if (strcmp("BRZR", input) == 0)
		return "42";

	if (strcmp("HALT", input) == 0)
    {
        has_halt = 1;
		return "99";
    }

	return NULL;
}

static char * del = " ";

void compile_line(VM *pvm, int line_number, char *line)
{
	char *plocation, *poperation, *poperand;
	const char *opcode;
	long int location, operand;
	char cell_buf[5];
    char line_buf[256];

    strcpy(line_buf, line);
	     
	plocation = strtok(line_buf, del);

	if (plocation == NULL)
	{
		print_compile_error(line_number, line, "Incorrectly delimited by spaces");
		exit(1);
	}


	location = compile_int(plocation);

	if (location < 0 || location >= 100)
	{
		print_compile_error(line_number, line, "Invalid location");
		exit(1);
	}

	poperation = strtok(NULL, del);

	if (poperation == NULL)
	{
		print_compile_error(line_number, line, "Incorrectly delimited by spaces");
		exit(1);
	}

	opcode = compile_opcode(poperation);

	if (opcode == NULL)
	{
		print_compile_error(line_number, line, "Invalid operation name");
		exit(1);
	}

	poperand = strtok(NULL, del);

	if (poperand == NULL)
	{
		print_compile_error(line_number, line, "Incorrectly delimited by spaces");
		exit(1);
	}

	operand = compile_int(poperand);

	if (operand < 0 || operand >=10000)
	{
		print_compile_error(line_number, line, "Invalid operand");
		exit(1);
	}
	
	if (strcmp(opcode, "22") == 0) /* SET */
		sprintf(cell_buf, "%04d", (int)operand);
	else
		sprintf(cell_buf, "%2s%02d", opcode, (int)operand);

	memcpy(pvm->memory[location], cell_buf, 4);
}

void compile(VM *pvm)
{
	int line_number = 0;
	char line_buffer[256];

	for (;;)
	{
		char *line = gets(line_buffer);
       	if (line == NULL)
            break;

		++line_number;
		compile_line(pvm, line_number, line);
	}

    if (!has_halt)
    {
        printf("Error: program has no HALT instruction.\n");
        exit(1);
    }
}

int vm_load(VM *pvm, int location)
{
    char buf[5];
    char *ptr = pvm->memory[location];

    memcpy(buf, ptr, 4);
    buf[4] = '\0';

    return atoi(buf);
}

void vm_store(VM *pvm, int location, int value)
{
    char buf[5];

    sprintf(buf, "%04d", value);
    memcpy(pvm->memory[pvm->operand], buf, 4);
}

void vm_check_overflow(VM *pvm)
{
    if (pvm->acc >= -999 && pvm->acc <= 9999)
        return;
        
    printf("Accumulator overflow: %d\n", pvm->acc);
	print_vm(pvm);
    exit(99);
}

void execute(VM *pvm)
{
    for (;;)
    {
        char instr_buf[5];

        if (pvm->ic < 0 || pvm->ic >= 100)
        {
            printf("Instruction Counter out of bound: %d\n", pvm->ic);
	        print_vm(pvm);
            exit(99);
        }

        memcpy(pvm->ir, pvm->memory[pvm->ic++], 4);
        pvm->ir[4] = '\0';

        memcpy(instr_buf, pvm->ir, 4);
        instr_buf[4] = '\0';
        pvm->operand = atoi(instr_buf+2);
        instr_buf[2] = '\0';
        pvm->opcode = atoi(instr_buf);

        switch (pvm->opcode)
        {
            case 10: /* READ */
                {
                    int input;
                    scanf("%d", &input);

                    /* TODO: validate input for range? */
                    vm_store(pvm, pvm->operand, input);

                    break;
                }

            case 11: /* WRIT */
                {
                    char buf[5];
                    strncpy(buf, pvm->memory[pvm->operand], 4);
                    buf[4] = '\0';
                    printf("%4s", buf);
                    break;
                }

            case 12: /* PRNT */
                {
                    char *ptr = pvm->memory[pvm->operand];

                    for (;;)
                    {
                        char ch;
                        char buf[3];
                        memcpy(buf, ptr, 2);
                        buf[2] = '\0';

                        ch = atoi(buf);

                        if (ch == '\0')
                            break;

                        if (ch != '\n' && (ch < 'A' || ch > 'Z'))
                        {
                            printf("Unknown character: %d\n", ch);
                            print_vm(pvm);
                            exit(99);
                        }

                        printf("%c", ch);
                        ptr += 2;
                    }

                    break;
                }

            case 20: /* LOAD */
                {
                    pvm->acc = vm_load(pvm, pvm->operand);
                    break;
                }
                
            case 21: /* STOR */
                {
                    vm_store(pvm, pvm->operand, pvm->acc);
                    break;
                }

            case 30: /* ADD */
                {
                    int value = vm_load(pvm, pvm->operand);
                    pvm->acc += value;
                    vm_check_overflow(pvm);
                    break;
                }

            case 31: /* SUB */
                {
                    int value = vm_load(pvm, pvm->operand);
                    pvm->acc -= value;
                    vm_check_overflow(pvm);
                    break;
                }

            case 32: /* DIV */
                {
                    int value = vm_load(pvm, pvm->operand);

                    if (value == 0)
                    {
                        printf("Divided by zero.\n");
	                    print_vm(pvm);
                        exit(99);
                    }

                    pvm->acc /= value;
                    vm_check_overflow(pvm);
                    break;
                }

            case 33: /* MULT */
                {
                    int value = vm_load(pvm, pvm->operand);
                    pvm->acc *= value;
                    vm_check_overflow(pvm);
                    break;
                }

            case 34: /* MOD */
                {
                    int value = vm_load(pvm, pvm->operand);
                    pvm->acc %= value;
                    vm_check_overflow(pvm);
                    break;
                }

            case 40: /* BRAN */
                {
                    pvm->ic = pvm->operand;
                    break;
                }

            case 41: /* BRNG */
                {
                    if (pvm->acc < 0)
                        pvm->ic = pvm->operand;

                    break;
                }

            case 42: /* BRZR */
                {
                    if (pvm->acc == 0)
                        pvm->ic = pvm->operand;

                    break;
                }

            case 99: /* HALT */
                {
                    print_vm(pvm);
                    return;
                }

            default: /* Unknown operation */
                {
                        printf("Unknown operation: %d\n", pvm->opcode);
	                    print_vm(pvm);
                        exit(99);
                }
        } /* switch */
    }
}

int main(void)
{
	VM vm;

	init_vm(&vm);

    printf("\n\n");
    printf("Compiling VM...\n");

	compile(&vm);
	//print_vm(&vm);

    printf("\n\n");
    printf("Executing VM...\n");

    stdin = fopen("/dev/tty", "r");

    execute(&vm);

	return 0;
}
