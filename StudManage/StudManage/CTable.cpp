#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "CMyFile.h"
#include "CTable.h"

#include "Exception.h"

#define USE_EXCEPTION

#define ADDR_RECORD(records, i, recordsize)   ((void *)((int)(records) + (i) * (recordsize)))

#define  INCREMENT  10000

//////////////////////////////////////////////////////////////////////////
//CTable相关
//////////////////////////////////////////////////////////////////////////
CTable::CTable()
{
    m_nrecords = 0;
}

CTable::~CTable()
{
    m_nrecords = 0;
}

unsigned long 
CTable::GetRecordsNum(void) const
{
    return m_nrecords;
}

void 
CTable::SetRecordsNum(unsigned long num)
{
    m_nrecords = num;
}

int 
CTable::LoadTable(char **ppbuf, const char *psztablefile, size_t recordsize) throw(CMyFileError)
{
    FILE *fp   = NULL;
    long size  = 0;     //文件大小
    long count = 0;     //文件中记录的数目
    int result = SUCCESS;
    char *precord = NULL;

    //make sure exist
    result = CMyFile::CheckFile(psztablefile);
    if (NOT_EXIST == result)
    {
        return result;
    }

    //now load into memory
    fp = fopen(psztablefile, "rb");
    if (NULL == fp)
    {
        result = FAILED;

    #ifdef USE_EXCEPTION
        throw CMyFileError("CTable::LoadTable FAILED, NOT EXIST: ", psztablefile);
    #else
        return result;  //ugly, 
    #endif
    }

    fseek(fp, 0, SEEK_END);
    if (ferror(fp))
    {
        perror("loadtable fseek error");

        result = FAILED;

    #ifdef USE_EXCEPTION
        fclose(fp);
        throw CMyFileError("CTable::LoadTable FAILED, SEEK ERROR: ", psztablefile);
    #else
        goto LOAD_ERROR;
    #endif
    }

    size = ftell(fp);
    if (ferror(fp))
    {
        perror("loadtable ftell error");

    #ifdef USE_EXCEPTION
        result = FAILED;
        fclose(fp);
        
        throw CMyFileError("CTable::LoadTable FAILED, FTELL ERROR: ", psztablefile);
    #else
        goto LOAD_ERROR;
    #endif
    }
    
    //首部是m_nrecords.
    bool IsEmpty = true;
    if (size)
    {
        IsEmpty = false;

        size -= sizeof(m_nrecords);
    }

    if (size % recordsize)
    {
        result = FAILED;

        //all right to do like this? with ui in logic ?????
        //showinfo("loadtable: data is corrupted\r\n");

    #ifdef USE_EXCEPTION
        fclose(fp);
        throw CMyFileError("CTable::LoadTable FAILED, CORRUPTED: ", psztablefile);
    #else
        goto LOAD_ERROR;
    #endif
    }

    count = size / recordsize;
    precord = (char *)malloc(size);    //count * recordsize    
    if (NULL == precord)
    {
        result = FAILED;

        //showinfo("loadtable: malloc failed\r\n");
        #ifdef USE_EXCEPTION
            fclose(fp);
            throw CMyFileError("CTable::LoadTable FAILED, MALLOC FAILED: ", psztablefile);
        #else
            goto LOAD_ERROR;
        #endif
    }


    if (IsEmpty)
    {
        fseek(fp, 0, SEEK_SET);
    }
    else
    {
        fseek(fp, sizeof(m_nrecords), SEEK_SET);
    }

    if (ferror(fp))
    {
        perror("loadtable fseek to begin error");
        result = FAILED;
        
        #ifdef USE_EXCEPTION
            fclose(fp);
            throw CMyFileError("CTable::LoadTable FAILED, FSEEK FAILED: ", psztablefile);
        #else
            goto LOAD_ERROR;
        #endif
    }

    m_nrecords = count;
    *ppbuf = precord;
    fread(*ppbuf, size, 1, fp);
   
    if (ferror(fp))
    {
        perror("loadtable fread error");

        result = FAILED;
        #ifdef USE_EXCEPTION
            fclose(fp);
            throw CMyFileError("CTable::LoadTable FAILED, FREAD ERROR: ", psztablefile);
        #else
            goto LOAD_ERROR;
        #endif
    }

LOAD_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    } 

    return result;
}

