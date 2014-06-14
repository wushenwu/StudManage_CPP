#include <fstream>
using namespace std;

#include <stdio.h>
#include <string.h>
#include "CMyFile.h"
#include "MyCopy.h"
#include "Exception.h"


//////////////////////////////////////////////////////////////////////////
//CMyFile相关
//////////////////////////////////////////////////////////////////////////
void 
CMyFile::SetFile(const char *pszfile)
{
   m_filename.SetString(pszfile);
}

const char *
CMyFile::GetFile(void) const
{
    return m_filename.GetString();
}


int
CMyFile::CheckFile(const char *pszfile) throw (CMyFileError)
{
#ifdef USE_FSTRSTREAM
    int result = SUCCESS;

    fstream fstr(pszfile, ios::in | ios::out | ios::binary);
    if (0 == fstr.is_open())
    {
        result = NOT_EXIST;

        throw CMyFileError("CMyFile::CheckFile NOT EXIST: ", pszfile);
    }
   
    fstr.close();
    return result;

#else
    FILE *fp   = NULL;
    int result = SUCCESS;
    
    fp = fopen(pszfile, "rb+");
    if (NULL == fp)
    {
        //try to create
        fp = fopen(pszfile, "wb+");
        if (NULL == fp)
        {
            result = NOT_EXIST;
        }
    }
    
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
#endif
}

int 
CMyFile::FileCopy(const char *pfilesrc, const char *pfiledst)
{
    int result = SUCCESS;
    
    result = mycopy_v2(pfilesrc, pfiledst);
    
    return result;
}

int
CMyFile::ReadData(void *pbuf,             
                unsigned long offset,    
                unsigned char size
                ) const  throw (CMyFileError)
                        
{
#ifdef USE_FSTRSTREAM
    int result = SUCCESS;

    ifstream ifstr(m_filename.GetString(), ios::in | ios::binary);
    if (0 == ifstr.is_open())
    {
        result = FAILED;
        ifstr.close();

        throw CMyFileError("CMyFile::ReadData FAILED, NOT EXIST: ", m_filename.GetString());
    }

    ifstr.seekg(offset, ios::beg);
    if (ifstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        ifstr.close();
        
        throw CMyFileError("CMyFile::ReadData FAILED, SEEK ERROR: ", m_filename.GetString());
    }

    ifstr.read((char *)pbuf, size);
    if (ifstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        ifstr.close();
        
        throw CMyFileError("CMyFile::ReadData FAILED, READ ERROR: ", m_filename.GetString());
    }

    ifstr.close();

    return result;
#else
    FILE *fp   = NULL;
    int result = SUCCESS;    
    
    fp = fopen(m_filename.GetString(), "rb");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    fseek(fp, offset, SEEK_SET);
    if (ferror(fp))
    {
        perror("Fetch1info_whole fseek error");
        
        result = FAILED;
        goto FETCH_WHOLE_ERROR;
    }
    
    fread(pbuf, size, 1, fp);
    if (ferror(fp))
    {
        perror("Fetch1info_whole fread error");
        
        result = FAILED;
        goto FETCH_WHOLE_ERROR;
    }
    
FETCH_WHOLE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
#endif
}

/*
Function: 向pszfile中写入大小为size字节的pbuf. （与类型无关）

Param   : void *pbuf 是要写入的信息
          initflg  1 表示初次写入，会将之前的内容清空。 0 继续添加。
  
Return  : SUCCESS 成功写入  FAILED 失败
*/
static int
storerecord(const char *pszfile, void *pbuf, unsigned char size, int initflg) throw (CMyFileError)
{
#ifdef USE_FSTRSTREAM
    int result = SUCCESS;

    if (1 == initflg)
    {
        ofstream ofstr(pszfile, ios::out | ios::binary);
        if (0 == ofstr.is_open() )
        {   
            result = FAILED;

            throw CMyFileError("CMyFile::storerecord 1 == initflg FAILED: ", pszfile);
        }

        ofstr.close();
        return result;
    }
    
    //0 == initflg
    fstream fstr(pszfile, ios::in | ios::out | ios::binary);
    if (0 == fstr.is_open())
    {
        result = FAILED;

        throw CMyFileError("CMyFile::storerecord 0 == initflg FAILED, NOT EXIST: ", pszfile);
    }

    fstr.seekp(0, ios::end);
    if (fstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        fstr.close();
            
        throw CMyFileError("CMyFile::storerecord 0 == initflg FAILED, SEEK ERROR: ", pszfile);
    }

    fstr.write((char *)pbuf, size);
    if (fstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        fstr.close();
        
        throw CMyFileError("CMyFile::storerecord 0 == initflg FAILED, WRITE ERROR: ", pszfile);
    }

    fstr.close();

    return result;

#else
    FILE *fp = NULL;
    int result = SUCCESS;
    
    if (1 == initflg)
    {
        fp = fopen(pszfile, "wb");  //reset the file
        goto STORERECORD_ERROR;
    }
    else
    {
        fp = fopen(pszfile, "rb+");
    }
    
    if (NULL == fp)
    {
        perror("storerecord fopen error");
        result = FAILED;
        return result;
    }
    
    result = fseek(fp, 0, SEEK_END);
    if (0 != result)
    {
        perror("storerecord fseek error");
        result = FAILED;
        goto STORERECORD_ERROR;
    }
    
    result = fwrite(pbuf, size, 1, fp);
    if (ferror(fp))
    {
        perror("storerecord fwrite error");
        result = FAILED;
        goto STORERECORD_ERROR;
    }
    
STORERECORD_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
#endif
}

