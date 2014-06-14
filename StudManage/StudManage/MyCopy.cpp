#include <stdio.h>
#include <math.h>
#include "MyCopy.h"

#define EXP_PER_TIME  10                                //
#define BYTES_PER_TIME  (pow(2, EXP_PER_TIME))          // 2^10
#define BYTES_REMAIN    (BYTES_PER_TIME - 1)    //2 ^ 10 - 1

int
mycopy_v2(const char *filesrc, const char *filedst)
{
    int result       = SUCCESS;
    FILE *fpsrc      = NULL;
    FILE *fpdst      = NULL;
    long lnfsize    =  0;
    unsigned ncount  = 0;
    unsigned nremain = 0;
    unsigned i       = 0;

#ifdef EXP_PER_TIME
    //char buf[BYTES_PER_TIME]      = {0};
    char buf[4096]   = {0};
#else
    char buf[8]      = {0};
#endif
    
    fpsrc = fopen(filesrc, "rb");
    if (NULL == fpsrc)
    {
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    fpdst = fopen(filedst, "wb");
    if (NULL == fpdst)
    {
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    //get file size, 
    fseek(fpsrc, 0, SEEK_END);
    if (ferror(fpsrc))
    {
        perror("mycopy_v2 fseek error");
        
        result = FAILED;
        goto V2_ERROR_PROCESS;
    }

    lnfsize = ftell(fpsrc);
    if (-1L == lnfsize)
    {
        result = 0;
        goto V2_ERROR_PROCESS;
    }
    
    //restore to the beginning
    fseek(fpsrc, 0, SEEK_SET);
    
#ifdef EXP_PER_TIME
    ncount = lnfsize >> EXP_PER_TIME;
    nremain = lnfsize & (int)BYTES_REMAIN;
    
#else
    //read 4 bytes every time.
    ncount  = lnfsize >> 2;     //ncount  = lnfsize / sizeof(int);
    nremain = lnfsize & 3;      //nremain = lnfsize % (sizeof(int) - 1); 
    
#endif
    
    //read and write now every 4 bytes
    for (i = 0; i < ncount; i++)
    {
    #ifdef EXP_PER_TIME
        fread(buf, BYTES_PER_TIME, 1, fpsrc);
    #else
        fread(buf, sizeof(int), 1, fpsrc);
    #endif

        if (ferror(fpsrc))
        {
            perror("Read Error When Copy 4 bytes");

            result = FAILED;
            goto V2_ERROR_PROCESS;
        }
        
    #ifdef EXP_PER_TIME
        fwrite(buf, BYTES_PER_TIME, 1, fpdst);
    #else
        fwrite(buf, sizeof(int), 1, fpdst);
    #endif
        if (ferror(fpdst))
        {
            perror("Write Error When Copy 4 bytes");

            result = FAILED;
            goto V2_ERROR_PROCESS;
        }
    }//end for copy 4bytes everytime
    
    //deal with the remaining
    if (0 == nremain)
    {
        goto V2_ERROR_PROCESS;
    }
    
    fread(buf, nremain, 1, fpsrc);
    if (ferror(fpsrc))
    {
        perror("Read Error When Copy remained bytes");

        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
    fwrite(buf, nremain, 1, fpdst);
    if (ferror(fpdst))
    {
        perror("Write Error When Write remained bytes");

        result = FAILED;
        goto V2_ERROR_PROCESS;
    }
    
V2_ERROR_PROCESS:
    if (fpsrc)
    {
        fclose(fpsrc);
        fpsrc = NULL;
    }
    
    if (fpdst)
    {
        fclose(fpdst);
        fpdst = NULL;
    }
    
    return result;
}