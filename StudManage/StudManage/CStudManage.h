#ifndef CSTUDMANAGE_H_CEA8D834_C30F_4c2f_B092_2FC6B0159058

#define CSTUDMANAGE_H_CEA8D834_C30F_4c2f_B092_2FC6B0159058

#include "CMyFile.h"
#include "CTable.h"
#include "UI.h"

class CInfoFile;
class CAddTableFile;
class CDelTableFile;

class CStudManage
{
public:
    /*
    Function: �����µ�ѧ����Ϣ����ϵͳ�����ܴ���һ������
    */
    static CStudManage *CreateSystem(void);

    /*
    Function: ������������ļ�����ʼ����ϵͳ
    Param   : pszinfofile ��ѧ����Ϣ�洢�ļ�
              pszaddtablefile ����ӱ��ļ�
              pszdeltablefile ��ɾ�����ļ�
    Return  : SUCCESS ��ʼ���ɹ������Կ�ʼ���У� 
              FAILED ��ʼ��ʧ�ܣ��޷���������
    */
    int Run(const char *pszinfofile = "info.txt", 
            const char *pszaddtablefile = "addtable.txt", 
            const char *pszdeltablefile = "deltable.txt");

    /*
    Function: �ر�ϵͳ
    */
    void End(void);

protected:
    CStudManage();

    ~CStudManage();

public:
    /*
    Function: ���û������ѧ����Ϣ��ӽ��ļ��С�
    Param	: student�д����ѧ��������Ϣ��szname, sznoΪ�䳤��������ѧ��
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
    int InfoAdd(tag_student *student, char *szname, char *szno);

    /*
    Function: ���û������ѧ����Ϣ����Ž���ģ����ѯ�����ϲ�ѯ��ȷ�����ѧ�����
    
    Param   : tag_student *student, char *szname, char *szno,    //Ҫ��ѯ��ѧ����Ϣ
            unsigned long id,                                  //Ҫ��ѯ�ı��
            int *indexary,                                     //���ղ�ѯ������Ŀ��ţ��±꣩
            int *num                                           //��ѯƥ�䵽������Ŀ
            tag_newinfo *flag                                  //��Ӧ�ֶ��Ƿ����������

    Return  : SUCCESS���ڣ�������indexary.
    NOT_EXIST �����ڣ� 
    FAILED  ��ѯʧ��
        
    */
    int
    InfoQueryAbout(tag_student *student, char *szname, char *szno,   
                    unsigned long infoid,                                  
                    int *indexary,                                     
                    int *num,                                           
                    tag_newinfo *flag
                    );

    /*
    Function: ������Ϣid���в�ѯ��ȷ���Ƿ�������ѧ����
                ��Ҫ����ѧ����Ϣ���޸ġ�ɾ�������� 
    Param	: ���id, ��id�Ǹ����ִ�ѧ����¼����̬�仯�ģ���Ҫ�û�����ģ����ѯ�������ȷ�ϡ�
    (���Ψһ)
    Return  :  NOT_EXIST�����ڣ� SUCCESS����,
    */
    int 
    InfoQueryById(unsigned long infoid);

    /*
    Function: ɾ��ָ����ŵ�ѧ����Ϣ  
    Param	: infoidΪ��ţ���Ϣ��ţ���g_add_table�ı仯���仯�����û�����ģ����ѯȷ�ϡ���        
    Return  :  NOT_EXIST for failed�� SUCCESS �ɹ�.
    Effect  : ����g_add_table, 
      ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table
      
        info_del(infoid):��
        ���ü�¼��g_add_table��ɾ����
        ͬʱ���Ǹ��������g_del_table. 
        ���ǵ����ܵ��������пռ�����ϣ���Ϊ���������
        ����    ����к��ʵ�ǰ��(size1, start1) ���� start1 + size1 = size, ������������пռ���������Ӧ��(size1, start1)����Ϊ(size1+size, start1)
        ����    ���(size,start)�к��ʵĺ��(size2, start2)���� start + size = start2, ��(size2, start2)Ӧ����Ϊ(size+size2, start)
    */
    int
    InfoDel(int infoid);

