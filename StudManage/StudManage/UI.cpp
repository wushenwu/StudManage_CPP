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
    ShowInfo("ѧ����Ϣ�洢����˵�:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "1. ���    : ���ѧ����Ϣ                                      \r\n"
        "2. ��ѯ    : ֧��ѧ����Ϣ�Լ���ŵĶ��ֶ�ģ����ѯ              \r\n"
        "3. ɾ��    : ����ɾ��ָ����ŵ�ѧ����Ϣ�����������ò�ѯȷ�ϱ��\r\n"
        "4. �޸�    : �����޸�ָ����ŵ�ѧ����Ϣ�����������ò�ѯȷ�ϱ��\r\n"
        "5. ��ʾ    : ��ʾ�����Ѵ洢ѧ����Ϣ                            \r\n"
        "6. �ռ�ͳ��: ��ʾ�ļ��ռ�ʹ�����                             \r\n"
        "7. ���䷽ʽ: Ĭ�����ϸ�ƥ��(FIT_EQUAL)                        \r\n"
        "8. ��Ƭ����:                                                  \r\n"
        "9. ��Ϣռ��: ��ѧ����Ϣ��С������ռ��                        \r\n"
        "10.ƽ����  : ��ʾƽ���ɼ�                                     \r\n"
        "0. �˳�                                        \r\n"
        "-------------------------------------------    \r\n"
        "���������ѡ��:             \r\n");
}

void
CPanel::ShowMenu(void)
{
    ShowInfo("ѧ������ϵͳ�˵�:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "              ѧ��������Ϣ(��ѡ��)                            \r\n"
        "1.���     ��                                                 \r\n"
        "2.��ѯName ��                                                 \r\n"
        "3.��ѯID   ��                                                 \r\n"
        "4.�޸�     ��                                                 \r\n"
        "5.ɾ��     ��                                                 \r\n"
        "              �γ���Ϣ                                        \r\n"
        "6.���     ��                                                 \r\n"
        "7.��ѯName ��                                                 \r\n"
        "8.��ѯID   ��                                                 \r\n"
        "9.�޸�     ��                                                 \r\n"
        "10.ɾ��    ��                                                 \r\n"
        "              ��ʦ��Ϣ                                        \r\n"
        "11.���    ��                                                 \r\n"
        "12.��ѯName��                                                 \r\n"
        "13.��ѯID  ��                                                 \r\n"
        "14.�޸�    ��                                                 \r\n"
        "15.ɾ��    ��                                                 \r\n"
        "              ѡ����Ϣ                                        \r\n"
        "16.���        ��                                             \r\n"
        "17.��ѯѧ��ID  ��                                             \r\n"
        "18.��ѯ�γ�ID  ��                                             \r\n"
        "19.�޸�        ��                                             \r\n"        
        "20.ɾ��        ��                                             \r\n"
        
        "0. �˳�                                        \r\n"
        "-------------------------------------------    \r\n"
        "���������ѡ��:             \r\n");
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
    ShowInfo("ѧ������ϵͳ�˵�:                                    \r\n"
        "--------------------------------------------------------------\r\n"
        "1.             ѧ��������Ϣ(��ѡ��)                            \r\n"
        "2.             �γ���Ϣ                                        \r\n"
        "3.             ��ʦ��Ϣ                                        \r\n"
        "4.             ѡ����Ϣ                                        \r\n"        
        "0.             �˳�                                        \r\n"
        "-------------------------------------------    \r\n"
        "���������ѡ��:             \r\n");
}

void
CPanel::ShowMenu_Sub(void)
{
    ShowInfo("-------------------------------------------          \r\n"
    "1.���     ��                                                 \r\n"
    "2.��ѯName ��                                                 \r\n"
    "3.��ѯID   ��                                                 \r\n"
    "4.�޸�     ��                                                 \r\n"
    "5.ɾ��     ��                                                 \r\n"
    "6.��ʾ     ��                                             \r\n"
    "0.�˳�                                                        \r\n"
    "-------------------------------------------                   \r\n"
        "���������ѡ��:             \r\n");
}

void
CPanel::ShowMenu_Sub_Select(void)
{
    ShowInfo("-------------------------------------------                   \r\n"
        "1.���           ��                                            \r\n"
        "2.��ѯStudID     ��                                            \r\n"
        "3.��ѯCourseID   ��                                            \r\n"
        "4.�޸�           ��                                            \r\n"
        "5.ɾ��           ��                                            \r\n"
        "6.��ʾ           ��                                            \r\n"
        "0.�˳�                                                        \r\n"
        "-------------------------------------------                   \r\n"
        "���������ѡ��:             \r\n");
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

    //�����������ֱ������ָ�����ַ�
    cin.ignore(cin.rdbuf()->in_avail(),     //��������ʣ��ĸ���
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
    
    ShowInfo("�Ƿ����������(Y/N)");
    if (IsSure())
    {
        GetInput(szname, "Enter Name: ");
        flag->name = 1;
    }
    
    ShowInfo("�Ƿ����ѧ�ţ�(Y/N)");
    if (IsSure())
    {
        GetInput(szno,   "Enter No:");
        flag->no = 1;
    }
    
    ShowInfo("�Ƿ�����Ա�(Y/N)");
    if (IsSure())
    {
        GetGender(&student->chgender, "Enter Gender: ");
        flag->gender = 1;
    }
    
    ShowInfo("�Ƿ�������գ�(Y/N)");
    if (IsSure())
    {
        GetBirthday(&student->ushbirth, "Enter Birthday(mmdd): ");
        flag->birth = 1;
    }
    
    ShowInfo("�Ƿ�����ɼ���(Y/N)");
    if(IsSure())
    {
        GetScore(&student->fscore, "Enter Score: ");
        flag->score = 1;
    }
    
    if (MODIFY == actiontype)
    {
        return;
    }
    
    ShowInfo("�Ƿ������ţ�(Y/N)");
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