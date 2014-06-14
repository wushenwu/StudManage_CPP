#ifndef DLINKLIST_H_AE4E0B52_43A7_4c27_AD23_5DEA083ECEEE

#define DLINKLIST_H_AE4E0B52_43A7_4c27_AD23_5DEA083ECEEE

#include <stdlib.h>
#include <assert.h>

//��Ԫģ�������ǰ����
template<typename T> class CDLinkList;

template<typename T>
class CNode
{
    friend class CDLinkList<T>;    //��Ԫģ��������ǰ����
public:
    CNode(T data)
    {
        m_data = data;
        m_pPre = NULL;
        m_pNext = NULL;
    }

public:
    T m_data;

//private:
public:
    CNode *m_pPre;
    CNode *m_pNext;
};

#if 0  //��� typedef CNode<T> * POSITION; �������CDLinkList<T>�仯���仯������

template<typename T>
struct POSITION_TYPE
{
    typedef CNode<T> * POSITION;
};

#else

#define POSITION_T  CNode<T> *

#endif

template<typename T>
class CDLinkList
{
public: //���������
    class iterator
    {
    public:
        iterator(CNode<T> *pNode = NULL)
        {
            m_pCurNode = pNode;
        }
        
        bool operator != (const iterator &obj)
        {
            return !(m_pCurNode == obj.m_pCurNode);
        }
        
        iterator operator ++ (int)
        {
            m_pCurNode = m_pCurNode->m_pNext;
            if (NULL == m_pCurNode)
            {
                return NULL;
            }
            
            return m_pCurNode->m_pPre;
        }
        
        iterator operator ++ ()
        {
            m_pCurNode = m_pCurNode->m_pNext;
            return m_pCurNode;
        }
        
        T operator *()
        {
            return m_pCurNode->m_data;
        }
        
    private:
        CNode<T> *m_pCurNode;
    };
    
    iterator begin(void)
    {
        return m_pHead;
    }
    
    iterator end(void)
    {
        return NULL;
    }

public:
    CDLinkList();
    ~CDLinkList();

public:
    inline int GetCount(void) const;
    
    inline bool IsEmpty(void) const;

    /*
    Function:������β��������ݣ�������λ�õĽ����Ϣ
    */
    POSITION_T AddTail(const T &Data);

    /*
    Function:������β���������ɾ��
    */
    void RemoveTail(void);

    /*
    Function:��ȡβ�������Ϣ
    */
    inline POSITION_T GetTail(void) const;

    /*
    Function:������ͷ��������ݣ�������λ�õĽ����Ϣ
    */
    POSITION_T AddHead(const T &Data);

    /*
    Function:������ͷ���������ɾ��
    */
    void RemoveHead(void);

    /*
    Function:��ȡͷ�������Ϣ
    */
    inline POSITION_T GetHead(void) const;

    /*
    Function:��ȡ�ض��±��λ�ã� ���ظ�λ�õĽ����Ϣ
    */
    POSITION_T GetPositionByIndex(int nIndex) const;

    /*
    Function:��ȡvalue��һ�γ���ʱ��λ�ã����ظ�λ�õĽ����Ϣ
    */
    POSITION_T GetPositionByValue(const T &Data) const;

    /*
    Function:��ȡ�ض��±괦���洢������
    */
    T GetDataByIndex(int nIndex) const;

    /*
    Function:��ȡ�ض�λ�ô�������
    */
    T GetDataByPosition(POSITION_T pos) const;

    /*
    Function:��ȡ�ض�λ�õ�ǰ��
    */
    inline POSITION_T GetPrior(POSITION_T pos) const;

    /*
    Function:��ȡ�ض�λ�õĺ��
    */
    inline POSITION_T GetNext(POSITION_T pos) const;

    /*
    Function:���ض�λ�õ�Ԫ��ɾ����
    */
    void Remove(POSITION_T pos);

    /*
    Function:�����ݲ��뵽�ض�λ�õ�ǰ��
    Remark  :������ṩposΪNULL, �����������ͷ��
    */
    void InsertB4(POSITION_T pos, const T &Data);

