// dataparse.c
#include "stdio.h"
#include "types.h"
#include "u16List.h"

//#define DATAPARSE_DBG
#define MAXBUFLEN 32
//#define MAXBUFLEN 5

typedef enum
{
    BITS_0 = 0,
    BITS_4 = 4,
    BITS_8 = 8,
    BITS_12 = 12,
    
    BYTE_BITS_MAX = BITS_8,
    DATA_BITS_MAX = BITS_12
} bit_status_e;

typedef struct 
{
    uint8 val;
    bit_status_e bits_valid;    
} uint8_read_s;

typedef struct 
{
    uint16 val;
    bit_status_e bits_valid;    
} data_read_s;

boolean getFileInOut(FILE **in, FILE **out);
void updateData(data_read_s* data, uint8_read_s *bt);

int main()
{
    FILE *fh_in;
    FILE *fh_out;
    boolean status = TRUE;
    
    int bt_rd_count = 0;
    int data_rd_count = 0;
    
    uint8_read_s bt_prev = {0, BITS_0};
    uint8_read_s bt_curr = {0, BITS_0};
    data_read_s data = {0, BITS_0};
    
    status = initList(MAXBUFLEN);
    
    status &= getFileInOut(&fh_in, &fh_out);
    
    if (status == TRUE)
    {
        while(!feof(fh_in))
        {
            if(fread(&bt_curr.val, 1, 1, fh_in))
            {
                bt_rd_count++;
                bt_curr.bits_valid = BITS_8;
                updateData(&data, &bt_curr);
                
                if(data.bits_valid == DATA_BITS_MAX)
                {
                    data_rd_count++;
                    // process data
                    //printf("%2d. %d\n", data_rd_count, data.val);
                    sortValue(data.val);
                    
                    // post-process
                    data.val = 0;
                    data.bits_valid = BITS_0;
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
    }
    
    // 
    if (status == TRUE)
    {
        printf("Parsed data.\n");        
    }
    
    // Cleanup
    resetList();
    fclose(fh_in);
    fclose(fh_out);
    return status;
}

boolean getFileInOut(FILE **in, FILE **out)
{
    int8 f_in[256] = "DataIn\\test1.bin";
    int8 f_out[256] = "dataparse.out";
    
    // Get input file
    #ifndef DATAPARSE_DBG
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
    #ifndef DATAPARSE_DBG
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

void updateData(data_read_s* data, uint8_read_s *bt)
{
    // bit_status_e 
    uint8 data_bits_rem = DATA_BITS_MAX - data->bits_valid;
    uint8 bt_bits_update;
    uint8 bt_bits_rem;
    uint8 bt_tmp;
    
    //printf("dataparse::updateData - data{%d, %d}, bt{%d, %d}\n", data->val, data->bits_valid, bt->val, bt->bits_valid);
    
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
    
    //printf("bt_bits_update %d, bt_bits_rem %d\n", bt_bits_update, bt_bits_rem);
    
    bt_tmp = bt->val >> (BYTE_BITS_MAX - bt_bits_update);
    
    data->val = data->val << bt_bits_update;
    data->val = data->val + bt_tmp;
    data->bits_valid = data->bits_valid + bt_bits_update;
    
    bt->val = bt->val << (BYTE_BITS_MAX - bt->bits_valid + bt_bits_update);
    bt->val = bt->val >> (BYTE_BITS_MAX - bt_bits_rem);
    bt->bits_valid = bt_bits_rem;
}
