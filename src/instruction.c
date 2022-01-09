#include "instruction.h"
#include "initialization.h"
#include "convert.h"
#include <stdlib.h>
#include <string.h>

extern ijvm_t ijvm;

void BIPUSH() {
    ijvm.pc++;
    int8_t a = ijvm.text[ijvm.pc];
    push(a);
    ijvm.pc++;
}

void DUP() {
    word_t a = top();
    push(a);
    ijvm.pc++;
}

void ERR() {
	printf("Error!\n");
	HALT();
}

void GOTO(){
    short offset = Short(ijvm.text[ijvm.pc+1],ijvm.text[ijvm.pc+2]);
    ijvm.pc += offset;
}

void HALT(){
    ijvm.pc = ijvm.text_size; // stop
}

void IADD(){
    word_t a = pop();
    word_t b = pop();
    push(a+b);
    ijvm.pc++;
}

void IAND(){
    word_t a = pop();
    word_t b = pop();
    push(a & b);
    ijvm.pc++;
}

void IFEQ(){
    word_t a = pop();
    if (a == 0) GOTO();
    else ijvm.pc += 2 + 1; // +2(short size) and +1
}

void IFLT(){
    word_t a = pop();
    if (a < 0) GOTO();
    else ijvm.pc += 2 + 1;
}

void IF_ICMPEQ(){
    word_t a = pop();
	word_t b = pop();
	if(a==b) GOTO();
	else ijvm.pc += 2 + 1;
}

void IINC(){
    unsigned short index = ijvm.text[ijvm.pc+1];
    int8_t constant = ijvm.text[ijvm.pc+2];
    ijvm.stack->frame_local_var[index] += constant;
    ijvm.pc += 3;
}

void ILOAD(){
    unsigned short index;
    if (ijvm.wide) {
        index = Short(ijvm.text[ijvm.pc+1],ijvm.text[ijvm.pc+2]);
        push(ijvm.stack->array[ijvm.stack->lv + 1 + index]);
        ijvm.pc += 3;
        ijvm.wide = false;
    }
    else {
        index = ijvm.text[ijvm.pc+1];
        push(ijvm.stack->frame_local_var[index]);
        ijvm.pc += 2;
    }
}

void IN(){
	word_t inp_stream = fgetc(ijvm.input);
	if (inp_stream != EOF) push(inp_stream);
	else push(0); 
	ijvm.pc++;
}

void INVOKEVIRTUAL(){
    word_t caller_pc = ijvm.pc;
    word_t caller_lv = ijvm.stack->lv;

    unsigned short method_addr = Short(ijvm.text[ijvm.pc+1],ijvm.text[ijvm.pc+2]);
    ijvm.pc = ijvm.constant[method_addr];   // set pc to method area

	//get # of args
    unsigned short arg_num = Short(ijvm.text[ijvm.pc],ijvm.text[ijvm.pc+1]);    // get number of arguments

	//get # of local_vars
	unsigned short local_area_size = Short(ijvm.text[ijvm.pc+2],ijvm.text[ijvm.pc+3]);    // get size of local var space

    word_t* arg = malloc(arg_num*sizeof(word_t));
    for (int i=0; i<arg_num; i++) arg[i] = pop(); // remove OBJREF and parameters
    ijvm.stack->sp++;
    ijvm.stack->lv = ijvm.stack->sp; // add link pointer
    for (int i=0; i<arg_num; i++) ijvm.stack->frame_local_var[i] = arg[arg_num - i - 1]; // add number of arguments again
    ijvm.stack->sp += local_area_size; // change size of last arg to local area size
    free(arg);
    push(caller_pc);;
    ijvm.stack->array[ijvm.stack->lv] = ijvm.stack->sp; // link pointer points to the caller's sp
    push(caller_lv);
    ijvm.pc += 4;
}

void IOR(){
    word_t a = pop();
	word_t b = pop();
	push(a|b);
    ijvm.pc++;
}

void IRETURN(){
    word_t return_value = pop();
    word_t link_ptr = ijvm.stack->array[ijvm.stack->lv];
    
    ijvm.pc = ijvm.stack->array[link_ptr]+3;
    ijvm.stack->sp = ijvm.stack->lv;
    ijvm.stack->lv = ijvm.stack->array[link_ptr+1];
    ijvm.stack->sp--;
    push(return_value);
}

void ISTORE(){
    unsigned short index;
    word_t word = pop();    // Pop word from stack and store in local frame
    if (ijvm.wide) {
        index = Short(ijvm.text[ijvm.pc+1],ijvm.text[ijvm.pc+2]);
        ijvm.stack->array[ijvm.stack->lv + 1 + index] = word;
        ijvm.pc += 3;
        ijvm.wide = false;
    }
    else {
        index = ijvm.text[ijvm.pc+1];
        ijvm.stack->frame_local_var[index] = word;
        ijvm.pc += 2;
    }
}

void ISUB(){
    word_t a = pop();
    word_t b = pop();
    push(b-a);
    ijvm.pc++;
}

void LDC_W(){
    byte_t byte1 = ijvm.text[ijvm.pc+1];
	byte_t byte2 = ijvm.text[ijvm.pc+2];
    unsigned short index = Short(byte1,byte2);  // Read arg of instruction (index)
    push(ijvm.constant[index]); // And push loaded word to stack
    ijvm.pc += 3;
}

void NOP(){
    ijvm.pc++;
}

void OUT(){
    word_t a = pop();
    if (ijvm.output==NULL) ijvm.output = stderr;
	fprintf(ijvm.output, "%c", a);
	ijvm.pc++;
}
    
void POP(){
    pop();
    ijvm.pc++;
}

void SWAP(){
    word_t a = pop();
    word_t b = pop();
    push(a);
    push(b);
    ijvm.pc++;
}

void WIDE(){
	ijvm.wide = true;
    ijvm.pc++;
}