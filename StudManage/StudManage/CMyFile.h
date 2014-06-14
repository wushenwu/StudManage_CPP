#ifndef CMyFile_H_CC91F08E_62AC_4c63_A060_570FFB2580DC

#define CMyFile_H_CC91F08E_62AC_4c63_A060_570FFB2580DC

#include "CMyString.h"
#include "Struct.h"
#include "CInfo.h"

//#define USE_FSTRSTREAM

class CMyFile
{
public:
    CMyFile() { }
    CMyFile(const char *pszfile)
        :m_filename(pszfile)
    {

    }

    /*
    Function: �����ļ��� ����ѧ����Ϣ�洢�ļ�����ӱ�ɾ����洢�ļ�
    */
    void SetFile(const char *pszfile);
    const char *GetFile(void) const;

    /*
    Function: ��m_filename��offset��ַ��ȡsize�ֽڵ�buf��
    
    Return : SUCCESS �ɹ���ȡ�� FAILED ʧ��
    */
    int ReadData(void *pbuf, unsigned long offset, unsigned char size) const /*throw (CMyFileError)*/;

    /*
    Function: ��m_filename�ļ���׷�Ӵ�СΪsize��buf
      
    Return  : SUCCESS �ɹ�д��  FAILED ʧ��
    */
    int AppendData(void *pbuf, unsigned char size);

    /*
    Function: ��m_filename�ļ���ա�
    
    Return  : SUCCESS �ɹ����  FAILED ʧ��
    */
    int ResetData(void);

    /*
    Function: ��m_filename�ļ���offset��ַ�����ݸ�дΪ��СΪsize�ֽڵ�buf.
    
    Return : SUCCESS �ɹ��޸ģ� FAILED ʧ��
    */
    int ModifyData(void *pbuf, unsigned long offset, unsigned char size) const;

    /*
    Function: ����ļ��Ƿ���ڣ� ����������ͼ�½���    
    Param   : pszfile �ļ����� ��MAX_INPUT���ƣ�
    Return  : SUCCESS ȷ�ϴ��ڻ����½��ɹ�
               NOT_EXIST �����ڣ��½�ʧ�ܣ�
    
    Remark  : ��������init�׶Σ������Ϣ�洢�ļ�����ӱ��ļ���ɾ�����ļ�
    */
    static int CheckFile(const char *pszfile) /*throw (CMyFileError)*/;

    /*
    Function: �ļ�������ÿ4�ֽڽ��У�
    Param   : 
    Return  : SUCCESS �ɹ������� FAILED ����ʧ��
    */
    static int FileCopy(const char *pfilesrc, const char *pfiledst);
    
protected:
    CMyString m_filename;
    
};

//��ΪCStudInfoFile, CCourseInfoFile, CTeacherInfoFile�Ļ���
class CBaseInfoFile: public CMyFile
{
public:
    CBaseInfoFile(const char *pszfile = "BaseInfo.txt")
        :CMyFile(pszfile)
    {

    }
public:
    /*
    Function: ����paddrecord��ȡ��Ϣ��pbuf��
    */
    virtual void
    ReadData(tag_addrecord *paddrecord,       //Ҫ��ȡ����Ϣ
            char *pbuf
            ) const;

    virtual int
    AppendData(CBaseInfo *pBaseInfo,    //Ҫ��ӵ���Ϣ
                unsigned long offset   //�ļ���ַ
               );

    virtual int
    ModifyData(CBaseInfo *pBaseInfo,   
                unsigned long offset                             
               );    
};

class CStudInfoFile: public CBaseInfoFile
{
public:
    CStudInfoFile(const char *pszfile = "StudInfo.txt")
        :CBaseInfoFile(pszfile)
    {

    }
};

class CCourseInfoFile: public CBaseInfoFile
{
public:
    CCourseInfoFile(const char *pszfile = "CourseInfo.txt")
        :CBaseInfoFile(pszfile)
    {

    }
};

class CTeacherInfoFile: public CBaseInfoFile
{
public:
    CTeacherInfoFile(const char *pszfile = "TeacherInfo.txt")
        :CBaseInfoFile(pszfile)
    {

    }
};

//ѡ����Ϣ
class CSelectInfoFile: public CBaseInfoFile
{
public:
    CSelectInfoFile(const char *pszfile = "SelectInfo.txt")
        :CBaseInfoFile(pszfile)
    {

    }

public:
#if 0
    /*
    Function: ����paddrecord��ȡ��Ϣ��������
    Effect  : ���û������ͷſռ�
    */
    virtual CBaseInfo *
    ReadData(tag_addrecord *paddrecord           //Ҫ��ȡ����Ϣ
            ) const;
    
    virtual int
    AppendData(CBaseInfo *pSelectInfo,    //Ҫ��ӵ���Ϣ
                unsigned long offset   //�ļ���ַ
                );
    
    virtual int
    ModifyData(CBaseInfo *pSelectInfo,   
                unsigned long offset                             
                );    
#endif
};