int
CTable::SaveTable(void *precords,       //内存中分配表的起始地址
                  int index,            //要写入的数据的起始下标
                  int nrecords,         //要写入的记录的个数,
                  size_t recordsize,    //每个记录的大小
                  const char *pszfile   //要写入的文件
                  )
{
    char tmpbuf[4096] = {0};
    unsigned long offset = index * recordsize + sizeof(m_nrecords); //首部的m_nrecords
    int result = SUCCESS;

    //先来把最头部的总记录个数更新
    CMyFile(pszfile).ModifyData(&m_nrecords, 0, sizeof(m_nrecords));

    //近乎4096字节写入
    unsigned ncount  = 4096 / recordsize;    
    for (unsigned long i = 0; i < nrecords / ncount; i++)
    {
        for (unsigned long j = 0; j < ncount; j++)
        {
            memcpy((char *)tmpbuf + j * recordsize,     //
                    (char *)precords + (index + i * ncount + j) * recordsize,
                    recordsize);
        }
        
        result = CMyFile(pszfile).ModifyData(tmpbuf,
                                            offset,
                                            nrecords * ncount
                                            );
        offset += ncount * recordsize;

        if (FAILED == result)
        {
            //showinfo("savetable storerecord add error");
            return result;
        }
    }
    
    unsigned long j = 0;
    for ( i = i * ncount;  i < nrecords; i++, j++)
    {
        memcpy((char *) tmpbuf + j * recordsize,
                (char *)precords + (index + i) * recordsize,
                recordsize);
    }
    
    result = CMyFile(pszfile).ModifyData(tmpbuf,
                                         offset,
                                         j * recordsize
                                         );
    if (FAILED == result)
    {
        //showinfo("savetable storerecord add error");
        return result;
    }
    
    return result;
}

int 
CTable::SaveTable(void *buf,
                  const char *psztablefile,
                  unsigned char recordsize
                  )
{
    char tmpbuf[4096] = {0};
    char szfile_bak[MAX_INPUT] = {0};    
    sprintf(szfile_bak, "%s_bak", psztablefile);
    
    //backup file
    int result = SUCCESS;
    result = CMyFile::FileCopy(psztablefile, szfile_bak);
    if (FAILED == result)
    {
        //showinfo("backup addtablefile error");
        return result;
    }
    
    //clear the file
    result = CMyFile(psztablefile).ResetData();
    if (FAILED == result)
    {
        //showinfo("savetable storerecord add init error");
        return result;
    } 

    //近乎4096字节写入
    unsigned ncount  = 4096 / recordsize;    
    for (unsigned long i = 0; i < m_nrecords / ncount; i++)
    {
        for (unsigned long j = 0; j < ncount; j++)
        {
            memcpy((char *)tmpbuf + j * recordsize,     //
                    (char *)buf + (i * ncount + j) * recordsize,
                    recordsize);
        }

        result = CMyFile(psztablefile).AppendData(tmpbuf,
                                                  ncount * recordsize
                                                  );
        if (FAILED == result)
        {
            //showinfo("savetable storerecord add error");
            return result;
        }
    }

    unsigned long j = 0;
    for ( i = i * ncount;  i < m_nrecords; i++, j++)
    {
        memcpy((char *) tmpbuf + j * recordsize,
                (char *)buf + i * recordsize,
                recordsize);
    }

    result = CMyFile(psztablefile).AppendData(tmpbuf,
                                              j * recordsize
                                             );
    if (FAILED == result)
    {
        //showinfo("savetable storerecord add error");
        return result;
     }
    
    return result;
}

void
CTable::InsertIntoTable(unsigned long *nrecords,         
                        void *records,                        
                        void *newrecord,                   
                        size_t recordsize,                   
                        int *index,                     
                        int (*compare)(void *, void *),
                        const char *pszfile
                        )
                
{
    int i = 0;
    int j = 0;
    
    //find the proper position
    for (i = *nrecords; i >= 1; i--)
    {
        //i will be the insert point.
        if (0 > compare( //(void *)((int)records + (i-1) * recordsize),
            ADDR_RECORD(records, i-1, recordsize), 
            newrecord) 
            )                   
        {
            break;
        }
    }
    
    //now we need to move i, i+1, .. nrecords-1 to i+1, i+2,...nrecords
    for (j = *nrecords; j >= i + 1; j--)
    {
        memcpy(//(void *)((int)records + j * recordsize), 
            ADDR_RECORD(records, j, recordsize),
            //(void *)((int)records + (j-1) * recordsize),
            ADDR_RECORD(records, j - 1, recordsize),
            recordsize);      
    }
    
    //now insert this record to i
    memcpy(//(void *)((int)records + i * recordsize), 
        ADDR_RECORD(records, i, recordsize),
        newrecord, recordsize);
    
    //update data
    *index = i;
    *nrecords = *nrecords + 1;

    /*
    现在应该更新相应的存储文件
    将records[i]...records[*nrecords - 1] 的记录都重新写入到文件中
    */
    SaveTable(records, i, *nrecords - i, recordsize, pszfile);
}

