/*
���ǹ���ÿ����Ϣ���������
1)StudInfo��StudInfoFile,           (Name, ID)
            StudAddTableFile,       (Offset, Size, NameSize)
                ------> StudAddTable (Offset, Size, NameSize)
                ------> StudIDTree, (ID, Offset)
                        StudNameTree (Name, Offset)

            StudDelTableFile,       (Offset, Size)          
                ------> StudDelTable (Offset, Size)
            

2) CourseInfo: CourseInfoFile,      (Name, ID)
               CourseAddTableFile,  (Offset, Size, NameSize)
                   ------> CourseAddTable (Offset, Size, NameSize)
                   ------> CourseIDTree (ID, Offset)
                           CourseNameTree (Name, Offset)

                CourseDelTableFile,   (Offset, Size)
                    ------> CourseDelTable (Offset, Size)

3) TeacherInfo: TeacherInfoFile ,       (Name, ID)
                TeacherAddTableFile,    (Offset, Size, NameSize)
                     ------> TeacherAddTable (Offset, Size, NameSize)
                     ------> TeacherIDTree (ID, Offset)
                             TeacherNameTree (Name, Offset)

                TeacherDelTableFile,    (Offset, Size)
                       ------> TeacherDelTable  (Offset, Size)
  
4) SelectInfo: SelectInfoFile,          (Name, ID)
                SelectAddTableFile,     (Offset, Size, NameSize)
                     ------> SelectAddTable (Offset, Size, NameSize)
                     ------> SelectIDTree (ID, Offset)
                             SelectNameTree (Name, Offset)

                SelectDelTableFile,     (Offset, Size)
                       ------> SelectDelTable (Offset, Size)

*/

#ifndef CINFOMANAGE_H_E586DD42_C5A6_4a3b_8EB5_939A46253E2E

#define CINFOMANAGE_H_E586DD42_C5A6_4a3b_8EB5_939A46253E2E

#include <stdlib.h>

#include "CMyFile.h"
#include "CTable.h"
#include "CInfoTree.h"
#include "MyBiTree.h"

class CSelectInfoManage;
class CManage;

class CBaseInfoManage
{
public:
    CBaseInfoManage(const char *pszInfoFile = ".\\Data\\BaseInfo.txt",
                    const char *pszAddTableFile = ".\\Data\\BaseAddTable.txt",
                    const char *pszDelTableFile = ".\\Data\\BaseDelTable.txt"
                    ) 
                    :m_BaseInfoFile(pszInfoFile),
                     m_BaseAddTableFile(pszAddTableFile),
                     m_BaseDelTableFile(pszDelTableFile)
                     
    {
        m_pBaseAddTable = NULL;
        m_pBaseDelTable = NULL;
    }

    virtual ~CBaseInfoManage()
    {
        if (m_pBaseAddTable != NULL)
        {
            delete m_pBaseAddTable;
            m_pBaseAddTable = NULL;
        }

        if (m_pBaseDelTable != NULL)
        {
            delete m_pBaseDelTable;
            m_pBaseDelTable = NULL;
        }
    }

    CBaseInfoFile &GetInfoFile(void) { return m_BaseInfoFile; }
    CBaseAddTableFile &GetAddTableFile(void) { return m_BaseAddTableFile; }
    CBaseDelTableFile &GetDelTableFile(void) { return m_BaseDelTableFile; }

    CAddTable *&GetAddTable(void) { return m_pBaseAddTable; }
    CDelTable *&GetDelTable(void) { return m_pBaseDelTable; }

    CMyBiTree<char *> &GetIDTree(void) {return m_IDTree;}
    CMyBiTree<char *> &GetNameTree(void) {return m_NameTree;}

    //���AddTable, InfoFile�ж�ȡID����Name��Insert(ID,Offset)
    void LoadTree(void);

    void RemoveFromTree(tag_addrecord *paddrecord);
    void RemoveFromTree(CBaseInfo *pBaseInfo, unsigned long offset);

    void InsertInfoTree(CBaseInfo *pBaseInfo, unsigned long offset);

    CBaseInfo *GetInfoByID(char *pszno);

    //����pbuf����CBaseInfo,�û������ͷ�
    virtual CBaseInfo *SetInfo(char *pbuf)
    {
        CBaseInfo *pBaseInfo = new CBaseInfo;
        pBaseInfo->SetInfo(pbuf);
        return pBaseInfo;
    }

    /*
    Function: ���û�����ȡһ����Ϣ���ݣ�������Ϣ����ָ��
    Effect  : �û�Ҫ�����ͷ�������ݡ�
    */
    virtual CBaseInfo *InputInfo(bool IsRandom = false, int nStart = 0, const char *ptag = "");

    /*
    Function: ���û�����ȡ�µ���Ϣ�ֶ�, ������flag, �������������Ϣ
    Param   : actiontype ��ΪQUERY����MODIFY,
              flag ���ڼ�¼�û�����Щ�ֶν���������
    Effect:   �û�Ҫ�����ͷ��������
    */
    virtual CBaseInfo *GetNewInfo(int actiontype, tag_newinfo *pflag);

