#include <fstream>
#include <cassert>

#include "ApplicationFractal.h"
#include "FrameDebuggerIntegration.h"
#include "RHINOTypes.h"
#include "Utils.h"

#include <fractal/fractalTypes.hlsli>

constexpr uint32_t TEXTURE_SIZE_X = 1024;
constexpr uint32_t TEXTURE_SIZE_Y = 1024;
static_assert(TEXTURE_SIZE_X % EXAMPLE_FRACTAL_THREADGROUP_X == 0);
static_assert(TEXTURE_SIZE_Y % EXAMPLE_FRACTAL_THREADGROUP_Y == 0);

constexpr uint32_t BACKBUFFER_SIZE_X = 512;
constexpr uint32_t BACKBUFFER_SIZE_Y = 512;

void ApplicationFractal::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();

    m_CBVSRVUAVHeap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 100, "CBVSRVUAVHeap");
    m_SMPHeap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SMP, 10, "SMPHeap");

    m_Color = m_RHI->CreateTexture2D(Dim3D{TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 1}, 1, TextureFormat::R32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "color0");
    m_BackbufferSrc = m_RHI->CreateTexture2D(Dim3D{BACKBUFFER_SIZE_X, BACKBUFFER_SIZE_Y, 1}, 1, TextureFormat::R8G8B8A8_UNORM, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "BackbufferSrc");
    m_Constants = m_RHI->CreateBuffer(sizeof(FractalSettings), ResourceHeapType::Default, ResourceUsage::ConstantBuffer | ResourceUsage::CopyDest, 0, "Constants");

    SamplerDesc samplerDesc{};
    samplerDesc.borderColor = BorderColor::OpaqueWhite;
    samplerDesc.comparisonFunc = ComparisonFunction::Always;
    m_Sampler = m_RHI->CreateSampler(samplerDesc);

    // Write descriptors to heaps

    m_CBVSRVUAVHeap->WriteCBV(WriteBufferDescriptorDesc{m_Constants, 0, sizeof(FractalSettings), 0, 0});
    m_CBVSRVUAVHeap->WriteSRV(WriteTexture2DDescriptorDesc{m_Color, 1});
    m_CBVSRVUAVHeap->WriteUAV(WriteTexture2DDescriptorDesc{m_Color, 2});
    m_CBVSRVUAVHeap->WriteUAV(WriteTexture2DDescriptorDesc{m_BackbufferSrc, 3});
    m_SMPHeap->WriteSMP(m_Sampler, 0);

    // Root signature and PSOs
    DescriptorRangeDesc space0UAVCBVSRVranges[] = {
            {DescriptorRangeType::UAV, 2, 2},
            {DescriptorRangeType::CBV, 0, 1},
            {DescriptorRangeType::SRV, 1, 1},
    };
    DescriptorRangeDesc space1SMPranges[] = {
            {DescriptorRangeType::SMP, 0, 1},
    };
    DescriptorSpaceDesc spaceDescs[] = {
            {DescriptorHeapType::SRV_CBV_UAV, 0, 0, std::size(space0UAVCBVSRVranges), space0UAVCBVSRVranges},
            {DescriptorHeapType::SMP, 1, 0, std::size(space1SMPranges), space1SMPranges},
    };

    m_RootSignature = m_RHI->SerializeRootSignature({std::size(spaceDescs), spaceDescs, "FractalRootSignature"});

    std::ifstream fractalShaderFile{"fractal.scar", std::ios::binary | std::ios::ate};
    assert(fractalShaderFile.is_open());
    auto fractalShaderBytecode = ReadBinary(fractalShaderFile);
    fractalShaderFile.close();
    m_FractalPSO = m_RHI->CompileSCARComputePSO(fractalShaderBytecode.data(), fractalShaderBytecode.size(), m_RootSignature, "FractalComputePSO");

    std::ifstream blitShaderFile{"blit.scar", std::ios::binary | std::ios::ate};
    assert(blitShaderFile.is_open());
    auto blitShaderBytecode = ReadBinary(blitShaderFile);
    blitShaderFile.close();
    m_BlitPSO = m_RHI->CompileSCARComputePSO(blitShaderBytecode.data(), blitShaderBytecode.size(), m_RootSignature, "BlitComputePSO");
}

