#ifndef STACK_H_5FEBF37E_9807_4edc_AEAF_D18800B8C31E

#define STACK_H_5FEBF37E_9807_4edc_AEAF_D18800B8C31E

#include "DLinkList.h"

template<typename T>
class CMyStack : private CDLinkList<T>
{
public:
    CDLinkList<T>::IsEmpty;   //�Լ̳��Ը���ĳ�Ա����������
    CDLinkList<T>::Walk;
    CDLinkList<T>::GetCount;
    
    void Push(T data)
    {
        AddTail(data);
    }

    void Pop()
    {
        if (!IsEmpty())
        {
            RemoveTail();
        }
    }

    T GetTop(void)
    {
        return GetTail()->m_data;
    }
};

#endif