#ifdef WIN32

#include "FrameDebuggerIntegration.h"
#include "ThirdParty/renderdoc_app.h"
#include <Windows.h>
#include <cassert>
#include <vector>
#include <sstream>
#include <comdef.h>
#include <iostream>
#include <pix3.h>

#include <NGFX_Injection.h>

RENDERDOC_API_1_4_2* RDOCIntegration::ms_RDCAPI = nullptr;
bool RDOCIntegration::ms_NGFXLoaded = false;

static void ReportInjectionError(const wchar_t* pMessage)
{
    OutputDebugStringW(pMessage);
    std::wcerr << pMessage << std::endl;

    // const auto pTitle = L"Injection Failure.";
    // MessageBoxW(NULL, pMessage, pTitle, MB_OK | MB_ICONERROR);
}

void RDOCIntegration::LoadRDOCAPI() {
    if (ms_RDCAPI)
        return;
    if (HMODULE mod = GetModuleHandleA("renderdoc.dll")) {
        pRENDERDOC_GetAPI RENDERDOC_GetAPI =
                (pRENDERDOC_GetAPI) GetProcAddress(mod, "RENDERDOC_GetAPI");
        RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_4_2, (void**) &ms_RDCAPI);
    }
}

void RDOCIntegration::LoadNGFX() {
// Injecting into a process follows this basic flow:
    //
    // 1) Enumerating/detecting the installations installed on the machine
    // 2) Choosing a particular installation to use
    // 3) Determining the activities/capabilities of the particular installation
    // 4) Choosing a particular activity to use
    // 5) Injecting into the application, checking for success

    // 1) First, find Nsight Graphics installations
    uint32_t numInstallations = 0;
    auto result = NGFX_Injection_EnumerateInstallations(&numInstallations, nullptr);
    if (numInstallations == 0 || NGFX_INJECTION_RESULT_OK != result)
    {
        std::wstringstream stream;
        stream << L"Could not find any Nsight Graphics installations to inject: " << result << "\n";
        stream << L"Please install Nsight Graphics to enable programmatic injection.";
        ReportInjectionError(stream.str().c_str());
        return;
    }

    std::vector<NGFX_Injection_InstallationInfo> installations(numInstallations);
    result = NGFX_Injection_EnumerateInstallations(&numInstallations, installations.data());
    if (numInstallations == 0 || NGFX_INJECTION_RESULT_OK != result)
    {
        std::wstringstream stream;
        stream << L"Could not find any Nsight Graphics installations to inject: " << result << "\n";
        stream << L"Please install Nsight Graphics to enable programmatic injection.";
        ReportInjectionError(stream.str().c_str());
        return;
    }

    // 2) We have at least one Nsight Graphics installation, find which
    // activities are available using the latest installation.
    const NGFX_Injection_InstallationInfo& installation = installations.back();

    // 3) Retrieve the count of activities so we can initialize our activity data to the correct size
    uint32_t numActivities = 0;
    result = NGFX_Injection_EnumerateActivities(&installation, &numActivities, nullptr);
    if (numActivities == 0 || NGFX_INJECTION_RESULT_OK != result)
    {
        std::wstringstream stream;
        stream << L"Could not find any activities in Nsight Graphics installation: " << result << "\n";
        stream << L"Please install Nsight Graphics to enable programmatic injection.";
        ReportInjectionError(stream.str().c_str());
        return;
    }

    // With the count of activities available, query their description
    std::vector<NGFX_Injection_Activity> activities(numActivities);
    result = NGFX_Injection_EnumerateActivities(&installation, &numActivities, activities.data());
    if (NGFX_INJECTION_RESULT_OK != result)
    {
        std::wstringstream stream;
        stream << L"NGFX_Injection_EnumerateActivities failed with" << result;
        ReportInjectionError(stream.str().c_str());
        return;
    }

    // 4) We have valid activities. From here, we choose an activity.
    // In this sample, we use "Frame Debugger" activity
    const NGFX_Injection_Activity* pActivityToInject = nullptr;
    for (const NGFX_Injection_Activity& activity : activities)
    {
        if (activity.type == NGFX_INJECTION_ACTIVITY_FRAME_DEBUGGER)
        {
            pActivityToInject = &activity;
            break;
        }
    }

    if (!pActivityToInject) {
        std::wstringstream stream;
        stream << L"Frame Debugger activity is not available" << result;
        ReportInjectionError(stream.str().c_str());
        return;
    }

    // 5) With the activity identified, Inject into the process, setup for the
    // Frame Debugger activity
    result = NGFX_Injection_InjectToProcess(&installation, pActivityToInject);
    if (NGFX_INJECTION_RESULT_OK != result)
    {
        std::wstringstream stream;
        stream << L"NGFX_Injection_InjectToProcess failed with" << result;
        ReportInjectionError(stream.str().c_str());
        return;
    }
    ms_NGFXLoaded = true;
}

void RDOCIntegration::StartCapture() {
    LoadRDOCAPI();
    if (ms_RDCAPI) {
        ms_RDCAPI->StartFrameCapture(NULL, NULL);
    }

#ifdef USE_PIX
    {
        PIXCaptureParameters captureParams{};
        const wchar_t* filename = L"./debug";
        static int captureCount = 1;
        wchar_t calculatedFilename[MAX_PATH]{};

        swprintf(calculatedFilename, sizeof(calculatedFilename) / sizeof(calculatedFilename[0]), L"%ls%d%ls", filename,
                 captureCount, L".wpix");
        ++captureCount;

        captureParams.GpuCaptureParameters.FileName = calculatedFilename;
        PIXBeginCapture(PIX_CAPTURE_GPU, &captureParams);
    }
#endif

    LoadNGFX();
    if (ms_NGFXLoaded) {
        NGFX_Injection_ExecuteActivityCommand();
    }
}

void RDOCIntegration::EndCapture() {
    if (ms_RDCAPI) {
        ms_RDCAPI->EndFrameCapture(NULL, NULL);
    }
#ifdef USE_PIX
    PIXEndCapture(FALSE);
#endif
}

#endif