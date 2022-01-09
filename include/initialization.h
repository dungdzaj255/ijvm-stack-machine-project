#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#include "ijvm.h"
#include "stack.h"
#include <convert.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MEMORY 99999
#define BLOCK_SIZE 4

typedef struct ijvm_t {
	word_t pc;
	bool wide;
	byte_t* binary;
	word_t binary_size;

	word_t* constant;
	word_t constant_origin;
	word_t constant_size;

	byte_t* text;
	word_t text_origin;
	word_t text_size;

	Stack* stack;
	FILE* input;
	FILE* output;
} ijvm_t;

void Constant();
void Text();
void InitStack();

#endif