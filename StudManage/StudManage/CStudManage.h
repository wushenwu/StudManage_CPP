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
    Function: 创建新的学生信息管理系统，仅能创建一个对象
    */
    static CStudManage *CreateSystem(void);

    /*
    Function: 设置相关配置文件，开始运行系统
    Param   : pszinfofile 是学生信息存储文件
              pszaddtablefile 是添加表文件
              pszdeltablefile 是删除表文件
    Return  : SUCCESS 初始化成功，可以开始运行， 
              FAILED 初始化失败，无法继续运行
    */
    int Run(const char *pszinfofile = "info.txt", 
            const char *pszaddtablefile = "addtable.txt", 
            const char *pszdeltablefile = "deltable.txt");

    /*
    Function: 关闭系统
    */
    void End(void);

protected:
    CStudManage();

    ~CStudManage();

public:
    /*
    Function: 将用户输入的学生信息添加进文件中。
    Param	: student中存放着学生定长信息，szname, szno为变长的姓名、学号
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
    int InfoAdd(tag_student *student, char *szname, char *szno);

    /*
    Function: 对用户输入的学生信息、编号进行模糊查询、联合查询，确认相关学生编号
    
    Param   : tag_student *student, char *szname, char *szno,    //要查询的学生信息
            unsigned long id,                                  //要查询的编号
            int *indexary,                                     //接收查询到的条目编号（下标）
            int *num                                           //查询匹配到的总条目
            tag_newinfo *flag                                  //相应字段是否进行了设置

    Return  : SUCCESS存在，并设置indexary.
    NOT_EXIST 不存在， 
    FAILED  查询失败
        
    */
    int
    InfoQueryAbout(tag_student *student, char *szname, char *szno,   
                    unsigned long infoid,                                  
                    int *indexary,                                     
                    int *num,                                           
                    tag_newinfo *flag
                    );

    /*
    Function: 根据信息id进行查询，确认是否存在相关学生，
                主要用于学生信息的修改、删除操作。 
    Param	: 编号id, 该id是根据现存学生记录，动态变化的，需要用户根据模糊查询结果进行确认。
    (结果唯一)
    Return  :  NOT_EXIST不存在， SUCCESS存在,
    */
    int 
    InfoQueryById(unsigned long infoid);

    /*
    Function: 删除指定编号的学生信息  
    Param	: infoid为编号（信息编号，随g_add_table的变化而变化），用户需用模糊查询确认。　        
    Return  :  NOT_EXIST for failed， SUCCESS 成功.
    Effect  : 更新g_add_table, 
      更新g_del_table， 根据情况对空闲空间进行整合，影响g_del_table
      
        info_del(infoid):　
        将该记录从g_add_table中删除，
        同时考虑更新有序表g_del_table. 
        考虑到可能的连续空闲空间的整合，分为以下情况：
        　　    如果有合适的前驱(size1, start1) 满足 start1 + size1 = size, 表明该两块空闲空间连续，则应将(size1, start1)更新为(size1+size, start1)
        　　    如果(size,start)有合适的后继(size2, start2)满足 start + size = start2, 则(size2, start2)应更新为(size+size2, start)
    */
    int
    InfoDel(int infoid);

    /*
    Function: 修改指定编号的学生信息　  
    Param	: tag_student *student, char *szname, char *szno,  为新的信息
                int infoid 指定编号
                tag_newinfo *flag  指示各字段是否设置        
    Return  :  NOT_EXIST for not exist, NO_SPACE for no space, SUCCESS for success
    
      Effect  : 更新g_add_table, 
      更新g_del_table， 根据情况对空闲空间进行整合，影响g_del_table
      
        info_modify(origin, new):
        　　index = info_query(origin)
        　　如果index == -1 表明不存在origin, 修改失败。
        　　oldsize = len(origin)
        　　newsize = len(new)
        　　如果index != -1, 它表示origin的(size, start)在g_add_table中的下标，取得start.
        　　根据oldsize和newsize的大小，分为以下情况：
        　　如果 newsize < oldsize, 则更改g_add_table中的(size, start) 为(newsize, start), 同时在g_del_table中有序的插入(oldsize - newsize, start + newsize)
        　　如果 newsize = oldsize, 则不发生变更
        　　如果newsize > oldsize, 则此时可以简单调用的info_del(origin)和info_add(string), 完成空间的释放和再整合，以及再分配。
        （当然，也可能出现该start前驱或者后继本来已经为空闲的情况，那么此时进行空闲的整合是最好的，但是我们还是将其纳入一般的流程进行处理）。
    */
    int
    InfoModify(tag_student *student, char *szname, char *szno, int infoid, tag_newinfo *flag);

    /*
    Function: 显示所有已存储的学生信息
    Param   : pfnshowinfo需用户提供的，用于显示的接口
    */
    void Display();

    /*
    对文件存储空间的统计显示
    */
    void SpaceStat(void);

    //碎片整理
    void Defrag(void);

    //统计各学生信息对整体存储文件的占比
    void InfoStat(void);

    //获取学生平均分，fscore用于接收平均分
    int GetAverScore(float *fscore);

    //将添加表、删除表保存
    void SaveTable(void);

    const CInfoFile *GetInfoFile(void) const;
    const CAddTableFile *GetAddTableFile(void) const;
    const CDelTableFile *GetDelTableFile(void) const;
    CAddTable *GetAddTable(void) const;
    CDelTable *GetDelTable(void) const;

private:
    /*
    Function: 系统初始化，检查学生信息存储文件是否存在，分配表是否存在。
              不存在则进行新建，将分配表加载到内存中
    Return  : SUCCESS 成功初始化
              FAILED  初始化失败
    */
    int Init(void);

    /*
    Function: 系统退出前的处理，备份分配表文件，将内存中的分配表写入到文件中
    */
    int Finish(void);

    //Nothing
    void SetNULL(void);

    //////////////////////////////////////////////////////////////////////////
    //下面是一些流程相关

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
Function: 处理因defrag未进行truncate, 
          利用addtable, deltable的最后一项来确定信息存储文件的大小。
Param   : pfilesize 用于接收存储文件大小
          pstudmanage 用于提供学生管理系统相关
Return : SUCCESS 成功  FAILED 失败
*/
int GetFileSize(unsigned long *pfilesize, const CStudManage *pstudmanage);

/*
Function: get the table that index belongs to, and set the max value of this range.
            用于space_stat 中设置使用标志U, F
Param   : buf is the storage buf.
          index_add, index_del 为space_stat所传进的参数，
          代表目前对g_add_del, g_del_table所遍历到的下标。
          NO_USE_MALLOC, USE_MALLOC_V1中恒为(0,0)
  
          pstudmanage 用于提供学生信息系统相关信息
*/
int
gettable(unsigned long index, unsigned long *max, 
         unsigned long index_add, unsigned long index_del,
         const CStudManage *pstudmanage);

#endif
