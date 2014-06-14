#ifndef EXCEPTION_H_95176836_844F_491d_817B_52CEFC41875F

#define EXCEPTION_H_95176836_844F_491d_817B_52CEFC41875F

#include <iostream>
#include <fstream>
using namespace std;

#define USE_EXCEPTION

class CMyError
{
public:
    virtual const char *GetLog(void) const = 0;
    virtual void WriteLog(const char *pszfile, int nline) = 0;
    
protected:
    static const char *m_pszfile;
};

class CMyFileError: public CMyError
{
public:
    CMyFileError(const char *pszerrinfo, const char *pszfile);
    
    virtual ~CMyFileError();

    virtual const char *GetLog(void) const;
    
    virtual void WriteLog(const char *pszfile, int nline);

private:
    char m_szerrinfo[256];
    fstream fout;  
};

#endif