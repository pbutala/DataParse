// u16List.h
#ifndef U16LIST_H
#define U16LIST_H

#include "stdio.h"
#include "types.h"

/*!Node for list of unsigned shorts*/
typedef struct
{
    /*!Value*/
    uint16 val;
    /*!Next node*/
    struct u16Node *next;
} u16Node;

boolean initList(uint32 maxLen);
void sortValue(uint16 val);
void resetList();
void printListToFile(FILE **fh);

#endif // U16LIST_H