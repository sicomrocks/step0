
#include <stdlib.h>
#include <stdio.h>

#include "instructions.h"
#include "parseExecute.h"

int ADD(unsigned int mot)
{ 	DEBUG_MSG("ADD instruction\n");
	long int res;
	
	res = registres[get_rs(mot)].valeur+registres[get_rt(mot)].valeur;
	
	if(res>0xffffffff || res<0)
	{return 1;}
	else {execute_cmd_lr(get_rd(mot),res); return 0;}
}

int ADDI(unsigned int mot)
{	DEBUG_MSG("ADDI instruction\n");
	long int res;
	
	res = registres[get_rs(mot)].valeur+registres[get_imm(mot)].valeur;
	
	if(res>0xffffffff || res<0)
	{return 1;}
	else {execute_cmd_lr(get_rt(mot),res); return 0;}
}

int SUB(unsigned int mot)
{	DEBUG_MSG("SUB instruction");
	long int res;
	
	res = registres[get_rs(mot)].valeur-registres[get_imm(mot)].valeur;
	
	if(res>0xffffffff || res<0)
	{return 1;}
	else {execute_cmd_lr(get_rt(mot),res); return 0;}

}

int MULT(unsigned int mot)
{	DEBUG_MSG("MULT instruction");
	long int res;
	res = registres[get_rs(mot)].valeur * registres[get_rs(mot)].valeur;
	execute_cmd_lr(34, (int)((res & 0xFFFFFFFF00000000) >> 32) );
	execute_cmd_lr(36, (int)( res & 0x00000000FFFFFFFF ));
	return 0;
}

int DIV(unsigned int mot)
{	DEBUG_MSG("DIV instruction");
	return 0;
}

int AND(unsigned int mot)
{	DEBUG_MSG("AND instruction");
	execute_cmd_lr(get_rd(mot),
		registres[get_rt(mot)].valeur & registres[get_rs(mot)].valeur );
	return 0;
}

int OR(unsigned int mot)
{	DEBUG_MSG("OR instruction");
	execute_cmd_lr(get_rd(mot),
		registres[get_rt(mot)].valeur | registres[get_rs(mot)].valeur );
	return 0;
}

int XOR(unsigned int mot)
{	DEBUG_MSG("XOR instruction");
	execute_cmd_lr(get_rd(mot),registres[get_rt(mot)].valeur ^ registres[get_rs(mot)].valeur  );
	return 0;
}

int ROTR(unsigned int mot) // ??
{	DEBUG_MSG("ROTR instruction");
	unsigned int temp;
	temp = registres[get_rt(mot)].valeur;
	if (temp == 'a') {
		DEBUG_MSG("shouldn't be there");
	}
	//execute_cmd_lr(get_rd(mot),
	return 0;
}

int SLL(unsigned int mot)
{	DEBUG_MSG("SLL instruction");
	execute_cmd_lr(get_rd(mot),registres[get_rs(mot)].valeur << get_sa(mot) );
	return 0;
}

int SRL(unsigned int mot)
{	DEBUG_MSG("SRL instruction");
	execute_cmd_lr(get_rd(mot),registres[get_rt(mot)].valeur << get_sa(mot) );
	return 0;
}

int SLT(unsigned int mot)
{	DEBUG_MSG("SLT instruction");
	if (registres[get_rs(mot)].valeur < registres[get_rt(mot)].valeur)
	execute_cmd_lr(get_rd(mot),1);
	else execute_cmd_lr(get_rd(mot),0);
	return 0;
}

int LW(unsigned int mot)
{	DEBUG_MSG("LW instruction");
	
	return 0;	
}

int SW(unsigned int mot)
{	DEBUG_MSG("SW instruction");
	
	return 0;
}

int LUI(unsigned int mot)
{	DEBUG_MSG("LUI instruction");
	execute_cmd_lr(get_rt(mot),get_imm(mot) <<16);
	return 0;
}

int MFHI(unsigned int mot)
{	DEBUG_MSG("MFHI instruction");
	execute_cmd_lr(get_rd(mot),registres[34].valeur);
	return 0;
}

int MFLO(unsigned int mot)
{	DEBUG_MSG("MFLO instruction");
	execute_cmd_lr(get_rd(mot),registres[36].valeur);
	return 0;
}

int BEQ(unsigned int mot)
{	DEBUG_MSG("BEQ instruction");
	
	if ( registres[get_rs(mot)].valeur == registres[get_rt(mot)].valeur )
	{ 	execute_cmd_lr(32,registres[32].valeur + (get_imm(mot)<<2));	
	}
	return 0;
}

int BNE(unsigned int mot)
{	DEBUG_MSG("BNE instruction");
	
	if (registres[get_rs(mot)].valeur!=registres[get_rt(mot)].valeur)
	{ 	execute_cmd_lr(32,registres[32].valeur + (get_imm(mot)<<2));	
	}
	return 0;
}

int BGTZ(unsigned int mot)
{	DEBUG_MSG("BGTZ instruction");
	
	if (registres[get_rs(mot)].valeur>0)
	{ 	execute_cmd_lr(32,registres[32].valeur + (get_imm(mot)<<2));	
	}
	return 0;
}

int BLEZ(unsigned int mot)
{	DEBUG_MSG("BLEZ instruction");
	
	if (registres[get_rs(mot)].valeur<=0)
	{ 	execute_cmd_lr(32,registres[32].valeur + (get_imm(mot)<<2));	
	}
	return 0;
}

int J(unsigned int mot)
{	DEBUG_MSG("J instruction");
	execute_cmd_lr(32, (registres[32].valeur & 0xF0000000)+(get_target(mot)<<2) );
	return 0;
}

int JAL(unsigned int mot)
{	DEBUG_MSG("JAL instruction");
	execute_cmd_lr(31,registres[32].valeur+8);
	execute_cmd_lr(32,(registres[32].valeur & 0xF0000000)+(get_target(mot)<<2) );
	return 0;
}

int JR(unsigned int mot)
{	DEBUG_MSG("JR instruction");
	execute_cmd_lr(32,registres[get_rs(mot)].valeur);
	return 0;
}

int NOP(unsigned int mot)
{	DEBUG_MSG("OR instruction");
	return 0;
}

int SYSCALL(unsigned int mot)
{	DEBUG_MSG("SYSCALL instruction");
	return 0;
}