    /*
    Function: ����flag, �û������pTmpInfo��ԭ��ϢpOldBaseInfo������������������Ϣ��������
    Effect  : �û�Ҫ�����ͷ��������
    */
    virtual CBaseInfo *SetNewInfo(CBaseInfo *pOldBaseInfo,
                                  CBaseInfo *pTmpInfo, 
                                  tag_newinfo flag);
    
    
    /*
    Function: ��������ĸ��ֶ�
    */
    virtual const char *ShowInfoField(void);

    /*
    Function: ���index��ָʾ����Ϣ��¼, ��pszInfo��
    */
    virtual void ShowInfo(int index, const CBaseInfo *pBaseInfo, char *pszInfo, CManage *pManage);

protected:
    CBaseInfoFile m_BaseInfoFile;
    CBaseAddTableFile m_BaseAddTableFile;
    CBaseDelTableFile m_BaseDelTableFile;

    CAddTable *m_pBaseAddTable;
    CDelTable *m_pBaseDelTable;

    CMyBiTree<char *> m_IDTree;     
    CMyBiTree<char *> m_NameTree;   
};

class CStudInfoManage: public CBaseInfoManage
{
    friend class CSelectInfoManage;
public:
    CStudInfoManage(const char *pszInfoFile = ".\\Data\\StudInfo.txt",
                    const char *pszAddTableFile = ".\\Data\\StudAddTable.txt",
                    const char *pszDelTableFile = ".\\Data\\StudDelTable.txt"
                    ) 
                    :CBaseInfoManage(pszInfoFile,
                                     pszAddTableFile,
                                     pszDelTableFile
                                     )
    {

    }

    virtual CBaseInfo *InputInfo(bool IsRandom = false, int nStart = 0, const char *ptag = "S");
};

class CCourseInfoManage: public CBaseInfoManage
{
    friend class CSelectInfoManage;
public:
    CCourseInfoManage(const char *pszInfoFile = ".\\Data\\CourseInfo.txt",
                    const char *pszAddTableFile = ".\\Data\\CourseAddTable.txt",
                    const char *pszDelTableFile = ".\\Data\\CourseDelTable.txt"
                   ) 
                    :CBaseInfoManage(pszInfoFile,
                                    pszAddTableFile,
                                    pszDelTableFile
                                   )
    {
        
    }

    virtual CBaseInfo *InputInfo(bool IsRandom = false, int nStart = 0, const char *ptag = "C");
};

class CTeacherInfoManage: public CBaseInfoManage
{
public:
    CTeacherInfoManage(const char *pszInfoFile = ".\\Data\\TeacherInfo.txt",
                        const char *pszAddTableFile = ".\\Data\\TeacherAddTable.txt",
                        const char *pszDelTableFile = ".\\Data\\TeacherDelTable.txt"
                        ) 
                        :CBaseInfoManage(pszInfoFile,
                                        pszAddTableFile,
                                        pszDelTableFile
                                        )
    {
        
    }

    virtual CBaseInfo *InputInfo(bool IsRandom = false, int nStart = 0, const char *ptag = "T");
};

class CSelectInfoManage: public CBaseInfoManage
{
public:
    CSelectInfoManage(const char *pszInfoFile = ".\\Data\\SelectInfo.txt",
                      const char *pszAddTableFile = ".\\Data\\SelectAddTable.txt",
                      const char *pszDelTableFile = ".\\Data\\SelectDelTable.txt"
                      ) 
                        :CBaseInfoManage(pszInfoFile,
                                        pszAddTableFile,
                                        pszDelTableFile
                                       )
    {

    }   
    
    //����pbuf����CBaseInfo
    virtual CBaseInfo *SetInfo(char *pbuf)
    {
        CSelectInfo *pBaseInfo = new CSelectInfo;
        pBaseInfo->SetInfo(pbuf);
        return pBaseInfo;
    }

    /*
    Function: ���û�����ȡһ����Ϣ���ݣ�������Ϣ����ָ��
    Effect  : �û�Ҫ�����ͷ�������ݡ�
    */
    virtual CBaseInfo *InputInfo(bool IsRandom = false, int nStart = 0, const char *ptag = "");

    /*
    Function: ���û�����ȡ�µ���Ϣ�ֶ�, ������flag, �������������Ϣ
    Param   : actiontype ��ΪQUERY����MODIFY,
              flag ���ڼ�¼�û�����Щ�ֶν���������
    Effect:   �û�Ҫ�����ͷ��������
    */
    virtual CBaseInfo *GetNewInfo(int actiontype, tag_newinfo *pflag);

    /*
    Function: ����flag, �û������pTmpInfo��ԭ��ϢpOldBaseInfo������������������Ϣ��������
    Effect  : �û�Ҫ�����ͷ��������
    */
    virtual CBaseInfo *SetNewInfo(CBaseInfo *pOldBaseInfo,
                                  CBaseInfo *pTmpInfo, 
                                  tag_newinfo flag);
    
    
    /*
    Function: ��������ĸ��ֶ�
    */
    virtual const char *ShowInfoField(void);

    /*
    Function: ���index��ָʾ����Ϣ��¼, ��pszInfo��
    */
    virtual void ShowInfo(int index, const CBaseInfo *pBaseInfo, char *pszInfo, CManage *pManage);
};


#endif