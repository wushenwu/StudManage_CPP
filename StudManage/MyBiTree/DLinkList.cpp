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

void 
CDLinkList::RemoveTail(void)
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
    CNode *pOldTail = m_pTail;

    //��ԭβ������ǰ����Ϊm_pTail
    m_pTail = m_pTail->m_pPre;

    delete pOldTail;
    pOldTail = NULL;
    
    m_nCount--;
}

POSITION 
CDLinkList::AddHead(int nData)
{
    CNode *pNewNode = new CNode(nData);

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

void
CDLinkList::RemoveHead(void)
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
    CNode *pOldHead = m_pHead;

    //��ԭͷָ��ĺ��Ϊ��ͷָ��
    m_pHead = m_pHead->m_pNext;

    //ɾ��ԭͷָ��
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




    
