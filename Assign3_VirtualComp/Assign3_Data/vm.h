#if !defined(__VM_H__)
#define __VM_H__

typedef struct vm
{
	int acc;
	int ic;
	char ir[5];
	int opcode;
	int operand;

	char memory[100][4];
} VM;

#endif /* __VM_H__ */
