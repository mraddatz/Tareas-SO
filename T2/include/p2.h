#ifndef P2_H
#define P2_H

// Retorna n bits de num partiendo desde start (0-index)
unsigned get_n_bits(unsigned num, unsigned n, unsigned start);

// Lee FRAM_SIZE bytes de data.bin y los copia en buffer
int read_disk(char* buffer[], unsigned addr);

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]);

#endif