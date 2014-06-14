#include <stdio.h>
#include <time.h>
#include "CInfo.h"
#include "ui.h"
#include "CInfoManage.h"
#include "CMyString.h"
#include "CManage.h"

void
CBaseInfoManage::LoadTree(void)
{
    //1. 从AddTable中获取Offset,
    //2. 根据Offset从InfoFile中读取addrecord,以获取ID和Name
    CAddTable *pAddTable = m_pBaseAddTable;
    tag_addrecord *pAddRecords = pAddTable->GetRecords();
    unsigned long nRecords =  pAddTable->GetRecordsNum();
    CBaseInfo *pBaseInfo = NULL;
    unsigned long offset = 0;
    bool IsExist = false;

    char buf[MAX_INPUT] = {0};
    
    for (int i = 0; i < nRecords; i++)
    {    
        m_BaseInfoFile.ReadData(&pAddRecords[i], buf);
        pBaseInfo = SetInfo(buf);
        if (NULL == pBaseInfo)
        {
            //cout << "Sth wrong" << endl;
        }

        offset = pAddRecords[i].ulstart;
        m_IDTree.Insert((char *)pBaseInfo->GetID(), offset);
        m_NameTree.Insert((char *)pBaseInfo->GetName(), offset);

        if (pBaseInfo != NULL)
        {
            delete pBaseInfo;
            pBaseInfo = NULL;
        }
    }    
}

void 
CBaseInfoManage::RemoveFromTree(tag_addrecord *paddrecord)
{
    char buf[MAX_INPUT] = {0};   
    m_BaseInfoFile.ReadData(paddrecord, buf);

    CBaseInfo *pBaseInfo = SetInfo(buf);
    if (NULL == pBaseInfo)
    {
        cout << "sth wrong"  << endl;
    }

    unsigned long offset = paddrecord->ulstart;
    RemoveFromTree(pBaseInfo, offset);

    if (pBaseInfo != NULL)
    {
        delete pBaseInfo;
        pBaseInfo = NULL;
    }
}

void
CBaseInfoManage::RemoveFromTree(CBaseInfo *pBaseInfo, unsigned long offset)
{   
    CTreeNode<char *> *pIDNode = m_IDTree.Find((char *)pBaseInfo->GetID(), offset);
    CTreeNode<char *> *pNameNode = m_NameTree.Find((char *)pBaseInfo->GetName(), offset);
    if (NULL == pIDNode
        || NULL == pNameNode)
    { 
        //Just for Debug
        pIDNode = m_IDTree.Find((char *)pBaseInfo->GetID(), offset);
        pNameNode = m_NameTree.Find((char *)pBaseInfo->GetName(), offset);
    }
    m_IDTree.Remove(pIDNode);
    m_NameTree.Remove(pNameNode);
}

void
CBaseInfoManage::InsertInfoTree(CBaseInfo *pBaseInfo, unsigned long offset)
{
    m_IDTree.Insert((char *)pBaseInfo->GetID(), offset);
    m_NameTree.Insert((char *)pBaseInfo->GetName(), offset); 
}

CBaseInfo *
CBaseInfoManage::GetInfoByID(char *pszno)
{
    //查找IDTree中符合该ID的结点，也可能有相同的ID，如选课中，
    CTreeNode<char *> *pQueryedNodes = m_IDTree.Find(pszno);
    
    if (NULL == pQueryedNodes)
    {
        //CYourPanel::ShowInfo("                      Info for this ID not exist\r\n");
        return NULL;
    }
    
    CAddTable *pAddTable = m_pBaseAddTable;    
    tag_addrecord *paddrecord = NULL;
    bool IsExist = true;
    CBaseInfo *pBaseInfo = NULL;
    unsigned long index = 0;
    char buf[MAX_INPUT] = {0};
    
    //根据记录在信息文件中的偏移，找到在添加表中的添加记录
    IsExist = pAddTable->ReadRecord(pQueryedNodes->GetInfoOffset(), &paddrecord, &index);
    if (!IsExist)
    {
        //sth wrong
        return NULL;
    }
    
    //根据添加记录，从信息文件中读取数据
    m_BaseInfoFile.ReadData(paddrecord, buf);
    pBaseInfo = SetInfo(buf);
    
    return pBaseInfo;
}

