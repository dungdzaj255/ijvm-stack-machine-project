#include "stack.h"
#include "initialization.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

extern ijvm_t ijvm;

word_t pop() {
    word_t popped_ele = top();
    ijvm.stack->sp--;
    return popped_ele; // size of stack reduced by 1
}

word_t top() {
    assert(ijvm.stack->sp >= 0);
    return ijvm.stack->array[ijvm.stack->sp]; // return the top ele
}   

void push(word_t num) {
    if (ijvm.stack->sp + 1 == ijvm.stack->size){
        ijvm.stack->size *= 2;
		ijvm.stack->array = realloc(ijvm.stack->array, ijvm.stack->size * sizeof(word_t)); // expand the size of stack
    }
    ijvm.stack->array[++ijvm.stack->sp] = num;  //push new element to top place of stack
}

word_t s_size(){
    return ijvm.stack->sp+1;
}