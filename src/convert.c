#include "convert.h"

// big endianness
int big_bytesToInt(byte_t* bytes) {
	return ((bytes[0]&0xFF)<<24) | ((bytes[1]&0xFF)<<16) | ((bytes[2]&0xFF)<<8) | (bytes[3]&0xFF);
}

// little endianness
int little_bytesToInt(byte_t* bytes) {
    return (bytes[0] + (bytes[1] << 8) + (bytes[2] << 16) + (bytes[3] << 24));
}

short Short(byte_t byte1, byte_t byte2){
	return (byte1<<8) | byte2;
}