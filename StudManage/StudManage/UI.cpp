#include <stdio.h>
#include <iostream>
using namespace std;

#include "UI.h"

#define CSTYLE  //use printf, scanf instead

char g_gender[4] = {'M',       //Male
                    'F',       //Female
                    'U',       //Unknown
                    'O'        //Other
};

char g_gender_desc[4][8] = { "Male",
                            "Female",
                             "Unknown",
                             "Other"                        
};

#if 0
void
CPanel::ShowMenu(void)
{
    ShowInfo("学生信息存储管理菜单:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "1. 添加    : 添加学生信息                                      \r\n"
        "2. 查询    : 支持学生信息以及编号的多字段模糊查询              \r\n"
        "3. 删除    : 仅能删除指定编号的学生信息，请首先利用查询确认编号\r\n"
        "4. 修改    : 仅能修改指定编号的学生信息，请首先利用查询确认编号\r\n"
        "5. 显示    : 显示所有已存储学生信息                            \r\n"
        "6. 空间统计: 显示文件空间使用情况                             \r\n"
        "7. 分配方式: 默认是严格匹配(FIT_EQUAL)                        \r\n"
        "8. 碎片整理:                                                  \r\n"
        "9. 信息占比: 各学生信息大小及总体占比                        \r\n"
        "10.平均分  : 显示平均成绩                                     \r\n"
        "0. 退出                                        \r\n"
        "-------------------------------------------    \r\n"
        "请输入你的选择:             \r\n");
}

void
CPanel::ShowMenu(void)
{
    ShowInfo("学生管理系统菜单:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "              学生基本信息(无选课)                            \r\n"
        "1.添加     ：                                                 \r\n"
        "2.查询Name ：                                                 \r\n"
        "3.查询ID   ：                                                 \r\n"
        "4.修改     ：                                                 \r\n"
        "5.删除     ：                                                 \r\n"
        "              课程信息                                        \r\n"
        "6.添加     ：                                                 \r\n"
        "7.查询Name ：                                                 \r\n"
        "8.查询ID   ：                                                 \r\n"
        "9.修改     ：                                                 \r\n"
        "10.删除    ：                                                 \r\n"
        "              老师信息                                        \r\n"
        "11.添加    ：                                                 \r\n"
        "12.查询Name：                                                 \r\n"
        "13.查询ID  ：                                                 \r\n"
        "14.修改    ：                                                 \r\n"
        "15.删除    ：                                                 \r\n"
        "              选课信息                                        \r\n"
        "16.添加        ：                                             \r\n"
        "17.查询学生ID  ：                                             \r\n"
        "18.查询课程ID  ：                                             \r\n"
        "19.修改        ：                                             \r\n"        
        "20.删除        ：                                             \r\n"
        
        "0. 退出                                        \r\n"
        "-------------------------------------------    \r\n"
        "请输入你的选择:             \r\n");
}

#endif

void
CPanel::ShowMenu(void)
{
    //
}

void
CPanel::ShowMenu_Main(void)
{
    ShowInfo("学生管理系统菜单:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "1.             学生基本信息(无选课)                            \r\n"
        "2.             课程信息                                        \r\n"
        "3.             老师信息                                        \r\n"
        "4.             选课信息                                        \r\n"        
        "0.             退出                                        \r\n"
        "-------------------------------------------    \r\n"
        "请输入你的选择:             \r\n");
}

void
CPanel::ShowMenu_Sub(void)
{
    ShowInfo("-------------------------------------------          \r\n"
    "1.添加     ：                                                 \r\n"
    "2.查询Name ：                                                 \r\n"
    "3.查询ID   ：                                                 \r\n"
    "4.修改     ：                                                 \r\n"
    "5.删除     ：                                                 \r\n"
    "6.显示     ：                                             \r\n"
    "0.退出                                                        \r\n"
    "-------------------------------------------                   \r\n"
        "请输入你的选择:             \r\n");
}

void
CPanel::ShowMenu_Sub_Select(void)
{
    ShowInfo("-------------------------------------------                   \r\n"
        "1.添加           ：                                            \r\n"
        "2.查询StudID     ：                                            \r\n"
        "3.查询CourseID   ：                                            \r\n"
        "4.修改           ：                                            \r\n"
        "5.删除           ：                                            \r\n"
        "6.显示           ：                                            \r\n"
        "0.退出                                                        \r\n"
        "-------------------------------------------                   \r\n"
        "请输入你的选择:             \r\n");
}

void
CPanel::ShowMenuEn(void)
{
    ShowInfo("This is the menu for student management:  \r\n"
        "--------------------------------------------   \r\n"
        "1. Add      : store one student info           \r\n"
        "2. Query    : query student info by info or id \r\n"
        "3. Del      : remove one student by id         \r\n"
        "4. Modify   : change origin to new by id       \r\n"
        "5. Display  : show all stored student info     \r\n"
        "6. Space    : show how space are used          \r\n"
        "7. Set Fit  :  default is fitequal             \r\n"
        "8. Defrag   :  reorganize free space           \r\n"
        "9. Info     :                                  \r\n"
        "10.Score    :                                  \r\n"
        "0. exit                                        \r\n"
        "-------------------------------------------    \r\n"
        "Please enter one number to select:             \r\n");
}

void
CPanel::ShowFit(void)
{
    ShowInfo("This is the fit way you can choose:\r\n"
            "---------------------------------------\r\n"
            "0. FIT_EQUAL       : find deled space whose size just equal to your input\r\n"
            "1. FIT_FIRST       : find the first \r\n"
            "2. FIT_ENOUGH      : find the smallest that >= \r\n"
            "3. FIT_EQUAL_ENOUGH: if no equal than enough \r\n"
            "4. FIT_EQUAL_FIRST : if no equal than the first\r\n"
            );
}

