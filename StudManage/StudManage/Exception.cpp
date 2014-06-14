#include "Exception.h"
#include "ui.h"

const char *CMyError::m_pszfile = "log.txt";

///////////////////////CMyFileErrorœ‡πÿ///////////////////////////////////////////////////

CMyFileError::CMyFileError(const char *pszerrinfo, const char *pszfile)
                            :fout(m_pszfile, ios::out | ios::app)
{
    strcpy(m_szerrinfo, pszerrinfo);
    strcat(m_szerrinfo, pszfile);
    
    if (0 == fout.is_open())
    {
        //do sth 
    }
}
 
CMyFileError::~CMyFileError() 
{
    fout.close();
}

const char * 
CMyFileError::GetLog(void) const
{
    return m_szerrinfo;
}

void 
CMyFileError::WriteLog(const char *pszfile, int nline)
{
    fout << pszfile << " " << nline << " ";
    fout << m_szerrinfo << endl;
}

