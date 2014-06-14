#include "ui.h"
#include "Exception.h"
#include "CManage.h"
#include "Fit.h"

CManage *
CManage::CreateSystem(void)
{
    static CManage *pobj = new CManage;
    return pobj;
}

int
CManage::Run(void)
{    
    int nresult = Init(&m_SelectInfoManage);
    if (FAILED == nresult)
    {
        return FAILED;
    }

    nresult = Init(&m_CourseInfoManage);
    nresult = Init(&m_TeacherInfoManage);
    nresult = Init(&m_StudInfoManage);
    
    
    unsigned long nselect = 0;
    while (true)
    {
        CYourPanel::ShowMenu_Main();
        
        CYourPanel::GetSelect(&nselect);
        
        if (0 == nselect)
        {
            break;
        }
        else if(nselect < 0 || nselect > 20) //change this according to the menu
        {
            CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
            continue;
        }
        
        Dispatch(nselect);
        
        CYourPanel::Continue();
    }
    
    return SUCCESS;
}

void
CManage::End(void)
{   
    delete this;
}

int
CManage::Init(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[MAX_INPUT] = {0};
    int result = SUCCESS;
    
    //DisableForceClose();

    //信息文件
    sprintf(szhitinfo, "Checking %s ......\r\n", pBaseInfoManage->GetInfoFile().GetFile());
    CYourPanel::ShowInfo(szhitinfo);
    
    try 
    {
        if (SUCCESS != CMyFile::CheckFile(pBaseInfoManage->GetInfoFile().GetFile()))
        {
            goto INIT_ERROR;
        }
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }
    
    //添加表
    sprintf(szhitinfo, "Loading %s ......\r\n", pBaseInfoManage->GetAddTableFile().GetFile());
    CYourPanel::ShowInfo(szhitinfo);

    try
    {
        pBaseInfoManage->GetAddTable() = new CAddTable;
        result = pBaseInfoManage->GetAddTable()->LoadTable(pBaseInfoManage->GetAddTableFile().GetFile());
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
    
    //删除表
    sprintf(szhitinfo, "Loading %s ......\r\n", pBaseInfoManage->GetDelTableFile().GetFile());
    CYourPanel::ShowInfo(szhitinfo);

    try
    {
        pBaseInfoManage->GetDelTable() = new CDelTable;
        result = pBaseInfoManage->GetDelTable()->LoadTable(pBaseInfoManage->GetDelTableFile().GetFile());
    }
    catch (CMyFileError &e)
    {
        e.WriteLog(__FILE__, __LINE__);
    }

    if (SUCCESS != result)
    {
        goto INIT_ERROR;
    }

    //IDTree,NameTree的加载
    sprintf(szhitinfo, "Loading NameTree, IDTree ......\r\n\r\n");
    CYourPanel::ShowInfo(szhitinfo);

    pBaseInfoManage->LoadTree();
    
INIT_ERROR:
    if (result != SUCCESS)
    {
        result = FAILED;
    }
    
    return result;
}

//////////////////////////////////////////////////////////////////////////
//流程性处理
//////////////////////////////////////////////////////////////////////////
void
CManage::Dispatch(unsigned long nselect)
{
    char szhitinfo[MAX_INPUT] = {0};
    float fscore = 0.0f;
    int result = 0;

    CBaseInfoManage *pBaseInfoManage = NULL;
    
    switch (nselect)
    {
    default:
        {
            CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
            return; 
        }

    case 1:
        {
            pBaseInfoManage = &m_StudInfoManage;
            break;
        }

    case 2:
        {
            pBaseInfoManage = &m_CourseInfoManage;
            break;
        }

    case 3:
        {
            pBaseInfoManage = &m_TeacherInfoManage;
            break;
        }

    case 4:
        {
            pBaseInfoManage = &m_SelectInfoManage;
            break;
        }
    }//end for switch
    
    unsigned long nsubselect = 0;

    while (true)
    {
        if (4 == nselect)
        {
            CYourPanel::ShowMenu_Sub_Select();
        }
        else
        {
            CYourPanel::ShowMenu_Sub();
        }

        CMyQueue<int> *pResultQueue = new CMyQueue<int>;
        int nCount = 0;
        CYourPanel::GetSelect(&nsubselect);
        switch (nsubselect)
        {
        default:
            {
                CYourPanel::ShowInfo("Please make the right decision ^_^\r\n");
                break;
            }
            
        case 0:
            {
                return;
            }
            
        case 1:
            {
                ProcessAdd(pBaseInfoManage);
                break;
            }
            
        case 2:
            {
                ProcessQueryByName(pBaseInfoManage, pResultQueue, &nCount);
                break;
            }
            
        case 3:
            {
                ProcessQueryByID(pBaseInfoManage, pResultQueue, &nCount);
                break;
            }
            
        case 4:
            {
                ProcessModify(pBaseInfoManage);
                break;
            }
            
        case 5:
            {
                ProcessDel(pBaseInfoManage);
                break;
            }
            
        case 6:
            {
                Display(pBaseInfoManage);
            }
        }//end for switch;

        if (pResultQueue != NULL)
        {
            delete pResultQueue;
            pResultQueue = NULL;
        }
    }

#if 0  
    case 4:
        {
            ProcessModify();
            break;
        }

    case 5:
        {
            //display:
            CYourPanel::ShowInfo("Displaying......\r\n");
            
            Display(&m_StudInfoManage);
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
#endif
}

void
CManage::ProcessAdd(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息 
    unsigned long num = 0;
    bool IsRandom = false;

    CYourPanel::ShowInfo("是否随机产生?(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::ShowInfo("请输入想产生的记录数: ");
        CYourPanel::GetSelect(&num);
        IsRandom = true;
    } 

LOOP:
    if (IsRandom
        && 0 == num--)
    {
        return;
    }

    //这里要做到多态性
    CBaseInfo *pBaseInfo = pBaseInfoManage->InputInfo(IsRandom);
    if (NULL == pBaseInfo)
    {
        return;
    }
    
    int result = InfoAdd(pBaseInfo, pBaseInfoManage);
    if (NO_SPACE == result)
    {
        CYourPanel::ShowInfo("Add failed, for No More Space\r\n");
    }
    else
    {
        //not a good habit to reuse result for different meanning.
        sprintf(szhitinfo, "\r\nADD: store into %d\r\n", result);
        //CYourPanel::ShowInfo(szhitinfo);
    }

    //SaveTable(pBaseInfoManage);

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }

    if (IsRandom)
    {
        goto LOOP;
    }
}

int 
CManage::InfoAdd(CBaseInfo *pBaseInfo, CBaseInfoManage *pBaseInfoManage)
{
    unsigned long ulstart     = 0;      //信息在文件中的偏移
    unsigned char uchsize     = 0;      //信息占用的总空间
    unsigned char uchnamesize = 0;      //信息中姓名占用的空间
    unsigned char uchnosize   = 0;      //信息中学号占用的空间
    unsigned char uchtmpsize  = 0;      //从g_del_table中查询到的符号条件空间的大小
    
    tag_addrecord addrecord = {0};
    tag_delrecord delrecord = {0};
    
    int index  = 0;
    int result = SUCCESS;
    
    //also store the '\0'
    uchnamesize = strlen(pBaseInfo->GetName()) + 1;
    uchnosize   = strlen(pBaseInfo->GetID()) +1;
    uchsize     = pBaseInfo->GetInfoSize();

    //to be updated (ulstart)
    addrecord.ulstart     = ulstart;
    addrecord.uchsize     = uchsize;
    addrecord.uchnamesize = uchnamesize;
    
    //使用g_del_table, 对已删除空间再利用，所以优先查询g_del_table
    result = GetSize(uchsize, &index, pBaseInfoManage);  

    //未找到，则追加至信息文件末尾
    if (NOT_EXIST == result)
    {
        //获取信息文件末尾
        GetFileSize(&ulstart, pBaseInfoManage);
        
        //信息文件处理
        result = pBaseInfoManage->GetInfoFile().AppendData(pBaseInfo, 
                                                           ulstart
                                                           );
        if (FAILED == result)
        {
            return result;
        }
        
        addrecord.ulstart     = ulstart;
        addrecord.uchsize     = uchsize;
        addrecord.uchnamesize = uchnamesize; 
        
        //添加表插入
        pBaseInfoManage->GetAddTable()->InsertIntoTable(&addrecord,
                                                        pBaseInfoManage->GetAddTableFile().GetFile()
                                                        );
        
    }//end for if(NOT_EXIST == result)
    else
    {  
        //有可用的已删除空间
        tag_delrecord *pdelrecords = pBaseInfoManage->GetDelTable()->GetRecords();
        uchtmpsize  = pdelrecords[index].uchsize;
        ulstart     = pdelrecords[index].ulstart;
        
        //信息文件中ulstart处数据的更新
        result = pBaseInfoManage->GetInfoFile().ModifyData(pBaseInfo,
                                                            ulstart
                                                            );
        if (FAILED == result)
        {
            return result;
        }
        
        //已删除空间被完全占用，
        if (uchtmpsize == uchsize)
        {
            //not necessary to update all, ulstart is necessary
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            addrecord.uchnamesize = uchnamesize;
            pBaseInfoManage->GetDelTable()->RemoveFromTable(index,
                                                            pBaseInfoManage->GetDelTableFile().GetFile());
            pBaseInfoManage->GetAddTable()->InsertIntoTable(&addrecord,
                                                            pBaseInfoManage->GetAddTableFile().GetFile()); //注意参数

        }
        else    //有剩余删除空间
        {
            delrecord.uchsize  = uchtmpsize - uchsize;
            delrecord.ulstart  =  ulstart + uchsize;
            pBaseInfoManage->GetDelTable()->UpdateTable(index, &delrecord,
                                                        pBaseInfoManage->GetDelTableFile().GetFile());
            //not necessary to update all, ulstart is necessary
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            addrecord.uchnamesize = uchnamesize;
            pBaseInfoManage->GetAddTable()->InsertIntoTable(&addrecord,
                                                            pBaseInfoManage->GetAddTableFile().GetFile()); //注意参数
        }
    }//end for NOT_EXIST != result
    
    //This is not a good idea using a var to represent more than one meaning.
    result = addrecord.ulstart;

    //处理IDTree, NameTree
    pBaseInfoManage->GetIDTree().Insert((char *)pBaseInfo->GetID(), result);
    pBaseInfoManage->GetNameTree().Insert((char *)pBaseInfo->GetName(), result);

    return result;
}

int
GetFileSize(unsigned long *filesize, CBaseInfoManage *pBaseInfoManage)
{
    FILE *fp    = NULL;
    unsigned long addoffset   = 0;
    unsigned long deloffset   = 0;
    long size   = 0;
    long offset = 0;
    int result  = SUCCESS;
    
    fp = fopen(pBaseInfoManage->GetInfoFile().GetFile(), "rb");
    if (NULL == fp)
    {
        result = FAILED;
        
#ifdef USE_EXCEPTION
        throw CMyFileError("GetFileSize FAILED, NOT EXIST: ", pBaseInfoManage->GetInfoFile().GetFile());
#else
        return result;
#endif
    }
    
    //因defrag，未进行truncate, 所以利用addtable和deltable最后一项纪录确认文件结束
    CAddTable *paddtable = pBaseInfoManage->GetAddTable();
    CDelTable *pdeltable = pBaseInfoManage->GetDelTable();
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
        throw CMyFileError("GetFileSize FAILED, FSEEK ERROR: ", pBaseInfoManage->GetInfoFile().GetFile());
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
        throw CMyFileError("GetFileSize FAILED, FTELL ERROR: ", pBaseInfoManage->GetInfoFile().GetFile());
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

void
CManage::Display(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[MAX_INPUT * 2] = {0};
    char buf[MAX_INPUT * 2] = {0};
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_student student       = {0};
    tag_addrecord *paddrecord = NULL;
    
    unsigned long ulstart = 0;
    unsigned long nrecords = 0;
    int result   = SUCCESS;
    unsigned long i = 0;
    
    CBaseInfo *pBaseInfo = NULL;    
    CAddTable *pAddTable = pBaseInfoManage->GetAddTable();
    tag_addrecord *paddrecords = pAddTable->GetRecords();
    nrecords = pAddTable->GetRecordsNum(); 

    unsigned long start = 0;
    unsigned long range = 0;
    CYourPanel::ShowInfo("显示指定范围内的多条记录，调试所用\r\n");
    sprintf(szhitinfo, "共有    %d  条记录\r\n", nrecords);
    CYourPanel::ShowInfo(szhitinfo);
    CYourPanel::ShowInfo("请输入起始index:  ");
    CYourPanel::GetSelect(&start);
    CYourPanel::ShowInfo("请输入显示条数:  ");
    CYourPanel::GetSelect(&range);
    
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for (i = start; i < nrecords && i < start + range; i++)
    {
        ulstart = paddrecords[i].ulstart;
        
        paddrecord = &paddrecords[i];
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord, buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);      //注意负责释放 
        if (NULL == pBaseInfo)
        {
            CYourPanel::ShowInfo("Display fetch1info error\r\n");
        }
        
        //sprintf(szhitinfo, "    start at: %d\r\n", ulstart);
        //CYourPanel::ShowInfo(szhitinfo);
        
        //Buffer Overflow perhaps here.
        pBaseInfoManage->ShowInfo(i, pBaseInfo, szhitinfo, this);
        CYourPanel::ShowInfo(szhitinfo);

        if (pBaseInfo != NULL)
        {
            delete pBaseInfo;
            pBaseInfo = NULL;
        }
    }

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }
}

