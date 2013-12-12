#ifndef _INSTRUCTIONS2_H_
#define _INSTRUCTIONS2_H_

#include <stdio.h>
#include <stdlib.h>
#include "notify.h"
#include "reg.h"
#include "memoire.h"

int ADD(unsigned int mot);
int ADDI(unsigned int mot);
int SUB(unsigned int mot);
int MULT(unsigned int mot);
int DIV(unsigned int mot);
int AND(unsigned int mot);
int OR(unsigned int mot);
int XOR(unsigned int mot);
int ROTR(unsigned int mot);
int SLL(unsigned int mot);
int SRL(unsigned int mot);
int SLT(unsigned int mot);
int LW(unsigned int mot);
int SW(unsigned int mot);
int LUI(unsigned int mot);
int MFHI(unsigned int mot);
int MFLO(unsigned int mot);
int BEQ(unsigned int mot);
int BNE(unsigned int mot);
int BGTZ(unsigned int mot);
int BLEZ(unsigned int mot);
int J(unsigned int mot);
int JAL(unsigned int mot);
int JR(unsigned int mot);
int SYSCALL(unsigned int mot);

	
#endif
