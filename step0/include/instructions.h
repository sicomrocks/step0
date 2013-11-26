#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include "notify.h"
#include "reg.h"
#include "memoire.h"

int ADD(INSTRUCTION inst);
int ADDI(INSTRUCTION inst);
int SUB(INSTRUCTION inst);
int MULT(INSTRUCTION ins);
int DIV(INSTRUCTION inst);
int AND(INSTRUCTION inst);
int OR(INSTRUCTION inst);
int XOR(INSTRUCTION inst);
int ROTR(INSTRUCTION inst);
int SLL(INSTRUCTION inst);
int SRL(INSTRUCTION inst);
int SLT(INSTRUCTION inst);
int LW(INSTRUCTION inst);
int SW(INSTRUCTION inst);
int LUI(INSTRUCTION inst);
int MFHI(INSTRUCTION inst);
int MFLO(INSTRUCTION inst);
int BEQ(INSTRUCTION inst);
int BNE(INSTRUCTION inst);
int BGTZ(INSTRUCTION inst);
int BLEZ(INSTRUCTION inst);
int J(INSTRUCTION inst);
int JAL(INSTRUCTION inst);
int JR(INSTRUCTION inst);
int SYSCALL(INSTRUCTION inst);

	
#endif
