#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "CStudManage.h"
#include "Fit.h"
#include "Exception.h"

CStudManage *
CStudManage::CreateSystem(void)
{
    static CStudManage *pobj = new CStudManage;
    return pobj;
}

int
CStudManage::Run(const char *pszinfofile, 
                 const char *pszaddtablefile, 
                 const char *pszdeltablefile)
{
    m_pinfofile = new CInfoFile;
    m_paddtablefile = new CAddTableFile;
    m_pdeltablefile = new CDelTableFile;

    m_paddtable = new CAddTable;
    m_pdeltable = new CDelTable;
    
    if (NULL == m_pinfofile
        || NULL == m_paddtablefile
        || NULL == m_pdeltablefile
        || NULL == m_paddtable
        || NULL == m_pdeltable)
    {
        return FAILED;
    }
    
    m_pinfofile->SetFile(pszinfofile);
    m_paddtablefile->SetFile(pszaddtablefile);
    m_pdeltablefile->SetFile(pszdeltablefile);

    int nresult = Init();
    if (FAILED == nresult)
    {
        return FAILED;
    }

    unsigned long nselect = 0;
    while (true)
    {
        CYourPanel::ShowMenu();
        
        CYourPanel::GetSelect(&nselect);
        
        if (0 == nselect)
        {
            break;
        }
        else if(nselect < 0 || nselect > 10) //change this according to the menu
        {
            CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
            continue;
        }
        
        Dispatch(nselect);
        
        //just for debug, can be removed
        ShowTable();
    }

    return SUCCESS;
}

void
CStudManage::End(void)
{
    Finish();

    delete this;
}

CStudManage::CStudManage()
{
    SetNULL();
}

void 
CStudManage::SetNULL(void)
{
    m_pinfofile = NULL;
    m_paddtablefile = NULL;
    m_pdeltablefile = NULL;
    
    m_paddtable = NULL;
    m_pdeltable = NULL;
}

CStudManage::~CStudManage()
{
#define DELETE_RES(pres)                         \
                        if ((pres) != NULL)      \
                        {                        \
                            delete (pres);       \
                            (pres) = NULL;       \
                        }                        
    
    DELETE_RES(m_pinfofile)
    DELETE_RES(m_paddtablefile)
    DELETE_RES(m_pdeltablefile)

    DELETE_RES(m_paddtable)
    DELETE_RES(m_pdeltable)
}

int
CStudManage::Finish(void)
{
    SaveTable();

    if (m_paddtable != NULL)
    {
        delete m_paddtable;
        m_paddtable = NULL;
    }

    if (m_pdeltable != NULL)
    {
        delete m_pdeltable;
        m_pdeltable = NULL;
    }

    return SUCCESS;
}

const CInfoFile *
CStudManage::GetInfoFile(void) const
{
    return m_pinfofile;
}

const CAddTableFile *
CStudManage::GetAddTableFile(void) const
{
    return m_paddtablefile;
}

const CDelTableFile *
CStudManage::GetDelTableFile(void) const
{
    return m_pdeltablefile;
}

CAddTable *
CStudManage::GetAddTable(void) const
{
    return m_paddtable;
}
    
CDelTable *
CStudManage::GetDelTable(void) const
{
    return m_pdeltable;
}

void
CStudManage::SaveTable(void)
{
    m_paddtable->SaveTable(m_paddtablefile->GetFile());
    m_pdeltable->SaveTable(m_pdeltablefile->GetFile());
}

int
CStudManage::Init(void)
{
    int result = SUCCESS;

    //DisableForceClose();

    try 
    {
        if (SUCCESS != CMyFile::CheckFile(m_pinfofile->GetFile()))
        {
            goto INIT_ERROR;
        }
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }
    
    try
    {
        result = m_paddtable->LoadTable(m_paddtablefile->GetFile());
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }
    
    if (SUCCESS != result)
    {
        //can do more error check for NOT_EXIST, FAILED
        goto INIT_ERROR;        
    }
    
    try
    {
        result = m_pdeltable->LoadTable(m_pdeltablefile->GetFile());
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }
    
    if (SUCCESS != result)
    {
        goto INIT_ERROR;
    }
    
INIT_ERROR:
    if (result != SUCCESS)
    {
        result = FAILED;
    }
    
    return result;
}