    /*
    Function: �޸�ָ����ŵ�ѧ����Ϣ��  
    Param	: tag_student *student, char *szname, char *szno,  Ϊ�µ���Ϣ
                int infoid ָ�����
                tag_newinfo *flag  ָʾ���ֶ��Ƿ�����        
    Return  :  NOT_EXIST for not exist, NO_SPACE for no space, SUCCESS for success
    
      Effect  : ����g_add_table, 
      ����g_del_table�� ��������Կ��пռ�������ϣ�Ӱ��g_del_table
      
        info_modify(origin, new):
        ����index = info_query(origin)
        �������index == -1 ����������origin, �޸�ʧ�ܡ�
        ����oldsize = len(origin)
        ����newsize = len(new)
        �������index != -1, ����ʾorigin��(size, start)��g_add_table�е��±꣬ȡ��start.
        ��������oldsize��newsize�Ĵ�С����Ϊ���������
        ������� newsize < oldsize, �����g_add_table�е�(size, start) Ϊ(newsize, start), ͬʱ��g_del_table������Ĳ���(oldsize - newsize, start + newsize)
        ������� newsize = oldsize, �򲻷������
        �������newsize > oldsize, ���ʱ���Լ򵥵��õ�info_del(origin)��info_add(string), ��ɿռ���ͷź������ϣ��Լ��ٷ��䡣
        ����Ȼ��Ҳ���ܳ��ָ�startǰ�����ߺ�̱����Ѿ�Ϊ���е��������ô��ʱ���п��е���������õģ��������ǻ��ǽ�������һ������̽��д�����
    */
    int
    InfoModify(tag_student *student, char *szname, char *szno, int infoid, tag_newinfo *flag);

    /*
    Function: ��ʾ�����Ѵ洢��ѧ����Ϣ
    Param   : pfnshowinfo���û��ṩ�ģ�������ʾ�Ľӿ�
    */
    void Display();

    /*
    ���ļ��洢�ռ��ͳ����ʾ
    */
    void SpaceStat(void);

    //��Ƭ����
    void Defrag(void);

    //ͳ�Ƹ�ѧ����Ϣ������洢�ļ���ռ��
    void InfoStat(void);

    //��ȡѧ��ƽ���֣�fscore���ڽ���ƽ����
    int GetAverScore(float *fscore);

    //����ӱ�ɾ������
    void SaveTable(void);

    const CInfoFile *GetInfoFile(void) const;
    const CAddTableFile *GetAddTableFile(void) const;
    const CDelTableFile *GetDelTableFile(void) const;
    CAddTable *GetAddTable(void) const;
    CDelTable *GetDelTable(void) const;

private:
    /*
    Function: ϵͳ��ʼ�������ѧ����Ϣ�洢�ļ��Ƿ���ڣ�������Ƿ���ڡ�
              ������������½������������ص��ڴ���
    Return  : SUCCESS �ɹ���ʼ��
              FAILED  ��ʼ��ʧ��
    */
    int Init(void);

    /*
    Function: ϵͳ�˳�ǰ�Ĵ������ݷ�����ļ������ڴ��еķ����д�뵽�ļ���
    */
    int Finish(void);

    //Nothing
    void SetNULL(void);

    //////////////////////////////////////////////////////////////////////////
    //������һЩ�������

    void Dispatch(int nselect);
    void ProcessAdd(void);
    void ProcessQuery(void);
    int ProcessDel(void);
    int ProcessModify(void);
    void ProcessFit(void);

    //used for debug
    void ShowTable(void);
    
private:
    CInfoFile *m_pinfofile;
    CAddTableFile *m_paddtablefile;
    CDelTableFile *m_pdeltablefile;

    CAddTable *m_paddtable;
    CDelTable *m_pdeltable;   

};

/*
Function: ������defragδ����truncate, 
          ����addtable, deltable�����һ����ȷ����Ϣ�洢�ļ��Ĵ�С��
Param   : pfilesize ���ڽ��մ洢�ļ���С
          pstudmanage �����ṩѧ������ϵͳ���
Return : SUCCESS �ɹ�  FAILED ʧ��
*/
int GetFileSize(unsigned long *pfilesize, const CStudManage *pstudmanage);

/*
Function: get the table that index belongs to, and set the max value of this range.
            ����space_stat ������ʹ�ñ�־U, F
Param   : buf is the storage buf.
          index_add, index_del Ϊspace_stat�������Ĳ�����
          ����Ŀǰ��g_add_del, g_del_table�����������±ꡣ
          NO_USE_MALLOC, USE_MALLOC_V1�к�Ϊ(0,0)
  
          pstudmanage �����ṩѧ����Ϣϵͳ�����Ϣ
*/
int
gettable(unsigned long index, unsigned long *max, 
         unsigned long index_add, unsigned long index_del,
         const CStudManage *pstudmanage);

#endif
