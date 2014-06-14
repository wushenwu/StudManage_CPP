#ifndef MYBITREE_H_D2A3F9C8_352D_4ce5_B745_AC400CDBBE1D

#define MYBITREE_H_D2A3F9C8_352D_4ce5_B745_AC400CDBBE1D

#include <iostream>
using namespace std;

#include "Stack.h"
#include "MyQueue.h"
#include "CInfo.h"

inline int EleCompare(char *psz1, char *psz2)
{
    return strcmp(psz1, psz2);
}

inline int EleCompare(int nData1, int nData2)
{
    return nData1 - nData2;
}

inline int EleCompare(CBaseInfo &BaseInfo1, CBaseInfo &BaseInfo2)
{
    int result;
    if (NULL == BaseInfo1.GetID())
    {
        result = strcmp(BaseInfo1.GetName(), BaseInfo2.GetName());
        if (0 == result)
        {
            return 0;
        }
        else if (0 > result)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    
    if (0 == strcmp(BaseInfo1.GetName(), BaseInfo2.GetName())
        && 0 == strcmp(BaseInfo1.GetID(), BaseInfo2.GetID()))
    {
        return 0;
    }
    else if (0 < strcmp(BaseInfo1.GetName(), BaseInfo2.GetName())
        || (0 == strcmp(BaseInfo1.GetName(), BaseInfo2.GetName())
        && 0 < strcmp(BaseInfo1.GetID(), BaseInfo2.GetID()))
        )
    {
        return 1;
    }
    else if ( 0 > strcmp(BaseInfo1.GetName(), BaseInfo2.GetName())
        || (0 == strcmp(BaseInfo1.GetName(), BaseInfo2.GetName())
        && 0 > strcmp(BaseInfo1.GetID(), BaseInfo2.GetID()))
        )
    {
        return -1;
    }
    else
    {
        return -2;
    }
}

template<typename T>
class CMyBiTree;

template<typename T>
class CTreeNode
{
    friend class CMyBiTree<T>;
public:
    CTreeNode(T Data, unsigned long nInfoOffset = 0)
    {
        m_Data = Data;
        m_pParent = NULL;
        m_pLeft   = NULL;
        m_pRight  = NULL;
        m_pNext   = NULL;
        m_pPre    = NULL;

        m_nBalanceFactor = 0;   //�½��ĸ߶Ȳ�Ϊ0

        //
        m_nInfoOffset = nInfoOffset;
    }

public:
    CTreeNode<T> *GetNext(void) { return m_pNext;}
    unsigned long GetInfoOffset(void) { return m_nInfoOffset; }
    T &GetData(void) { return m_Data; }

private:
    T m_Data;            //�������
    CTreeNode<T> *m_pParent;   //�����ָ��
    CTreeNode<T> *m_pLeft;     //����ָ��
    CTreeNode<T> *m_pRight;    //�Һ���ָ��
    CTreeNode<T> *m_pNext;     //��Կ��ܵ���ͬ���ݵ���������
    CTreeNode<T> *m_pPre;       //��Կ��ܵ���ͬ���ݵ���������

    int m_nBalanceFactor;       //�߶Ȳƽ������BalanceFactor, for AVL

    //��Ϣ��������Ϣ�ļ��е�ƫ��
    unsigned long m_nInfoOffset;
};

template<>
CTreeNode<char *>::CTreeNode<char *>(char *Data, unsigned long nInfoOffset)
{
    //m_Data = Data;
    m_Data = new char[strlen(Data) + 1];
    strcpy(m_Data, Data);

    m_pParent = NULL;
    m_pLeft   = NULL;
    m_pRight  = NULL;
    m_pNext   = NULL;
    m_pPre    = NULL;
    
    m_nBalanceFactor = 0;   //�½��ĸ߶Ȳ�Ϊ0

    //
    m_nInfoOffset = nInfoOffset;
}

template<typename T>
class CMyBiTree
{
public:
    enum ACTION {REMOVE = -1, INSERT = 1};
    enum HEAVY{LEFT = 1, RIGHT = -1};   //����أ������ұ��أ��߶Ȳ����ʱ�漰��

public:
    CMyBiTree()
    {
        m_pRoot = NULL;
        m_nCount  = 0;        
    }

    ~CMyBiTree();

    unsigned int GetCount(void) const
    {
        return m_nCount;
    }

    /*
    Function: ��ȡ�����
    */
    CTreeNode<T> *GetRoot(void) const
    {
        return m_pRoot;
    }

    /*
    Function: ��Data ���뵽pTargetNode����߻����ұߣ������ظý��
    */
    //CTreeNode *Insert(T Data, CTreeNode *pTargetNode, bool IsLeft);

    /*
    Function: ��Ը����pRoot��ǰ��������������������������ݹ鷽ʽ
    */
    void PreOrder(CTreeNode<T> *pRoot);
    void MidOrder(CTreeNode<T> *pRoot);
    void PostOrder(CTreeNode<T> *pRoot);

    //������б���
    void LevelWalk(CTreeNode<T> *pRoot);

    /*
    Function: ��Ը����pRoot��ǰ��������������������������ǵݹ鷽ʽ
    */
    //void PreOrder_Non(CTreeNode *pRoot);
    //void MidOrder_Non(CTreeNode *pRoot);
    //void PostOrder_Non(CTreeNode *pRoot);

    ////////////////  ��Զ����������Ĵ���  //////////////////////////////////////////
    /*
    Function:��������Data ����Ĳ��뵽������������. ��֧���ظ����ݵĲ���
    */
    void Insert(T Data, unsigned long nInfoOffset = 0);

    /*
    Function:���Ҹ�����Data�Ƿ���ڣ���������򷵻���Ӧ�����Ϣ������NULL
    */
    CTreeNode<T> *Find(T Data);

    /*
    Function:�����������Data���ļ���ƫ��nInfoOffset�Ľ�㣬��������򷵻���Ӧ�Ľ����Ϣ��
             ����NULL��

    */
    CTreeNode<T> *Find(T Data, unsigned long nInfoOffset);

    /*
    Function: ɾ��������Ŀ���㣬��Ŀ�����ͨ��Find����
    */
    void Remove(CTreeNode<T> *pDelNode);

    /*
    Function: ɾ���������ӽ��Ľ�㣬������ת��������ת
             ʹ�ñ�ɾ������������������ý�㣬
             ���߱�ɾ�������ұ���С������ý��
    */
    void Remove_TwoChild_RotateLeft(CTreeNode<T> *pDelNode);
    void Remove_TwoChild_RotateRight(CTreeNode<T> *pDelNode);

    /*
    Function: ���������pModifyNode��ԭֵ��Ϊ��ֵnNewData
    */
    void Modify(CTreeNode<T> *pModifyNode, T NewData, unsigned long nInfoOffset = 0);

    //////////////// ����ƽ����AVL���   /////////////////////////////////////////
    /*
    Function: ����Ŀ����ĸ߶Ȳƽ�����ӣ�
    */
    void AdjustBF(CTreeNode<T> *pTargetNode, enum ACTION action);

    /*
    Function: ���Ŀ�������ת����
    */
    void Rotate(CTreeNode<T> *pTargetNode);
    void RotateSingleLeft(CTreeNode<T> *pTargetNode);
    void RotateSingleRight(CTreeNode<T> *pTargetNode);
    void RotateDoubleLeft(CTreeNode<T> *pTargetNode);
    void RotateDoubleRight(CTreeNode<T> *pTargetNode);

    /*
    Function:���Ŀ����ĸ߶Ȳ����
    */
    void RotateLeft_AdjustHeight(CTreeNode<T> *pTargetNode);
    void RotateRight_AdjustHeight(CTreeNode<T> *pTargetNode);

 
private:
    /*
    Function: �������ɾ����㣬��������������
    */
    void PostOrder_Del(CTreeNode<T> *pRoot);

    /*
    Function: ����ͬԪ�ص������Ĵ���, ɾ�������ӽ�㣬˫���ӽ��
    */
    void RemoveFromLinkListHead_One(CTreeNode<T> *pDelNode);
    void RemoveFromLinkListHead_Two(CTreeNode<T> *pDelNode);

private:
    CTreeNode<T> *m_pRoot;     //�����ָ��

    //����
    unsigned int m_nCount;  //�������
    static enum HEAVY heavy;
};

template<typename T>
CMyBiTree<T>::HEAVY CMyBiTree<T>::heavy = CMyBiTree<T>::RIGHT;

////////////////  ʵ��  ///////////////////////////////////////
template<typename T>
void 
CMyBiTree<T>::PostOrder_Del(CTreeNode<T> *pRoot)
{
    if (pRoot != NULL)
    {        
        PostOrder_Del(pRoot->m_pLeft);        
        PostOrder_Del(pRoot->m_pRight);
        
        delete pRoot;
        pRoot = NULL;
        m_nCount--;
    }
}


template<typename T>
CMyBiTree<T>::~CMyBiTree<T>()
{
    //���ú��������ɾ�����
    PostOrder_Del(m_pRoot);
    m_pRoot = NULL;
}

#if 0
template<typename T>
CTreeNode<T> *
CMyBiTree<T>::Insert(T Data, CTreeNode<T> *pTargetNode, bool IsLeft)
{
    CTreeNode<T> *pNewNode = new CTreeNode<T>(Data);
    if (NULL == pNewNode)
    {
        //TODO
    }

    //����
    if (NULL == m_pRoot)
    {
        m_pRoot = pNewNode;
    }
    else
    {
        //�������ң����ø��������Һ���
        if (IsLeft)
        {
            pTargetNode->m_pLeft = pNewNode;    
        }
        else
        {
            pTargetNode->m_pRight = pNewNode;
        }

        //�����½��ĸ����
        pNewNode->m_pParent = pTargetNode;
    }

    m_nCount++;

    AdjustBF(pNewNode, INSERT);

    return pNewNode;
}
#endif

template<typename T>
void 
CMyBiTree<T>::PreOrder(CTreeNode<T> *pRoot)
{
    if (pRoot != NULL)
    {
        cout << pRoot->m_Data << " ";

        PreOrder(pRoot->m_pLeft);
        PreOrder(pRoot->m_pRight);
    }
}

template<typename T>
void 
CMyBiTree<T>::MidOrder(CTreeNode<T> *pRoot)
{
    if (pRoot != NULL)
    {        
        MidOrder(pRoot->m_pLeft);

    
        cout << pRoot->m_Data << " "
             << "[ " << pRoot->m_nBalanceFactor << " ] ";

        #if 0   //this is just used for debugging

        if (abs(pRoot->m_nBalanceFactor) == 2 
            && pRoot->m_pLeft == NULL && pRoot->m_pRight == NULL
            )
        {
            cout << pRoot->m_Data << " " << endl;
            getchar();
        }

        if (pRoot->m_nBalanceFactor == 1
            && pRoot->m_pRight == NULL
            )
        {
            cout << pRoot->m_Data << " " << endl;
            getchar();
        }

        if (pRoot->m_nBalanceFactor == -1
            && pRoot->m_pLeft == NULL
            )
        {
            cout << pRoot->m_Data << " " << endl;
            getchar();
        }

        if (pRoot->m_pLeft != NULL
            && pRoot->m_pRight == NULL
            && pRoot->m_nBalanceFactor == 0
            )
        {
            cout << pRoot->m_Data << " " << endl;
            getchar();
        }

        if (pRoot->m_pLeft == NULL
            && pRoot->m_pRight != NULL
            && pRoot->m_nBalanceFactor == 0
            )
        {
            cout << pRoot->m_Data << " " << endl;
            getchar();
        }


        if (abs(pRoot->m_nBalanceFactor) >= 2 )
        {
            cout << "MidOrder >= 2" << endl;
            getchar();
        }
    #endif

        MidOrder(pRoot->m_pRight);
    }
}

template<typename T>
void 
CMyBiTree<T>::PostOrder(CTreeNode<T> *pRoot)
{
    if (pRoot != NULL)
    {        
        PostOrder(pRoot->m_pLeft);        
        PostOrder(pRoot->m_pRight);

        cout << pRoot->m_Data << " ";
    }
}

#if 0   //TBD
template<typename T>
void 
CMyBiTree<T>::PreOrder_Non(CTreeNode<T> *pRoot)
{
    if (NULL == pRoot)
    {
        return;
    }

    CMyStack<CTreeNode<T> *> stack;

    while (true)
    {
        cout << pRoot->m_Data << " ";

        if (pRoot->m_pRight != NULL)
        {
            stack.Push(pRoot->m_pRight);
        }

        if (pRoot->m_pLeft != NULL )
        {
            stack.Push(pRoot->m_pLeft);
        }
    
        int nCount = stack.GetCount();
        if (0 == nCount)
        {
            return;
        }
        else
        {
            pRoot = stack.GetTop();

            stack.Pop();
        }
    }      
}

template<typename T>
void 
CMyBiTree<T>::MidOrder_Non(CTreeNode<T> *pRoot)
{
    if (NULL == pRoot)
    {
        return;
    }

    CMyStack<CTreeNode<T> *> stack;

    bool flg_push = true;

    while (true)
    {
        //���Һ���ѹջ
        if (pRoot->m_pRight != NULL && flg_push)
        {
            stack.Push(pRoot->m_pRight);
            cout << "   " << stack.GetTop()->m_Data << " ";
        }

        //������ѹջ
        if (pRoot != NULL && flg_push)
        {
            stack.Push(pRoot);
            cout << "   " << stack.GetTop()->m_Data << " ";
        }

        /* �������� */
        if (pRoot->m_pLeft != NULL && flg_push)
        {
            //���Ӳ��գ���������Ϊ���ڵ����
            pRoot = pRoot->m_pLeft; 
            
            flg_push = true;
        }
        else
        {
            int nCount = stack.GetCount();
            if (0 == nCount)
            {
                return;
            }
            else if (nCount > 2)
            {
                //��ʱ���Ƚ����������� (��)
                cout << endl << (stack.GetTop()->m_Data) << " ";
                stack.Pop();
                
                //�ٽ��丸������
                cout << endl << (stack.GetTop()->m_Data) << " ";
                stack.Pop();
                
                pRoot = stack.GetTop();
            }

            nCount = stack.GetCount();
            if(2 == nCount)
            {
                //��ʱ�϶����ң��н��
                pRoot = stack.GetTop();
                cout << endl << (pRoot->m_Data) << " ";
                stack.Pop();
                
                pRoot = stack.GetTop();
            }
        }
    }
}

template<typename T>
void 
CMyBiTree<T>::PostOrder_Non(CTreeNode<T> *pRoot)
{
    assert(pRoot != NULL);

    CMyStack<CTreeNode<T> *> stack;
    CTreeNode<T> *pCurNode = pRoot;

    while (true)
    {
        //����ǰ���ѹջ
        if (pCurNode != NULL)
        {
            stack.Push(pCurNode);
        }

        //����ǰ�����Һ���ѹջ
        if (pCurNode->m_pRight != NULL)
        {
            stack.Push(pCurNode->m_pRight);
        }

        //��ǰ�������ӷǿգ���������Ϊ��ǰ��㣬��������
        if (pCurNode->m_pLeft != NULL)
        {
            pCurNode = pCurNode->m_pLeft;
        }
        else
        {
            /* ��ǰ��������Ϊ�գ���Ҫ�����Һ��� */

            //���ҽ�㣬��ջtop��Ϊ�Һ���
            if (pCurNode->m_pRight != NULL)
            {
                pCurNode = stack.GetTop();
                stack.Pop();
            }
            else
            {
                //û�ҽ�㣬�����Ҷ������꣬������˽��
                cout << pCurNode->m_Data << endl;
                stack.Pop();
                
            }
        }       
    }//end while (
}
#endif

template<typename T>
void 
CMyBiTree<T>::LevelWalk(CTreeNode<T> *pRoot)
{
    CMyQueue<CTreeNode<T> *> queue;
    
    CTreeNode<T> *pCurNode = pRoot;
    while (pCurNode != NULL)
    {
        cout << pCurNode->m_Data << " "
             //<< "[ " << pCurNode->m_nBalanceFactor << " ] ";
             << "[ " << pCurNode->m_nInfoOffset << " ] "
            << endl;

        if (pCurNode->m_pLeft != NULL)
        {
            queue.EnQueue(pCurNode->m_pLeft);
        }

        if (pCurNode->m_pRight != NULL)
        {
            queue.EnQueue(pCurNode->m_pRight);
        }

        if (queue.IsEmpty())
        {
            break;
        }

        pCurNode = queue.GetFront();
        queue.DeQueue();
    }
    cout << endl;
}

////////////////// ������������� /////////////////////////////////
template<typename T>
void
CMyBiTree<T>::Insert(T Data, unsigned long nInfoOffset)
{
    if (0 == nInfoOffset)
    {
        //cout << "error"  << endl;
    }
    CTreeNode<T> *pNewNode = new CTreeNode<T>(Data, nInfoOffset);
    
    if (NULL == pNewNode)
    {
        int i = 0;
        cin >> i;
    }

    //��Ϊ������
    if (NULL == m_pRoot)
    {
        m_pRoot = pNewNode;
        m_nCount++;

        AdjustBF(pNewNode, INSERT);
        return;
    }

    /*����������������Ĳ��뵽������������ 
    1) ���ҵ�Ҫ�����λ�� ��С �� �� �ң�
    2) ������ؽ��Ĺ�ϵ
    */
    CTreeNode<T> *pCurNode = m_pRoot;
    while (true)
    {
        //
        //if (pNewNode->m_Data == pCurNode->m_Data)
        if (0 == EleCompare(pNewNode->m_Data, pCurNode->m_Data))
        {
            //pCurNode pNewNode pNextNode
            CTreeNode<T> *pNext = pCurNode->m_pNext;
            pNewNode->m_pNext = pNext;
            pCurNode->m_pNext = pNewNode;
            if (pNext != NULL)
            {
                pNext->m_pPre = pNewNode;
            }
            
            pNewNode->m_pPre = pCurNode;

            m_nCount++;
            return;
        }
        else if (0 > EleCompare(pNewNode->m_Data, pCurNode->m_Data)) //����������
        {
            //�������������pCurNode������Ϊ�գ���ɲ���pCurNode�����㴦��
            if (NULL == pCurNode->m_pLeft)
            {
                pCurNode->m_pLeft = pNewNode;
                pNewNode->m_pParent = pCurNode;
                m_nCount++;

                AdjustBF(pNewNode, INSERT);
                return;
            }

            pCurNode = pCurNode->m_pLeft;
        }
        else                                    //����������
        {
            //�������������pCurNode���ҽ��Ϊ�գ���ɲ���pCurNode���ҽ�㴦��
            if (NULL == pCurNode->m_pRight)
            {
                pCurNode->m_pRight = pNewNode;
                pNewNode->m_pParent = pCurNode;
                m_nCount++;

                AdjustBF(pNewNode, INSERT);
                return;
            }

            pCurNode = pCurNode->m_pRight;
        }   
    }//end while(true)
}

template<typename T>
CTreeNode<T> *
CMyBiTree<T>::Find(T Data)
{
    CTreeNode<T> *pCurNode = m_pRoot;

    while (pCurNode != NULL)
    {
        //if (Data == pCurNode->m_Data)
        if (0 == EleCompare(Data, pCurNode->m_Data))
        {
            break;
        }

        //if (Data < pCurNode->m_Data)  //����������
        if (0 > EleCompare(Data, pCurNode->m_Data))
        {
            pCurNode = pCurNode->m_pLeft;
        }
        else
        {
            pCurNode = pCurNode->m_pRight;
        }
    }

    return pCurNode;
}

template<typename T>
CTreeNode<T> *
CMyBiTree<T>::Find(T Data, unsigned long nInfoOffset)
{
    CTreeNode<T> *pCurNode = m_pRoot;
    
    while (pCurNode != NULL) 
    {
        //if (Data == pCurNode->m_Data)
        if (0 == EleCompare(Data, pCurNode->m_Data) )            
        {
            while (pCurNode != NULL && pCurNode->m_nInfoOffset != nInfoOffset)
            {
                pCurNode = pCurNode->m_pNext;
            }
            break;
        }
        
        //if (Data < pCurNode->m_Data)  //����������
        if (0 > EleCompare(Data, pCurNode->m_Data))
        {
            pCurNode = pCurNode->m_pLeft;
        }
        else
        {
            pCurNode = pCurNode->m_pRight;
        }
    }
    
    return pCurNode;
}

/*
������������ӽ��Ĵ�����������ת�����ñ�ɾ������Ҳ���Сֵ�滻ɾ�����
*/
template<typename T>
void
CMyBiTree<T>::Remove_TwoChild_RotateLeft(CTreeNode<T> *pDelNode)
{
    //������������ӽ��Ĵ���
    assert(pDelNode->m_pLeft != NULL
         && pDelNode->m_pRight != NULL);

    CTreeNode<T> *pChildLeft = pDelNode->m_pLeft;  //����
    CTreeNode<T> *pChildRight = pDelNode->m_pRight; //�Һ���

    //�ҵ�pDelNode�ұ���С���
    CTreeNode<T> *pReplaceNode = pDelNode->m_pRight;
    while (pReplaceNode->m_pLeft != NULL)
    {
        pReplaceNode = pReplaceNode->m_pLeft;
    }

    //����pReplaceNode��pDelNode���н���
    //pDelNode->m_Data = pReplaceNode->m_Data
    T Data = pDelNode->m_Data;
    pDelNode->m_Data = pReplaceNode->m_Data;
    pReplaceNode->m_Data = Data;
    
    //����������Ҫɾ��pReplaceNode��
    //��pReplaceNode����ΪҶ�ӽ�㣬Ҳ���ܽ������ӣ����Ե���ǰ���ʵ��
    Remove(pReplaceNode); 
}

template<typename T>
void
CMyBiTree<T>::RemoveFromLinkListHead_Two(CTreeNode<T> *pDelNode)
{
    /*
        pParent

         pDelNode  pNext

      pLeftChild    pRightChild
    */
    CTreeNode<T> *pNext = pDelNode->m_pNext;
    assert(pNext != NULL);

    CTreeNode<T> *pLeftChild = pDelNode->m_pLeft;
    CTreeNode<T> *pRightChild = pDelNode->m_pRight;
    
    pLeftChild->m_pParent = pNext;
    pRightChild->m_pParent = pNext;

    pNext->m_pLeft = pLeftChild;
    pNext->m_pRight = pRightChild;

    pNext->m_pPre = NULL;
    pNext->m_nBalanceFactor = pDelNode->m_nBalanceFactor;

    CTreeNode<T> *pParent = pDelNode->m_pParent;
    if (NULL == pParent)
    {
        m_pRoot = pNext;
        
        delete pDelNode;
        pDelNode = NULL;
        m_nCount--;
        return;
    }

    if (pDelNode == pParent->m_pLeft)
    {
        pParent->m_pLeft = pNext;
    }
    else    
    {
        pParent->m_pRight = pNext;
    }

    pNext->m_pParent = pParent;

    delete pDelNode;
    pDelNode = NULL;
    m_nCount--;
    return;
}

/*
������������ӽ��Ĵ�����������ת�����ñ�ɾ�����������ֵ�滻ɾ�����
*/
template<typename T>
void
CMyBiTree<T>::Remove_TwoChild_RotateRight(CTreeNode<T> *pDelNode)
{
    //������������ӽ��Ĵ���
    assert(pDelNode->m_pLeft != NULL
         && pDelNode->m_pRight != NULL);

    if (pDelNode->m_pNext != NULL)
    {
        RemoveFromLinkListHead_Two(pDelNode);
        return;
    }

    CTreeNode<T> *pChildLeft = pDelNode->m_pLeft;  //����
    CTreeNode<T> *pChildRight = pDelNode->m_pRight; //�Һ���

    //�ҵ�pDelNode��������
    CTreeNode<T> *pReplaceNode = pDelNode->m_pLeft;
    while (pReplaceNode->m_pRight != NULL)
    {
        pReplaceNode = pReplaceNode->m_pRight;
    }

    //����pReplaceNode��pDelNode���н���
    //pDelNode->m_Data = pReplaceNode->m_Data
    T Data = pDelNode->m_Data;
    pDelNode->m_Data = pReplaceNode->m_Data;
    pReplaceNode->m_Data = Data;

    CTreeNode<T> *pNext = pDelNode->m_pNext;
    pDelNode->m_pNext = pReplaceNode->m_pNext;
    pReplaceNode->m_pNext = pNext;

    unsigned long offset = pDelNode->m_nInfoOffset;
    pDelNode->m_nInfoOffset = pReplaceNode->m_nInfoOffset;
    pReplaceNode->m_nInfoOffset = offset;

    //����������Ҫɾ��pReplaceNode��
    //��pReplaceNode����ΪҶ�ӽ�㣬Ҳ���ܽ������ӣ����Ե���ǰ���ʵ��
    Remove(pReplaceNode);    
}

template<typename T>
void 
CMyBiTree<T>::RemoveFromLinkListHead_One(CTreeNode<T> *pDelNode)
{
     /*
            pParent

             pDelNode   pNext

        pChild


     ���¶��ϵĽ��д���
    */
    assert(pDelNode != NULL);

    CTreeNode<T> *pNext = pDelNode->m_pNext;
    
    //1)�ȴ���Child���, ��Left, ����Right
    CTreeNode<T> *pChild = pDelNode->m_pRight;
    bool IsLeft = false;
    if (pDelNode->m_pLeft != NULL)
    {
        pChild = pDelNode->m_pLeft;
        IsLeft = true;
    }
    
    //����Child�ĸ��ڵ�
    pChild->m_pParent = pNext;

    if (IsLeft)
    {
        pNext->m_pLeft = pChild;
    }
    else
    {
        pNext->m_pRight = pChild;
    }
      
    pNext->m_pPre = NULL;
    pNext->m_nBalanceFactor = pDelNode->m_nBalanceFactor;

    //2)�Ƿ�Ϊ���������
    if (pDelNode == m_pRoot)
    {
        m_pRoot = pNext;
        
        delete pDelNode;
        m_nCount--;
        return;
    }
    
    /*  �Ǹ����,�и����*/
    //3)��ȷ��T����Ǹ��������ӣ������Һ���
    CTreeNode<T> *pParentNode = pDelNode->m_pParent;
    bool IsLeft_Parent = false;
    if (pDelNode == pParentNode->m_pLeft)
    {
        IsLeft_Parent = true;
    }
    
    //4)���������P�ĺ��ӣ�����ң�ΪT���ĺ���
    if (IsLeft_Parent)
    {
        pParentNode->m_pLeft = pNext;
    }
    else
    {
        pParentNode->m_pRight = pNext;
    }
    
    //5)�ٵ���T���ĺ��ӵĸ����Ϊ���P
    pNext->m_pParent = pParentNode;
    
    //6)ɾ��Ŀ���㣬������һ
    delete pDelNode;
    m_nCount--;
    return;        
}

template<typename T>
void 
CMyBiTree<T>::Remove(CTreeNode<T> *pDelNode)
{
    if (NULL == pDelNode)
    {
        return;
    }

    /*
    ������Ϊ����������������ͬKey�Ľ����˴��������ԣ�pDelNodeҲ�����������еĽ�㣺
    1)��Ȼ��ԭ�е�˼·���з��ࣺҶ�ӽ�㣬��һ�����ӵĽ�㣬���������ӵĽ��
    2)����Щ�����������Ĵ�����
        a)Ҷ�ӽ�㣺�����еĽ��������������ǳ����޺����⣬��û�и���㡣
                    ����ģ�������ͷ��㣬�����и�������
        b)������
        c)˫���ӵ����
    */


    //Ҷ�ӽ��
    if (NULL == pDelNode->m_pLeft
        && NULL == pDelNode->m_pRight)  
    {
        //�������ΪΨһ�Ľ��, �����ڸ��ڵ�
        if (pDelNode == m_pRoot)    
        {
            //m_pRoot = NULL;
            m_pRoot = pDelNode->m_pNext;
            if (pDelNode->m_pNext != NULL)
            {
                pDelNode->m_pNext->m_pPre = NULL;
                pDelNode->m_pNext->m_nBalanceFactor = pDelNode->m_nBalanceFactor;
            }
            //����㣬���Һ��Ӿ������ڣ��������

            //��������߶Ȳ�
            delete pDelNode;
            m_nCount--;
            return;
        }

        //�϶����ڸ���㣬����Ҫ�ж���Ϊ���������ӣ������Һ���
        //�����������Ѿ��б仯
        CTreeNode<T> *pParentNode = pDelNode->m_pParent;
        if (NULL == pParentNode)
        {
            //��ʱpDelNode���������еĽ����
            //pPre pDelNode pNext
            CTreeNode<T> *pPre = pDelNode->m_pPre;
            CTreeNode<T> *pNext = pDelNode->m_pNext;

            pPre->m_pNext = pNext;
            if (pNext != NULL)
            {
                pNext->m_pPre = pPre;
            }
            
            delete pDelNode;
            m_nCount--;
            return;            
        }

        //����pDelNoe���������еĽ�㣬��������
        //��Ҳ���е����ġ�
        int nBF = pParentNode->m_nBalanceFactor;
        CTreeNode<T> *pNext = pDelNode->m_pNext;

        if (pDelNode == pParentNode->m_pLeft)
        { 
            pParentNode->m_pLeft = pNext;
            //���ǵ�RemoveҶ�ӽڵ�������ԣ����ȶ��丸���ĸ߶Ȳ���е���
            pParentNode->m_nBalanceFactor += ((-1) * heavy);
        }
        else
        {
            pParentNode->m_pRight = pNext;
            pParentNode->m_nBalanceFactor -= ((-1) * heavy);
        }

        if (pNext != NULL)
        {
            pNext->m_pPre = NULL;
            pNext->m_nBalanceFactor = pDelNode->m_nBalanceFactor;
            pNext->m_pParent = pParentNode;
        }

        //��Ҫ������ת
        if (2 == abs(pParentNode->m_nBalanceFactor))
        {
            CTreeNode<T> *pParentParentNode = pParentNode->m_pParent;
            bool IsParentLeft = false;
            if (pParentParentNode != NULL
                && pParentParentNode->m_pLeft == pParentNode)
            {
                IsParentLeft = true;
            }

            Rotate(pParentNode);
            
            //�����ڵ�ĸ߶ȱ仯��
            if (pParentParentNode != NULL)
            {
                if (IsParentLeft 
                    && abs(pParentParentNode->m_pLeft->m_nBalanceFactor) != abs(nBF)
                    )
                {
                   AdjustBF(pParentParentNode->m_pLeft, REMOVE);
                }
                else if (!IsParentLeft
                        && abs(pParentParentNode->m_pRight->m_nBalanceFactor) != abs(nBF)
                        )
                {
                    AdjustBF(pParentParentNode->m_pRight, REMOVE);
                }
            }
        }
        else    //�߶Ȳ����ֵ��2��������ת
        {
            if (abs(pParentNode->m_nBalanceFactor) - abs(nBF) <= 0)
            {
                //�����߶Ȳ�
                AdjustBF(pParentNode, REMOVE);
            }
        }
        
        delete pDelNode;
        m_nCount--;
        return;
    }
    //���������ӽ�� 
    else if (pDelNode->m_pLeft != NULL
            && pDelNode->m_pRight != NULL)
    {
        Remove_TwoChild_RotateRight(pDelNode);
        //Remove_TwoChild_RotateLeft(pDelNode);
    }
    //����һ�����ӽ��
    else    
    {
        /*
        �漰���Ľ�㣺P (�����), T(Ŀ����,pDelNode), C(child���, ����ΪLeft, Ҳ����ΪRight)
        1)��ȷ��C�����Left, ����Right
        2)�Ƿ��и���㣬���ֳ����������
        3)��ȷ��T����Ǹ��������ӣ������Һ���
        4)���������P�ĺ��ӣ�����ң�ΪT���ĺ���
        5)�ٵ���T���ĺ��ӵĸ����Ϊ���P
        6)ɾ��Ŀ���㣬������һ
        */

        //��Ϊ�����˶���ͬԪ�ص��������������ԣ����ȸý�����������׽��
        if (pDelNode->m_pNext != NULL)
        {
            RemoveFromLinkListHead_One(pDelNode);
            return;
        }
        
        //1)��ȷ��C�����Left, ����Right
        CTreeNode<T> *pChild = pDelNode->m_pRight;
        if (pDelNode->m_pLeft != NULL)
        {
            pChild = pDelNode->m_pLeft;
        }
       
        //2)�Ƿ�Ϊ���������
        if (pDelNode == m_pRoot)
        {
            m_pRoot = pChild;
            AdjustBF(pChild, REMOVE);

            delete pDelNode;
            m_nCount--;
            return;
        }

        /*  �Ǹ����,�и����*/
        //3)��ȷ��T����Ǹ��������ӣ������Һ���
        CTreeNode<T> *pParentNode = pDelNode->m_pParent;
        bool IsLeft_Parent = false;
        if (pDelNode == pParentNode->m_pLeft)
        {
            IsLeft_Parent = true;
        }

        //4)���������P�ĺ��ӣ�����ң�ΪT���ĺ���
        if (IsLeft_Parent)
        {
            pParentNode->m_pLeft = pChild;
        }
        else
        {
            pParentNode->m_pRight = pChild;
        }

        //5)�ٵ���T���ĺ��ӵĸ����Ϊ���P
        pChild->m_pParent = pParentNode;

        //6)ɾ��Ŀ���㣬������һ
        //AdjustBF(pDelNode, REMOVE); //��Ӧ����pDelNode
        AdjustBF(pChild, REMOVE);
        delete pDelNode;
        m_nCount--;
        return;        
    }//end for ����һ�����ӽ��
}

template<typename T>
void 
CMyBiTree<T>::Modify(CTreeNode<T> *pModifyNode, T NewData, unsigned long nInfoOffset)
{
    assert(pModifyNode != NULL);

    Remove(pModifyNode);
    Insert(NewData, nInfoOffset);
}

/////////////////   ƽ����������  /////////////////////////////////
template<typename T>
void 
CMyBiTree<T>::AdjustBF(CTreeNode<T> *pTargetNode, enum ACTION action)
{
BEGIN:
    if(NULL == pTargetNode)
    {
        return;
    }

    CTreeNode<T> *pParentNode = pTargetNode->m_pParent;
    if (NULL == pParentNode)
    {
        return;
    }

    /*
    1) ����pTargetNode �丸���ĸ߶Ȳ�϶��ǻᷢ���仯��
        ��action(��INSERT,����REMOVE)�ͺ��ӣ��󣬻����ң���ȷ��
        INSERT ���ӻ���REMOVE�Һ��ӣ���ʹ�ø߶Ȳ�����1
        INSERT �Һ��ӻ���REMOVE���ӣ���ʹ�ø߶Ȳ��С1

    2) ��Σ�����Ӱ�츸�����ĸ߶Ȳ�
       ���ǲ���Ӱ������
       INSERTʱ�����pTargetNode�����ĸ߶Ȳ�ľ���ֵ��С�ˣ��򲻻�Ӱ�츸�����
       REMOVEʱ�����pTargetNode�����ĸ߶Ȳ�ľ���ֵ����ˣ��򲻻�Ӱ�츸�����

       ����ΪӰ����������ʱ��Ҫ���������������ĸ߶Ȳ�
    */

    //�ж��Ǹ��������ӣ������Һ���
    bool IsLeft = false;
    if (pTargetNode == pParentNode->m_pLeft)
    {
        IsLeft = true;
    }

    //���游���ĸ߶Ȳ�
    int nBF = pParentNode->m_nBalanceFactor;

    //INSERT ���ӻ���REMOVE�Һ��ӣ���ʹ�ø߶Ȳ��С1(1Ϊ�����߸���)
    int factor = (-1) * heavy;
    if ( (INSERT == action && IsLeft)
         || (REMOVE == action && !IsLeft)
        )
    {
        pParentNode->m_nBalanceFactor -= (1 * factor);
    }
    //INSERT �Һ��ӻ���REMOVE���ӣ���ʹ�ø߶Ȳ�����1(1Ϊ�����߸�)
    else if ( (INSERT == action && !IsLeft )
             || (REMOVE == action && IsLeft)
            )
    {
        pParentNode->m_nBalanceFactor += (1 * factor);
    }

    /*added at 20140529 
    �������pParentNode�ĸ߶Ȳ����ֵΪ2������Ҫ������ת�����ˡ�
    */
    if (2 == abs(pParentNode->m_nBalanceFactor))
    {
        CTreeNode<T> *pParentParentNode = pParentNode->m_pParent;  //���游���ĸ����
        bool IsParentLeft = false;
        if (pParentParentNode != NULL
            && pParentParentNode->m_pLeft == pParentNode)
        {
            IsParentLeft = true;
        }

        Rotate(pParentNode);

        if (INSERT == action)       //INSERT���������������Ӱ��߶Ȳ�
        {
            return;
        }
        else if (REMOVE == action)  //REMOVE��������ܻ�������Ӱ��߶Ȳ�
        {
            //��������ĸ��������߱仯�ˣ�����Ը����ĸ������е���
            if (pParentParentNode != NULL)
            {
                if (IsParentLeft 
                    && abs(pParentParentNode->m_pLeft->m_nBalanceFactor) != abs(nBF)
                    )
                {
                    pTargetNode = pParentParentNode->m_pLeft;
                    goto BEGIN;
                }
                else if (!IsParentLeft
                        && abs(pParentParentNode->m_pRight->m_nBalanceFactor) != abs(nBF)
                        )
                {
                    pTargetNode = pParentParentNode->m_pRight;
                    goto BEGIN;
                }
                else
                {
                    return;
                }
            }
            else
            {
                return; 
            }
        }  
    }

    /*added end*/

    //INSERTʱ�����pTargetNode�����ĸ߶Ȳ�ľ���ֵ��С�ˣ��򲻻�Ӱ�츸�����
    //REMOVEʱ�����pTargetNode�����ĸ߶Ȳ�ľ���ֵ����ˣ��򲻻�Ӱ�츸�����
    int nChange = abs(pParentNode->m_nBalanceFactor) - abs(nBF);
    if ( (INSERT == action && nChange < 0)
        || (REMOVE == action && nChange > 0)
        )
    {
        return;
    }
    //��������Ӱ�츸�����
    else
    {
        pTargetNode = pParentNode;
        //AdjustBF(pTargetNode, action);
        goto BEGIN;
    } 
}

template<typename T>
void
CMyBiTree<T>::Rotate(CTreeNode<T> *pTargetNode)
{
    if (NULL == pTargetNode)
    {
        return;
    }

    /*��pParentNode��ΪĿ������㣬
    ��ת����עpParentNode�ĸ߶Ȳ�Լ������Һ��ӽ���о���ֵ���
      ��Ϊ:
      1) (2,1)      ����ת
      2) (2,-1)     ��˫��ת  ����������ת����������ת��
      3) (-2, -1)   �ҵ���ת 
      4) (-2, 1)    ��˫��ת   ����������ת����������ת

      ע��������heavy = RIGHT = -1�������
    */

    int factor = (-1) * heavy;  //����heavy = LEFT, RIGHT�ĵ�����ע�⴦��������RIGHT.

    //����ת
    if (2 * factor == pTargetNode->m_nBalanceFactor)
    {
        CTreeNode<T> *pRightChild = pTargetNode->m_pRight;
        CTreeNode<T> *pRightLeftChild = pRightChild->m_pLeft;

        //����ת
        if (1 * factor == pRightChild->m_nBalanceFactor)
        {
            RotateSingleLeft(pTargetNode);
        
            //�����������߶Ȳ�
            pTargetNode->m_nBalanceFactor = 0;
            pRightChild->m_nBalanceFactor = 0;
        }
        //��˫��ת
        else if (-1 * factor == pRightChild->m_nBalanceFactor)
        {
            RotateDoubleRight(pTargetNode);
            
            //�����������߶Ȳ�
            if (0 == pRightLeftChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = 0;
                pRightChild->m_nBalanceFactor = 0;
            }
            else if (1 * factor == pRightLeftChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = -1 * factor;
                pRightChild->m_nBalanceFactor = 0;
            }
            else if (-1 * factor == pRightLeftChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = 0;
                pRightChild->m_nBalanceFactor = 1 * factor;
            }
            else
            {
                cout << "��˫��ת����" << endl;
                getchar();
            }

            pRightLeftChild->m_nBalanceFactor = 0;
        }
        else    //Remove����г���
        {
            //http://adtinfo.org/libavl.html/Deleting-an-AVL-Node-Step-4-_002d-Rebalance.html#173
            RotateSingleLeft(pTargetNode);

            pRightChild->m_nBalanceFactor = (-1) * factor;
            pTargetNode->m_nBalanceFactor = 1 * factor;
        }
    }
    //����ת
    else if (-2 * factor == pTargetNode->m_nBalanceFactor)
    {
        CTreeNode<T> *pLeftChild = pTargetNode->m_pLeft;
        CTreeNode<T> *pLeftRightChild = pLeftChild->m_pRight;

        //�ҵ���ת
        if (-1 * factor == pLeftChild->m_nBalanceFactor)
        {
            RotateSingleRight(pTargetNode);
            
            //��ת������߶Ȳ�
            pTargetNode->m_nBalanceFactor = 0;
            pLeftChild->m_nBalanceFactor = 0;
        }
        //��˫��ת
        else if (1 * factor == pLeftChild->m_nBalanceFactor)
        {
            RotateDoubleLeft(pTargetNode);

            //��ת������߶Ȳ�
            if (0 == pLeftRightChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = 0;
                pLeftChild->m_nBalanceFactor = 0;
            }
            else if ((-1) * factor == pLeftRightChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = 1 * factor;
                pLeftChild->m_nBalanceFactor = 0;
            }
            else if (1 * factor == pLeftRightChild->m_nBalanceFactor)
            {
                pTargetNode->m_nBalanceFactor = 0;
                pLeftChild->m_nBalanceFactor = (-1) * factor;
            }
            else
            {
                cout << "��˫ѡ�� Sth wrong" << endl;
                getchar();
            }

            pLeftRightChild->m_nBalanceFactor = 0;            
        }
        else
        {
            //http://adtinfo.org/libavl.html/AVL-Deletion-Symmetric-Case.html
            RotateSingleRight(pTargetNode);

            pLeftChild->m_nBalanceFactor = 1 * factor;
            pTargetNode->m_nBalanceFactor = (-1) * factor;
        }
    }
    else
    {
        cout << "��תSth wrong" << endl;
        getchar();
    }
}

template<typename T>
void
CMyBiTree<T>::RotateSingleLeft(CTreeNode<T> *pTargetNode)
{
    /*
        pParent
            
             pTarget

                    pRightChild

            pRightLeftChild


    ��򵥵����ӣ�
    10(pTarget)
        15
            16


    ���ձ�����ӣ�
            50
        45      55 ��pTarget)
      40      53    60
                  58   65
                       ������65���������Ҳ����Ӻ��ӣ���������ת


  pParent
  
        pTarget
    
            pRightChild
      
      pRightLeftChild

    */
    CTreeNode<T> *pParentNode = pTargetNode->m_pParent;    //���ܲ�����
    CTreeNode<T> *pRightChild = pTargetNode->m_pRight;
    CTreeNode<T> *pRightLeftChild = pRightChild->m_pLeft;  //���ܲ�����

    /*���ĵ����϶��������������ģ�����㣬�󣨻��ң����ӣ���ʼ�հ���һ��ͳһ��˳�������е���
        �����϶��£����¶��ϵȵȣ� ������ѭ���϶��µĴ���
    */  

    //���ø����ĺ���
    if (pParentNode != NULL)
    {
        if (pParentNode->m_pLeft == pTargetNode)
        {
            pParentNode->m_pLeft = pRightChild;
        }
        else
        {
            pParentNode->m_pRight = pRightChild;
        }
    }
    else
    {
        m_pRoot = pRightChild;
    }

    //�����Һ��ӵĸ����(NULL���ԣ�
    pRightChild->m_pParent = pParentNode; 

    //�����Һ��ӵ�����
    pRightChild->m_pLeft = pTargetNode;

    //����Ŀ����ĸ����
    pTargetNode->m_pParent = pRightChild;

    //����Ŀ������Һ��ӣ����޶����ԣ�
    pTargetNode->m_pRight = pRightLeftChild;

    //�����Һ��ӵ����ӵĸ����
    if (pRightLeftChild != NULL)
    {
        pRightLeftChild->m_pParent = pTargetNode;
    }    
}

template<typename T>
void
CMyBiTree<T>::RotateSingleRight(CTreeNode<T> *pTargetNode)
{
    /*
                pParent
    
            pTarget
      
     pLeftChild
        
            pLeftRightChild


    ��ֱ�۵�һ�����ӣ�
        10(pTargetNode)
      8
    6

   ��ӵĴ��������ӣ�
                    50
            45��pTarget      55
        40    48                60
     35    43

    ��ʱ��35���������Ҳ����Ӻ��ӣ����������ҵ���ת

            pParent
  
        pTarget
    
  pLeftChild
      
       pLeftRightChild
    */
    CTreeNode<T> *pParentNode = pTargetNode->m_pParent;    //����û��
    CTreeNode<T> *pLeftChild = pTargetNode->m_pLeft;
    CTreeNode<T> *pLeftRightChild = pLeftChild->m_pRight;  //����û��

    /*
    ��ѭһ��ͳһ�ĵ���˳�����ﰴ�����϶��µĴ���
    */

    //���ø����ĺ���
    if (pParentNode != NULL)
    {
        if (pParentNode->m_pLeft == pTargetNode)
        {
            pParentNode->m_pLeft = pLeftChild;
        }
        else
        {
            pParentNode->m_pRight = pLeftChild;
        }      
    }
    else
    {
        m_pRoot = pLeftChild;
    }

    //�������ӵĸ��ڵ�
    pLeftChild->m_pParent = pParentNode;

    //�������ӵ��Һ���
    pLeftChild->m_pRight = pTargetNode; 

    //����Ŀ����ĸ����
    pTargetNode->m_pParent = pLeftChild;

    //����Ŀ���������
    pTargetNode->m_pLeft = pLeftRightChild;
    
    //�������ӵ��Һ��ӵĸ��ڵ�
    if (pLeftRightChild != NULL)
    {
        pLeftRightChild->m_pParent = pTargetNode; 
    }    
}

template<typename T>
void
CMyBiTree<T>::RotateDoubleLeft(CTreeNode<T> *pTargetNode)
{
    /*
    ��˫��ת������������ת����������ת
            pTarget
        pLeftChild
                pLeftRightChilds

    ��ֱ�۵����ӣ�
            10
         6
           8        

    */
    RotateSingleLeft(pTargetNode->m_pLeft);
    RotateSingleRight(pTargetNode);
} 

template<typename T>
void
CMyBiTree<T>::RotateDoubleRight(CTreeNode<T> *pTargetNode)
{
    /*
    ��˫��ת������������ת����������ת
        pTarget
            pRightChild
         pRightLeftChild


    ��ֱ�۵�����
        10
            15
          13
    */
    RotateSingleRight(pTargetNode->m_pRight);
    RotateSingleLeft(pTargetNode);
}

#if 0
template<typename T>
void
CMyBiTree<T>::RotateLeft_AdjustHeight(CTreeNode<T> *pTargetNode)
{
    /*
    ���������ת�ĸ߶Ȳ����
    �Լ���˫��ת�еĵ�һ������������ת

    */
    int factor = (-1) * heavy;
    if (2 * factor == pTargetNode->m_nBalanceFactor)
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pRight = 0;
    }
    else if (1 * factor == pTargetNode->m_nBalanceFactor
             && (-1) * factor == pTargetNode->m_pRight->m_nBalanceFactor)   //�����
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pRight->m_nBalanceFactor= 0;
    }
    else if (1 * factor == pTargetNode->m_nBalanceFactor
            && 1 * factor == pTargetNode->m_pRight->m_nBalanceFactor)   //�ұ���
    {
        pTargetNode->m_nBalanceFactor = (-1) * factor;
        pTargetNode->m_pRight->m_nBalanceFactor = 0;
    }
    else
    {
        int error;
        cout << "Adjust Height wrong" << endl;
        cin >> error;
    }
}

template<typename T>
void
CMyBiTree<T>::RotateRight_AdjustHeight(CTreeNode<T> *pTargetNode)
{
    /*
    ������ҵ���ת
    �Լ���˫��ת�еĵ�һ��������������ת
    */
    int factor = (-1) * heavy;
    if ((-2) * factor == pTargetNode->m_nBalanceFactor)
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pLeft->m_nBalanceFactor = 0;
    }
    else if ((-1) * factor == pTargetNode->m_nBalanceFactor
             && (-1) * factor == pTargetNode->m_pLeft->m_nBalanceFactor)    //�����
    {
        pTargetNode->m_nBalanceFactor = 1 * factor;
        pTargetNode->m_pLeft->m_nBalanceFactor = 0;
    }
    else if ((-1) * factor == pTargetNode->m_nBalanceFactor
            && 1 * factor == pTargetNode->m_pLeft->m_nBalanceFactor)    //�ұ���
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pLeft->m_nBalanceFactor = 0;
    }
    else
    {
        int error;
        cout << "Adjust Height wrong" << endl;
        cin >> error;
    }
}
#endif

#endif