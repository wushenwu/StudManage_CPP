/*
���ݹ�ϵ��
ѧ����Ϣ (ѧ��ID������)
�γ���Ϣ (�γ�ID������)
��ʦ��Ϣ (��ʦID������)  
ѡ����Ϣ ( (�γ�ID��ѧ��ID)�� �ɼ�)  

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

    //��Info��Ϣ��ŵ�pbuf��
    virtual void GetInfo(char *pbuf)
    {
        const char *pszName = m_strName.GetString();
        const char *pszID = m_strID.GetString();
        size_t namesize = strlen(pszName) + 1; //����һ��
        size_t nosize   = strlen(pszID) + 1;    //����һ��
          
        memcpy(pbuf, pszName, namesize);
        memcpy(pbuf + namesize, pszID, nosize);
    }

    //��pbuf������Info
    virtual void SetInfo(char *pbuf)
    {
        SetName(pbuf);
        SetID(&pbuf[strlen(pbuf) + 1]);
    }

    //����pBaseInfo��ռ�ܿռ��С
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

    //����pBaseInfo��ռ�ܿռ��С
    virtual unsigned char GetInfoSize()
    {
        return CBaseInfo::GetInfoSize() + sizeof(float);
    }

protected:
    //CMyString *m_pstrStudID;      //����CBaseInfo::m_strName������ģ��
    //CMyString *m_pstrCourseID;    //����CBaseInfo::m_strID������ģ��
    float m_fScore;
};

#endif