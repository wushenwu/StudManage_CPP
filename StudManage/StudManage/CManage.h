#ifndef CMANAGE_H_D538C7C8_F8C3_4a19_A634_7A002347D254
#define CMANAGE_H_D538C7C8_F8C3_4a19_A634_7A002347D254

#include "CInfoManage.h"

class CManage
{
    friend CSelectInfoManage;
public:
    /*
    Function: 创建新的学生管理系统，仅能创建一个对象
    */
    static CManage *CreateSystem(void);

    /*
    Function: 设置相关配置文件，开始运行系统
    Param   : pszinfofile 是学生信息存储文件
              pszaddtablefile 是添加表文件
              pszdeltablefile 是删除表文件
    Return  : SUCCESS 初始化成功，可以开始运行， 
              FAILED 初始化失败，无法继续运行
    */
    int Run(void);

    /*
    Function: 关闭系统
    */
    void End(void);

    
protected:
    void ProcessAdd(CBaseInfoManage *pBaseInfoManage);
    int ProcessDel(CBaseInfoManage *pBaseInfoManage);
    
    //查询信息，pindex用于接收查询到的记录在队列中
    bool ProcessQueryByID(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult);
    void ProcessQueryByName(CBaseInfoManage *pBaseInfoManage, CMyQueue<int> *pResultQueue, int *pnResult);

    int ProcessModify(CBaseInfoManage *pBaseInfoManage);

    /*
    Function: 系统初始化，检查信息存储文件是否存在，分配表是否存在。
              不存在则进行新建，将分配表加载到内存中
              根据分配表，读取信息数据，构建二叉树NameTree, IDTree
    Return  : SUCCESS 成功初始化
              FAILED  初始化失败
    */
    int Init(CBaseInfoManage *pBaseInfoManage);

    /*
    Function: 将信息数据添加进文件中。
    Param	: pBaseInfo包含要添加的信息数据, pBaseInfoManage管理文件等相关
    Return  :  NO_SPACE 添加失败， 其他表示添加的位置.
    Effect  : 更新g_add_table, g_del_table
  
    info_add(student, szname, szno):
    　　sizereal = 该学生全部信息长度
    　　如果没有从g_del_table中查询到合适（ 在后面阐述）的(start，size)记录，
    则在文件末尾添加信息，并在g_add_table中追加(start, size, namesize)记录。
    　　
    　　若从g_del_table中查询到一个合适的（在后面阐述) size的记录(start, start)，
    则将学生信息拷贝至start指示的文件偏移处。
    根据sizereal和size的关系，有以下几种情况：
    　　  sizereal = size, 则将(start, start)从g_del_table中删除，
    同时将(start, sizereal, namesize)有序的插入到g_add_table中 (start作为keyword)
    sizereal < size, 则将g_del_table中的该(start, size)改为(start + sizereal, size - sizereal), 
    同时，将(start, sizereal, namesize)有序的插入到g_add_table中。
      
    */
    int InfoAdd(CBaseInfo *pBaseInfo, CBaseInfoManage *pBaseInfoManage);

    //将pBaseInfoManage中的index所指示的记录予以删除。
    int InfoDel(CBaseInfoManage *pBaseInfoManage, int index);

    //对pBaseInfoManage中的index所指示的记录根据flag设置为pTmpBaseInfo所代表的新信息
    int InfoModify(CBaseInfoManage *pBaseInfoManage, 
                    CBaseInfo *pTmpBaseInfo, 
                    tag_newinfo flag, 
                    int index);


    /*
    Function: 显示所有已存储的信息 (Just For Debug)
    Param   : 
    */
    void Display(CBaseInfoManage *pBaseInfoManage);

    //将pBaseInfoManage的添加表、删除表保存
    void SaveTable(CBaseInfoManage *pBaseInfoManage);

    //Used for Debug, can be removed
    void ShowTable(CBaseInfoManage *pBaseInfoManage);

private:
    //////////////////////////////////////////////////////////////////////////
    //下面是一些流程相关
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
    Function: 处理因defrag未进行truncate, 
              利用addtable, deltable的最后一项来确定信息存储文件的大小。
    Param   : pfilesize 用于接收存储文件大小
              pstudmanage 用于提供学生管理系统相关
    Return : SUCCESS 成功  FAILED 失败
    */
    int GetFileSize(unsigned long *pfilesize, CBaseInfoManage *pBaseInfoManage);

#endif