void
CManage::SaveTable(CBaseInfoManage *pBaseInfoManage)
{
    pBaseInfoManage->GetAddTable()->SaveTable(pBaseInfoManage->GetAddTableFile().GetFile());
    pBaseInfoManage->GetDelTable()->SaveTable(pBaseInfoManage->GetDelTableFile().GetFile());
}

void
CManage::ShowTable(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[64] = {0};
    unsigned long nrecords = 0;
    unsigned long i = 0;
    
    CYourPanel::ShowInfo("\r\n[ Debug ]showing g_add_table (start, size) ......\r\n");    
    CYourPanel::ShowInfo("g_add_table:\r\n");
    
    CAddTable *paddtable = pBaseInfoManage->GetAddTable();
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
    
    CDelTable *pdeltable = pBaseInfoManage->GetDelTable();
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

int 
Foo(void)
{
    return 1;
}

void
CManage::ProcessQueryByName(CBaseInfoManage *pBaseInfoManage, 
                            CMyQueue<int> *pResultQueue, 
                            int *pnResult)
{    
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char buf[MAX_INPUT * 2] = {0};
    char szname[MAX_INPUT]    = {0};
    
    int result                = 0;     //ugly
    int i  = 0;
    
    int num                   = 0;      //查询匹配到的总数目
    unsigned long index       = 0;      //查询到的记录在添加表的下标

    
    CYourPanel::GetInput(szname, "请输入要查询的姓名(或者StudID)(暂不支持模糊查询)\r\n");

    //可能有同名的存在，二叉树的结点可能挂有链表
    CTreeNode<char *> *pQueryedNodes = pBaseInfoManage->GetNameTree().Find(szname);
    
    if (NULL == pQueryedNodes)
    {
        CYourPanel::ShowInfo("Query: not exist\r\n");
        return;
    }
    
    CAddTable *pAddTable = pBaseInfoManage->GetAddTable();    
    CBaseInfo *pBaseInfo = NULL;
    tag_addrecord *paddrecord = NULL;
    bool IsExist = false;
    
    //查询结果输出
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for ( ; pQueryedNodes != NULL; pQueryedNodes = pQueryedNodes->GetNext())
    {
        //根据InOffset，从添加表中读取添加记录
        IsExist = pAddTable->ReadRecord(pQueryedNodes->GetInfoOffset(), &paddrecord, &index);
        if (!IsExist)
        {
            //sth wrong
            continue;
        }

        //根据添加记录，从信息文件中读取信息数据
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord,buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);  //注意负责释放
        
        if (NULL == pBaseInfo)
        {
            CYourPanel::ShowInfo("process_query fetch1info error");
        }

        //将添加记录的下标入队列保存，为删除或者修改服务
        pResultQueue->EnQueue(index);
        *pnResult = i + 1;
        
        pBaseInfoManage->ShowInfo(i++, pBaseInfo, szhitinfo, this);
        CYourPanel::ShowInfo(szhitinfo);

        if (pBaseInfo != NULL)
        {
            delete pBaseInfo;
            pBaseInfo = NULL;
        }
    }

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }
}

