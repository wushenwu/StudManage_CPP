#ifndef DLINKLIST_H_AE4E0B52_43A7_4c27_AD23_5DEA083ECEEE

#define DLINKLIST_H_AE4E0B52_43A7_4c27_AD23_5DEA083ECEEE

#include <stdlib.h>
#include <assert.h>

//友元模板类的向前声明
template<typename T> class CDLinkList;

template<typename T>
class CNode
{
    friend class CDLinkList<T>;    //友元模板类需向前声明
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

#if 0  //针对 typedef CNode<T> * POSITION; 不会根据CDLinkList<T>变化而变化的问题

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
public: //迭代器相关
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
    Function:向链表尾部添加数据，并返回位置的结点信息
    */
    POSITION_T AddTail(const T &Data);

    /*
    Function:将链表尾部结点数据删除
    */
    void RemoveTail(void);

    /*
    Function:获取尾部结点信息
    */
    inline POSITION_T GetTail(void) const;

    /*
    Function:向链表头部添加数据，并返回位置的结点信息
    */
    POSITION_T AddHead(const T &Data);

    /*
    Function:将链表头部结点数据删除
    */
    void RemoveHead(void);

    /*
    Function:获取头部结点信息
    */
    inline POSITION_T GetHead(void) const;

    /*
    Function:获取特定下标的位置， 返回该位置的结点信息
    */
    POSITION_T GetPositionByIndex(int nIndex) const;

    /*
    Function:获取value第一次出现时的位置，返回该位置的结点信息
    */
    POSITION_T GetPositionByValue(const T &Data) const;

    /*
    Function:获取特定下标处所存储的数据
    */
    T GetDataByIndex(int nIndex) const;

    /*
    Function:获取特定位置处的数据
    */
    T GetDataByPosition(POSITION_T pos) const;

    /*
    Function:获取特定位置的前驱
    */
    inline POSITION_T GetPrior(POSITION_T pos) const;

    /*
    Function:获取特定位置的后继
    */
    inline POSITION_T GetNext(POSITION_T pos) const;

    /*
    Function:将特定位置的元素删除。
    */
    void Remove(POSITION_T pos);

    /*
    Function:将数据插入到特定位置的前面
    Remark  :如果所提供pos为NULL, 则将数据添加至头部
    */
    void InsertB4(POSITION_T pos, const T &Data);

    /*
    Function:将数据插入到特定位置的后面
    Remark  :如果所提供pos为NULL, 则将数据添加至尾部
    */
    void InsertAft(POSITION_T pos, const T &Data);

    /*
    Function:修改特定位置的数据
    */
    void SetData(POSITION_T pos, const T &Data);

    /*
    Function: 遍历链表，输出相关结点数据，仅是测试用
    */
    void Walk(void);

    /*
    Function: 清空链表
    */
    void Clear(void);
       
private:
    int m_nCount;       //链表元素数
    CNode<T> *m_pHead;     //链表头指针
    CNode<T> *m_pTail;     //链表尾指针
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

    //如果为空链表，第一次插入的情况
    if (NULL == m_pTail)
    {
        //使得链表头指针、尾指针均指向该结点 (已优化)
        m_pHead = pNewNode;     //m_pHead = m_pTail = pNewNode;
    }
    else
    {
        //使得该结点成为原尾部结点的后继
        m_pTail->m_pNext = pNewNode;

        //该结点的前驱是原尾部结点
        pNewNode->m_pPre = m_pTail;   
    }

    //尾部结点指向该结点
    m_pTail = pNewNode;
    m_nCount++;

    return pNewNode;
}

template<typename T>
void 
CDLinkList<T>::RemoveTail(void)
{
    //如果是空链表，没有结点的情况
    if (NULL == m_pTail)
    {
        return;
    }
    else if (NULL == m_pTail->m_pPre)      //如果仅有一个结点
    {
        //原尾部结点没有前驱

        //
        m_pHead = NULL;
    }
    else
    {
        //将原尾部结点的前驱的后继指向NULL
        m_pTail->m_pPre->m_pNext = NULL;       
    }

    //保存原结点，然后释放
    CNode<T> *pOldTail = m_pTail;

    //将原尾部结点的前驱设为m_pTail
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

    if (NULL == m_pHead)    //为空链接
    {
        //将链表头指针、尾指针都指向该结点(已优化)
        m_pTail = pNewNode;
    }
    else
    {
        //让该新结点指向原首结点
        pNewNode->m_pNext = m_pHead;

        //同时原首结点的前驱设为该新结点
        m_pHead->m_pPre = pNewNode;
    }

    //让链表头指针指向该新结点
    m_pHead = pNewNode;

    m_nCount++;

    return pNewNode;
}

template<typename T>
void
CDLinkList<T>::RemoveHead(void)
{
    if (NULL == m_pHead)    //空链表
    {
        return;
    }
    else if (NULL == m_pHead->m_pNext)  //仅有一个结点     
    {
        m_pTail = NULL;
    }
    
    //保存原链表头指针
    CNode<T> *pOldHead = m_pHead;

    //设原头指针的后继为新头指针
    m_pHead = m_pHead->m_pNext;

    //删除原头指针
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