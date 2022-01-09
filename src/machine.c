#include "ijvm.h"
#include "convert.h"
#include "initialization.h"
#include "instruction.h"
#include "stack.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ijvm_t ijvm;

int init_ijvm(char *binary_file)
{
	FILE *fp;
	//check if the file can be opened
	if(!(fp = fopen(binary_file, "rb"))) {
		fprintf(stderr,"Failed to open binary file.\n");
		return -1;
  	}
	
	// read the binary
  	ijvm.pc = 0;
	ijvm.wide = false;
	ijvm.binary = malloc(MAX_MEMORY * sizeof(byte_t));
	ijvm.binary_size = 0;
	word_t inputByte;
	while ((inputByte = fgetc(fp)) != EOF && ijvm.binary_size < MAX_MEMORY) {
		ijvm.binary[ijvm.binary_size] = inputByte;
		ijvm.binary_size++;
	}
	fclose(fp);
	
	Constant();
	Text();
	InitStack();
	return 0;
}

void destroy_ijvm()
{
  	// Reset ijvm state
	ijvm.wide = false;
	if (ijvm.binary!=NULL) free(ijvm.binary);
	if (ijvm.constant!=NULL) free(ijvm.constant);
	if (ijvm.text!=NULL) free(ijvm.text);

	// Reset stack
	if (ijvm.stack->array!=NULL) free(ijvm.stack->array);
	if (ijvm.stack->frame_local_var!=NULL) free(ijvm.stack->frame_local_var);
	if (ijvm.stack!=NULL) free(ijvm.stack);
	ijvm.input = NULL;
	ijvm.output = NULL;
}	

void run()
{
	while (ijvm.pc<ijvm.text_size && ijvm.pc>=0) {
		if (!step()) break; // break if there's no next instruction to read
	}
}

bool step() {
	if(ijvm.pc >= ijvm.text_size) return false;
	switch(ijvm.text[ijvm.pc]){
		case OP_WIDE:
			WIDE();
			step();
			break;
		case OP_BIPUSH:
			BIPUSH();
			break;
		case OP_DUP:
			DUP();
			break;
		case OP_IADD:
			IADD();
			break;
		case OP_ISUB:
			ISUB();
			break;
		case OP_IAND:
			IAND();
			break;
		case OP_IOR:
			IOR();
			break;
		case OP_POP:
			POP();
			break;
		case OP_SWAP:
			SWAP();
			break;
		case OP_IN:
			IN();
			break;
		case OP_OUT:
			OUT();
			break;
		case OP_NOP:
			NOP();
			break;
		case OP_ERR:
			ERR();
			break;
		case OP_HALT:
			HALT();
			break;
		case OP_GOTO:
			GOTO();
			break;
		case OP_IFEQ:
			IFEQ();
			break;
		case OP_IFLT:
			IFLT();
			break;
		case OP_ICMPEQ:
			IF_ICMPEQ();
			break;
		case OP_LDC_W:
			LDC_W();
			break;
		case OP_ILOAD:
			ILOAD();
			break;
		case OP_ISTORE:
			ISTORE();
			break;
		case OP_IINC:
			IINC();
			break;
		case OP_INVOKEVIRTUAL:
			INVOKEVIRTUAL();
			break;
		case OP_IRETURN:
			IRETURN();
			break;
		default:
			HALT();

			break;
	}
	return true;
}

byte_t *get_text()
{
	return ijvm.text;
}

int get_program_counter()
{
	return ijvm.pc;
}

byte_t get_instruction()
{
  return ijvm.text[ijvm.pc];
}

int text_size() 
{
	return ijvm.text_size;
}

word_t tos()
{
	return top();
}

word_t* get_stack()
{
	return ijvm.stack->array;
}

int stack_size(){
	return s_size();
}

void set_input(FILE *fp)
{
	ijvm.input = fp;
}

void set_output(FILE *fp)
{
	ijvm.output = fp;
}

word_t get_local_variable(int i)
{
	if (ijvm.stack->lv == 0) {
		return ijvm.stack->frame_local_var[i];
	} 
	else {
		word_t offset = ijvm.stack->lv + 1;
		return ijvm.stack->array[offset + i];
	}
}

word_t get_constant(int i) {
    return ijvm.constant[i];
}

bool finished(){
	return ijvm.pc == ijvm.text_size;
}