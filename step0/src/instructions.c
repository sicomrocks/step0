
#include <stdlib.h>
#include <stdio.h>

#include "instructions.h"
#include "parseExecute.h"

int ADD(INSTRUCTION inst)
{ 	DEBUG_MSG("ADD instruction\n");
	long int res;
	//printf("op[0] = %s, op[1] = %s, op[2] = %s\n",inst.ops[0],inst.ops[1],inst.ops[2]);
	int op1;
	op1=(int)strtol(inst.ops[0],NULL,2);
	int op2;
	op2=(int)strtol(inst.ops[1],NULL,2);
	int op3;
	op3=(int)strtol(inst.ops[2],NULL,2);
	//printf("op1 = %d, op2 = %d, op3 = %d\n",op1,op2,op3);
	
	res = registres[op1].valeur+registres[op2].valeur;
	
	if(res>0xffffffff || res<0)
	{return 0;}
	else {execute_cmd_lr(op3,res); return 1;}
}

int ADDI(INSTRUCTION inst)
{	DEBUG_MSG("ADDI instruction\n");
	long int res;
	int op1;
	op1=(int)strtol(inst.ops[0],NULL,2);
	int op2;
	op2=(int)strtol(inst.ops[1],NULL,2);
	int op3;
	op3=(int)strtol(inst.ops[2],NULL,2);
	res = registres[op1].valeur+registres[op3].valeur;
	
	if(res>0xffffffff || res<0)
	{return 0;}
	else {
		registres[op2].valeur=res;
		return 1;}
}

int SUB(INSTRUCTION inst)
{
	return 0;
}

int MULT(INSTRUCTION ins)
{
	return 0;
}

int DIV(INSTRUCTION inst)
{
	return 0;
}

int AND(INSTRUCTION inst)
{
	return 0;
}

int OR(INSTRUCTION inst)
{
	return 0;
}

int XOR(INSTRUCTION inst)
{
	return 0;
}

int ROTR(INSTRUCTION inst)
{
	return 0;
}

int SLL(INSTRUCTION inst)
{
	return 0;
}

int SRL(INSTRUCTION inst)
{
	return 0;
}

int SLT(INSTRUCTION inst)
{
	return 0;
}

int LW(INSTRUCTION inst)
{
	return 0;	
}

int SW(INSTRUCTION inst)
{
	return 0;
}

int LUI(INSTRUCTION inst)
{
	return 0;
}

int MFHI(INSTRUCTION inst)
{
	return 0;
}

int MFLO(INSTRUCTION inst)
{
	return 0;
}

int BEQ(INSTRUCTION inst)
{
	return 0;
}

int BNE(INSTRUCTION inst)
{
	return 0;
}

int BGTZ(INSTRUCTION inst)
{
	return 0;
}

int BLEZ(INSTRUCTION inst)
{
	return 0;
}

int J(INSTRUCTION inst)
{
	return 0;
}

int JAL(INSTRUCTION inst)
{
	return 0;
}

int JR(INSTRUCTION inst)
{
	return 0;
}

int SYSCALL(INSTRUCTION inst)
{
	return 0;
}
