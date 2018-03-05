#include "stdlib.h"
#include "u16CBuf.h"

static uint16 *buf;
static uint16 MAXSIZE = 0;
static uint16 next_idx;
static uint16 COUNT = 0;

/****************************************************************/
/*!
@brief
Add a new value to the buffer

@param val
Value to add to buffer
*/
void addValue(uint16 val)
{
    if(buf != NULL)
    {
        buf[next_idx] = val;
        next_idx = (next_idx + 1) % MAXSIZE;
        if(COUNT < MAXSIZE) COUNT++;
    }
}

/****************************************************************/
/*!
@brief
Initialize the circular buffer.

@param num_elements
Specify number of elements to allocate. (buffer size)

@return boolean
TRUE : if success, FALSE: else 

*/
boolean initBuffer(uint16 num_elements)
{
    if(buf != NULL)
    {
        return FALSE;
    }
    buf = (uint16*)malloc(num_elements*sizeof(uint16));
    if(buf == NULL)
    {
        printf("Error allocating memory for buffer.\n");
        return FALSE;
    }
    next_idx = 0;
    COUNT = 0;
    MAXSIZE = num_elements;
    return TRUE;
}

/****************************************************************/
/*!
@brief
Resets the buffer and deallocated any allocated memory.
*/
void resetBuffer()
{
    if(buf != NULL)
        free(buf);
    next_idx = 0;
    COUNT = 0;
    MAXSIZE = 0;
}

/****************************************************************/
/*!
@brief
Prints buffered values to file.

@param fh
Pointer to file handle.
*/
void printBufToFile(FILE **fh)
{
    uint16 idx = next_idx;
    if(COUNT < MAXSIZE)
    {
       idx = next_idx + MAXSIZE - COUNT;
       idx = idx % MAXSIZE;
    }
    
    do
    {
        fprintf(*fh, "%d\n", buf[idx]);
        idx = (idx + 1) % MAXSIZE;
    } while(idx != next_idx);
}