//ProcessQueryByID 和ProcessQueryByName 很类似，可进行整合。
//Do Not Repeat By Yourself.
bool
CManage::ProcessQueryByID(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult)
{    
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char buf[MAX_INPUT] = {0};
    char szno[MAX_INPUT]    = {0};
    
    int result                = 0;     //ugly
    int i  = 0;
    int offset = 0;             //接收查询到的偏移
    unsigned long index = 0;    //查询到的记录在添加表中的下标   
    
    
    CYourPanel::GetInput(szno, "请输入ID(或者CourseID)(暂不支持模糊处理)\r\n");

    //查找IDTree中符合该ID的结点，也可能有相同的ID，如选课中，
    CTreeNode<char *> *pQueryedNodes = pBaseInfoManage->GetIDTree().Find(szno);
    
    if (NULL == pQueryedNodes)
    {
        CYourPanel::ShowInfo("Query: not exist\r\n");
        return false;
    }
    
    CAddTable *pAddTable = pBaseInfoManage->GetAddTable();    
    tag_addrecord *paddrecord = NULL;
    bool IsExist = true;
    CBaseInfo *pBaseInfo = NULL;

    //查询结果输出
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for (i = 0; pQueryedNodes != NULL; pQueryedNodes = pQueryedNodes->GetNext(), i++)
    {
        //根据记录在信息文件中的偏移，找到在添加表中的添加记录
        IsExist = pAddTable->ReadRecord(pQueryedNodes->GetInfoOffset(), &paddrecord, &index);
        if (!IsExist)
        {
            //sth wrong
            return false;
        }

        //根据添加记录，从信息文件中读取数据
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord, buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);   //注意负责释放

        if (NULL == pBaseInfo)
        {
            CYourPanel::ShowInfo("process_query fetch1info error");
        }

        //将在添加表中的下标入队列保存，服务于后续的查询、修改
        pResultQueue->EnQueue(index);
        *pnResult = i + 1;

        pBaseInfoManage->ShowInfo(i, pBaseInfo, szhitinfo, this);
        CYourPanel::ShowInfo(szhitinfo);

        if (pBaseInfo != NULL)
        {
            delete pBaseInfo;
            pBaseInfo = NULL;
        }
    }

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }

    return IsExist;
}

