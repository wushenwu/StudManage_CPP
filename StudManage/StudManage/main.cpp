#include "CStudManage.h"
#include "CManage.h"

int
main(void)
{
    int nresult = SUCCESS;

    CManage *pManageSystem = CManage::CreateSystem();
    nresult = pManageSystem->Run();
    pManageSystem->End();

#if 0
    CStudManage *psystem = CStudManage::CreateSystem();
    nresult = psystem->Run();
    psystem->End();
#endif
    return nresult;
}

