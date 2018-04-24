#ifndef P2_H
#define P2_H

// retorna la respresentacion binaria del entero k
unsigned int int_to_bin(unsigned int k);

// 
unsigned int* parse_int(unsigned int n);

// Lee FRAM_SIZE bytes de data.bin y los copia en buffer
int read_disk(char* buffer[], unsigned int addr);

#endif