int
CManage::ProcessDel(CBaseInfoManage *pBaseInfoManage)
{
       //Used for Debug
//     while (pBaseInfoManage->GetIDTree().GetCount() != 0)
//     {
//         InfoDel(pBaseInfoManage, 0);
//         //Display(pBaseInfoManage);
//     }
//     return SUCCESS;

    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szno[MAX_INPUT] = {0};
    unsigned long index     = 0;  
    int result                = 0;
    int ch                    = 0;
    int nCount                = 0;
    
    tag_addrecord *paddrecord = NULL;
    tag_student student    = {0};
    char szname[MAX_INPUT] = {0};
    
    bool IsByID = true;
    CMyQueue<int> *pResultQueue = new CMyQueue<int>;

    CYourPanel::ShowInfo("仅能根据具体ID或者名称进行删除，请根据查询功能确认\r\n");
    CYourPanel::ShowInfo("默认是根据ID(选课中是CourseID)来操作，也可根据Name(选课中是StudID),\r\n"
                        "是否默认？(Y/N) >>  ");
    if (!CYourPanel::IsSure())
    {
        IsByID = false;
    }

    if (IsByID)
    {
        ProcessQueryByID(pBaseInfoManage, pResultQueue, &nCount);
    }
    else
    {
        ProcessQueryByName(pBaseInfoManage, pResultQueue, &nCount);
    }

    if (0 == nCount)
    {
        return NOT_EXIST;
    }
    else 
    {
        unsigned long nselect = 0;
        if (nCount > 1)
        {
            CYourPanel::ShowInfo("请选择具体的index，进行操作 >> \r\n");
            CYourPanel::GetSelect(&nselect);
        }
        
        for (int i = 0; !pResultQueue->IsEmpty() && i <= nselect; i++)
        {
            //队列中保存的是添加表中的下标
            index = pResultQueue->GetFront();
            pResultQueue->DeQueue();
        }

        if (pResultQueue->IsEmpty() && nselect != nCount - 1)
        {
            CYourPanel::ShowInfo("Please make your right decision^_^\r\n");
            return NOT_EXIST;
        }
    }

    CYourPanel::ShowInfo("Are you sure to delete this one? (Y/N) >>");
    if (!CYourPanel::IsSure())
    {
        return result;
    }
    
    result = InfoDel(pBaseInfoManage, index); 
    CYourPanel::ShowInfo("Del ok\r\n");

    if (pResultQueue != NULL )
    {
        delete pResultQueue;
        pResultQueue = NULL;
    }
    
    //SaveTable(pBaseInfoManage);
    
    return result;
}