    /*
    Function:�����ݲ��뵽�ض�λ�õĺ���
    Remark  :������ṩposΪNULL, �����������β��
    */
    void InsertAft(POSITION_T pos, const T &Data);

    /*
    Function:�޸��ض�λ�õ�����
    */
    void SetData(POSITION_T pos, const T &Data);

    /*
    Function: �������������ؽ�����ݣ����ǲ�����
    */
    void Walk(void);

    /*
    Function: �������
    */
    void Clear(void);
       
private:
    int m_nCount;       //����Ԫ����
    CNode<T> *m_pHead;     //����ͷָ��
    CNode<T> *m_pTail;     //����βָ��
};

template<typename T>
CDLinkList<T>::CDLinkList<T>()
{
    m_nCount = 0;
    m_pHead = NULL;
    m_pTail = NULL;
}

template<typename T>
CDLinkList<T>::~CDLinkList<T>()
{
    Clear();   
}

template<typename T>
int 
CDLinkList<T>::GetCount(void)  const
{
    return m_nCount;
}

template<typename T>
bool 
CDLinkList<T>::IsEmpty(void)   const
{
    return 0 == m_nCount;
}

template<typename T>
POSITION_T
CDLinkList<T>::GetTail(void)   const
{
    return m_pTail;
}

template<typename T>
POSITION_T
CDLinkList<T>::GetHead(void) const
{
    return m_pHead;
}

template<typename T>
POSITION_T
CDLinkList<T>::GetPrior(POSITION_T pos)  const
{
    return pos->m_pPre;
}

template<typename T>
POSITION_T
CDLinkList<T>::GetNext(POSITION_T pos)   const
{
    return pos->m_pNext;
}

template<typename T>
POSITION_T
CDLinkList<T>::AddTail(const T &Data)
{
    CNode<T> *pNewNode = NULL;
    pNewNode = new CNode<T>(Data);

    //���Ϊ��������һ�β�������
    if (NULL == m_pTail)
    {
        //ʹ������ͷָ�롢βָ���ָ��ý�� (���Ż�)
        m_pHead = pNewNode;     //m_pHead = m_pTail = pNewNode;
    }
    else
    {
        //ʹ�øý���Ϊԭβ�����ĺ��
        m_pTail->m_pNext = pNewNode;

        //�ý���ǰ����ԭβ�����
        pNewNode->m_pPre = m_pTail;   
    }

    //β�����ָ��ý��
    m_pTail = pNewNode;
    m_nCount++;

    return pNewNode;
}

template<typename T>
void 
CDLinkList<T>::RemoveTail(void)
{
    //����ǿ�����û�н������
    if (NULL == m_pTail)
    {
        return;
    }
    else if (NULL == m_pTail->m_pPre)      //�������һ�����
    {
        //ԭβ�����û��ǰ��

        //
        m_pHead = NULL;
    }
    else
    {
        //��ԭβ������ǰ���ĺ��ָ��NULL
        m_pTail->m_pPre->m_pNext = NULL;       
    }

    //����ԭ��㣬Ȼ���ͷ�
    CNode<T> *pOldTail = m_pTail;

    //��ԭβ������ǰ����Ϊm_pTail
    m_pTail = m_pTail->m_pPre;

    delete pOldTail;
    pOldTail = NULL;
    
    m_nCount--;
}

template<typename T>
POSITION_T 
CDLinkList<T>::AddHead(const T &Data)
{
    CNode<T> *pNewNode = new CNode<T>(Data);

    if (NULL == m_pHead)    //Ϊ������
    {
        //������ͷָ�롢βָ�붼ָ��ý��(���Ż�)
        m_pTail = pNewNode;
    }
    else
    {
        //�ø��½��ָ��ԭ�׽��
        pNewNode->m_pNext = m_pHead;

        //ͬʱԭ�׽���ǰ����Ϊ���½��
        m_pHead->m_pPre = pNewNode;
    }

    //������ͷָ��ָ����½��
    m_pHead = pNewNode;

    m_nCount++;

    return pNewNode;
}

