#ifndef CMYSTRING_H_A5D8D5F8_B1DC_46d7_9E79_C4E8EAF111A7

#define CMYSTRING_H_A5D8D5F8_B1DC_46d7_9E79_C4E8EAF111A7

#define REFCOUNT

class CMyString
{
public:
    CMyString();
    CMyString(const char *psz);
    CMyString(const CMyString &str);

    virtual ~CMyString();

public:
    //构造字符串
    void SetString(const char *psz);
    void SetString(const CMyString &str);

    inline const char *GetString(void) const;   //
    
    inline size_t GetLen(void)  const;

    const char *StringCopy(const char *psz);
    const char *StringCopy(const CMyString &str);

    const char *StringCat(const char *psz);
    const char *StringCat(const CMyString &str);

    /*
    Function: 字符串比较
    Return : true 该字符串与psz相等  false 不相等
    */
    bool StringCompare(const char *psz) const;
    bool StringCompare(const CMyString &str) const;

    /*
    Function: 查找一个字符
    Return : 如果存在，则返回指向该位置的指针，否则返回NULL。
             其中StringChr返回的是第一次出现的位置，而StringRchr返回的是最后一次出现的位置
    */
    const char *StringChr(int ch) const;
    const char *StringRchr(int ch) const;

    /*
    Function: 查找任何几个字符, 查找任何一组字符第1次在字符串中出现的位置
    Return: 返回一个指向第一个匹配group中任何一个字符的位置， 如果未找到，则返回NULL
    */
    const char *StringPbrk(const char *pszgroup) const;
    const char *StringPbrk(const CMyString &str) const;


    /*
    Function: 查找子串
    Return: 在该字符串中查找psz第一次出现的起始位置，并返回一个指向该位置的指针。
            如果没有找到，则返回NULL
    */
    const char *StringStr(const char *psz) const;
    const char *StringStr(const CMyString &str) const;

    //对字符串进行大小写转换
    void StringToUpper(void);
    void StringToLower(void);


private:
    void StringInit(void);

#ifdef REFCOUNT
    //引用计数相关
    void AddRef(void);
    void Release(void);
#endif

private:
    char *m_psz;        //字符串存储地址
    size_t m_nlen;      //字符串实际占用长度
    size_t m_nspace;    //所申请空间的总长度

#ifdef REFCOUNT
    int *m_prefcount;   //引用计数
#endif

};

inline const char *
CMyString::GetString(void) const
{
    return m_psz;
}

inline size_t 
CMyString::GetLen(void) const
{
    return m_nlen;
}


#endif