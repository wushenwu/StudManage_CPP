#ifndef CMANAGE_H_D538C7C8_F8C3_4a19_A634_7A002347D254
#define CMANAGE_H_D538C7C8_F8C3_4a19_A634_7A002347D254

#include "CInfoManage.h"

class CManage
{
    friend CSelectInfoManage;
public:
    /*
    Function: �����µ�ѧ������ϵͳ�����ܴ���һ������
    */
    static CManage *CreateSystem(void);

    /*
    Function: ������������ļ�����ʼ����ϵͳ
    Param   : pszinfofile ��ѧ����Ϣ�洢�ļ�
              pszaddtablefile ����ӱ��ļ�
              pszdeltablefile ��ɾ�����ļ�
    Return  : SUCCESS ��ʼ���ɹ������Կ�ʼ���У� 
              FAILED ��ʼ��ʧ�ܣ��޷���������
    */
    int Run(void);

    /*
    Function: �ر�ϵͳ
    */
    void End(void);

    
protected:
    void ProcessAdd(CBaseInfoManage *pBaseInfoManage);
    int ProcessDel(CBaseInfoManage *pBaseInfoManage);
    
    //��ѯ��Ϣ��pindex���ڽ��ղ�ѯ���ļ�¼�ڶ�����
    bool ProcessQueryByID(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult);
    void ProcessQueryByName(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult);

    int ProcessModify(CBaseInfoManage *pBaseInfoManage);

    /*
    Function: ϵͳ��ʼ���������Ϣ�洢�ļ��Ƿ���ڣ�������Ƿ���ڡ�
              ������������½������������ص��ڴ���
              ���ݷ������ȡ��Ϣ���ݣ�����������NameTree, IDTree
    Return  : SUCCESS �ɹ���ʼ��
              FAILED  ��ʼ��ʧ��
    */
    int Init(CBaseInfoManage *pBaseInfoManage);

    /*
    Function: ����Ϣ������ӽ��ļ��С�
    Param	: pBaseInfo����Ҫ��ӵ���Ϣ����, pBaseInfoManage�����ļ������
    Return  :  NO_SPACE ���ʧ�ܣ� ������ʾ��ӵ�λ��.
    Effect  : ����g_add_table, g_del_table
  
    info_add(student, szname, szno):
    ����sizereal = ��ѧ��ȫ����Ϣ����
    �������û�д�g_del_table�в�ѯ�����ʣ� �ں����������(start��size)��¼��
    �����ļ�ĩβ�����Ϣ������g_add_table��׷��(start, size, namesize)��¼��
    ����
    ��������g_del_table�в�ѯ��һ�����ʵģ��ں������) size�ļ�¼(start, start)��
    ��ѧ����Ϣ������startָʾ���ļ�ƫ�ƴ���
    ����sizereal��size�Ĺ�ϵ�������¼��������
    ����  sizereal = size, ��(start, start)��g_del_table��ɾ����
    ͬʱ��(start, sizereal, namesize)����Ĳ��뵽g_add_table�� (start��Ϊkeyword)
    sizereal < size, ��g_del_table�еĸ�(start, size)��Ϊ(start + sizereal, size - sizereal), 
    ͬʱ����(start, sizereal, namesize)����Ĳ��뵽g_add_table�С�
      
    */
    int InfoAdd(CBaseInfo *pBaseInfo, CBaseInfoManage *pBaseInfoManage);

    //��pBaseInfoManage�е�index��ָʾ�ļ�¼����ɾ����
    int InfoDel(CBaseInfoManage *pBaseInfoManage, int index);

    //��pBaseInfoManage�е�index��ָʾ�ļ�¼����flag����ΪpTmpBaseInfo�����������Ϣ
    int InfoModify(CBaseInfoManage *pBaseInfoManage, 
                    CBaseInfo *pTmpBaseInfo, 
                    tag_newinfo flag, 
                    int index);


    /*
    Function: ��ʾ�����Ѵ洢����Ϣ (Just For Debug)
    Param   : 
    */
    void Display(CBaseInfoManage *pBaseInfoManage);

    //��pBaseInfoManage����ӱ�ɾ������
    void SaveTable(CBaseInfoManage *pBaseInfoManage);

    //Used for Debug, can be removed
    void ShowTable(CBaseInfoManage *pBaseInfoManage);

private:
    //////////////////////////////////////////////////////////////////////////
    //������һЩ�������
    void Dispatch(unsigned long nselect);

protected:
    CManage() {}
    ~CManage () {}

protected:
    CStudInfoManage m_StudInfoManage;    
    CCourseInfoManage m_CourseInfoManage;
    CTeacherInfoManage m_TeacherInfoManage;
    CSelectInfoManage m_SelectInfoManage;    
};

    /*
    Function: ������defragδ����truncate, 
              ����addtable, deltable�����һ����ȷ����Ϣ�洢�ļ��Ĵ�С��
    Param   : pfilesize ���ڽ��մ洢�ļ���С
              pstudmanage �����ṩѧ������ϵͳ���
    Return : SUCCESS �ɹ�  FAILED ʧ��
    */
    int GetFileSize(unsigned long *pfilesize, CBaseInfoManage *pBaseInfoManage);

#endif