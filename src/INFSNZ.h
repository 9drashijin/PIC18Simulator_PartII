#ifndef INFSNZ_H
#define INFSNZ_H

#define 	WREG 	0xFE8
#define		BSR 	0xFE0

extern char FSR[];

void infsnz(Bytecode *code);

#endif // INFSNZ_H