int 
CMyFile::AppendData(void *pbuf, unsigned char size)
{
    //追加数据
    int initflg = 0; 
   return storerecord(m_filename.GetString(),
                        pbuf,
                        size,
                        initflg   
                        );
}

int 
CMyFile::ResetData(void)
{
    //初次写入，进行清空
    int initflg = 1; 

    return storerecord(m_filename.GetString(),
                        "",
                        0,
                        initflg       
                        );
}

int 
CMyFile::ModifyData(void *pbuf, unsigned long offset, unsigned char size) const throw (CMyFileError)
{
#ifdef USE_FSTRSTREAM
    int result = SUCCESS;

    fstream fstr(m_filename.GetString(), ios::in | ios::out | ios::binary);
    if (0 ==fstr.is_open())
    {
        result = FAILED;

        throw CMyFileError("CMyFile::ModifyData FAILE, NOT EXIST: ", m_filename.GetString());
    }

    fstr.seekp(offset, ios::beg);
    if (fstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        fstr.close();

        throw CMyFileError("CMyFile::ModifyData FAILE, SEEK ERROR: ", m_filename.GetString());
    }

    fstr.write((char *)pbuf, size);
    if (fstr.rdstate() != ios::goodbit)
    {
        result = FAILED;
        fstr.close();
        
        throw CMyFileError("CMyFile::ModifyData FAILE, WRITE ERROR: ", m_filename.GetString());
    }

    fstr.close();
    return result;
#else
    FILE *fp    = NULL;
    int result  = SUCCESS;
    
    fp = fopen(m_filename.GetString(), "rb+");
    if (NULL == fp)
    {
        result = FAILED;
        return result;
    }
    
    fseek(fp, offset, SEEK_SET);    
    if (ferror(fp))
    {
        perror("StoreInfo_whole fseek error");
        
        result = FAILED;
        goto STOREINFO_WHOLE_ERROR;
    }
    
    fwrite(pbuf, size, 1, fp);
    if (ferror(fp))
    {
        perror("StoreInfo_whole fwrite error");
        
        result = FAILED;
        goto STOREINFO_WHOLE_ERROR;
    }
    
STOREINFO_WHOLE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
#endif
}

//////////////////////////////////////////////////////////////////////////
//CBaseInfoFile相关
//////////////////////////////////////////////////////////////////////////
void
CBaseInfoFile::ReadData(tag_addrecord *paddrecord,       //要读取的信息,
                        char *pbuf
                        ) const
{
    int nresult = CMyFile::ReadData(pbuf,
                                    paddrecord->ulstart,
                                    paddrecord->uchsize
                                    );    
    if (FAILED == nresult)
    {
        return;
    } 
}

int
CBaseInfoFile::AppendData(CBaseInfo *pBaseInfo,    //要添加的信息
                            unsigned long offset   //文件地址
                            )
{     
    return ModifyData(pBaseInfo, offset);
}

int
CBaseInfoFile::ModifyData(CBaseInfo *pBaseInfo,   
                        unsigned long offset                             
                        )
{
    char buf[MAX_INPUT * 2] = {0};
    pBaseInfo->GetInfo(buf);
    
    return CMyFile::ModifyData(buf, offset, pBaseInfo->GetInfoSize());
}

