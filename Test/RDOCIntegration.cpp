#include "RDOCIntegration.h"
#include "ThirdParty/renderdoc_app.h"
#include <Windows.h>

RENDERDOC_API_1_4_2* RDOCIntegration::m_RDCAPI = nullptr;

void RDOCIntegration::LoadAPI() {
    if (m_RDCAPI)
        return;
    if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI =
                (pRENDERDOC_GetAPI) GetProcAddress(mod, "RENDERDOC_GetAPI");
        RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_4_2, (void**) &m_RDCAPI);
    }
}

void RDOCIntegration::StartCapture() {
    LoadAPI();
    if (m_RDCAPI) {
        m_RDCAPI->StartFrameCapture(NULL, NULL);
    }
}

void RDOCIntegration::EndCapture() {
    if (m_RDCAPI) {
        m_RDCAPI->EndFrameCapture(NULL, NULL);
    }
}