int
CManage::InfoDel(CBaseInfoManage *pBaseInfoManage, int index)
{
    tag_delrecord delrecord = {0};
    unsigned char uchsize = 0;
    unsigned long ulstart = 0;
    
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT]   = {0};
    
    int result = SUCCESS;
    
    //根据添加记录的下标，获取要删除的信息的管理数据(Offset, Size)
    tag_addrecord *paddrecords = pBaseInfoManage->GetAddTable()->GetRecords();
    uchsize  = paddrecords[index].uchsize;
    ulstart = paddrecords[index].ulstart;
    
    delrecord.uchsize = uchsize;
    delrecord.ulstart = ulstart;

    //先来处理IDTree, NameTree
    pBaseInfoManage->RemoveFromTree(&paddrecords[index]);
    
    //再处理分配表
    pBaseInfoManage->GetDelTable()->InserIntoTable(&delrecord,
                                                  pBaseInfoManage->GetDelTableFile().GetFile()); 
    pBaseInfoManage->GetAddTable()->RemoveFromTable(index,
                                                    pBaseInfoManage->GetAddTableFile().GetFile());     
    
    return result;
}

int
CManage::ProcessModify(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[MAX_INPUT] = {0};    //输出的提示性信息
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    tag_newinfo flag          = {0};
    tag_addrecord *paddrecord = NULL;
    unsigned long infoid      = 0;
    int result                = SUCCESS;     //ugly
    int ch = 0;    
    int i  = 0;
    
    unsigned long index         = 0;  
    
    CMyQueue<int> *pResultQueue = new CMyQueue<int>;
    int nCount = 0;
    bool IsByID = true;
    CYourPanel::ShowInfo("仅能根据具体的ID或者名称进行修改，请根据查询功能确认\r\n");
    CYourPanel::ShowInfo("默认是根据ID(选课中是CourseID)来操作，也可根据Name(选课中是StudID), \r\n"
                         "是否默认？(Y/N) >>  ");
    if (!CYourPanel::IsSure())
    {
        IsByID = false;
    }
    
    //根据用户提供的ID或者名称，来罗列所有可能的数据
    if (IsByID)
    {
        ProcessQueryByID(pBaseInfoManage, pResultQueue, &nCount);
    }
    else
    {
        ProcessQueryByName(pBaseInfoManage, pResultQueue, &nCount);
    }
    
    if (0 == nCount)
    {
        return NOT_EXIST;
    }
    else 
    {
        unsigned long nselect = 0;
        if (nCount > 1)
        {
            CYourPanel::ShowInfo("请选择具体的index，进行操作 >> \r\n");
            CYourPanel::GetSelect(&nselect);
        }
        
        for (int i = 0; !pResultQueue->IsEmpty() && i <= nselect; i++)
        {
            //index是相关信息在添加表的中的下标
            index = pResultQueue->GetFront();
            pResultQueue->DeQueue();
        }
         
        if (pResultQueue->IsEmpty() && nselect != nCount - 1)
        {
            CYourPanel::ShowInfo("Please make your right decision^_^");
            return NOT_EXIST;
        }
    }

    CYourPanel::ShowInfo("Are you sure to modify this one? (Y/N) >>");
    if (!CYourPanel::IsSure())
    {
        return result;
    }
    
    //Modify
    CBaseInfo *pTmpBaseInfo = pBaseInfoManage->GetNewInfo(MODIFY, &flag);
    if (NULL == pTmpBaseInfo)
    {
        return FAILED;
    }

    result = InfoModify(pBaseInfoManage, pTmpBaseInfo, flag, index);
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
    
    //SaveTable(pBaseInfoManage);

    if (pTmpBaseInfo != NULL)
    {
        delete pTmpBaseInfo;
        pTmpBaseInfo = NULL;
    }
    
    return result;
}

