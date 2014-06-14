#if 0

#include <iostream>
using namespace std;

#include <assert.h>

#include "DLinkList.h"

CDLinkList::CDLinkList()
{
    m_nCount = 0;
    m_pHead = NULL;
    m_pTail = NULL;
}

CDLinkList::~CDLinkList()
{
    Clear();   
}

POSITION
CDLinkList::AddTail(int nData)
{
    CNode *pNewNode = NULL;
    pNewNode = new CNode(nData);

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

void 
CDLinkList::RemoveTail(void)
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
    CNode *pOldTail = m_pTail;

    //将原尾部结点的前驱设为m_pTail
    m_pTail = m_pTail->m_pPre;

    delete pOldTail;
    pOldTail = NULL;
    
    m_nCount--;
}

POSITION 
CDLinkList::AddHead(int nData)
{
    CNode *pNewNode = new CNode(nData);

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

void
CDLinkList::RemoveHead(void)
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
    CNode *pOldHead = m_pHead;

    //设原头指针的后继为新头指针
    m_pHead = m_pHead->m_pNext;

    //删除原头指针
    delete pOldHead;
    pOldHead = NULL;

    m_nCount--;
}

POSITION 
CDLinkList::GetPositionByIndex(int nIndex)  const
{
    CNode *pCurNode = m_pHead;
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

POSITION 
CDLinkList::GetPositionByValue(int nData)   const
{
    CNode *pCurNode = m_pHead;
    while (pCurNode != NULL
           && pCurNode->m_data != nData)
    {
        pCurNode = pCurNode->m_pNext;
    }
    
    return pCurNode;
}

int 
CDLinkList::GetDataByIndex(int nIndex)  const
{
    return GetPositionByIndex(nIndex)->m_data;
}

int
CDLinkList::GetDataByPosition(POSITION pos) const
{
    return pos->m_data;
}

void 
CDLinkList::Remove(POSITION pos)
{
    if (NULL == pos)
    {
        return;
    }

    CNode *pPre = pos->m_pPre;
    CNode *pNext = pos->m_pNext;

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

void 
CDLinkList::InsertB4(POSITION pos, int nData) 
{   
    if (NULL == pos)
    {
        AddHead(nData);
        return;
    }

    //pPre pNewNode pos
    CNode *pPre = pos->m_pPre;
    CNode *pNewNode = new CNode(nData);

    if (NULL == pPre)
    {
        AddHead(nData);
        return;
    }

    pPre->m_pNext       = pNewNode;
    pNewNode->m_pPre    = pPre;
    pNewNode->m_pNext   = pos;
    pos->m_pPre         = pNewNode;
    
    m_nCount++;
}

void 
CDLinkList::InsertAft(POSITION pos, int nData) 
{   
    if (NULL == pos)
    {
        AddTail(nData);
        return;
    }
    
    //pos pNewNode pNext
    CNode *pNext = pos->m_pNext;
    CNode *pNewNode = new CNode(nData);

    if (NULL == pNext)
    {
        AddTail(nData);
        return;
    }
    
    pos->m_pNext        = pNewNode;
    pNewNode->m_pPre    = pos;
    pNewNode->m_pNext   = pNext;
    pNext->m_pPre       = pNewNode;

    m_nCount++;
}
   
void 
CDLinkList::SetData(POSITION pos, int nData)
{
    assert(pos != NULL);
    pos->m_data = nData;
}

void 
CDLinkList::Walk(void)
{
    CNode *pCurNode = m_pHead;
    
    cout << "Walking List: ";
    while (pCurNode != NULL )
    {
        cout << pCurNode->m_data << " ";
        
        pCurNode = pCurNode->m_pNext;
    }
    cout << endl;
}

void
CDLinkList::Clear(void)
{
    CNode *pCurNode = m_pHead;
    CNode *pNextNode = pCurNode;

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




    
