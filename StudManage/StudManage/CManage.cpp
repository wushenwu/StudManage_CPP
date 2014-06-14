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

    //��Ϣ�ļ�
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
    
    //��ӱ�
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
    
    //ɾ����
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

    //IDTree,NameTree�ļ���
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
//�����Դ���
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
    char szhitinfo[MAX_INPUT] = {0};    //�������ʾ����Ϣ 
    unsigned long num = 0;
    bool IsRandom = false;

    CYourPanel::ShowInfo("�Ƿ��������?(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::ShowInfo("������������ļ�¼��: ");
        CYourPanel::GetSelect(&num);
        IsRandom = true;
    } 

LOOP:
    if (IsRandom
        && 0 == num--)
    {
        return;
    }

    //����Ҫ������̬��
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
    unsigned long ulstart     = 0;      //��Ϣ���ļ��е�ƫ��
    unsigned char uchsize     = 0;      //��Ϣռ�õ��ܿռ�
    unsigned char uchnamesize = 0;      //��Ϣ������ռ�õĿռ�
    unsigned char uchnosize   = 0;      //��Ϣ��ѧ��ռ�õĿռ�
    unsigned char uchtmpsize  = 0;      //��g_del_table�в�ѯ���ķ��������ռ�Ĵ�С
    
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
    
    //ʹ��g_del_table, ����ɾ���ռ������ã��������Ȳ�ѯg_del_table
    result = GetSize(uchsize, &index, pBaseInfoManage);  

    //δ�ҵ�����׷������Ϣ�ļ�ĩβ
    if (NOT_EXIST == result)
    {
        //��ȡ��Ϣ�ļ�ĩβ
        GetFileSize(&ulstart, pBaseInfoManage);
        
        //��Ϣ�ļ�����
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
        
        //��ӱ����
        pBaseInfoManage->GetAddTable()->InsertIntoTable(&addrecord,
                                                        pBaseInfoManage->GetAddTableFile().GetFile()
                                                        );
        
    }//end for if(NOT_EXIST == result)
    else
    {  
        //�п��õ���ɾ���ռ�
        tag_delrecord *pdelrecords = pBaseInfoManage->GetDelTable()->GetRecords();
        uchtmpsize  = pdelrecords[index].uchsize;
        ulstart     = pdelrecords[index].ulstart;
        
        //��Ϣ�ļ���ulstart�����ݵĸ���
        result = pBaseInfoManage->GetInfoFile().ModifyData(pBaseInfo,
                                                            ulstart
                                                            );
        if (FAILED == result)
        {
            return result;
        }
        
        //��ɾ���ռ䱻��ȫռ�ã�
        if (uchtmpsize == uchsize)
        {
            //not necessary to update all, ulstart is necessary
            addrecord.uchsize  = uchsize;
            addrecord.ulstart  = ulstart;
            addrecord.uchnamesize = uchnamesize;
            pBaseInfoManage->GetDelTable()->RemoveFromTable(index,
                                                            pBaseInfoManage->GetDelTableFile().GetFile());
            pBaseInfoManage->GetAddTable()->InsertIntoTable(&addrecord,
                                                            pBaseInfoManage->GetAddTableFile().GetFile()); //ע�����

        }
        else    //��ʣ��ɾ���ռ�
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
                                                            pBaseInfoManage->GetAddTableFile().GetFile()); //ע�����
        }
    }//end for NOT_EXIST != result
    
    //This is not a good idea using a var to represent more than one meaning.
    result = addrecord.ulstart;

    //����IDTree, NameTree
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
    
    //��defrag��δ����truncate, ��������addtable��deltable���һ���¼ȷ���ļ�����
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
    //ȡ����
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
    CYourPanel::ShowInfo("��ʾָ����Χ�ڵĶ�����¼����������\r\n");
    sprintf(szhitinfo, "����    %d  ����¼\r\n", nrecords);
    CYourPanel::ShowInfo(szhitinfo);
    CYourPanel::ShowInfo("��������ʼindex:  ");
    CYourPanel::GetSelect(&start);
    CYourPanel::ShowInfo("��������ʾ����:  ");
    CYourPanel::GetSelect(&range);
    
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for (i = start; i < nrecords && i < start + range; i++)
    {
        ulstart = paddrecords[i].ulstart;
        
        paddrecord = &paddrecords[i];
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord, buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);      //ע�⸺���ͷ� 
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
    char szhitinfo[MAX_INPUT] = {0};    //�������ʾ����Ϣ
    char buf[MAX_INPUT * 2] = {0};
    char szname[MAX_INPUT]    = {0};
    
    int result                = 0;     //ugly
    int i  = 0;
    
    int num                   = 0;      //��ѯƥ�䵽������Ŀ
    unsigned long index       = 0;      //��ѯ���ļ�¼����ӱ���±�

    
    CYourPanel::GetInput(szname, "������Ҫ��ѯ������(����StudID)(�ݲ�֧��ģ����ѯ)\r\n");

    //������ͬ���Ĵ��ڣ��������Ľ����ܹ�������
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
    
    //��ѯ������
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for ( ; pQueryedNodes != NULL; pQueryedNodes = pQueryedNodes->GetNext())
    {
        //����InOffset������ӱ��ж�ȡ��Ӽ�¼
        IsExist = pAddTable->ReadRecord(pQueryedNodes->GetInfoOffset(), &paddrecord, &index);
        if (!IsExist)
        {
            //sth wrong
            continue;
        }

        //������Ӽ�¼������Ϣ�ļ��ж�ȡ��Ϣ����
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord,buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);  //ע�⸺���ͷ�
        
        if (NULL == pBaseInfo)
        {
            CYourPanel::ShowInfo("process_query fetch1info error");
        }

        //����Ӽ�¼���±�����б��棬Ϊɾ�������޸ķ���
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