int 
CStudManage::InfoAdd(tag_student *pstudent, char *pszname, char *pszno)
{
    unsigned long ulstart     = 0;      //学生信息在文件中的偏移
    unsigned char uchsize     = 0;      //学生信息占用的总空间
    unsigned char uchnamesize = 0;      //学生姓名占用的空间
    unsigned char uchnosize   = 0;      //学生学号占用的空间
    unsigned char uchtmpsize  = 0;      //从g_del_table中查询到的符号条件空间的大小
    
    tag_addrecord addrecord = {0};
    tag_delrecord delrecord = {0};
    
    int index  = 0;
    int result = SUCCESS;
    
    //also store the '\0'
    uchnamesize = strlen(pszname) + 1;
    uchnosize   = strlen(pszno) +1;
    uchsize     = sizeof(tag_student) + uchnamesize + uchnosize;
    
    //使用g_del_table, 对已删除空间再利用，所以优先查询g_del_table
    result = GetSize(uchsize, &index, this);  
    if (NOT_EXIST == result)
    {
        try 
        {
            GetFileSize(&ulstart, this);
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        
        try 
        {
            result = m_pinfofile->AppendData(pstudent, pszname, pszno,
                                        ulstart,             
                                        uchnamesize,
                                        uchnosize
                                        );
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        
        if (FAILED == result)
        {
            return result;
        }
        
        addrecord.ulstart     = ulstart;
        addrecord.uchsize     = uchsize;
        addrecord.uchnamesize = uchnamesize;
        m_paddtable->InsertIntoTable(&addrecord);                            
        
    }//end for if(NOT_EXIST == result)
    else
    {     
        tag_delrecord *pdelrecords = m_pdeltable->GetRecords();
        uchtmpsize  = pdelrecords[index].uchsize;
        ulstart     = pdelrecords[index].ulstart;
        
        try 
        {
            result = m_pinfofile->AppendData(pstudent, pszname, pszno,
                                        ulstart,
                                        uchnamesize,
                                        uchnosize);
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        
        if (FAILED == result)
        {
            return result;
        }
        
        if (uchtmpsize == uchsize)
        {
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            m_pdeltable->RemoveFromTable(index);
            m_paddtable->InsertIntoTable(&addrecord); //注意参数
        }
        else
        {
            delrecord.uchsize  = uchtmpsize - uchsize;
            delrecord.ulstart  =  ulstart + uchsize;
            m_pdeltable->UpdateTable(index, &delrecord);
            
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            m_paddtable->InsertIntoTable(&addrecord); //注意参数
        }
    }//end for NOT_EXIST != result
    
    //This is not a good idea using a var to represent more than one meaning.
    result = addrecord.ulstart;
    return result;
}

int
CStudManage::InfoQueryAbout(tag_student *student, char *szname, char *szno,   
               unsigned long infoid,                                  
               int *indexary,                                     
               int *num,                                           
               tag_newinfo *flag
                    )
{
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    unsigned long nrecords    = 0;
    
    tag_addrecord *paddrecord = NULL;
    tag_student tmpstudent    = {0};
    char sztmpname[MAX_INPUT] = {0};
    char sztmpno[MAX_INPUT]   = {0};
    
    FILE *fp   = NULL;
    int result = NOT_EXIST;
    unsigned long i = 0;
    
    if (1 == flag->name)
    {
        uchnamesize = strlen(szname) + 1;
    }
    
    if (1 == flag->no)
    {
        uchnosize = strlen(szno) + 1;
    }
    
    *num = 0;   //not a good idea to use the param

    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    nrecords = m_paddtable->GetRecordsNum();
    for (i = 0; i < nrecords; i++)
    {
        //对多字段进行联合查询
        
        if (uchnamesize > paddrecords[i].uchnamesize
            || uchnosize > (paddrecords[i].uchsize 
                            - paddrecords[i].uchnamesize
                             - sizeof(tag_student))
            )
        {
            continue;
        }
        
        paddrecord = &paddrecords[i];
        try
        {
            result = m_pinfofile->ReadData(&tmpstudent, sztmpname, sztmpno,
                                    paddrecord
                                    );
        }
        catch(CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }

        if (FAILED == result)
        {
            return FAILED;
        }
        
        if (uchnamesize > 1
            && (NULL == strstr(sztmpname, szname))
            )
        {
            continue;
        }
        
        if (uchnosize > 1
            && (NULL == strstr(sztmpno, szno))
            )
        {
            continue;
        }
        
        if (1 == flag->gender
            && student->chgender != tmpstudent.chgender)
        {
            continue;
        }
        
        if (1 == flag->score
            && tmpstudent.fscore < student->fscore)
        {
            continue;
        }
        
        if (1 == flag->birth
            && tmpstudent.ushbirth < student->ushbirth)
        {
            continue;
        }
        
        if (1 == flag->infoid
            && infoid != i)
        {
            continue;
        }
        
        *(indexary + *num) = i;
        *num += 1;       
    }
    
    if (0 != *num)
    {
        result = SUCCESS;
    }
    
    return result;
}

int 
CStudManage::InfoQueryById(unsigned long infoid)
{
    unsigned long nrecords = 0;
    
    nrecords = m_paddtable->GetRecordsNum();
    if (infoid >= 0 && infoid < nrecords)
    {
        return SUCCESS;
    }
    else
    {
        return NOT_EXIST;
    }   
}

int
CStudManage::InfoDel(int infoid)
{
    tag_delrecord delrecord = {0};
    unsigned char uchsize = 0;
    unsigned long ulstart = 0;
    
    tag_student student    = {0};
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]   = {0};
 
    int result = SUCCESS;
    
    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    uchsize  = paddrecords[infoid].uchsize;
    ulstart = paddrecords[infoid].ulstart;
    
    delrecord.uchsize = uchsize;
    delrecord.ulstart = ulstart;
    
    m_pdeltable->InserIntoTable(&delrecord); 
    m_paddtable->RemoveFromTable(infoid);     
    
    return result;
}

int
CStudManage::InfoModify(tag_student *student, char *szname, char *szno, int infoid,
                        tag_newinfo *flag)
{
    tag_delrecord delrecord  = {0};
    tag_addrecord addrecord  = {0};
    unsigned char uchnewsize = 0;
    unsigned char ucholdsize = 0;
    unsigned char uchsize    = 0;
    unsigned char uchnamesize = 0;
    unsigned char uchnosize   = 0;
    unsigned long ulstart   = 0;
    
    tag_addrecord *paddrecord = NULL;
    tag_student tmpstudent    = {0};
    char sztmpname[MAX_INPUT] = {0};
    char sztmpno[MAX_INPUT]   = {0};
    
    int result = SUCCESS; 
    
    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    ulstart     = paddrecords[infoid].ulstart; 
    
    //get the old info first
    paddrecord = &paddrecords[infoid];

    try 
    {
        result = m_pinfofile->ReadData(&tmpstudent, sztmpname, sztmpno, 
                                    paddrecord
                                    );
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }

    if (FAILED == result)
    {
        //showinfo("info_modify fetch1info error\r\n");
        return result;
    }

    ucholdsize = paddrecords[infoid].uchsize;
    uchnewsize = sizeof(tag_student);  //to be updated

    uchnamesize = paddrecords[infoid].uchnamesize;  //to be updated, maybe
    uchnosize   = ucholdsize - uchnamesize - sizeof(tag_student);

    //new info for gender
    if (1 == flag->gender)
    {
        tmpstudent.chgender = student->chgender; 
    }

    if (1 == flag->birth)
    {
        tmpstudent.ushbirth = student->ushbirth;
    }

    if (1 == flag->score)
    {
        tmpstudent.fscore = student->fscore;
    }
      
    //we will write sztmpname
    if (1 == flag->name)
    {
        strcpy(sztmpname, szname);
        uchnamesize = strlen(sztmpname) + 1;
    }

    uchnewsize += uchnamesize;

    if (1 == flag->no)
    {
        strcpy(sztmpno, szno);
        uchnosize = strlen(sztmpno) + 1;
    }

    uchnewsize += uchnosize;
    
    // this will generate unused space, we need to update g_add_table and g_del_table;
    if (uchnewsize < ucholdsize)
    {
        try 
        {
            result = m_pinfofile->ModifyData(&tmpstudent, sztmpname, sztmpno,
                                        ulstart,
                                        uchnamesize,
                                        uchnosize
                                        );
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
                                   
        if (FAILED == result)
        {
            return result;
        }
        
        addrecord.uchsize  = uchnewsize;
        addrecord.ulstart = ulstart;
        addrecord.uchnamesize = uchnamesize;
        m_paddtable->UpdateTable(infoid, &addrecord);
        
        delrecord.uchsize  = ucholdsize - uchnewsize;
        delrecord.ulstart = ulstart + uchnewsize;
        m_pdeltable->InserIntoTable(&delrecord);  //注意参数
    }//end for if (uchnewsize < ucholdsize)
    else if (uchnewsize == ucholdsize)
    {
        try 
        {   
            result = m_pinfofile->ModifyData(&tmpstudent, sztmpname, sztmpno,
                                        ulstart,
                                        uchnamesize,
                                        uchnosize
                                        );
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        
        if (FAILED == result)
        {
            return result;
        }
    }//end for if (uchnewsize == ucholdsize)
    else
    {
        InfoDel(infoid);
        result = InfoAdd(&tmpstudent, sztmpname, sztmpno);
    }//end for if (uchnewsize > ucholdsize)    
    
    return result;
}

void
CStudManage::Display(void)
{
    char szhitinfo[MAX_INPUT * 2] = {0};
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_addrecord *paddrecord = NULL;
    
    unsigned long ulstart = 0;
    unsigned long nrecords = 0;
    int result   = SUCCESS;
    unsigned long i = 0;
    
    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    nrecords = m_paddtable->GetRecordsNum(); 
        
    CYourPanel::ShowInfo("id   name   no   gender   birthday   score\r\n");
    for (i = 0; i < nrecords; i++)
    {
        ulstart = paddrecords[i].ulstart;
        
        paddrecord = &paddrecords[i];
        result = m_pinfofile->ReadData(&student, szname, szno, 
                                        paddrecord);
        if (FAILED == result)
        {
            CYourPanel::ShowInfo("Display fetch1info error\r\n");
        }
        
        sprintf(szhitinfo, "    start at: %d\r\n", ulstart);
        CYourPanel::ShowInfo(szhitinfo);
        
        sprintf(szhitinfo, "%d   %s   %s   %c   %d   %f\r\n",
                i,
                szname,
                szno,
                student.chgender,
                student.ushbirth,
                student.fscore
                );
        CYourPanel::ShowInfo(szhitinfo);
    }
}

/*
Function: 显示buf[index, index+1, .. index+len-1]间的字符
*/
void
showbuf(char *buf, int index, int len,
        void (*pfnshowinfo)(const char *))
{
    char szhitinfo[8] = {0};
    int i;
    
    //ugly
    if (index < 0)
    {
        index = 0;
    }
    
    for (i = index; i < index + len; i++)
    {
        if ('\0' == buf[i])
        {
            pfnshowinfo("\\0 ");
        }
        else
        {
            sprintf(szhitinfo, "%02X ", (unsigned char)buf[i]);  
            pfnshowinfo(szhitinfo);
        }
    }
}

int
space_stat(unsigned long startaddr, unsigned long size, //连续地址空间信息
           int relativeaddrflg,     //相对 or 绝对
           const char *szfile,            //为文件相对地址时，指定文件 
           const CStudManage *pstudmanage,
           void (*pfnshowinfo)(const char *)
           )
{
    char szhitinfo[32] = {0};
    char buf[SPLIT * 2] = {0};
    unsigned long index_add = 0;   //添加表下标
    unsigned long index_del = 0;   //删除表下标
    unsigned long index_buf = 0;   //地址空间偏移
    unsigned long readcount = 0;   //每SPLIT字节读取的次数
    unsigned char readremain = 0;  //不足SPLIT字节读取的剩余字节
    unsigned long max       = 0;    

    int col       = 0;    
    int table     = 0;
    int start     = 0;
    int len       = 0;
    int remain    = 0;
    int i         = 0;
    char flag     = '\0'; //F or U
    int result    = SUCCESS;

    CMyFile fileobj(szfile);

    table     = ADD;
    max       = 0; 
    
    readcount = size / (SPLIT);
    readremain = (unsigned char)(size % (SPLIT));

    for (index_buf = startaddr; index_buf < startaddr + size; index_buf++)
    {
        //每行开始读取buf
        if (0 == col % SPLIT)
        {
            col = 0;

            if (0 == readcount)
            {
                try 
                {
                    result = fileobj.ReadData(buf, index_buf, readremain);
                }
                catch (CMyFileError &e)
                {
                    e.WriteLog(__FILE__, __LINE__);
                }
            }
            else
            {
                //每SPLIT字节读取
                try 
                {
                    result = fileobj.ReadData(buf, index_buf, SPLIT);
                }
                catch (CMyFileError &e)
                {
                    e.WriteLog(__FILE__, __LINE__);
                }
                if (SUCCESS == result)
                {
                    readcount--;
                }
            }

            if (SUCCESS != result)
            {
                return result;
            }           
        }//end for if (0 == col % (SPLIT))        

        //this index belong to which one? g_add_table, g_del_table ?
        if (index_buf >= max)
        {
            table = gettable(index_buf, &max, index_add, index_del, pstudmanage);  
            
            if (ADD == table)
            {
                //not a good habit to reuse the param
                index_add++;
            }
            else
            {
                index_del++;
            }
        }//end for if (index_buf >= max)
        else
        {
            //flag keeps the same until max
            //so no need to gettable again.
        }        

        if (ADD == table)       //used
        {
            flag = 'U';
        }
        else if (DEL == table)  //free
        {
            flag = 'F';
        }
        else
        {
            //sth wrong?
        }

        //输出开头的偏移
        if (0 == col % (SPLIT))
        {
            sprintf(szhitinfo, "%p: ", index_buf); 
            pfnshowinfo(szhitinfo);
        }
       
        //输出各空间的状态标志
        sprintf(szhitinfo, "%c ", flag);
        pfnshowinfo(szhitinfo);
  
        //输出各空间相应的字符
        if ((col != 0)
            &&(0 == (col + 1) % (SPLIT)) 
            )
        {
            pfnshowinfo("    ");

            start = 0;
            len   = (SPLIT);
            showbuf(buf, start, len, pfnshowinfo);

            pfnshowinfo("\r\n");
        }
        col += 1;
    } //end for ( ; index_buf < size; index_buf++)
    
    //对最后不足一个SPLIT的处理
    remain = (SPLIT) - col % (SPLIT);
    for (i = 0; i < remain; i++)
    {
        pfnshowinfo("  ");
    }
    pfnshowinfo("  ");
    
    len   = (SPLIT) - remain;
    start = 0;
    
    showbuf(buf, start, len, pfnshowinfo);
    
    return result;
}

int
space_stat_for_file(const char *pszfile, 
                    const CStudManage *pstudmanage, //用于传输所需的管理系统信息
                    void (*pfnshowinfo)(const char *))
{
    unsigned long startaddr = 0; //
    unsigned long filesize = 0;

    int result = SUCCESS;

    try 
    {
        result = GetFileSize(&filesize, pstudmanage);
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }

    if (FAILED == result)
    {
        return result;
    }

    result = space_stat(startaddr, filesize, RELATIVE, pszfile, pstudmanage, pfnshowinfo);
    if (FAILED == result)
    {
        return result;
    }

    return result;
}

void
CStudManage::SpaceStat()
{
    space_stat_for_file(m_pinfofile->GetFile(), this, CYourPanel::ShowInfo);
}

int
moveforward(int index, unsigned char step, const CStudManage *pstudmanage)
{
    char strinfobuf[MAX_INPUT * 2] = {0};
    unsigned long  nrecords = 0;
    unsigned long ulstart = 0;
    unsigned char uchsize = 0;
    
    int result = 0;
    unsigned long i = 0;

    CAddTable *paddtable = pstudmanage->GetAddTable();
    tag_addrecord *paddrecords = paddtable->GetRecords();

    const CInfoFile *pinfofile = pstudmanage->GetInfoFile();
    
    nrecords = paddtable->GetRecordsNum();
    for (i = index; i <= nrecords; i++)
    {
        ulstart = paddrecords[i - 1].ulstart;
        uchsize = paddrecords[i - 1].uchsize;
        
        const CMyFile *pfile = pinfofile;

        try 
        {
            result = pfile->ReadData(strinfobuf, ulstart, uchsize);
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        if (FAILED == result)
        {
            //showinfo("moveforward fetch error\r\n");
            return result;
        }
        
        //move from ulstart to ulstart - step
        try
        {
            result = pfile->ModifyData(strinfobuf, ulstart - step, uchsize);
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }

        if (FAILED == result)
        {
            //showinfo("moveforward store error\r\n");
            return result;
        }
    }
    
    //now need to retail the file size
    //TODO
    
    return result;
}

void
CStudManage::Defrag(void)
{
    unsigned char size_del = 0;
    unsigned long start_del = 0;
    unsigned char size_add = 0;
    unsigned long start_add = 0;    
    int nrecords_add = 0;
    unsigned long i = 0, j = 0, k = 0;
    
    nrecords_add = m_paddtable->GetRecordsNum();
    i            = m_pdeltable->GetRecordsNum(); 

    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    tag_delrecord *pdelrecords = m_pdeltable->GetRecords();
    
    //walk through del_table
    for ( ;i >= 1; i--)
    {
        size_del  = pdelrecords[i - 1].uchsize;
        start_del = pdelrecords[i - 1].ulstart;        
        
        //find the first proper index (size, start) in add_table which just follows the del record
        for (j = nrecords_add ; j >= 1; j--)
        {
            size_add  = paddrecords[j - 1].uchsize;
            start_add = paddrecords[j - 1].ulstart;
            
            if (start_add == start_del + size_del)
            {
                //we've already gotten the index
                break;
            }
        }
        
        //now need to move info forward
        moveforward(j, size_del, this);
        
        //remove (size_del, start_del) from del_table
        pdelrecords[i - 1].uchsize  = 0; //size
        pdelrecords[i - 1].ulstart = 0; //start
        m_pdeltable->SetRecordsNum(m_pdeltable->GetRecordsNum() - 1);
        
        //update (size, start) in add_table to (size, start - size_del)
        for (k = nrecords_add; (j >= 1) && (k >= j); k--)
        {
            paddrecords[k - 1].ulstart = paddrecords[k - 1].ulstart - size_del;            
        }           
    }

    SaveTable();
}

//////////////////////////////////////////////////////////////////////////
//流程性处理
//////////////////////////////////////////////////////////////////////////
void
CStudManage::Dispatch(int nselect)
{
    char szhitinfo[MAX_INPUT] = {0};
    float fscore = 0.0f;
    int result = 0;
    
    switch (nselect)
    {
    default:
        {
            CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
            break;
        }
        
    case 1:
        {
            ProcessAdd();
            break;
        }
        
    case 2:
        {
            ProcessQuery();
            break;
        }
        
    case 3:
        {
            ProcessDel();
            break;
        }
        
    case 4:
        {
            ProcessModify();
            break;
        }
        
    case 5:
        {
            //display:
            CYourPanel::ShowInfo("Displaying......\r\n");
            
            Display();
            break;
        }
        
    case 6:
        {
            //statistics
            CYourPanel::ShowInfo("Statisticsing......U for Used, F for Free\r\n\r\n");
            
            SpaceStat();
            break;
        }
        
    case 7:
        {
            ProcessFit();
            break;
        }
        
    case 8:
        {
            Defrag();            
            break;
        }
        
    case 9:
        {
            InfoStat();
            break;
        }
        
    case 10:
        {
            result = GetAverScore(&fscore);
            if (SUCCESS == result)
            {
                sprintf(szhitinfo, "averscore is %f\r\n", fscore);
                CYourPanel::ShowInfo(szhitinfo);
            }
            
            break;
        }
    }//end for switch
}

void
CStudManage::ProcessAdd(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    char           chgender   = 'F';    //
    unsigned short ushbirth   = 1212;   //mmdd
    float          fscore     = 0.0f;
    tag_student student       = {0};
    int result                = 0;     //ugly
    
    //Add
    CYourPanel::GetInput(szname, "Enter Name:   ");
    CYourPanel::GetInput(szno,   "Enter No  :   ");
    CYourPanel::GetGender(&chgender, "Enter Gender:   ");
    CYourPanel::GetBirthday(&ushbirth, "Enter Birthday(mmdd):   ");
    CYourPanel::GetScore(&fscore, "Enter Score:   ");
    
    student.chgender = chgender;
    student.ushbirth = ushbirth;
    student.fscore   = fscore;    
    
    result = InfoAdd(&student, szname, szno);
    if (NO_SPACE == result)
    {
        CYourPanel::ShowInfo("Add failed, for No More Space\r\n");
    }
    else
    {
        //not a good habit to reuse result for different meanning.
        sprintf(szhitinfo, "\r\nADD: store into %d\r\n", result);
        CYourPanel::ShowInfo(szhitinfo);
    }
    
    SaveTable();
}

void
CStudManage::ProcessQuery(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_newinfo flag          = {0};
    tag_addrecord *paddrecord = NULL;
    unsigned long infoid      = 0;
    
    int result                = 0;     //ugly
    int ch = 0;    
    int i  = 0;
    
    int indexary[MAX_INPUT]   = {0};    //接收查询到的编号
    int num                   = 0;      //查询匹配到的总数目
    int index                 = 0;
    char szqueryed[MAX_INPUT*2] = {0};
    
    //Query
    CYourPanel::ShowInfo("支持对学生基本信息、姓名、学号和编号的模糊查询、联合查询\r\n");
    CYourPanel::GetNewInfo(&student, szname, szno, &infoid, QUERY, &flag);
    
    if (0 == *(unsigned char*)(&flag))
    {
        return;
    }
    
    result = InfoQueryAbout(&student, szname, szno,
        infoid,
        indexary,
        &num,
        &flag
        );
    
    if (NOT_EXIST == result)
    {
        CYourPanel::ShowInfo("Query: not exist\r\n");
        return;
    }
    
    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    const CInfoFile *pinfofile = m_pinfofile;

    //模糊查询结果输出
    CYourPanel::ShowInfo("Queryed: id  name     no      gender birthday  score \r\n");
    for (i = 0; i < num; i++)
    {
        //查询到的字符串的(size, start)记录在g_add_table中的下标
        index = indexary[i];    
        
        paddrecord = &paddrecords[index];
        result = pinfofile->ReadData(&student, szname, szno, 
            paddrecord);
        
        if (FAILED == result)
        {
            CYourPanel::ShowInfo("process_query fetch1info error");
        }
        
        CYourPanel::Show1Info(&student, szname, szno, infoid);
    }
}

int
CStudManage::ProcessDel(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    unsigned long infoid      = 0;  
    int result                = 0;
    int ch                    = 0;
    
    tag_addrecord *paddrecord = NULL;
    tag_student student    = {0};
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]   = {0};
    
    CYourPanel::ShowInfo("仅能删除具体编号的学生信息，请根据模糊查询确认编号\r\n");
    CYourPanel::ShowInfo("请输入编号：");
    CYourPanel::GetSelect(&infoid);
    
    result = InfoQueryById(infoid);   
    if (NOT_EXIST == result)
    {
        sprintf(szhitinfo, "%d does not exist\r\n", infoid);
        CYourPanel::ShowInfo(szhitinfo);
        return result;
    }
    
    paddrecord = &(m_paddtable->GetRecords())[infoid];
    result = m_pinfofile->ReadData(&student, szname, szno, 
                                    paddrecord
                                   );
    if (FAILED == result)
    {
        CYourPanel::ShowInfo("info_del fetch1info error\r\n");
        return result;
    }
    
    CYourPanel::ShowInfo("queryed:  id  name   no    gender  birthday  score\r\n");
    CYourPanel::Show1Info(&student, szname, szno, infoid);
    
    CYourPanel::ShowInfo("Are you sure to delete this one? (Y/N) >>");
    if (!CYourPanel::IsSure())
    {
        return result;
    }
    
    result = InfoDel(infoid); 
    CYourPanel::ShowInfo("Del ok\r\n");
    
    SaveTable();
    
    return result;
}

int
CStudManage::ProcessModify(void)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_newinfo flag          = {0};
    tag_addrecord *paddrecord = NULL;
    unsigned long infoid      = 0;
    int result                = SUCCESS;     //ugly
    int ch = 0;    
    int i  = 0;
    
    int indexary[MAX_INPUT]   = {0};    //接收查询到的编号
    int num                   = 0;      //查询匹配到的总数目
    int index                 = 0;
    char szqueryed[MAX_INPUT*2] = {0};
    
    CYourPanel::ShowInfo("仅能修改指定编号的学生信息，可通过模糊查询确认编号\r\n");
    CYourPanel::ShowInfo("请输入具体编号: ");
    CYourPanel::GetSelect(&infoid);
    
    result = InfoQueryById(infoid);
    if (NOT_EXIST == result)
    {
        CYourPanel::ShowInfo("NOT EXIST\r\n");
        return result;
    }
    
    tag_addrecord *paddrecords = m_paddtable->GetRecords();
    const CInfoFile *pinfofile = m_pinfofile;
    paddrecord = &paddrecords[infoid];
    result = pinfofile->ReadData(&student, szname, szno, 
                                paddrecord
                                );
    if (FAILED == result)
    {
        CYourPanel::ShowInfo("Process_modify fetch1info error\r\n");
        return result;
    }
    
    CYourPanel::ShowInfo("queryed: id  name   no  gender birthday score\r\n");
    CYourPanel::Show1Info(&student, szname, szno, infoid);
    
    CYourPanel::ShowInfo("Are you sure to modify? (Y/N) >>");
    if (!CYourPanel::IsSure())
    {
        return result;
    }      
    
    //Modify
    CYourPanel::ShowInfo("支持对学生基本信息、姓名、学号修改\r\n");
    CYourPanel::GetNewInfo(&student, szname, szno, &infoid, MODIFY, &flag);
    
    if (0 == *(unsigned char *)&flag)
    {
        return SUCCESS;
    }
    
    result = InfoModify(&student, szname, szno, infoid, &flag);
    if (NOT_EXIST == result)
    {
        sprintf(szhitinfo, "Modify: %d not exist\r\n", infoid);
        CYourPanel::ShowInfo(szhitinfo);
    }
    else if (NO_SPACE == result)
    {
        CYourPanel::ShowInfo("Modify: no more space to store newinput\r\n");
    }
    else
    {
        CYourPanel::ShowInfo("Modify ok\r\n");
    }
    
    SaveTable();
    
    return result;
}

void
CStudManage::ProcessFit(void)
{
    unsigned long nselect = 0;
    
    //set fit
    CYourPanel::ShowInfo("Please choose the fit way\r\n");    
    CYourPanel::ShowFit();    
    CYourPanel::GetSelect(&nselect);
    
    if (nselect < FIT_EQUAL || nselect > FIT_EQUAL_FIRST )  //change accordingly
    {
        CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
    }
    else
    {
        SetFit(nselect);
    }
}

void
CStudManage::ShowTable(void)
{
    char szhitinfo[64] = {0};
    unsigned long nrecords = 0;
    unsigned long i = 0;
    
    CYourPanel::ShowInfo("\r\n[ Debug ]showing g_add_table (start, size) ......\r\n");    
    CYourPanel::ShowInfo("g_add_table:\r\n");

    CAddTable *paddtable = m_paddtable;
    tag_addrecord *paddrecords = paddtable->GetRecords();
    
    nrecords = paddtable->GetRecordsNum();
    for (i = 0; i < nrecords; i++)
    {
        //the start is relative to the same base
        sprintf(szhitinfo, "(%d %d) ", paddrecords[i].ulstart, 
                paddrecords[i].uchsize);
        CYourPanel::ShowInfo(szhitinfo);
    }
    CYourPanel::ShowInfo("\r\n\r\n");
    
    CYourPanel::ShowInfo("g_del_table:\r\n");

    CDelTable *pdeltable = m_pdeltable;
    tag_delrecord *pdelrecords = pdeltable->GetRecords();

    nrecords = pdeltable->GetRecordsNum();
    for (i = 0; i < nrecords; i++)
    {
        sprintf(szhitinfo, "(%d %d) ", pdelrecords[i].ulstart, 
                        pdelrecords[i].uchsize);
        CYourPanel::ShowInfo(szhitinfo);
    }
    CYourPanel::ShowInfo("\r\n\r\n"); 
}

void
CStudManage::InfoStat(void)
{
    char szhitinfo[MAX_INPUT] = {0};
    unsigned char uchsize  = 0;
    unsigned long nrecords = 0;
    unsigned long filesize = 0; 
    int  result   = SUCCESS;
    unsigned long i = 0;
    
    try 
    {
        result = GetFileSize(&filesize, this);
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }

    if (FAILED == result)
    {
        CYourPanel::ShowInfo("info_stat getfilesize error");
        return;
    }
    
    CYourPanel::ShowInfo("id: 占用大小  占比\r\n");
    
    CAddTable *paddtable = m_paddtable;
    tag_addrecord *paddrecords = paddtable->GetRecords();
    
    nrecords = paddtable->GetRecordsNum();
    for (i = 0; i < nrecords; i++)
    {
        uchsize = paddrecords[i].uchsize;
        sprintf(szhitinfo, "%d: %d  %f\r\n", i, uchsize, uchsize * 1.0f/filesize);
        CYourPanel::ShowInfo(szhitinfo);
    } 
}

int
CStudManage::GetAverScore(float *fscore)
{
    unsigned long nrecords = 0;
    
    tag_student student = {0};
    tag_addrecord *paddrecord = NULL;
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]    = {0};
    
    float totalscore = 0.0;
    float averscore  = 0.0;
    int result = SUCCESS;
    unsigned long i = 0;
    
    CAddTable *paddtable = m_paddtable;
    tag_addrecord *paddrecords = paddtable->GetRecords();
    
    const CInfoFile *pinfofile = m_pinfofile;
    
    nrecords = paddtable->GetRecordsNum();
    for (i = 0; i < nrecords; i++)
    {
        paddrecord = &paddrecords[i];
        result = pinfofile->ReadData(&student, szname, szno,
                                    paddrecord);
        if (FAILED == result)
        {
            CYourPanel::ShowInfo("process_averscore fetch1info error");
            return result;
        }
        
        totalscore += student.fscore;
    }
    
    if (nrecords)
    {
        averscore = totalscore / nrecords;
        
        *fscore = averscore;
    }
    
    return result;
}

int
GetFileSize(unsigned long *filesize, const CStudManage *pstudmanage)
{
    FILE *fp    = NULL;
    unsigned long addoffset   = 0;
    unsigned long deloffset   = 0;
    long size   = 0;
    long offset = 0;
    int result  = SUCCESS;
    
    fp = fopen(pstudmanage->GetInfoFile()->GetFile(), "rb");
    if (NULL == fp)
    {
        result = FAILED;

    #ifdef USE_EXCEPTION
        throw CMyFileError("GetFileSize FAILED, NOT EXIST: ", pstudmanage->GetInfoFile()->GetFile());
    #else
        return result;
    #endif
    }
    
    //因defrag，未进行truncate, 所以利用addtable和deltable最后一项纪录确认文件结束
    CAddTable *paddtable = pstudmanage->GetAddTable();
    CDelTable *pdeltable = pstudmanage->GetDelTable();
    tag_addrecord *paddrecords = paddtable->GetRecords();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();
    unsigned long naddrecords = paddtable->GetRecordsNum();
    unsigned long ndelrecords = pdeltable->GetRecordsNum();
    if (0 != naddrecords)
    {
        addoffset = paddrecords[naddrecords- 1].ulstart
            + paddrecords[naddrecords - 1].uchsize;
        
    }
    
    if (0 != ndelrecords)
    {
        deloffset = pdelrecords[ndelrecords - 1].ulstart
            + pdelrecords[ndelrecords - 1].uchsize;    
    }
    //取大者
    offset = (addoffset > deloffset ? addoffset : deloffset);       
    
    
    fseek(fp, offset, SEEK_CUR);
    if (ferror(fp))
    {
        perror("Getfilesize fseek error");
        
        result = FAILED;

    #ifdef USE_EXCEPTION
        throw CMyFileError("GetFileSize FAILED, FSEEK ERROR: ", pstudmanage->GetInfoFile()->GetFile());
    #else
        goto GETFILESIZE_ERROR;
    #endif
    }
    
    size = ftell(fp);
    if (ferror(fp))
    {
        perror("Getfilesize ftell error");
        
        result = FAILED;
        
    #ifdef USE_EXCEPTION
        throw CMyFileError("GetFileSize FAILED, FTELL ERROR: ", pstudmanage->GetInfoFile()->GetFile());
    #else
        goto GETFILESIZE_ERROR;
    #endif
    }
    
    *filesize = size;
    
GETFILESIZE_ERROR:
    if (fp)
    {
        fclose(fp);
        fp = NULL;
    }
    
    return result;
}

int
gettable(unsigned long index, unsigned long *max, unsigned long index_add, unsigned long index_del,
         const CStudManage *pstudmanage)
{
    unsigned char size_add  = 0;
    unsigned long start_add = 0;
    unsigned char size_del  = 0;
    unsigned long start_del = 0;
    int table               = 0;
    
    CAddTable *paddtable = pstudmanage->GetAddTable();
    tag_addrecord *paddrecords = paddtable->GetRecords();
    
    if (index_add != paddtable->GetRecordsNum())
    {
        size_add  = paddrecords[index_add].uchsize;
        start_add = paddrecords[index_add].ulstart;
    }
    
    CDelTable *pdeltable = pstudmanage->GetDelTable();
    tag_delrecord *pdelrecords = pdeltable->GetRecords();
    
    if (index_del != pdeltable->GetRecordsNum())
    {
        size_del  = pdelrecords[index_del].uchsize;
        start_del = pdelrecords[index_del].ulstart;
    }
    
    if ((index >= start_add) && (index < start_add + size_add))
    {
        *max = start_add + size_add;
        return ADD;
    }
    
    if ((index >= start_del) && (index <= start_del + size_del))
    {
        *max = start_del + size_del;
        return DEL;
    }
    
    //should not come to this point.
    return table;
}


