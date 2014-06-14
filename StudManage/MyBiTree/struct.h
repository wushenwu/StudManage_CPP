#ifndef STRUCT_H_XXXX

#define STRUCT_H_XXXX

#include <iostream>
using namespace std;

struct tag_info
{
public:
    tag_info(int n1 = -1, int n2 = -1)
    {
        m_n1 = n1;
        m_n2 = n2;

    }

public:
    friend ostream & operator << (ostream &obj, tag_info &info)
    {
        obj << " ( " << info.m_n1 << ", " << info.m_n2 << " )" << endl;
        return obj;
    }
public:
    int m_n1;
    int m_n2;
};

#endif