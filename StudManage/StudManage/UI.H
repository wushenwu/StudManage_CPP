#ifndef UI_H_AC585B18_21AE_4066_87E8_F057F3B7A2AB

#define UI_H_AC585B18_21AE_4066_87E8_F057F3B7A2AB

#include "Struct.h"
#include "Const.h"

class CPanel
{
public:
    /*
    Function: menu options for user to select.
    */
    static void ShowMenu(void);

    static void ShowMenuEn(void);

    static void ShowMenu_Main(void);
    static void ShowMenu_Sub(void);
    static void ShowMenu_Sub_Select(void);

    /*
    Function: show fit ways you can choose 
    */
    static void ShowFit(void);

    /*
    Function: show info for users
          encapsulation of printf
    */
    static void ShowInfo(const char *szhitinfo);

    /*
    Function: show one specific student info.
    */
    static void
    Show1Info(const tag_student *student, const char *szname, const char *szno, int infoid);

    /*
    Function: deal with user selection for menu
    */
    static void GetSelect(unsigned long *pnselect);

    /*
    Function: deal with gender input
    */
    static void GetGender(char *gender, const char *szhitinfo);

    /*
    Function: deal with birthday input (mmdd)
    */
    static void GetBirthday(unsigned short *birthday, const char *szhitinfo);

    /*
    Function: deal with score input
    */
    static void GetScore(float *fscore, const char *szhitinfo);

    /*
    Function: deal with user input,
                like string to add, del, query, or modify
    */
    static void GetInput(char *chinput, const char *szhitinfo);

    static void GetInput(float *pfinput, const char *szhitinfo);

    /*
    Function: Hit Enter to continue
    */
    static void Continue(void);

    /*
    Function: 确认是否进行删除或者修改操作 
    */
    static bool IsSure(void);
   
    /*
    Function: get new info to query or modify
              如果输入了相应的字段，则设置tag_newinfo *flag.
    Param   : actiontype 可能为MODIFY, QUERY
    */
    static void
    GetNewInfo(tag_student *student, char *szname, char *szno, unsigned long *infoid, 
               int actiontype,
               tag_newinfo *flag
               ); 

    
};

class CYourPanel: public CPanel
{
public:
    //rewrite the method as you will
};
#endif