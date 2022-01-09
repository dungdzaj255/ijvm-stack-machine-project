#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "ijvm.h"

void BIPUSH();
void DUP();
void ERR();
void GOTO();
void HALT();
void IADD();
void IAND();
void IFEQ();
void IFLT();
void IF_ICMPEQ();
void IINC();
void ILOAD();
void IN();
void INVOKEVIRTUAL();
void IOR();
void IRETURN();
void ISTORE();
void ISUB();
void LDC_W();
void NOP();
void OUT();
void POP();
void SWAP();
void WIDE();

#endif