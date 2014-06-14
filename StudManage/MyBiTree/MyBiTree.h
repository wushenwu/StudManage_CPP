#ifndef MYBITREE_H_D2A3F9C8_352D_4ce5_B745_AC400CDBBE1D

#define MYBITREE_H_D2A3F9C8_352D_4ce5_B745_AC400CDBBE1D

#include <iostream>
using namespace std;

#include "Stack.h"
#include "MyQueue.h"
#include "struct.h"

int EleCompare(const char *psz1, const char *psz2)
{
    return strcmp(psz1, psz2);
}

int EleCompare(tag_info &info1, tag_info &info2)
{
    if (-1 == info1.m_n2)
    {
        if (info1.m_n1 == info2.m_n1)
        {
            return 0;
        }
        else if (info1.m_n1 < info2.m_n1)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }

    if (info1.m_n1 == info2.m_n1
        && info1.m_n2 == info2.m_n2)
    {
        return 0;
    }
    else if (info1.m_n1 > info2.m_n1
        || (info1.m_n1 == info2.m_n1
            && info1.m_n2 > info2.m_n2)
            )
    {
        return 1;
    }
    else if ( info1.m_n1 < info2.m_n1
        || (info1.m_n1 == info2.m_n1
            && info1.m_n2 < info2.m_n2)
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
    CTreeNode(T Data)
    {
        m_Data = Data;
        m_pParent = NULL;
        m_pLeft   = NULL;
        m_pRight  = NULL;

        m_nBalanceFactor = 0;   //新结点的高度差为0
    }
    
private:
    T m_Data;            //结点数据
    CTreeNode<T> *m_pParent;   //父结点指针
    CTreeNode<T> *m_pLeft;     //左孩子指针
    CTreeNode<T> *m_pRight;    //右孩子指针

    int m_nBalanceFactor;       //高度差，平衡因子BalanceFactor, for AVL
};

template<>
CTreeNode<char *>::CTreeNode<char *>(char *Data)
{
    //m_Data = Data;
    m_Data = new char[strlen(Data) + 1];
    strcpy(m_Data, Data);

    m_pParent = NULL;
    m_pLeft   = NULL;
    m_pRight  = NULL;
    
    m_nBalanceFactor = 0;   //新结点的高度差为0
}

template<typename T>
class CMyBiTree
{
public:
    enum ACTION {REMOVE = -1, INSERT = 1};
    enum HEAVY{LEFT = 1, RIGHT = -1};   //左边重，还是右边重，高度差调整时涉及。
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
    Function: 获取根结点
    */
    CTreeNode<T> *GetRoot(void) const
    {
        return m_pRoot;
    }

    /*
    Function: 将Data 插入到pTargetNode的左边或者右边，并返回该结点
    */
    //CTreeNode *Insert(T Data, CTreeNode *pTargetNode, bool IsLeft);

    /*
    Function: 针对根结点pRoot的前序遍历、中序遍历、后序遍历，递归方式
    */
    void PreOrder(CTreeNode<T> *pRoot);
    void MidOrder(CTreeNode<T> *pRoot);
    void PostOrder(CTreeNode<T> *pRoot);

    //按层进行遍历
    void LevelWalk(CTreeNode<T> *pRoot);

    /*
    Function: 针对根结点pRoot的前序遍历、中序遍历、后序遍历，非递归方式
    */
    //void PreOrder_Non(CTreeNode *pRoot);
    //void MidOrder_Non(CTreeNode *pRoot);
    //void PostOrder_Non(CTreeNode *pRoot);

    ////////////////  针对二叉排序树的处理  //////////////////////////////////////////
    /*
    Function:将给定的Data 有序的插入到二叉排序树中. 不支持重复数据的插入
    */
    void Insert(T Data);

    /*
    Function:查找给定的Data是否存在，如果存在则返回相应结点信息，否则NULL
    */
    CTreeNode<T> *Find(T Data);

    CMyQueue<T> *Find(T Data, bool IsSpecial);

    /*
    Function: 删除给定的目标结点，该目标结点可通过Find查找
    */
    void Remove(CTreeNode<T> *pDelNode);

    /*
    Function: 删除有两个子结点的结点，向左旋转或向右旋转
             使得被删除结点的左边最大数替代该结点，
             或者被删除结点的右边最小数替代该结点
    */
    void Remove_TwoChild_RotateLeft(CTreeNode<T> *pDelNode);
    void Remove_TwoChild_RotateRight(CTreeNode<T> *pDelNode);

    /*
    Function: 将给定结点pModifyNode的原值该为新值nNewData
    */
    void Modify(CTreeNode<T> *pModifyNode, T NewData);

    //////////////// 二叉平衡树AVL相关   /////////////////////////////////////////
    /*
    Function: 调整目标结点的高度差（平衡因子）
    */
    void AdjustBF(CTreeNode<T> *pTargetNode, enum ACTION action);

    /*
    Function: 针对目标结点的旋转处理
    */
    void Rotate(CTreeNode<T> *pTargetNode);
    void RotateSingleLeft(CTreeNode<T> *pTargetNode);
    void RotateSingleRight(CTreeNode<T> *pTargetNode);
    void RotateDoubleLeft(CTreeNode<T> *pTargetNode);
    void RotateDoubleRight(CTreeNode<T> *pTargetNode);

    /*
    Function:针对目标结点的高度差调整
    */
    void RotateLeft_AdjustHeight(CTreeNode<T> *pTargetNode);
    void RotateRight_AdjustHeight(CTreeNode<T> *pTargetNode);

 
private:
    /*
    Function: 后序遍历删除结点，服务于析构函数
    */
    void PostOrder_Del(CTreeNode<T> *pRoot);

    void MidOrder(CTreeNode<tag_info> *pRoot, 
                CMyQueue<tag_info> *pResultQueue, 
                tag_info &TargetInfo);

private:
    CTreeNode<T> *m_pRoot;     //根结点指针

    //辅助
    unsigned int m_nCount;  //结点总数
    static enum HEAVY heavy;
};

template<typename T>
CMyBiTree<T>::HEAVY CMyBiTree<T>::heavy = CMyBiTree<T>::RIGHT;

////////////////  实现  ///////////////////////////////////////
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
    //运用后序遍历来删除结点
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

    //空树
    if (NULL == m_pRoot)
    {
        m_pRoot = pNewNode;
    }
    else
    {
        //根据左右，设置父结点的左、右孩子
        if (IsLeft)
        {
            pTargetNode->m_pLeft = pNewNode;    
        }
        else
        {
            pTargetNode->m_pRight = pNewNode;
        }

        //设置新结点的父结点
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
        //将右孩子压栈
        if (pRoot->m_pRight != NULL && flg_push)
        {
            stack.Push(pRoot->m_pRight);
            cout << "   " << stack.GetTop()->m_Data << " ";
        }

        //将自身压栈
        if (pRoot != NULL && flg_push)
        {
            stack.Push(pRoot);
            cout << "   " << stack.GetTop()->m_Data << " ";
        }

        /* 处理左孩子 */
        if (pRoot->m_pLeft != NULL && flg_push)
        {
            //左孩子不空，让左孩子作为根节点继续
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
                //此时，先将自身结点输出 (中)
                cout << endl << (stack.GetTop()->m_Data) << " ";
                stack.Pop();
                
                //再将其父结点输出
                cout << endl << (stack.GetTop()->m_Data) << " ";
                stack.Pop();
                
                pRoot = stack.GetTop();
            }

            nCount = stack.GetCount();
            if(2 == nCount)
            {
                //此时肯定是右，中结点
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
        //将当前结点压栈
        if (pCurNode != NULL)
        {
            stack.Push(pCurNode);
        }

        //将当前结点的右孩子压栈
        if (pCurNode->m_pRight != NULL)
        {
            stack.Push(pCurNode->m_pRight);
        }

        //当前结点的左孩子非空，则将左孩子作为当前结点，继续进行
        if (pCurNode->m_pLeft != NULL)
        {
            pCurNode = pCurNode->m_pLeft;
        }
        else
        {
            /* 当前结点的左孩子为空，需要处理右孩子 */

            //有右结点，则栈top就为右孩子
            if (pCurNode->m_pRight != NULL)
            {
                pCurNode = stack.GetTop();
                stack.Pop();
            }
            else
            {
                //没右结点，则左、右都处理完，可输出此结点
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
             ;

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

////////////////// 二叉排序树相关 /////////////////////////////////
template<typename T>
void
CMyBiTree<T>::Insert(T Data)
{
    CTreeNode<T> *pNewNode = new CTreeNode<T>(Data);
    
    if (NULL == pNewNode)
    {
        //TBD
    }

    //尚为空树，
    if (NULL == m_pRoot)
    {
        m_pRoot = pNewNode;
        m_nCount++;

        AdjustBF(pNewNode, INSERT);
        return;
    }

    /*接下来，就是有序的插入到二叉排序树中 
    1) 先找到要插入的位置 （小 左， 大 右）
    2) 更改相关结点的关系
    */
    int result = 0;
    CTreeNode<T> *pCurNode = m_pRoot;
    while (true)
    {
        result = EleCompare(pNewNode->m_Data, pCurNode->m_Data);
        //不支持重复结点的插入
        //if (pNewNode->m_Data == pCurNode->m_Data)
        if (0 == result)
        {
            //TBD
            if (pNewNode != NULL)
            {
                delete pNewNode;
                pNewNode = NULL;
            }
            return;
        }
        else if (0 > result) //查找左子树
        {
            //如果即将遍历的pCurNode的左结点为空，则可插入pCurNode的左结点处。
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
        else                                    //查找右子树
        {
            //如果即将遍历的pCurNode的右结点为空，则可插入pCurNode的右结点处。
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

        //if (Data < pCurNode->m_Data)  //查找左子树
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

template<>
void 
CMyBiTree<tag_info>::MidOrder(CTreeNode<tag_info> *pRoot, CMyQueue<tag_info> *pResultQueue, tag_info &TargetInfo)
{
    if (pRoot != NULL)
    {   
        if (pRoot->m_Data.m_n1 != TargetInfo.m_n1)
        {
            return;
        }
        
        MidOrder(pRoot->m_pLeft, pResultQueue, TargetInfo);
        
        if (pRoot->m_Data.m_n1 == TargetInfo.m_n1)
        {
            pResultQueue->EnQueue(pRoot->m_Data);
        }
        
        MidOrder(pRoot->m_pRight, pResultQueue, TargetInfo);
    }
}


template<>
CMyQueue<tag_info> *
CMyBiTree<tag_info>::Find(tag_info Data, bool IsSpecial)
{
    CTreeNode<tag_info> *pCurNode = m_pRoot;
    CMyQueue<tag_info> *pResultQueue = new CMyQueue<tag_info>;
    
    while (pCurNode != NULL)
    {
        //if (Data == pCurNode->m_Data)
        if (0 == EleCompare(Data, pCurNode->m_Data))
        {
            MidOrder(pCurNode, pResultQueue, Data);
            break;
        }
        
        //if (Data < pCurNode->m_Data)  //查找左子树
        else if (0 > EleCompare(Data, pCurNode->m_Data))
        {
            if (Data.m_n1 == pCurNode->m_Data.m_n1)
            {
                pResultQueue->EnQueue(pCurNode->m_Data);
            }

            pCurNode = pCurNode->m_pLeft;
        }
        else
        {
            if (Data.m_n1 == pCurNode->m_Data.m_n1)
            {
                pResultQueue->EnQueue(pCurNode->m_Data);
            }

            pCurNode = pCurNode->m_pRight;
        }
    }
    
    //return pCurNode;
    return pResultQueue;
}


/*
针对有两个孩子结点的处理，向左旋转——让被删除结点右侧最小值替换删除结点
*/
template<typename T>
void
CMyBiTree<T>::Remove_TwoChild_RotateLeft(CTreeNode<T> *pDelNode)
{
    //针对有两个孩子结点的处理
    assert(pDelNode->m_pLeft != NULL
         && pDelNode->m_pRight != NULL);

    CTreeNode<T> *pChildLeft = pDelNode->m_pLeft;  //左孩子
    CTreeNode<T> *pChildRight = pDelNode->m_pRight; //右孩子

    //找到pDelNode右边最小结点
    CTreeNode<T> *pReplaceNode = pDelNode->m_pRight;
    while (pReplaceNode->m_pLeft != NULL)
    {
        pReplaceNode = pReplaceNode->m_pLeft;
    }

    //将该pReplaceNode与pDelNode进行交换
    //pDelNode->m_Data = pReplaceNode->m_Data
    T Data = pDelNode->m_Data;
    pDelNode->m_Data = pReplaceNode->m_Data;
    pReplaceNode->m_Data = Data;
    
    //现在我们需要删除pReplaceNode，
    //但pReplaceNode可能为叶子结点，也可能仅有左孩子，可以调用前面的实现
    Remove(pReplaceNode); 
}

/*
针对有两个孩子结点的处理，向右旋转——让被删除结点左侧最大值替换删除结点
*/
template<typename T>
void
CMyBiTree<T>::Remove_TwoChild_RotateRight(CTreeNode<T> *pDelNode)
{
    //针对有两个孩子结点的处理
    assert(pDelNode->m_pLeft != NULL
         && pDelNode->m_pRight != NULL);

    CTreeNode<T> *pChildLeft = pDelNode->m_pLeft;  //左孩子
    CTreeNode<T> *pChildRight = pDelNode->m_pRight; //右孩子

    //找到pDelNode左边最大结点
    CTreeNode<T> *pReplaceNode = pDelNode->m_pLeft;
    while (pReplaceNode->m_pRight != NULL)
    {
        pReplaceNode = pReplaceNode->m_pRight;
    }

    //将该pReplaceNode与pDelNode进行交换
    //pDelNode->m_Data = pReplaceNode->m_Data
    T Data = pDelNode->m_Data;
    pDelNode->m_Data = pReplaceNode->m_Data;
    pReplaceNode->m_Data = Data;

    //现在我们需要删除pReplaceNode，
    //但pReplaceNode可能为叶子结点，也可能仅有左孩子，可以调用前面的实现
    Remove(pReplaceNode);    
}

template<typename T>
void 
CMyBiTree<T>::Remove(CTreeNode<T> *pDelNode)
{
    if (NULL == pDelNode)
    {
        return;
    }

    //叶子结点
    if (NULL == pDelNode->m_pLeft
        && NULL == pDelNode->m_pRight)  
    {
        //根结点作为唯一的结点, 不存在父节点
        if (pDelNode == m_pRoot)    
        {
            m_pRoot = NULL;

            //无需调整高度差
            delete pDelNode;
            m_nCount--;
            return;
        }

        //肯定存在父结点，但是要判断是为父结点的左孩子，还是右孩子
        CTreeNode<T> *pParentNode = pDelNode->m_pParent;
        int nBF = pParentNode->m_nBalanceFactor;

        if (pDelNode == pParentNode->m_pLeft)
        { 
            pParentNode->m_pLeft = NULL;
            //考虑到Remove叶子节点的特殊性，首先对其父结点的高度差进行调整
            pParentNode->m_nBalanceFactor += ((-1) * heavy);
        }
        else
        {
            pDelNode->m_pParent->m_pRight = NULL;
            pParentNode->m_nBalanceFactor -= ((-1) * heavy);
        }

        //需要进行旋转
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
            
            //父父节点的高度变化了
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
        else    //高度差绝对值非2，无需旋转
        {
            if (abs(pParentNode->m_nBalanceFactor) - abs(nBF) <= 0)
            {
                //调整高度差
                AdjustBF(pParentNode, REMOVE);
            }
        }
        
        delete pDelNode;
        m_nCount--;
        return;
    }
    //有两个孩子结点 
    else if (pDelNode->m_pLeft != NULL
            && pDelNode->m_pRight != NULL)
    {
        Remove_TwoChild_RotateRight(pDelNode);
        //Remove_TwoChild_RotateLeft(pDelNode);
    }
    //仅有一个孩子结点
    else    
    {
        /*
        涉及到的结点：P (父结点), T(目标结点,pDelNode), C(child结点, 可能为Left, 也可能为Right)
        1)先确定C结点是Left, 还是Right
        2)是否有父结点，区分出根结点特例
        3)再确定T结点是父结点的左孩子，还是右孩子
        4)调整父结点P的孩子（左或右）为T结点的孩子
        5)再调整T结点的孩子的父结点为结点P
        6)删除目标结点，计数减一
        */

        //1)先确定C结点是Left, 还是Right
        CTreeNode<T> *pChild = pDelNode->m_pRight;
        if (pDelNode->m_pLeft != NULL)
        {
            pChild = pDelNode->m_pLeft;
        }
       
        //2)是否为根结点特例
        if (pDelNode == m_pRoot)
        {
            m_pRoot = pChild;
    
            //
            AdjustBF(pChild, REMOVE);

            delete pDelNode;
            m_nCount--;
            return;
        }

        /*  非根结点,有父结点*/
        //3)再确定T结点是父结点的左孩子，还是右孩子
        CTreeNode<T> *pParentNode = pDelNode->m_pParent;
        bool IsLeft_Parent = false;
        if (pDelNode == pParentNode->m_pLeft)
        {
            IsLeft_Parent = true;
        }

        //4)调整父结点P的孩子（左或右）为T结点的孩子
        if (IsLeft_Parent)
        {
            pParentNode->m_pLeft = pChild;
        }
        else
        {
            pParentNode->m_pRight = pChild;
        }

        //5)再调整T结点的孩子的父结点为结点P
        pChild->m_pParent = pParentNode;

        //6)删除目标结点，计数减一
        //AdjustBF(pDelNode, REMOVE); //不应该是pDelNode
        AdjustBF(pChild, REMOVE);
        delete pDelNode;
        m_nCount--;
        return;        
    }//end for 仅有一个孩子结点
}

template<typename T>
void 
CMyBiTree<T>::Modify(CTreeNode<T> *pModifyNode, T NewData)
{
    assert(pModifyNode != NULL);

    Remove(pModifyNode);
    Insert(NewData);
}

/////////////////   平衡二叉树相关  /////////////////////////////////
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
    1) 首先pTargetNode 其父结点的高度差，肯定是会发生变化的
        由action(是INSERT,还是REMOVE)和孩子（左，还是右）来确定
        INSERT 左孩子或者REMOVE右孩子，都使得高度差增加1
        INSERT 右孩子或者REMOVE左孩子，都使得高度差减小1

    2) 其次，可能影响父父结点的高度差
       考虑不会影响的情况
       INSERT时，如果pTargetNode父结点的高度差的绝对值变小了，则不会影响父父结点
       REMOVE时，如果pTargetNode父结点的高度差的绝对值变大了，则不会影响父父结点

       其他为影响的情况，此时需要继续调整父父结点的高度差
    */

    //判定是父结点的左孩子，还是右孩子
    bool IsLeft = false;
    if (pTargetNode == pParentNode->m_pLeft)
    {
        IsLeft = true;
    }

    //保存父结点的高度差
    int nBF = pParentNode->m_nBalanceFactor;

    //INSERT 左孩子或者REMOVE右孩子，都使得高度差减小1(1为正或者负，)
    int factor = (-1) * heavy;
    if ( (INSERT == action && IsLeft)
         || (REMOVE == action && !IsLeft)
        )
    {
        pParentNode->m_nBalanceFactor -= (1 * factor);
    }
    //INSERT 右孩子或者REMOVE左孩子，都使得高度差增加1(1为正或者负)
    else if ( (INSERT == action && !IsLeft )
             || (REMOVE == action && IsLeft)
            )
    {
        pParentNode->m_nBalanceFactor += (1 * factor);
    }

    /*added at 20140529 
    如果现在pParentNode的高度差绝对值为2，则需要进行旋转处理了。
    */
    if (2 == abs(pParentNode->m_nBalanceFactor))
    {
        CTreeNode<T> *pParentParentNode = pParentNode->m_pParent;  //保存父结点的父结点
        bool IsParentLeft = false;
        if (pParentParentNode != NULL
            && pParentParentNode->m_pLeft == pParentNode)
        {
            IsParentLeft = true;
        }

        Rotate(pParentNode);

        if (INSERT == action)       //INSERT情况，不会再向上影响高度差
        {
            return;
        }
        else if (REMOVE == action)  //REMOVE情况，可能会再向上影响高度差
        {
            //如果父结点的父结点的树高变化了，则还需对父结点的父结点进行调整
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

    //INSERT时，如果pTargetNode父结点的高度差的绝对值变小了，则不会影响父父结点
    //REMOVE时，如果pTargetNode父结点的高度差的绝对值变大了，则不会影响父父结点
    int nChange = abs(pParentNode->m_nBalanceFactor) - abs(nBF);
    if ( (INSERT == action && nChange < 0)
        || (REMOVE == action && nChange > 0)
        )
    {
        return;
    }
    //其他均会影响父父结点
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

    /*将pParentNode作为目标调整点，
    旋转仅关注pParentNode的高度差，以及其左右孩子结点中绝对值最大
      分为:
      1) (2,1)      左单旋转
      2) (2,-1)     右双旋转  （先向右旋转，再向左旋转）
      3) (-2, -1)   右单旋转 
      4) (-2, 1)    左双旋转   （先向左旋转，再向右旋转

      注：这是在heavy = RIGHT = -1的情况下
    */

    int factor = (-1) * heavy;  //根据heavy = LEFT, RIGHT的调整。注意处理基础是RIGHT.

    //左旋转
    if (2 * factor == pTargetNode->m_nBalanceFactor)
    {
        CTreeNode<T> *pRightChild = pTargetNode->m_pRight;
        CTreeNode<T> *pRightLeftChild = pRightChild->m_pLeft;

        //左单旋转
        if (1 * factor == pRightChild->m_nBalanceFactor)
        {
            RotateSingleLeft(pTargetNode);
        
            //现在来调整高度差
            pTargetNode->m_nBalanceFactor = 0;
            pRightChild->m_nBalanceFactor = 0;
        }
        //右双旋转
        else if (-1 * factor == pRightChild->m_nBalanceFactor)
        {
            RotateDoubleRight(pTargetNode);
            
            //现在来调整高度差
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
                cout << "右双旋转出错" << endl;
                getchar();
            }

            pRightLeftChild->m_nBalanceFactor = 0;
        }
        else    //Remove情况中出现
        {
            //http://adtinfo.org/libavl.html/Deleting-an-AVL-Node-Step-4-_002d-Rebalance.html#173
            RotateSingleLeft(pTargetNode);

            pRightChild->m_nBalanceFactor = (-1) * factor;
            pTargetNode->m_nBalanceFactor = 1 * factor;
        }
    }
    //右旋转
    else if (-2 * factor == pTargetNode->m_nBalanceFactor)
    {
        CTreeNode<T> *pLeftChild = pTargetNode->m_pLeft;
        CTreeNode<T> *pLeftRightChild = pLeftChild->m_pRight;

        //右单旋转
        if (-1 * factor == pLeftChild->m_nBalanceFactor)
        {
            RotateSingleRight(pTargetNode);
            
            //旋转后调整高度差
            pTargetNode->m_nBalanceFactor = 0;
            pLeftChild->m_nBalanceFactor = 0;
        }
        //左双旋转
        else if (1 * factor == pLeftChild->m_nBalanceFactor)
        {
            RotateDoubleLeft(pTargetNode);

            //旋转后调整高度差
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
                cout << "左双选择 Sth wrong" << endl;
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
        cout << "旋转Sth wrong" << endl;
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


    最简单的例子：
    10(pTarget)
        15
            16


    最普遍的例子：
            50
        45      55 （pTarget)
      40      53    60
                  58   65
                       现在在65的左侧或者右侧添加孩子，均会左单旋转


  pParent
  
        pTarget
    
            pRightChild
      
      pRightLeftChild

    */
    CTreeNode<T> *pParentNode = pTargetNode->m_pParent;    //可能不存在
    CTreeNode<T> *pRightChild = pTargetNode->m_pRight;
    CTreeNode<T> *pRightLeftChild = pRightChild->m_pLeft;  //可能不存在

    /*结点的调整肯定都是两两处理的，父结点，左（或右）孩子，可始终按照一个统一的顺序来进行调整
        如自上而下，自下而上等等， 这里遵循自上而下的处理
    */  

    //设置父结点的孩子
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

    //设置右孩子的父结点(NULL可以）
    pRightChild->m_pParent = pParentNode; 

    //设置右孩子的左孩子
    pRightChild->m_pLeft = pTargetNode;

    //设置目标结点的父结点
    pTargetNode->m_pParent = pRightChild;

    //设置目标结点的右孩子（有无都可以）
    pTargetNode->m_pRight = pRightLeftChild;

    //设置右孩子的左孩子的父结点
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


    最直观的一个例子：
        10(pTargetNode)
      8
    6

   最复杂的代表性例子：
                    50
            45（pTarget      55
        40    48                60
     35    43

    此时在35的左侧或者右侧添加孩子，都会引起右单旋转

            pParent
  
        pTarget
    
  pLeftChild
      
       pLeftRightChild
    */
    CTreeNode<T> *pParentNode = pTargetNode->m_pParent;    //可能没有
    CTreeNode<T> *pLeftChild = pTargetNode->m_pLeft;
    CTreeNode<T> *pLeftRightChild = pLeftChild->m_pRight;  //可能没有

    /*
    遵循一个统一的调整顺序，这里按照自上而下的处理
    */

    //设置父结点的孩子
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

    //设置左孩子的父节点
    pLeftChild->m_pParent = pParentNode;

    //设置左孩子的右孩子
    pLeftChild->m_pRight = pTargetNode; 

    //设置目标结点的父结点
    pTargetNode->m_pParent = pLeftChild;

    //设置目标结点的左孩子
    pTargetNode->m_pLeft = pLeftRightChild;
    
    //设置左孩子的右孩子的父节点
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
    左双旋转——先向左旋转，再向右旋转
            pTarget
        pLeftChild
                pLeftRightChilds

    最直观的例子：
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
    右双旋转——先向右旋转，再向左旋转
        pTarget
            pRightChild
         pRightLeftChild


    最直观的例子
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
    这包括左单旋转的高度差调整
    以及左双旋转中的第一步——先左旋转

    */
    int factor = (-1) * heavy;
    if (2 * factor == pTargetNode->m_nBalanceFactor)
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pRight = 0;
    }
    else if (1 * factor == pTargetNode->m_nBalanceFactor
             && (-1) * factor == pTargetNode->m_pRight->m_nBalanceFactor)   //左边重
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pRight->m_nBalanceFactor= 0;
    }
    else if (1 * factor == pTargetNode->m_nBalanceFactor
            && 1 * factor == pTargetNode->m_pRight->m_nBalanceFactor)   //右边重
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
    这包括右单旋转
    以及右双旋转中的第一步——先向右旋转
    */
    int factor = (-1) * heavy;
    if ((-2) * factor == pTargetNode->m_nBalanceFactor)
    {
        pTargetNode->m_nBalanceFactor = 0;
        pTargetNode->m_pLeft->m_nBalanceFactor = 0;
    }
    else if ((-1) * factor == pTargetNode->m_nBalanceFactor
             && (-1) * factor == pTargetNode->m_pLeft->m_nBalanceFactor)    //左边重
    {
        pTargetNode->m_nBalanceFactor = 1 * factor;
        pTargetNode->m_pLeft->m_nBalanceFactor = 0;
    }
    else if ((-1) * factor == pTargetNode->m_nBalanceFactor
            && 1 * factor == pTargetNode->m_pLeft->m_nBalanceFactor)    //右边重
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