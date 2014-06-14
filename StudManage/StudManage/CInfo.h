/*
数据关系：
学生信息 (学生ID、姓名)
课程信息 (课程ID、名称)
老师信息 (老师ID、姓名)  
选课信息 ( (课程ID、学生ID)、 成绩)  

*/

#ifndef CINFO_H_69C95585_D27F_4de7_B8C9_E8570DFF36EE

#define CINFO_H_69C95585_D27F_4de7_B8C9_E8570DFF36EE

#include <string.h>
#include "CMyString.h"

class CBaseInfo
{
public:
    CBaseInfo(const char *pszname = "", const char *pszno = "")
        :m_strName(pszname), m_strID(pszno)
    {
        
    }
public:
    const char *GetName(void) const { return m_strName.GetString(); }
    const char *GetID(void) const { return m_strID.GetString(); }

    void SetName(const char *pszName) { m_strName.SetString(pszName); }
    void SetID(const char *pszID) { m_strID.SetString(pszID); }

    //将Info信息存放到pbuf中
    virtual void GetInfo(char *pbuf)
    {
        const char *pszName = m_strName.GetString();
        const char *pszID = m_strID.GetString();
        size_t namesize = strlen(pszName) + 1; //保持一致
        size_t nosize   = strlen(pszID) + 1;    //保持一致
          
        memcpy(pbuf, pszName, namesize);
        memcpy(pbuf + namesize, pszID, nosize);
    }

    //从pbuf中设置Info
    virtual void SetInfo(char *pbuf)
    {
        SetName(pbuf);
        SetID(&pbuf[strlen(pbuf) + 1]);
    }

    //返回pBaseInfo所占总空间大小
    virtual unsigned char GetInfoSize()
    {
        return m_strName.GetLen() + 1 + m_strID.GetLen() + 1;
    }

protected:
    CMyString  m_strName; 
    CMyString m_strID;   
};

class CStudInfo: public CBaseInfo
{
    //Add your data here
};

class CCourseInfo: public CBaseInfo
{
    //Add your data here
};

class CTeacherInfo: public CBaseInfo
{
    //Add your data here
};

class CSelectInfo: public CBaseInfo
{
public:
    CSelectInfo() { m_fScore = 0.0f; }
    
    //const char *GetStudID(void) const { return m_strName.GetString(); }
    //const char *GetCourseID(void) const { return m_strID.GetString(); } 
    //void SetStudID(const char *pszName) { m_strName.SetString(pszName); }
    //void SetCourseID(const char *pszID) { m_strID.SetString(pszID); }

    float GetScore(void) const { return m_fScore; }
    void SetScore(float fScore) { m_fScore = fScore; }

    virtual void GetInfo(char *pbuf)
    {
        CBaseInfo::GetInfo(pbuf);
        memcpy(pbuf + CBaseInfo::GetInfoSize(), &m_fScore, sizeof(float));
    }

    virtual void SetInfo(char *pbuf)
    {
        CBaseInfo::SetInfo(pbuf);
        float *pfscore = (float *)(pbuf + CBaseInfo::GetInfoSize());
        SetScore(*pfscore);
    }

    //返回pBaseInfo所占总空间大小
    virtual unsigned char GetInfoSize()
    {
        return CBaseInfo::GetInfoSize() + sizeof(float);
    }

protected:
    //CMyString *m_pstrStudID;      //利用CBaseInfo::m_strName来进行模拟
    //CMyString *m_pstrCourseID;    //利用CBaseInfo::m_strID来进行模拟
    float m_fScore;
};

#endif