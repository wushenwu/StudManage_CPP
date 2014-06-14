#include "CStudManage.h"
#include "Fit.h"
#include "CInfoManage.h"

//the way you can get suitable size, can only be dealed within this file
static int g_fitWAY = FIT_EQUAL;

int 
FitEqual(unsigned char requestsize, int *index, const CStudManage *pstudmanage)
{
    int result = NOT_EXIST;
    unsigned long i = 0;

    CDelTable *pdeltable = pstudmanage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0; i < nrecords; i++)
    {
        if (requestsize == pdelrecords[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
FitEnough(unsigned char requestsize, int *index, const CStudManage *pstudmanage)
{
    int result = NOT_EXIST;
    int min    = -1;
    unsigned long i      = 0;

    CDelTable *pdeltable = pstudmanage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0, min = -1; i < nrecords; i++)
    {
        if (requestsize <= pdelrecords[i].uchsize)
        {
            if ((-1 == min) 
                || (pdelrecords[i].uchsize < pdelrecords[min].uchsize)
                )
            {
                min = i;
            }
        }
    }

    if (-1 != min)
    {
        *index = min;
        result = SUCCESS;
    }

    return result;
}

int 
FitFirst(unsigned char requestsize, int *index, const CStudManage *pstudmanage)
{
    int result = NOT_EXIST;
    unsigned long i      = 0;

    CDelTable *pdeltable = pstudmanage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0; i < nrecords; i++)
    {
        if (requestsize <= pdelrecords[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
GetSize(unsigned char requestsize, int *index, const CStudManage *pstudmanage)
{
    int result = NOT_EXIST;

    switch (g_fitWAY)
    {
    case FIT_EQUAL:
        {
            result = FitEqual(requestsize, index, pstudmanage);
            break;
        }       

    case FIT_ENOUGH:
        {
            result = FitEnough(requestsize, index, pstudmanage);
            break;
        }        

    case FIT_FIRST:
        {
            result = FitFirst(requestsize, index, pstudmanage);
            break;
        }        

    case FIT_EQUAL_ENOUGH:
        {
            result = FitEqual(requestsize, index, pstudmanage);
            if (NOT_EXIST == result)
            {
                result = FitEnough(requestsize, index, pstudmanage);
            }
            break;
        }        

    case FIT_EQUAL_FIRST:
        {
            result = FitEqual(requestsize, index, pstudmanage);
            if (NOT_EXIST == result)
            {
                result = FitFirst(requestsize, index, pstudmanage);
            }
            break;
        }        

    default:
        {
            //sth wrong ?
            result = NOT_EXIST;
            break;
        }        
    }

    return result;
}

void
SetFit(int nselect)
{
   g_fitWAY = nselect;
}

/////////////////////////////////////////////////////////////////////////
int 
FitEqual(unsigned char requestsize, int *index, CBaseInfoManage *pBaseInfoManage)
{
    int result = NOT_EXIST;
    unsigned long i = 0;

    CDelTable *pdeltable = pBaseInfoManage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0; i < nrecords; i++)
    {
        if (requestsize == pdelrecords[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
FitEnough(unsigned char requestsize, int *index, CBaseInfoManage *pBaseInfoManage)
{
    int result = NOT_EXIST;
    int min    = -1;
    unsigned long i      = 0;

    CDelTable *pdeltable = pBaseInfoManage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0, min = -1; i < nrecords; i++)
    {
        if (requestsize <= pdelrecords[i].uchsize)
        {
            if ((-1 == min) 
                || (pdelrecords[i].uchsize < pdelrecords[min].uchsize)
                )
            {
                min = i;
            }
        }
    }

    if (-1 != min)
    {
        *index = min;
        result = SUCCESS;
    }

    return result;
}

int 
FitFirst(unsigned char requestsize, int *index, CBaseInfoManage *pBaseInfoManage)
{
    int result = NOT_EXIST;
    unsigned long i      = 0;

    CDelTable *pdeltable = pBaseInfoManage->GetDelTable();
    unsigned long nrecords = pdeltable->GetRecordsNum();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    for (i = 0; i < nrecords; i++)
    {
        if (requestsize <= pdelrecords[i].uchsize)
        {
            *index = i;
            result = SUCCESS;

            break;
        }
    }

    return result;
}

int 
GetSize(unsigned char requestsize, int *index, CBaseInfoManage *pBaseInfoManage)
{
    int result = NOT_EXIST;

    switch (g_fitWAY)
    {
    case FIT_EQUAL:
        {
            result = FitEqual(requestsize, index, pBaseInfoManage);
            break;
        }       

    case FIT_ENOUGH:
        {
            result = FitEnough(requestsize, index, pBaseInfoManage);
            break;
        }        

    case FIT_FIRST:
        {
            result = FitFirst(requestsize, index, pBaseInfoManage);
            break;
        }        

    case FIT_EQUAL_ENOUGH:
        {
            result = FitEqual(requestsize, index, pBaseInfoManage);
            if (NOT_EXIST == result)
            {
                result = FitEnough(requestsize, index, pBaseInfoManage);
            }
            break;
        }        

    case FIT_EQUAL_FIRST:
        {
            result = FitEqual(requestsize, index, pBaseInfoManage);
            if (NOT_EXIST == result)
            {
                result = FitFirst(requestsize, index, pBaseInfoManage);
            }
            break;
        }        

    default:
        {
            //sth wrong ?
            result = NOT_EXIST;
            break;
        }        
    }

    return result;
}