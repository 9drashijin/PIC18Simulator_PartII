#include "Bytecode.h"
#include "INFSNZ.h"
#include <stdio.h>
#include "CException.h"

char FSR[0x1000];

void infsnz(Bytecode *code){
	if (code->operand2 == -1){code->operand2 = F;}		//default if no value input (empty = -1)
	if (code->operand3 == -1){code->operand3 = ACCESS;}	//default if no value input
	
	if (code->operand2 == BANKED || code->operand2 == ACCESS ){Throw(INVALID_OPERAND);}		// operand 2 with ACCESS or BANKED
	else if (code->operand3 == W || code->operand3 == F){Throw(INVALID_OPERAND);}					// operand 3 with WREG or FileReg
	else if (code->operand2 <-5 || code->operand2 >1){Throw(INVALID_OPERAND);}						// operand 2 more than -5 or 1
	else if (code->operand3 <-5 || code->operand3 >1){Throw(INVALID_OPERAND);}						// operand 3 more than -5 or 1
		
	if(code->operand1 > 0xff || code->operand1 < 0x00){Throw(INVALID_RANGE);}			// error range of input
	else{
		if	(code->operand2 == 1 || code->operand2 == F ){
		if(code->operand1 < 0x80){
			FSR[code->operand1] = FSR[code->operand1] += 1;
			if(FSR[code->operand1] != 0){
				code->absoluteAddress += 2;	// skip next instruction
			}
			else if(FSR[code->operand1] == 0){
				code->absoluteAddress += 1; // continue next instruction
			}
		}
		else if(code->operand1 >= 0x80){
			FSR[code->operand1+(0x0F00)] = FSR[code->operand1+(0x0F00)] += 1;
			if(FSR[code->operand1+(0x0F00)] != 0){
				code->absoluteAddress += 2;	// skip next instruction
			}
			else if(FSR[code->operand1+(0x0F00)] == 0){
				code->absoluteAddress += 1; // continue next instruction
			}
		}
		}
		else if(code->operand2 == 0 || code->operand2 == W){
		if(code->operand1 < 0x80){
			FSR[WREG] = FSR[code->operand1] += 1;
			if(FSR[WREG] != 0){
				code->absoluteAddress += 2;	// skip next instruction
			}
			else if(FSR[WREG] == 0){
				code->absoluteAddress += 1; // continue next instruction
			}
		}
		else if(code->operand1 >= 0x80){
			FSR[WREG+(0x0F00)] = FSR[code->operand1+(0x0F00)] += 1;
			if(FSR[WREG+(0x0F00)] != 0){
				code->absoluteAddress += 2;	// skip next instruction
			}
			else if(FSR[WREG+(0x0F00)] == 0){
				code->absoluteAddress += 1; // continue next instruction
			}
		}
		}
		if (code->operand3 == 1 || code->operand3 == BANKED){
			if(FSR[BSR] > 15 || FSR[BSR]<0){FSR[BSR] = 0;Throw(INVALID_BSR);}
			if(code->operand1 < 0x80){FSR[BSR] = 0x00;}
			else if(code->operand1 >= 0x80){FSR[BSR] = 0x0F;}
			FSR[code->operand1 + (FSR[BSR]*256)]; ///(FSR[BSR]*256) same as shift << 8 bit to left, 2^8 is 256.
		}
	}
}