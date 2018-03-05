// u16 circular buffer 
#ifndef U16CBUF_H
#define U16CBUF_H

#include "stdio.h"
#include "types.h"

boolean initBuffer(uint16 num_elements);
void resetBuffer();
void addValue(uint16 val);
void printBufToFile(FILE **fh);

#endif
