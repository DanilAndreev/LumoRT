#pragma once

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include <RHINO.h>

class ApplicationFractal {
public:
    void Init(RHINO::BackendAPI api) noexcept;
    void InitSwapchain(void* surfaceDesc) noexcept;
    void Logic() noexcept;
    void ReleaseSwapchain() noexcept;
    void Release() noexcept;
protected:
    static constexpr uint32_t SWAPCHAIN_BUFFERS_COUNT = 3;


    RHINO::RHINOInterface* m_RHI = nullptr;

    RHINO::DescriptorHeap* m_CBVSRVUAVHeap = nullptr;
    RHINO::DescriptorHeap* m_SMPHeap = nullptr;

    RHINO::RootSignature* m_RootSignature = nullptr;
    RHINO::ComputePSO* m_FractalPSO = nullptr;
    RHINO::ComputePSO* m_BlitPSO = nullptr;

    RHINO::Swapchain* m_Swapchain = nullptr;
protected:
    size_t m_Frame = 0;
    RHINO::Buffer* m_Constants = nullptr;
    RHINO::Texture2D* m_Color = nullptr;
    RHINO::Texture2D* m_BackbufferSrc = nullptr;

    RHINO::Sampler* m_Sampler = nullptr;
};