//ProcessQueryByID ��ProcessQueryByName �����ƣ��ɽ������ϡ�
//Do Not Repeat By Yourself.
bool
CManage::ProcessQueryByID(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult)
{    
    char szhitinfo[MAX_INPUT] = {0};    //�������ʾ����Ϣ
    char buf[MAX_INPUT] = {0};
    char szno[MAX_INPUT]    = {0};
    
    int result                = 0;     //ugly
    int i  = 0;
    int offset = 0;             //���ղ�ѯ����ƫ��
    unsigned long index = 0;    //��ѯ���ļ�¼����ӱ��е��±�   
    
    
    CYourPanel::GetInput(szno, "������ID(����CourseID)(�ݲ�֧��ģ������)\r\n");

    //����IDTree�з��ϸ�ID�Ľ�㣬Ҳ��������ͬ��ID����ѡ���У�
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

    //��ѯ������
    CYourPanel::ShowInfo(pBaseInfoManage->ShowInfoField());
    for (i = 0; pQueryedNodes != NULL; pQueryedNodes = pQueryedNodes->GetNext(), i++)
    {
        //���ݼ�¼����Ϣ�ļ��е�ƫ�ƣ��ҵ�����ӱ��е���Ӽ�¼
        IsExist = pAddTable->ReadRecord(pQueryedNodes->GetInfoOffset(), &paddrecord, &index);
        if (!IsExist)
        {
            //sth wrong
            return false;
        }

        //������Ӽ�¼������Ϣ�ļ��ж�ȡ����
        pBaseInfoManage->GetInfoFile().ReadData(paddrecord, buf);
        pBaseInfo = pBaseInfoManage->SetInfo(buf);   //ע�⸺���ͷ�

        if (NULL == pBaseInfo)
        {
            CYourPanel::ShowInfo("process_query fetch1info error");
        }

        //������ӱ��е��±�����б��棬�����ں����Ĳ�ѯ���޸�
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

    char szhitinfo[MAX_INPUT] = {0};    //�������ʾ����Ϣ
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

    CYourPanel::ShowInfo("���ܸ��ݾ���ID�������ƽ���ɾ��������ݲ�ѯ����ȷ��\r\n");
    CYourPanel::ShowInfo("Ĭ���Ǹ���ID(ѡ������CourseID)��������Ҳ�ɸ���Name(ѡ������StudID),\r\n"
                        "�Ƿ�Ĭ�ϣ�(Y/N) >>  ");
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
            CYourPanel::ShowInfo("��ѡ������index�����в��� >> \r\n");
            CYourPanel::GetSelect(&nselect);
        }
        
        for (int i = 0; !pResultQueue->IsEmpty() && i <= nselect; i++)
        {
            //�����б��������ӱ��е��±�
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
    
    //������Ӽ�¼���±꣬��ȡҪɾ������Ϣ�Ĺ�������(Offset, Size)
    tag_addrecord *paddrecords = pBaseInfoManage->GetAddTable()->GetRecords();
    uchsize  = paddrecords[index].uchsize;
    ulstart = paddrecords[index].ulstart;
    
    delrecord.uchsize = uchsize;
    delrecord.ulstart = ulstart;

    //��������IDTree, NameTree
    pBaseInfoManage->RemoveFromTree(&paddrecords[index]);
    
    //�ٴ�������
    pBaseInfoManage->GetDelTable()->InserIntoTable(&delrecord,
                                                  pBaseInfoManage->GetDelTableFile().GetFile()); 
    pBaseInfoManage->GetAddTable()->RemoveFromTable(index,
                                                    pBaseInfoManage->GetAddTableFile().GetFile());     
    
    return result;
}

int
CManage::ProcessModify(CBaseInfoManage *pBaseInfoManage)
{
    char szhitinfo[MAX_INPUT] = {0};    //�������ʾ����Ϣ
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
    CYourPanel::ShowInfo("���ܸ��ݾ����ID�������ƽ����޸ģ�����ݲ�ѯ����ȷ��\r\n");
    CYourPanel::ShowInfo("Ĭ���Ǹ���ID(ѡ������CourseID)��������Ҳ�ɸ���Name(ѡ������StudID), \r\n"
                         "�Ƿ�Ĭ�ϣ�(Y/N) >>  ");
    if (!CYourPanel::IsSure())
    {
        IsByID = false;
    }
    
    //�����û��ṩ��ID�������ƣ����������п��ܵ�����
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
            CYourPanel::ShowInfo("��ѡ������index�����в��� >> \r\n");
            CYourPanel::GetSelect(&nselect);
        }
        
        for (int i = 0; !pResultQueue->IsEmpty() && i <= nselect; i++)
        {
            //index�������Ϣ����ӱ���е��±�
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
    
    //���ȸ���index��ȡ��ԭ��Ϣ
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

    //��������Ϣ
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
        
        //������ӱ��м�¼
        pBaseInfoManage->GetAddTable()->UpdateTable(index, &addrecord,
                                                    pBaseInfoManage->GetAddTableFile().GetFile());
        
        //���µ�ɾ���ռ����
        delrecord.uchsize  = ucholdsize - uchnewsize;
        delrecord.ulstart = ulstart + uchnewsize;
        pBaseInfoManage->GetDelTable()->InserIntoTable(&delrecord,
                                                    pBaseInfoManage->GetDelTableFile().GetFile());  //ע�����

        //����IDTree, NameTree
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

    
        //����IDTree, NameTree
        pBaseInfoManage->RemoveFromTree(pOldBaseInfo, ulstart);
        pBaseInfoManage->InsertInfoTree(pNewBaseInfo, ulstart);

    }//end for if (uchnewsize == ucholdsize)
    else
    {
        InfoDel(pBaseInfoManage, index);
        result = InfoAdd(pNewBaseInfo, pBaseInfoManage);

        //IDTree, NameTree�Ѿ���InfoDel, InfoAdd�д���
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
 