void 
CTable::UpdateTable(void *precords,          
                 size_t recordsize,      
                 int index,              
                 void *newrecord,
                 const char *pszfile
                 )
{
    memcpy(ADDR_RECORD(precords, index, recordsize), //records[index],
           newrecord, recordsize);


    SaveTable(precords, index, 1, recordsize, pszfile);
}

void 
CTable::RemoveFromTable(unsigned long *pnrecords,     
                     void *precords,                      
                     size_t recordsize,                  
                     int index,
                     const char *pszfile
                        )
{
    if (0 == *pnrecords)
    {
        return;
    }

    unsigned long i = 0;
    
    //move index+1, index+2, ..., nrecords-1 to index, index+1, ..., nrecords-2
    for (i = index; i < *pnrecords - 1; i++)
    {
        memcpy(ADDR_RECORD(precords, i, recordsize),
               ADDR_RECORD(precords, i+1, recordsize),
               recordsize);
    }
    
    memset(ADDR_RECORD(precords, *pnrecords-1, recordsize), //records[*nrecords - 1], 
            0, recordsize);
    
    *pnrecords = *pnrecords - 1;

    SaveTable(precords, index, *pnrecords - index, recordsize, pszfile);
}

//////////////////////////////////////////////////////////////////////////
//CAddTable相关
//////////////////////////////////////////////////////////////////////////

CAddTable::CAddTable()
{
    m_precords = NULL;
}

CAddTable::~CAddTable()
{
    if (m_precords != NULL)
    {
        free(m_precords);
        m_precords = NULL;
    }
}

tag_addrecord *
CAddTable::GetRecords(void) const
{
    return m_precords;
}

int 
CAddTable::LoadTable(const char *pszaddtablefile)
{
    char *ptmp = NULL;
    int nresult = CTable::LoadTable(&ptmp,
                                    pszaddtablefile,
                                    sizeof(tag_addrecord)
                                    );
    m_precords = (tag_addrecord *)ptmp;

    return nresult;
}

int
CAddTable::SaveTable(const char *pszaddtablefile)
{
    return CTable::SaveTable(m_precords,
                            pszaddtablefile,
                            sizeof(tag_addrecord)
                            );
}

int
compare_addrecord(void *record, void *addrecord)
{
    unsigned long x = 0, y = 0;
    
    x = ((tag_addrecord *)record)->ulstart;
    y = ((tag_addrecord *)addrecord)->ulstart;
    
    return x - y;
}

void
CAddTable::InsertIntoTable(tag_addrecord *paddrecord, const char *pszfile)
{
    size_t newsize = 0;
    int index      = 0;
    
    if (m_nrecords % INCREMENT == 0 )
    {
        newsize = (m_nrecords + INCREMENT) * sizeof(tag_addrecord)// 增加INCREMENT个records
            + sizeof(m_nrecords);                 //for m_nrecords
        
        
        m_precords = (tag_addrecord *)realloc(m_precords, newsize);
    }
    
    if (NULL == m_precords)
    {
        //showinfo("Insertintoaddtable realloc error\r\n");
    }
    
    CTable::InsertIntoTable(&m_nrecords,
                            m_precords,
                            paddrecord,
                            sizeof(tag_addrecord),
                            &index,
                            compare_addrecord,          //回调函数
                            pszfile
                           );
}

void
CAddTable::UpdateTable(int index, tag_addrecord *paddrecord, const char *pszfile)
{
    CTable::UpdateTable(m_precords,
                        sizeof(tag_addrecord),
                        index,
                        paddrecord,
                        pszfile
                        );
}

void
CAddTable::RemoveFromTable(int index, const char *pszfile)
{
    size_t newsize = 0;
    
    CTable::RemoveFromTable(&m_nrecords,
                            m_precords,
                            sizeof(tag_addrecord),
                            index,
                            pszfile
                            );
    
    //table nrecords already updated,  -1
    newsize = sizeof(m_nrecords)
                + m_nrecords * sizeof(tag_addrecord);
    
    m_precords = (tag_addrecord *)realloc(m_precords,
                    newsize  
                 );
    
    if (NULL == m_precords)
    {
        //showinfo("Removefromaddtable realloc error\r\n");
    }
}

