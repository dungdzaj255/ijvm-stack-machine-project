#include "initialization.h"

extern ijvm_t ijvm;

void Constant()
{
    byte_t* array = malloc(BLOCK_SIZE * sizeof(byte_t));

    // get constant origin
    for (int i = 0; i < BLOCK_SIZE; ++i) {
		array[i] = ijvm.binary[BLOCK_SIZE + i];
    }
	ijvm.constant_origin = big_bytesToInt(array);

    // get constant size
	for (int i = 0; i < BLOCK_SIZE; ++i) {
		array[i] = ijvm.binary[BLOCK_SIZE * 2 + i];
    }
	ijvm.constant_size = big_bytesToInt(array) / BLOCK_SIZE;

    // get constant
	ijvm.constant = malloc(ijvm.constant_size * sizeof(word_t));
	for(int i = 0, chunk = 0; i < ijvm.constant_size;  ++i, chunk += BLOCK_SIZE){
		ijvm.constant[i] = big_bytesToInt(ijvm.binary + BLOCK_SIZE * 3 + chunk);
	}	
	free(array);
}

void Text()
{
    int startByte = BLOCK_SIZE * 3 + ijvm.constant_size * BLOCK_SIZE;
	byte_t* array = malloc(BLOCK_SIZE * sizeof(byte_t));
	
    // get text origin
	for (int i = 0; i < BLOCK_SIZE; ++i) {
		array[i] = ijvm.binary[startByte + i];
    }
	ijvm.text_origin = big_bytesToInt(array);

    // get text size
	for (int i = 0; i < BLOCK_SIZE; ++i) {
		array[i] = ijvm.binary[startByte + BLOCK_SIZE + i];
    }
	ijvm.text_size = big_bytesToInt(array);

    // get text
	ijvm.text = malloc(ijvm.text_size * sizeof(byte_t));
	for (int i = 0; i < ijvm.text_size; ++i) {
		ijvm.text[i] = ijvm.binary[startByte + BLOCK_SIZE * 2 + i];
    }
	free(array);
}

void InitStack(){
	ijvm.stack = malloc(sizeof(Stack));
	ijvm.stack->size = ijvm.text_size;
	ijvm.stack->array = malloc(sizeof(word_t) * ijvm.stack->size);
	ijvm.stack->frame_local_var = malloc(10000 * sizeof(word_t*));

	ijvm.stack->sp = -1;
	ijvm.stack->lv = 0;
}