//////////////////////////////////////////////////////////////////////////
//CSelectInfoFile相关
//////////////////////////////////////////////////////////////////////////
#if 0
CBaseInfo *
CSelectInfoFile::ReadData(tag_addrecord *paddrecord       //要读取的信息
                        ) const
{
    char pbuf[MAX_INPUT * 2] = {0};
    int nresult = CMyFile::ReadData(pbuf,
                                    paddrecord->ulstart,
                                    paddrecord->uchsize
                                    );    
    if (FAILED == nresult)
    {
        return NULL;
    }
    
    float fScore = 0.0f;
    memcpy(&fScore, pbuf + paddrecord->uchsize - sizeof(float), sizeof(float));

    CSelectInfo *pSelectInfo = new CSelectInfo;
    CBaseInfo *pBaseInfo = NULL;
    pBaseInfo = CBaseInfoFile::ReadData(paddrecord);
    if (NULL == pBaseInfo || NULL == pSelectInfo)
    {
        return NULL;
    }

    pSelectInfo->SetName(pBaseInfo->GetName());
    pSelectInfo->SetID(pBaseInfo->GetID());
    pSelectInfo->SetScore(fScore);

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }
    
    return pSelectInfo;    
}

int
CSelectInfoFile::AppendData(CBaseInfo *pSelectInfo,    //要添加的信息
                          unsigned long offset   //文件地址
                          )
{     
    return ModifyData(pSelectInfo, offset);
}

int
CSelectInfoFile::ModifyData(CBaseInfo *pSelectInfo,   
                          unsigned long offset                             
                          )
{
    const char *pszStudID = pSelectInfo->GetName();
    const char *pszCourseID = pSelectInfo->GetID();
    size_t studidsize = strlen(pszStudID) + 1;
    size_t courseidsize   = strlen(pszCourseID) + 1;
    
    //注意这里的转换
    CSelectInfo *pConvertInfo = (CSelectInfo *)pSelectInfo;
    float fScore = pConvertInfo->GetScore();
    
    CBaseInfoFile::ModifyData(pSelectInfo, offset);
    
    return CMyFile::ModifyData(&fScore,
                               offset + studidsize + courseidsize, 
                               sizeof(float)
                               );
}
#endif

//////////////////////////////////////////////////////////////////////////
//CInfoFile相关
//////////////////////////////////////////////////////////////////////////
CInfoFile::CInfoFile(const char *pszfile)
{
    SetFile(pszfile);
}

int
CInfoFile::ReadData(tag_student *pstudent, char *pszname, char *pszno,  
                    tag_addrecord *paddrecord                        
                    ) const
{
    char pbuf[MAX_INPUT * 2] = {0};
    int nresult = CMyFile::ReadData(pbuf,
                      paddrecord->ulstart,
                      paddrecord->uchsize);

    if (FAILED == nresult)
    {
        return nresult;
    }

    unsigned char uchsize    = 0;
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    
    uchsize     = paddrecord->uchsize;
    uchnamesize = paddrecord->uchnamesize;
    uchnosize   = uchsize - uchnamesize - sizeof(tag_student);

    memcpy(pstudent, pbuf, sizeof(tag_student));
    memcpy(pszname, pbuf + sizeof(tag_student), uchnamesize);
    memcpy(pszno, pbuf + sizeof(tag_student) + uchnamesize, uchnosize);

    return SUCCESS;    
}


int
CInfoFile::AppendData(tag_student *pstudent, char *pszname, char *pszno,   //学生信息
          unsigned long offset,                                         //文件地址
          unsigned char namesize,                                       //学生姓名占用空间大小，
          unsigned char nosize                                          // 
          )
{
    char buf[MAX_INPUT * 2] = {0};
    memcpy(buf, pstudent, sizeof(tag_student));
    memcpy(buf + sizeof(tag_student), pszname, namesize);
    memcpy(buf + sizeof(tag_student) + namesize, pszno, nosize);
    
    return CMyFile::ModifyData(buf, offset, sizeof(tag_student) + namesize + nosize);
}

int
CInfoFile::ModifyData(tag_student *pstudent, char *szname, char *szno,   
           unsigned long offset,                              
           unsigned char namesize,                          
           unsigned char nosize
              )
{
    char buf[MAX_INPUT * 2] = {0};
    memcpy(buf, pstudent, sizeof(tag_student));
    memcpy(buf + sizeof(tag_student), szname, namesize);
    memcpy(buf + sizeof(tag_student) + namesize, szno, nosize);
    
    return CMyFile::ModifyData(buf, offset, sizeof(tag_student) + namesize + nosize);
}

//////////////////////////////////////////////////////////////////////////
CAddTableFile::CAddTableFile(const char *pszfile)
{
    SetFile(pszfile);
}

CDelTableFile::CDelTableFile(const char *pszfile)
{
    SetFile(pszfile);
}

