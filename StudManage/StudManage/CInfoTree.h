/*
BaseInfoTree:  BaseInfoIDTree, BaseInfoIDTreeFile
               BaseInfoNameTree, BaseInfoNameTreeFile

StudInfoTree: StudInfoIDTree, StudInfoIDTreeFile
              StudInfoNameTree, StudInfoNameTreeFile

CourseInfoTree: CourseInfoIDTree, CourseInfoIDTreeFile
                CourseInfoNameTree, CourseInfoNameTreeFile

TeacherInfoTree: TeacherInfoIDTree, TeacherInfoIDTreeFile
                 TeacherInfoNameTree, TeacherInfoNameTreeFile

SelectInfoTree: SelectInfoStudIDTree, SelectInfoStudIDTreeFile
                SelectInfoCourseIDTree, SelectInfoCourseIDTreeFile
*/
#ifndef CINFOTREE_H_52539051_33E1_4b3d_B0DE_BB07A1785CB9

#define CINFOTREE_H_52539051_33E1_4b3d_B0DE_BB07A1785CB9

#include "MyBiTree.h"
#include "CMyFile.h"

#if 0
#define IS_FILE true

//二叉树在文件中的存储信息
class CFileNodeInfo
{
    //可使用友元来访问处理 
public:
    CFileNodeInfo(int nParentOffset = -1, 
                 int nLeftOffset = -1,
                 int nRightOffset = -1,
                 int nInfoOffset = -1)
    {
        m_nParentOffset = nParentOffset;
        m_nLeftOffset = nLeftOffset;
        m_nRightOffset = nRightOffset;
        m_nInfoOffset = nInfoOffset;
    }

protected:
    int m_nParentOffset; //该结点的父结点在二叉树文件中的偏移
    int m_nLeftOffset;  //该结点的左孩子在二叉树文件中的偏移
    int m_nRightOffset; //该结点的右孩子在二叉树文件中的偏移
    int m_nInfoOffset;  //该结点对应信息在信息文件中的偏移
};

class CBaseInfoTree
{
public:
    CBaseInfoTree(const char *pszIDTreeFile = "IDTree.txt",
                  const char *pszNameTreeFile = "NameTree.txt")
                  : m_IDTreeFile(pszIDTreeFile),
                    m_NameTreeFile(pszNameTreeFile),
                    m_IDTree(IS_FILE, &m_IDTreeFile),
                    m_NameTree(IS_FILE, &m_NameTreeFile)
    {

    }
    
protected:
    CMyFile m_IDTreeFile;
    CMyFile m_NameTreeFile;

    CMyBiTree<char *> m_IDTree;
    CMyBiTree<char *> m_NameTree;
};

class CStudInfoTree: public CBaseInfoTree
{
public:
    CStudInfoTree(const char *pszIDTreeFile = "StudIDTree.txt",
                  const char *pszNameTreeFile = "StudNameTree.txt")
                  :CBaseInfoTree(pszIDTreeFile, pszNameTreeFile)
    {

    }
};

class CCourseInfoTree: public CBaseInfoTree
{
public:
    CCourseInfoTree(const char *pszIDTreeFile = "CourseIDTree.txt",
                  const char *pszNameTreeFile = "CourseNameTree.txt")
                  :CBaseInfoTree(pszIDTreeFile, pszNameTreeFile)
    {

    }
};

class CTeacherInfoTree: public CBaseInfoTree
{
public:
    CTeacherInfoTree(const char *pszIDTreeFile = "TeacherIDTree.txt",
                  const char *pszNameTreeFile = "TeacherNameTree.txt")
                  :CBaseInfoTree(pszIDTreeFile, pszNameTreeFile)
    {

    }
};

class CSelectInfoTree: public CBaseInfoTree
{
public:
    CSelectInfoTree(const char *pszIDTreeFile = "SelectIDTree.txt",
                  const char *pszNameTreeFile = "SelectNameTree.txt")
                  :CBaseInfoTree(pszIDTreeFile, pszNameTreeFile)
    {

    }
};

#endif
#endif