bool
CAddTable::ReadRecord(unsigned long offset, tag_addrecord **ppaddrecord, unsigned long *pindex)
{
    int lo = 0;
    int hi = m_nrecords;
    int mid = 0;
    unsigned long start = 0;

    while (lo <= hi)
    {
        mid = 1 / 2.0 * (lo + hi);

        start = m_precords[mid].ulstart;
        if (offset == start)
        {
            *ppaddrecord = &m_precords[mid];
            *pindex = mid;
            return true;
        }
        else if (offset > start)
        {
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
//CDelTable相关
//////////////////////////////////////////////////////////////////////////
CDelTable::CDelTable()
{
    m_precords = NULL;
}

CDelTable::~CDelTable()
{
    if (m_precords != NULL)
    {
        free(m_precords);
        m_precords = NULL;
    }
}

tag_delrecord *
CDelTable::GetRecords(void) const
{
    return m_precords;
}

int
CDelTable::LoadTable(const char *pszdeltablefile)
{
    char *ptmp = NULL;
    int nresult = CTable::LoadTable(&ptmp,
                                    pszdeltablefile,
                                    sizeof(tag_delrecord)
                                    );
    m_precords = (tag_delrecord *)ptmp;
    return nresult;
}

int
CDelTable::SaveTable(const char *pszdeltable)
{
    return CTable::SaveTable(m_precords,
                            pszdeltable,
                            sizeof(tag_delrecord)
                            );
}

void
CDelTable::Reorganize(unsigned long index, const char *pszfile)
{
    unsigned long indexpre = 0;
    unsigned char sizepre  = 0;
    unsigned long startpre = 0;
    
    unsigned long indexaft = 0; 
    unsigned char  sizeaft  = 0;
    unsigned long  startaft = 0;
    
    unsigned char size     = 0;
    unsigned long start    = 0;
    
    tag_delrecord delrecord;
    CDelTable *pdeltable = NULL;
    
    size  = m_precords[index].uchsize;
    start = m_precords[index].ulstart;   
    
    //aft
    if ((index + 1) != m_nrecords)
    {
        indexaft = index + 1;
        sizeaft  = m_precords[indexaft].uchsize;
        startaft = m_precords[indexaft].ulstart;
        
        if (startaft == start + size)
        {
            delrecord.uchsize  = size + sizeaft;
            delrecord.ulstart  = start;
            UpdateTable(index, &delrecord, pszfile);
            RemoveFromTable(indexaft, pszfile);            
            //showinfo("reorganize space aft happens\r\n");
        }
    }//end for aft
    
    //pre
    if (index != 0)
    {
        indexpre = index - 1;  
        sizepre  = m_precords[indexpre].uchsize;
        startpre = m_precords[indexpre].ulstart;
        
        if (start == startpre + sizepre)
        {
            //take care, may size already updated, we need to use g_del_table[index]
            delrecord.uchsize  = sizepre +m_precords[index].uchsize;
            delrecord.ulstart  = startpre;
            UpdateTable(indexpre, &delrecord, pszfile);
            RemoveFromTable(index, pszfile);
            //showinfo("reorganize space pre happens\r\n");
        }   
    }//end for pre 
}

int
compare_delrecord(void *record, void *delrecord)
{
    unsigned long x = 0, y = 0;
    
    x = ((tag_delrecord *)record)->ulstart;
    y = ((tag_delrecord *)delrecord)->ulstart;
    
    return x - y;
}

void 
CDelTable::InserIntoTable(tag_delrecord *pdelrecord, const char *pszfile)
{
    size_t newsize = 0;
    int index      = 0;
    
    newsize = (m_nrecords + 1) * sizeof(tag_delrecord) //增加1各records
                + sizeof(m_nrecords);        //for m_nrecords
    
    m_precords = (tag_delrecord *)realloc(m_precords, newsize);
    
    if (NULL == m_precords)
    {
        //sth wrong
        //showinfo("Insertintodeltable realloc error\r\n");
    }
    
    CTable::InsertIntoTable(&m_nrecords,
                            m_precords,
                            pdelrecord,
                            sizeof(tag_delrecord),
                            &index,
                            compare_delrecord,
                            pszfile
                            );
    
    Reorganize(index, pszfile);
}

void
CDelTable::UpdateTable(int index, tag_delrecord *pdelrecord, const char *pszfile)
{
    CTable::UpdateTable(m_precords,
                        sizeof(tag_delrecord),
                        index,
                        pdelrecord,
                        pszfile
                        );
}

void
CDelTable::RemoveFromTable(int index, const char *pszfile)
{
    size_t newsize = 0;
    
    CTable::RemoveFromTable(&m_nrecords,
                            m_precords,
                            sizeof(tag_delrecord),
                            index,
                            pszfile
                           );
    
    //deltable nrecords already updated, -1
    newsize = sizeof(m_nrecords)
                + m_nrecords * sizeof(tag_delrecord);
    
    m_precords = (tag_delrecord *)realloc(m_precords,
                                            newsize
                                         );
    if (NULL == m_precords)
    {
        //showinfo("Removefromdeltable realloc error\r\n");
    }
}