int
CManage::InfoModify(CBaseInfoManage *pBaseInfoManage, 
                    CBaseInfo *pTmpBaseInfo, 
                    tag_newinfo flag, 
                    int index)
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
    char sztmpname[MAX_INPUT] = {0};
    char sztmpno[MAX_INPUT]   = {0};
    char buf[MAX_INPUT] = {0};
    
    int result = SUCCESS; 
    
    //首先根据index获取得原信息
    tag_addrecord *paddrecords = pBaseInfoManage->GetAddTable()->GetRecords();   
    paddrecord = &paddrecords[index];

    pBaseInfoManage->GetInfoFile().ReadData(paddrecord, buf);
    CBaseInfo *pOldBaseInfo = pBaseInfoManage->SetInfo(buf);
    CBaseInfo *pNewBaseInfo = NULL;
    if (NULL == pOldBaseInfo)
    {
        //showinfo("info_modify fetch1info error\r\n");
        result = FAILED;
        goto END;
    }

    //设置新信息
    pNewBaseInfo = pBaseInfoManage->SetNewInfo(pOldBaseInfo, pTmpBaseInfo, flag);
    if (NULL == pNewBaseInfo)
    {
        result = FAILED;
        goto END;
    }

    ucholdsize = pOldBaseInfo->GetInfoSize();
    uchnewsize = pNewBaseInfo->GetInfoSize();
    ulstart = paddrecord->ulstart;
    uchnamesize = strlen(pNewBaseInfo->GetName()) + 1;

    addrecord.uchsize  = uchnewsize;
    addrecord.ulstart = ulstart;
    addrecord.uchnamesize = uchnamesize;
    
    // this will generate unused space, we need to update g_add_table and g_del_table;
    if (uchnewsize < ucholdsize)
    {
        try 
        {
            result = pBaseInfoManage->GetInfoFile().ModifyData(pNewBaseInfo,
                                                                ulstart
                                                                );
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
                                   
        if (FAILED == result)
        {
            goto END;
        }
        
        //更新添加表中记录
        pBaseInfoManage->GetAddTable()->UpdateTable(index, &addrecord,
                                                    pBaseInfoManage->GetAddTableFile().GetFile());
        
        //有新的删除空间可用
        delrecord.uchsize  = ucholdsize - uchnewsize;
        delrecord.ulstart = ulstart + uchnewsize;
        pBaseInfoManage->GetDelTable()->InserIntoTable(&delrecord,
                                                    pBaseInfoManage->GetDelTableFile().GetFile());  //注意参数

        //处理IDTree, NameTree
        pBaseInfoManage->RemoveFromTree(pOldBaseInfo, ulstart);
        pBaseInfoManage->InsertInfoTree(pNewBaseInfo, ulstart);

    }//end for if (uchnewsize < ucholdsize)
    else if (uchnewsize == ucholdsize)
    {
        try 
        {   
            result = pBaseInfoManage->GetInfoFile().ModifyData(pNewBaseInfo, ulstart);
        }
        catch (CMyFileError &e)
        {
            e.WriteLog(__FILE__, __LINE__);
        }
        
        if (FAILED == result)
        {
            goto END;
        }

        pBaseInfoManage->GetAddTable()->UpdateTable(index, &addrecord,
                                                    pBaseInfoManage->GetAddTableFile().GetFile());

    
        //处理IDTree, NameTree
        pBaseInfoManage->RemoveFromTree(pOldBaseInfo, ulstart);
        pBaseInfoManage->InsertInfoTree(pNewBaseInfo, ulstart);

    }//end for if (uchnewsize == ucholdsize)
    else
    {
        InfoDel(pBaseInfoManage, index);
        result = InfoAdd(pNewBaseInfo, pBaseInfoManage);

        //IDTree, NameTree已经在InfoDel, InfoAdd中处理
    }//end for if (uchnewsize > ucholdsize) 
    
END:
    if (pOldBaseInfo != NULL)
    {
        delete pOldBaseInfo;
        pOldBaseInfo = NULL;
    }

    if (pNewBaseInfo != NULL)
    {
        delete pNewBaseInfo;
        pNewBaseInfo = NULL;
    }

    return result;
}
 

