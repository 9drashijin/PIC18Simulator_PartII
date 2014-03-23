#ifndef	__BYTECODE_H__
#define	__BYTECODE_H__

typedef enum {INVALID_RANGE,
			  INVALID_BSR,
			  INVALID_OPERAND
			 }CException;

typedef enum {ACCESS = -5,
			  BANKED = -4,
			  F = -3,
			  W = -2
			  } SyntaxError; // empty ==-1

typedef enum 	{
					ADDWF, 	//
					CPFSEQ, //
					DECF,	//
					INFSNZ,	//
					MOVWF,
					RLNCF,
					SUBFWB,
					TSTFSZ,	//
					BSF,	//
					BTFSS,
					BN,		//
					BNZ,
					CALL,
					NOP,
					RETFIE,	//
					ANDLW,
					MOVLW,	//
					XORLW,
					TBLRD_POSTINC, //+*
					TBLWT_PREINC //*+
				} Mnemonic;
				
typedef struct {
	Mnemonic	mnemonic;
	char *name;
} Instruction;

typedef struct {
	Instruction *instruction;
	int operand1;
	int operand2;
	int operand3;
	int absoluteAddress;
} Bytecode;

#endif	// __BYTECODE_H__