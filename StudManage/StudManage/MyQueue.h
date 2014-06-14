#ifndef MYQUEUE_H_CFC9197A_8101_42e2_A84B_92117E846AE5

#define MYQUEUE_H_CFC9197A_8101_42e2_A84B_92117E846AE5

#include "DLinkList.h"

template<typename T>
class CMyQueue: private CDLinkList<T>
{
public:
    CDLinkList<T>::IsEmpty;
    CDLinkList<T>::Walk;

    void EnQueue(T data)
    {
        AddTail(data);
    }

    void DeQueue(void)
    {
        if (!IsEmpty())
        {
            RemoveHead();
        } 
    }

    T GetFront(void)
    {
        return GetHead()->m_data;
    }
};

#endif