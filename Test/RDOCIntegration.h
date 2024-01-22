#pragma once

struct RENDERDOC_API_1_4_1;
typedef RENDERDOC_API_1_4_1 RENDERDOC_API_1_4_2;

class RDOCIntegration
{
public:
    static void StartCapture();
    static void EndCapture();

private:
    static void LoadAPI();
    static RENDERDOC_API_1_4_2* m_RDCAPI;
};
