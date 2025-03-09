#pragma once

#ifdef WIN32
#include <Windows.h>
#endif // WIN32


#include <RHINO.h>

class ApplicationRT {
public:
    void Init(RHINO::BackendAPI api) noexcept;
    void InitSwapchain(void* surfaceDesc) noexcept;
    void Logic() noexcept;
    void ReleaseSwapchain() noexcept;
    void Release() noexcept;
protected:
    RHINO::RHINOInterface* m_RHI = nullptr;
    RHINO::Swapchain* m_Swapchain = nullptr;

    RHINO::RTPSO* m_PSO = nullptr;
    RHINO::RootSignature* m_RootSignature = nullptr;
    RHINO::DescriptorHeap* m_Heap = nullptr;
    RHINO::Texture2D* m_Backbuffer = nullptr;
};