/////////////////�����ļ���أ������ļ���������ļ���///////////////////////////////////
class CBaseAddTableFile: public CMyFile
{
public:
    CBaseAddTableFile(const char *pszfile = "BaseAddTable.txt")
        :CMyFile(pszfile)
    {
    
    }
};

class CBaseDelTableFile: public CMyFile
{
public:
    CBaseDelTableFile(const char *pszfile = "BaseDelTable.txt")
        :CMyFile(pszfile)
    {

    }
};

class CBaseAddTreeFile: public CMyFile
{
public:
    CBaseAddTreeFile(const char *pszfile = "BaseAddTree.txt")
        :CMyFile(pszfile)
    {

    }
};

class CStudAddTableFile: public CBaseAddTableFile
{
public:
    CStudAddTableFile(const char *pszfile = "StudAddTable.txt")
        :CBaseAddTableFile(pszfile)
    {

    }
};

class CStudDelTableFile: public CBaseDelTableFile
{
public:
    CStudDelTableFile(const char *pszfile = "StudDelTable.txt")
        :CBaseDelTableFile(pszfile)
    {

    }
};

class CStudAddTreeFile: public CBaseAddTreeFile
{
public:
    CStudAddTreeFile(const char *pszfile = "StudAddTree.txt")
        :CBaseAddTreeFile(pszfile)
    {

    }
};

class CCourseAddTableFile: public CBaseAddTableFile
{
public:
    CCourseAddTableFile(const char *pszfile = "CourseAddTable.txt")
        :CBaseAddTableFile(pszfile)
    {

    }
};

class CCourseDelTableFile: public CBaseDelTableFile
{
public:
    CCourseDelTableFile(const char *pszfile = "CourseDelTable.txt")
        :CBaseDelTableFile(pszfile)
    {

    }
};

class CCourseAddTreeFile: public CBaseAddTreeFile
{
public:
    CCourseAddTreeFile(const char *pszfile = "CourseAddTree.txt")
        :CBaseAddTreeFile(pszfile)
    {

    }
};

class CTeacherAddTableFile: public CBaseAddTableFile
{
public:
    CTeacherAddTableFile(const char *pszfile = "TeacherAddTable.txt")
        :CBaseAddTableFile(pszfile)
    {

    }
};

class CTeacherDelTableFile: public CBaseDelTableFile
{
public:
    CTeacherDelTableFile(const char *pszfile = "TeacherDelTable.txt")
        :CBaseDelTableFile(pszfile)
    {

    }
};

class CTeacherAddTreeFile: public CBaseAddTreeFile
{
public:
    CTeacherAddTreeFile(const char *pszfile = "TeacherAddTree.txt")
        :CBaseAddTreeFile(pszfile)
    {

    }
};

class CSelectAddTableFile: public CBaseAddTableFile
{
public:
    CSelectAddTableFile(const char *pszfile = "SelectAddTable.txt")
        :CBaseAddTableFile(pszfile)
    {

    }
};

class CSelectDelTableFile: public CBaseDelTableFile
{
public:
    CSelectDelTableFile(const char *pszfile = "SelectDelTable.txt")
        :CBaseDelTableFile(pszfile)
    {

    }
};

class CSelectAddTreeFile: public CBaseAddTreeFile
{
public:
    CSelectAddTreeFile(const char *pszfile = "SelectAddTree.txt")
        :CBaseAddTreeFile(pszfile)
    {

    }
};

//////////////////////////////////////////////////////////////////////////


//ѧ����Ϣ�洢�ļ����
class CInfoFile: public CMyFile       
{
public:
    CInfoFile(const char *pszfile = "info.txt");

    int
    ReadData(tag_student *pstudent, char *pszname, char *pszno,  //���ն�ȡ��ѧ������Ϣ
             tag_addrecord *paddrecord                      //Ҫ��ȡ��ѧ����Ϣ
              ) const;

    int
    AppendData(tag_student *pstudent, char *pszname, char *pszno,   //ѧ����Ϣ
               unsigned long offset,                             //�ļ���ַ
               unsigned char namesize,                          //ѧ������ռ�ÿռ��С��
               unsigned char nosize);                           // 

    /*
    Function: ��student, szname, sznoָ����ѧ����Ϣ������ļ���
    Param   : tag_student *student, char *szname, char *szno,   //ѧ����Ϣ
            unsigned long start,                              //���봫�����ļ���ַ
            unsigned char namesize ,                          //ѧ������ռ�ÿռ��С
            unsigned char nosize                              //ѧ��ѧ��ռ�ÿռ��С

    Return  : SUCCESS �ɹ��洢  FAILED  �洢ʧ��
    */
    int
    ModifyData(tag_student *pstudent, char *szname, char *szno,   
                unsigned long offset,                              
                unsigned char namesize ,                          
                unsigned char nosize
              );
   
};

//��ӱ�洢�ļ����
class CAddTableFile: public CMyFile
{
public:
    CAddTableFile(const char *pszfile = "addtable.txt");

};

//ɾ����洢�ļ����
class CDelTableFile: public CMyFile
{
public:
    CDelTableFile(const char *pszfile = "deltable.txt");
};

#endif