void ApplicationFractal::InitSwapchain(void* surfaceDesc) noexcept {
    using namespace RHINO;

    SwapchainDesc swapchainDesc{};
    swapchainDesc.format = TextureFormat::R8G8B8A8_UNORM;
    swapchainDesc.width = BACKBUFFER_SIZE_X;
    swapchainDesc.height = BACKBUFFER_SIZE_X;
    swapchainDesc.buffersCount = SWAPCHAIN_BUFFERS_COUNT;
    swapchainDesc.surfaceDesc = surfaceDesc;
    swapchainDesc.windowed = true;
    m_Swapchain = m_RHI->CreateSwapchain(swapchainDesc);
}

void ApplicationFractal::Logic() noexcept {
#ifdef WIN32
    RDOCIntegration::StartCapture();
#endif
    using namespace RHINO;
    ++m_Frame;

    Semaphore* semaphore = m_RHI->CreateSyncSemaphore(0);

    CommandList* cmd = m_RHI->AllocateCommandList("FrameCommandList");
    // ResourceBarrierDesc barrierDesc{};
    // barrierDesc.resource = m_Color;
    // barrierDesc.type = ResourceBarrierType::Transition;
    // cmd->ResourceBarrier(barrierDesc);

    Buffer* constatnsStaging = m_RHI->CreateBuffer(sizeof(FractalSettings), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "ConstantsStaging");
    auto* mapped = static_cast<FractalSettings*>(m_RHI->MapMemory(constatnsStaging, 0, sizeof(FractalSettings)));
    assert(mapped);
    mapped->backbufferWidth = TEXTURE_SIZE_X;
    mapped->backbufferHeight = TEXTURE_SIZE_Y;
    mapped->time = static_cast<float>(m_Frame) / 100.0f;
    m_RHI->UnmapMemory(constatnsStaging);

    cmd->CopyBuffer(constatnsStaging, m_Constants, 0, 0, sizeof(FractalSettings));

    cmd->SetRootSignature(m_RootSignature);
    cmd->SetHeap(m_CBVSRVUAVHeap, 0, m_SMPHeap, 0);
    cmd->SetComputePSO(m_FractalPSO);
    cmd->Dispatch({TEXTURE_SIZE_X / EXAMPLE_FRACTAL_THREADGROUP_X, TEXTURE_SIZE_Y / EXAMPLE_FRACTAL_THREADGROUP_Y, 1});

    ResourceBarrierDesc barrier{};
    barrier.resource = m_Color;
    barrier.type = ResourceBarrierType::UAV;
    cmd->ResourceBarrier(barrier);

    cmd->SetComputePSO(m_BlitPSO);
    cmd->Dispatch({BACKBUFFER_SIZE_X / EXAMPLE_FRACTAL_BLIT_THREADGROUP_X, BACKBUFFER_SIZE_Y / EXAMPLE_FRACTAL_BLIT_THREADGROUP_Y, 1});
    m_RHI->SubmitCommandList(cmd);
    m_RHI->SignalFromQueue(semaphore, 1);

    m_RHI->SemaphoreWaitFromHost(semaphore, 1, ~0);
    constatnsStaging->Release();
    m_RHI->SwapchainPresent(m_Swapchain, m_BackbufferSrc, BACKBUFFER_SIZE_X, BACKBUFFER_SIZE_Y);

#ifdef WIN32
    RDOCIntegration::EndCapture();
#endif
}

void ApplicationFractal::ReleaseSwapchain() noexcept {
    m_Swapchain->Release();
}

void ApplicationFractal::Release() noexcept {
    m_Constants->Release();
    m_Color->Release();
    m_Sampler->Release();

    m_CBVSRVUAVHeap->Release();
    m_SMPHeap->Release();

    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}