CBaseInfo *
CBaseInfoManage::InputInfo(bool IsRandom, int nStart, const char *ptag)
{
    char szname[MAX_INPUT]    = {0};
    char szno[MAX_INPUT]      = {0};
    
    if (!IsRandom)
    {
        CYourPanel::ShowInfo("请输入信息:   \r\n");    
        CYourPanel::GetInput(szname, "Enter Name:   ");
        CYourPanel::GetInput(szno,   "Enter No  :   ");
    }
    else
    {
        static int ncout = nStart;
        
        if (0 == nStart)
        {
            ncout++;
        }
        else
        {
            ncout = nStart;
        }

        char sztmp[MAX_INPUT] = {0};
        strcpy(szno, ptag);
        strcat(szno, itoa(ncout, sztmp, 10));

        srand((unsigned)time(NULL));
        int num = (rand() + ncout) % 32 + 1;
        int i = 0;
        while (i != num)
        {
            szname[i] = rand() % 26 + 'a';
            i++;
        }  
        
        //cout << szname << endl;
    }
    
    //需用户负责释放，不好的设计
    CBaseInfo *pBaseInfo = new CBaseInfo;
    if (pBaseInfo != NULL)
    {
        pBaseInfo->SetName(szname);
        pBaseInfo->SetID(szno);
    }
    
    return pBaseInfo;
}

CBaseInfo *
CBaseInfoManage::GetNewInfo(int actiontype,
                           tag_newinfo *pflag
                           )
{
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT] = {0};

    //需用户负责释放，不好的设计
    CBaseInfo *pBaseInfo = new CBaseInfo;
    if (NULL == pBaseInfo)
    {
        return pBaseInfo;
    }
    
    CYourPanel::ShowInfo("请输入新信息：\r\n");
    CYourPanel::ShowInfo("是否包括姓名？(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::GetInput(szname, "Enter Name: ");

        pflag->name = 1;
        pBaseInfo->SetName(szname);

    }
    
    CYourPanel::ShowInfo("是否包括ID？(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::GetInput(szno,   "Enter No:");

        pflag->no = 1;
        pBaseInfo->SetID(szno);
    }

    return pBaseInfo;
}

CBaseInfo * 
CBaseInfoManage::SetNewInfo(CBaseInfo *pOldBaseInfo,
                            CBaseInfo *pTmpInfo, 
                            tag_newinfo flag)
{
    //需用户负责释放，不好的设计
    CBaseInfo *pNewBaseInfo = new CBaseInfo;
    if (NULL == pNewBaseInfo )
    {
        return NULL;
    }

    if (1 == flag.name )
    {
        pNewBaseInfo->SetName(pTmpInfo->GetName());
    }
    else
    {
        pNewBaseInfo->SetName(pOldBaseInfo->GetName());
    }

    if (1 == flag.no)
    {
        pNewBaseInfo->SetID(pTmpInfo->GetID());
    }
    else
    {
        pNewBaseInfo->SetID(pOldBaseInfo->GetID());
    }

    return pNewBaseInfo;
}

const char *
CBaseInfoManage::ShowInfoField(void)
{
    return "\r\nindex   name        no \r\n";
}

void
CBaseInfoManage::ShowInfo(int index, const CBaseInfo *pBaseInfo, char *pszInfo, CManage *pManage)
{
    sprintf(pszInfo, "%d   %s       %s\r\n",
                         index,
                         pBaseInfo->GetName(),
                         pBaseInfo->GetID()
                         );
}

//////////////////////////////////////////////////////////////////////////

CBaseInfo *
CStudInfoManage::InputInfo(bool IsRandom, int nStart, const char *ptag)
{
    static nCount = nStart;
    nCount++;

    return this->CBaseInfoManage::InputInfo(IsRandom, nCount, "S");
}

//////////////////////////////////////////////////////////////////////////
CBaseInfo *
CCourseInfoManage::InputInfo(bool IsRandom, int nStart,  const char *ptag)
{
    static nCount = nStart;
    nCount++;
    return this->CBaseInfoManage::InputInfo(IsRandom, nCount, "C");
}

//////////////////////////////////////////////////////////////////////////
CBaseInfo *
CTeacherInfoManage::InputInfo(bool IsRandom, int nStart, const char *ptag)
{
    static nCount = nStart;
    nCount++;
    return this->CBaseInfoManage::InputInfo(IsRandom,  nCount, "T");
}

