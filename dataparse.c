// dataparse.c
#include "stdio.h"
#include "types.h"
#include "u16CBuf.h"
#include "u16List.h"

//#define DATAPARSE_DBG
//#define AUTODATAFILE

#ifdef DATAPARSE_DBG
#define MAXBUFLEN 5
#else
#define MAXBUFLEN 32
#endif

/*!
@brief
Enumeration for bit status to indicate number of parsed bits.
 */
typedef enum
{
    BITS_0 = 0,
    BITS_4 = 4,
    BITS_8 = 8,
    BITS_12 = 12,
    
    BYTE_BITS_MAX = BITS_8,
    DATA_BITS_MAX = BITS_12
} bit_status_e;

/*!
@brief
Structure to store state for parsed byte value.
*/
typedef struct 
{
    uint8 val;
    bit_status_e bits_valid;    
} byte_read_s;

/*!
@brief
Structure to store state for parsed data value.
*/
typedef struct 
{
    uint16 val;
    bit_status_e bits_valid;    
} data_read_s;

boolean getFileInOut(FILE **in, FILE **out);
void updateData(data_read_s* data, byte_read_s *bt);

int main()
{
    FILE *fh_in;
    FILE *fh_out;
    boolean status = TRUE;
    
    int bt_rd_count = 0;
    int data_rd_count = 0;
    
    byte_read_s bt_prev = {0, BITS_0};
    byte_read_s bt_curr = {0, BITS_0};
    data_read_s data = {0, BITS_0};
    
    status = getFileInOut(&fh_in, &fh_out); // get in and out file handles.
    if(status == TRUE)
    {
        status = initList(MAXBUFLEN); // initialize list to help with sorting.
        status &= initBuffer(MAXBUFLEN); // initialize circular buffer.
    }
    
    if (status == TRUE)
    {
        
        while(!feof(fh_in))
        {
            if(fread(&bt_curr.val, 1, 1, fh_in)) // read a byte
            {
                bt_rd_count++;
                bt_curr.bits_valid = BITS_8;
                
                updateData(&data, &bt_curr); // parse data bits from read byte
                
                if(data.bits_valid == DATA_BITS_MAX) // if 12 bits of data parsed
                {
                    data_rd_count++;
                    // process data
                    addValue(data.val); // add parsed value to buffer
                    sortValue(data.val); // sort and add parsed value to list if within highest 32 values.
                    
                    // post-process
                    // Since parsed data is processed, reset data state 
                    data.val = 0;
                    data.bits_valid = BITS_0;
                    
                    // Parse (if any) remaining bits from the byte into data
                    if(bt_curr.bits_valid > BITS_0)
                    {
                        updateData(&data, &bt_curr);
                    }
                }
            }
        }
        // Write to file
        fprintf(fh_out, "%s", "--Sorted Max 32 Values--\n");
        printListToFile(&fh_out);
        fprintf(fh_out, "%s", "--Last 32 Values--\n");
        printBufToFile(&fh_out);
        
    }
    
    // 
    if (status == TRUE)
    {
        fclose(fh_in);
        fclose(fh_out);        
        printf("Parsed data.\n"); 
    }
    
    // Cleanup
    resetBuffer();
    resetList();
    
    return 0;
}

/****************************************************************/
/*!
@brief
Populate file handles.

@param **in
Pointer to input file handle

@param **out
Pointer to output file handle

@return 
TRUE: if files acquired successfully, FALSE: else
*/
boolean getFileInOut(FILE **in, FILE **out)
{
    int8 f_in[256] = "DataIn\\test3.bin";
    int8 f_out[256] = "dataparse.out";
    
    // Get input file
    #ifndef AUTODATAFILE
    printf("Input file name: ");
    scanf("%s", f_in);
    #endif
    
    *in = fopen(f_in, "rb");
    if(!*in)
    {
        printf("Error opening data file.\n");
        return FALSE;
    }

    // Create output file
    #ifndef AUTODATAFILE
    printf("Output file name: ");
    scanf("%s", f_out);
    #endif

    *out = fopen(f_out, "wb");
    if(!*out)
    {
        printf("Error creating output file.\n");
        return FALSE;
    }
    
    return TRUE;
}

/****************************************************************/
/*!
@brief
Parses byte input stream into 12 bit data stream.

@param *data
Data handle with parsed data information.

@param *bt
Received 'byte' handle with parsed byte information.

@return 
*/
void updateData(data_read_s *data, byte_read_s *bt)
{ 
    uint8 data_bits_rem = DATA_BITS_MAX - data->bits_valid; // num of data bits needed to acquire 12 bits
    uint8 bt_bits_update; // number of bits to update from byte into data
    uint8 bt_bits_rem;  // number of bits of byte remaining to be parsed to data
    uint8 bt_tmp;
    
    // Compute number of bits to parse from byte.
    // Compute number of bite remaining after parse.
    if(bt->bits_valid > data_bits_rem) 
    {
        bt_bits_update = data_bits_rem;
        bt_bits_rem = bt->bits_valid - bt_bits_update;
    }
    else
    {
        bt_bits_update = bt->bits_valid;
        bt_bits_rem = 0;
    }
    
    // Value corresponding to byte bits to be parsed
    bt_tmp = bt->val >> (BYTE_BITS_MAX - bt_bits_update);
    
    // Parse requisite bite to data
    data->val = data->val << bt_bits_update;
    data->val = data->val + bt_tmp;
    data->bits_valid = data->bits_valid + bt_bits_update;
    
    // Post process byte with remaining bits to be parsed
    bt->val = bt->val << (BYTE_BITS_MAX - bt->bits_valid + bt_bits_update);
    bt->val = bt->val >> (BYTE_BITS_MAX - bt_bits_rem);
    bt->bits_valid = bt_bits_rem;
}
