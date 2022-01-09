#ifndef STACK_H
#define STACK_H

#include "ijvm.h"

#define LOCAL_MAX (1<<16)
#define MVAR_MAX_SIZE   65536      // WIDE: 2^16 
#define MAIN_FRAME      0xEEEEEEEE

typedef struct Stack {
    word_t sp;
    word_t* array;
    word_t size;
    word_t* frame_local_var;
    word_t lv;
} Stack;


void push(word_t num);
word_t pop();
word_t top();
word_t s_size();

#endif