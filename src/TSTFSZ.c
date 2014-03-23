#include "Bytecode.h"
#include "TSTFSZ.h"
#include <stdio.h>
#include "CException.h"

char FSR[0x1000];

void tstfsz(Bytecode *code){
	if 		(code->operand2 != -1 && code->operand3 ==-1 ){code->operand3 = code->operand2;} //if operand2 not empty and operand3 empty then replace value of operand2 to 3
	else if (code->operand2 != -1){Throw(INVALID_OPERAND);} //if operand2 not empty and operand3 have value throw exception
	
	if (code->operand3 == -1){code->operand3 = ACCESS;}	//default if no value input
	else if (code->operand3 <-5 || code->operand3 >1){Throw(INVALID_OPERAND);}
	else if (code->operand3 == W || code->operand3 == F){Throw(INVALID_OPERAND);}
	
	if (code->operand1 > 0xff || code->operand1 < 0x00){Throw(INVALID_RANGE);}// error range of input
	else{
		if(FSR[code->operand1] == 0){
			code->absoluteAddress += 2;	// skip next instruction
		}
		else{
			code->absoluteAddress += 1; // continue next instruction
		}
		if (code->operand3 == 1 || code->operand3 == BANKED){
			if(FSR[BSR] > 15 || FSR[BSR]<0){FSR[BSR] = 0;Throw(INVALID_BSR);}
			if(code->operand1 < 0x80){FSR[BSR] = 0x00;}
			else if(code->operand1 >= 0x80){FSR[BSR] = 0x0F;}
			FSR[code->operand1 + (FSR[BSR]*256)]; ///(FSR[BSR]*256) same as shift << 8 bit to left, 2^8 is 256.
		}
	}
}