//////////////////////////////////////////////////////////////////////////
CBaseInfo *
CSelectInfoManage::InputInfo(bool IsRandom, int nStart, const char *ptag)
{
    char szStudID[MAX_INPUT]    = {0};
    char szCourseID[MAX_INPUT]      = {0};
    float fscore = 0.0f;
    static int ncout = nStart;
    ncout++;
    
    if (!IsRandom)
    {
        CYourPanel::ShowInfo("请输入信息: (自己确保ID存在)  \r\n");    
        CYourPanel::GetInput(szStudID, "学生ID:   ");
        CYourPanel::GetInput(szCourseID,   "课程ID:   ");
        CYourPanel::GetInput(&fscore,   "成绩  :   ");
    }
    else
    {
        char sztmp[MAX_INPUT] = {0};
        szStudID[0] = 'S';        
        strcat(szStudID, itoa(ncout % 100000, sztmp, 10));
        
        //cout << szno << endl;
        
        sztmp[0] = '\0';
        szCourseID[0] = 'C';
        strcat(szCourseID, itoa(ncout * rand() % 1000, sztmp, 10));

        srand((unsigned)time(NULL));
        fscore = float((rand() + ncout)% 100);
    }
    
    //需用户负责释放，不好的设计
    CSelectInfo *pSelctInfo = new CSelectInfo;
    if (pSelctInfo != NULL)
    {
//         strcat(szStudID, "_|");
//         strcat(szCourseID, "_|");
//         pSelctInfo->SetName(CMyString(szStudID).StringCat(szCourseID));
//         pSelctInfo->SetID(CMyString(szCourseID).StringCat(szStudID));
        pSelctInfo->SetName(szStudID);
        pSelctInfo->SetID(szCourseID);
        pSelctInfo->SetScore(fscore);        
    }
    
    return pSelctInfo;
}

CBaseInfo *
CSelectInfoManage::GetNewInfo(int actiontype,
                             tag_newinfo *pflag
                            )
{
    char szname[MAX_INPUT] = {0};
    char szno[MAX_INPUT] = {0};
    float fscore = 0.0f;
    
    //需用户负责释放，不好的设计
    CSelectInfo *pSelecInfo = new CSelectInfo;
    if (NULL == pSelecInfo)
    {
        return pSelecInfo;
    }
    
    CYourPanel::ShowInfo("请输入新信息：\r\n");
    CYourPanel::ShowInfo("是否包括学生ID？(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::GetInput(szname, "请输入学生ID: ");
        
        pflag->name = 1;
        pSelecInfo->SetName(szname);
        
    }
    
    CYourPanel::ShowInfo("是否包括课程ID？(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::GetInput(szno,   "请输入课程ID:");
        
        pflag->no = 1;
        pSelecInfo->SetID(szno);
    }

    CYourPanel::ShowInfo("是否包括成绩？(Y/N)");
    if (CYourPanel::IsSure())
    {
        CYourPanel::GetScore(&fscore,   "请输入成绩:");
        
        pflag->score = 1;
        pSelecInfo->SetScore(fscore);
    }

    return pSelecInfo;
}

CBaseInfo * 
CSelectInfoManage::SetNewInfo(CBaseInfo *pOldBaseInfo,
                            CBaseInfo *pTmpInfo, 
                            tag_newinfo flag)
{
    //需用户负责释放，不好的设计
    CSelectInfo *pNewBaseInfo = new CSelectInfo;
    if (NULL == pNewBaseInfo )
    {
        return NULL;
    }
    
    if (1 == flag.name )
    {
        pNewBaseInfo->SetName(pTmpInfo->GetName());
    }
    else
    {
        pNewBaseInfo->SetName(pOldBaseInfo->GetName());
    }
    
    if (1 == flag.no)
    {
        pNewBaseInfo->SetID(pTmpInfo->GetID());
    }
    else
    {
        pNewBaseInfo->SetID(pOldBaseInfo->GetID());
    }

    if (1 == flag.score)
    {
        pNewBaseInfo->SetScore(((CSelectInfo *)pTmpInfo)->GetScore());
    }
    else
    {
        pNewBaseInfo->SetScore(((CSelectInfo *)pOldBaseInfo)->GetScore());
    }
    
    return pNewBaseInfo;
}

const char *
CSelectInfoManage::ShowInfoField(void)
{
    return "\r\nindex   StudID    StudName    CourseID    CourseName    Score\r\n";
}

void
CSelectInfoManage::ShowInfo(int index, const CBaseInfo *pBaseInfo, char *pszInfo, CManage *pManage)
{
    CBaseInfo *pStudInfo = pManage->m_StudInfoManage.GetInfoByID((char *)pBaseInfo->GetName());   //选课的Name正是StudID
    CBaseInfo *pCourseInfo = pManage->m_CourseInfoManage.GetInfoByID((char *)pBaseInfo->GetID()); //选课的ID正是CourseID
    sprintf(pszInfo, "%d    %s   %s  %s  %s  %f\r\n",
        index,
        pBaseInfo->GetName(),
        pStudInfo == NULL ? "NOT_EXIST" : pStudInfo->GetName(),
        pBaseInfo->GetID(),
        pCourseInfo == NULL ? "NOT_EXIST" : pCourseInfo->GetName(),
        ((CSelectInfo *)pBaseInfo)->GetScore()
        );

    if (pStudInfo != NULL)
    {
        delete pStudInfo;
        pStudInfo = NULL;
    }

    if (pCourseInfo != NULL)
    {
        delete pCourseInfo;
        pCourseInfo = NULL;
    }
}