void 
CPanel::GetSelect(unsigned long *pnselect)
{
    //take care of the input
#ifdef CSTYLE
    scanf("%d", pnselect);
    fflush(stdin);
#else
    cin >> *pnselect;

    //清除缓冲区，直到遇到指定的字符
    cin.ignore(cin.rdbuf()->in_avail(),     //缓冲区中剩余的个数
                '\n'
                );
#endif
}

void
GetCH(char *pch)
{
#ifdef CSTYLE
    scanf("%c", pch);
    fflush(stdin);
#else
    cin.get();
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif
}

void
GetInt(unsigned long *pint)
{
#ifdef CSTYLE
    scanf("%ud", pint);
    fflush(stdin);
#else
    cin >> *pint;
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif
}

bool 
IsValidGender(char gender)
{
    int valid = 0;
    
    if (g_gender[0] == gender
        ||g_gender[1] == gender
        || g_gender[2] == gender
        || g_gender[3] == gender
        )
    {
        return true;  
    }
    
    return false;
}

void
CPanel::GetGender(char *gender, const char *szhitinfo)
{
    char szinfo[MAX_INPUT] = {0};
    int valid = 0;
    int i     = 0;
    
    while (1)
    {
        while (i < 4)
        {
            sprintf(szinfo, "%c for %s ", g_gender[i], g_gender_desc[i]);
            ShowInfo(szinfo);
            
            i++;
        }
        ShowInfo("\r\n");
        i = 0;
        
        ShowInfo(szhitinfo);
        GetCH(gender);
        
        if (IsValidGender(*gender))
        {
            break;
        }       
        
        ShowInfo("\r\nPlease make the right decision^_^\r\n");
    }
}

void
CPanel::GetBirthday(unsigned short *birthday, const char *szhitinfo)
{
    while (1)
    {
        ShowInfo("Birthday Format is MMDD, like 1212 or 0101\r\n");
        ShowInfo(szhitinfo);

#ifdef CSTYLE
        scanf("%ud", birthday);
        fflush(stdin);
#else
        cin >> *birthday;
        cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif
        
        if (
             (*birthday <= 229)
            ||

            (   *birthday >= 101
                && *birthday <= 1231
            )
        )
        {
            break;
        }
        
        ShowInfo("\r\nPlease make the right decision^_^\r\n");
    }
}

void
CPanel::GetScore(float *fscore, const char *szhitinfo)
{
    ShowInfo(szhitinfo);

#ifdef CSTYLE
    scanf("%f", fscore);
    fflush(stdin);
#else
    cin >> *fscore;
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif
}

void
CPanel::GetInput(char *chinput, const char *szhitinfo)
{
    ShowInfo(szhitinfo);
    
#ifdef CSTYLE
    scanf("%255s", chinput);
    fflush(stdin);
#else
    //255 is for the MAX_INPUT 256
    cin.width(255);
    cin >> chinput;
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif
}

bool
CPanel::IsSure(void)
{
    int ch = 0;
#ifdef CSTYLE
    ch = getchar();
    fflush(stdin);
#else
    ch = cin.get();
    cin.ignore(cin.rdbuf()->in_avail(), '\n');
#endif

    if ('Y' == ch)
    {
        return true;
    }

    return false;
}

void 
CPanel::GetInput(float *pfinput, const char *szhitinfo)
{
    ShowInfo(szhitinfo);
    scanf("%f", pfinput);
}

void
CPanel::GetNewInfo(tag_student *student, char *szname, char *szno, unsigned long *infoid, 
                    int actiontype,
                    tag_newinfo *flag
                    )
{
    int ch = 0;
    
    ShowInfo("是否包括姓名？(Y/N)");
    if (IsSure())
    {
        GetInput(szname, "Enter Name: ");
        flag->name = 1;
    }
    
    ShowInfo("是否包括学号？(Y/N)");
    if (IsSure())
    {
        GetInput(szno,   "Enter No:");
        flag->no = 1;
    }
    
    ShowInfo("是否包括性别？(Y/N)");
    if (IsSure())
    {
        GetGender(&student->chgender, "Enter Gender: ");
        flag->gender = 1;
    }
    
    ShowInfo("是否包括生日？(Y/N)");
    if (IsSure())
    {
        GetBirthday(&student->ushbirth, "Enter Birthday(mmdd): ");
        flag->birth = 1;
    }
    
    ShowInfo("是否包括成绩？(Y/N)");
    if(IsSure())
    {
        GetScore(&student->fscore, "Enter Score: ");
        flag->score = 1;
    }
    
    if (MODIFY == actiontype)
    {
        return;
    }
    
    ShowInfo("是否包括编号？(Y/N)");
    if (IsSure())
    {
        ShowInfo("Enter id: ");
        GetInt(infoid);
        
        flag->infoid = 1;
    }    
}

void
CPanel::ShowInfo(const char *szhitinfo)
{
    //can be changed according to your own situation
#ifdef CSTYLE
    printf("%s", szhitinfo);
#else
    cout << szhitinfo;
    cout.flush();
#endif
}

void
CPanel::Show1Info(const tag_student *student, const char *szname, const char *szno, int infoid)
{
    char szhitinfo[MAX_INPUT * 2] = {0};
    
    sprintf(szhitinfo, "%d   %s   %s   %c   %d   %f\r\n",
        infoid,
        szname,
        szno,
        student->chgender,
        student->ushbirth,
        student->fscore
        );
    ShowInfo(szhitinfo);
}

void
CPanel::Continue(void)
{
    ShowInfo("Hit Enter to Continue........\r\n");
    fflush(stdin);
    getchar();
}