#ifndef CPFSEQ_H
#define CPFSEQ_H

#define 	WREG 	0xFE8
#define		BSR 	0xFE0

extern char FSR[];

void cpfseq(Bytecode *code);

#endif // CPFSEQ_H
