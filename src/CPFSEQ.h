#ifndef _CPFSEQ_H_
#define _CPFSEQ_H_

#define 	WREG 	0xFE8
#define		BSR 	0xFE0

extern char FSR[];

void cpfseq(Bytecode *code);

#endif // CPFSEQ_H
