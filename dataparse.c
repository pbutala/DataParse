// dataparse.c
#include "stdio.h"

#define DEBUG
#define TRUE 0
#define FALSE 1

int getFileInOut(FILE **in, FILE **out);

int main()
{
    FILE *fh_in;
    FILE *fh_out;
    int status = TRUE;
    
    status = getFileInOut(&fh_in, &fh_out);
    if (status == TRUE)
    {
        // Write to file
        fprintf(fh_out, "%s", "--Sorted Max 32 Values--\n");
        fprintf(fh_out, "%s", "--Last 32 Values--\n");
    }
    
    // 
    if (status == TRUE)
    {
        printf("Parsed data.\n");        
    }
    
    // Cleanup
    fclose(fh_in);
    fclose(fh_out);
    return status;
}

int getFileInOut(FILE **in, FILE **out)
{
    char f_in[256] = "DataIn\\test1.bin";
    char f_out[256] = "dataparse.out";
    
    // Get input file
    #ifndef DEBUG
    printf("Input file name: ");
    scanf("%s", f_in);
    #endif
    
    *in = fopen(f_in, "r");
    if(!*in)
    {
        printf("Error opening data file.\n");
        return FALSE;
    }

    // Create output file
    #ifndef DEBUG
    printf("Output file name: ");
    scanf("%s", f_out);
    #endif

    *out = fopen(f_out, "w");
    if(!*out)
    {
        printf("Error creating output file.\n");
        return FALSE;
    }
    
    return TRUE;
}