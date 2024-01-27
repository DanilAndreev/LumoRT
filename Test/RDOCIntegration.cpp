#include "RDOCIntegration.h"
#include "ThirdParty/renderdoc_app.h"
#include <Windows.h>
#include <cassert>
#include <comdef.h>
#include <iostream>
#include <pix3.h>

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

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)

    {
        PIXCaptureParameters captureParams{};
        const wchar_t* filename = WIDE1("./debug");
        static int captureCount = 1;
        wchar_t calculatedFilename[MAX_PATH]{};

        swprintf(calculatedFilename, sizeof(calculatedFilename) / sizeof(calculatedFilename[0]), L"%ls%d%ls", filename,
                 captureCount, L".wpix");
        ++captureCount;

        captureParams.GpuCaptureParameters.FileName = calculatedFilename;
        HRESULT hr = PIXBeginCapture(PIX_CAPTURE_GPU, &captureParams);
        if (hr != S_OK) {
            _com_error err(hr);
            std::cerr << "Pix init falied: " << err.ErrorMessage() << std::endl;
        }
    }
}

void RDOCIntegration::EndCapture() {
    if (m_RDCAPI) {
        m_RDCAPI->EndFrameCapture(NULL, NULL);
    }
    PIXEndCapture(FALSE);
}
