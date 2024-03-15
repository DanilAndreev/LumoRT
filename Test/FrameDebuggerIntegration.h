#pragma once

#ifdef WIN32

struct RENDERDOC_API_1_4_1;
typedef RENDERDOC_API_1_4_1 RENDERDOC_API_1_4_2;

class RDOCIntegration
{
public:
    static void StartCapture();
    static void EndCapture();

private:
    static void LoadRDOCAPI();
    static void LoadNGFX();
    static RENDERDOC_API_1_4_2* ms_RDCAPI;
    static bool ms_NGFXLoaded;
};

#endif