template<typename T>
void
CDLinkList<T>::RemoveHead(void)
{
    if (NULL == m_pHead)    //������
    {
        return;
    }
    else if (NULL == m_pHead->m_pNext)  //����һ�����     
    {
        m_pTail = NULL;
    }
    
    //����ԭ����ͷָ��
    CNode<T> *pOldHead = m_pHead;

    //��ԭͷָ��ĺ��Ϊ��ͷָ��
    m_pHead = m_pHead->m_pNext;

    //ɾ��ԭͷָ��
    delete pOldHead;
    pOldHead = NULL;

    m_nCount--;
}

template<typename T>
POSITION_T 
CDLinkList<T>::GetPositionByIndex(int nIndex)  const
{
    CNode<T> *pCurNode = m_pHead;
    for (int i = 0; i < nIndex; i++)
    {
        if (NULL == pCurNode)
        {
            return NULL;
        }

        pCurNode = pCurNode->m_pNext;           
    }

    return pCurNode;
}

template<typename T>
POSITION_T 
CDLinkList<T>::GetPositionByValue(const T &Data)   const
{
    CNode<T> *pCurNode = m_pHead;
    while (pCurNode != NULL
           && pCurNode->m_data != Data)
    {
        pCurNode = pCurNode->m_pNext;
    }
    
    return pCurNode;
}

template<typename T>
T 
CDLinkList<T>::GetDataByIndex(int nIndex)  const
{
    return GetPositionByIndex(nIndex)->m_data;
}

template<typename T>
T
CDLinkList<T>::GetDataByPosition(POSITION_T pos) const
{
    return pos->m_data;
}

template<typename T>
void 
CDLinkList<T>::Remove(POSITION_T pos)
{
    if (NULL == pos)
    {
        return;
    }

    CNode<T> *pPre = pos->m_pPre;
    CNode<T> *pNext = pos->m_pNext;

    if (GetHead() == pos)
    {
        RemoveHead();
    }
    else if (GetTail() == pos)
    {
        RemoveTail();
    }
    else
    {
        //pPre pos pNext
        pPre->m_pNext = pNext;
        pNext->m_pPre = pPre;

        delete pos;
        m_nCount--;
    }    
}

template<typename T>
void 
CDLinkList<T>::InsertB4(POSITION_T pos, const T &Data) 
{   
    if (NULL == pos)
    {
        AddHead(Data);
        return;
    }

    //pPre pNewNode pos
    CNode<T> *pPre = pos->m_pPre;
    CNode<T> *pNewNode = new CNode<T>(Data);

    if (NULL == pPre)
    {
        AddHead(Data);
        return;
    }

    pPre->m_pNext       = pNewNode;
    pNewNode->m_pPre    = pPre;
    pNewNode->m_pNext   = pos;
    pos->m_pPre         = pNewNode;
    
    m_nCount++;
}

template<typename T>
void 
CDLinkList<T>::InsertAft(POSITION_T pos, const T &Data) 
{   
    if (NULL == pos)
    {
        AddTail(Data);
        return;
    }
    
    //pos pNewNode pNext
    CNode<T> *pNext = pos->m_pNext;
    CNode<T> *pNewNode = new CNode<T>(Data);

    if (NULL == pNext)
    {
        AddTail(Data);
        return;
    }
    
    pos->m_pNext        = pNewNode;
    pNewNode->m_pPre    = pos;
    pNewNode->m_pNext   = pNext;
    pNext->m_pPre       = pNewNode;

    m_nCount++;
}

template<typename T> 
void 
CDLinkList<T>::SetData(POSITION_T pos, const T &Data)
{
    assert(pos != NULL);
    pos->m_data = Data;
}

template<typename T>
void 
CDLinkList<T>::Walk(void)
{
    CNode<T> *pCurNode = m_pHead;
    
    cout << "Walking List: ";
    while (pCurNode != NULL )
    {
        cout << pCurNode->m_data << " ";
        
        pCurNode = pCurNode->m_pNext;
    }
    cout << endl;
}

template<typename T>
void
CDLinkList<T>::Clear(void)
{
    CNode<T> *pCurNode = m_pHead;
    CNode<T> *pNextNode = pCurNode;

    while(pCurNode != NULL)
    {
        pNextNode = pCurNode->m_pNext;

        delete pCurNode;
        pCurNode = pNextNode;
    }

    m_pHead = NULL;
    m_pTail = NULL;
}

#endif