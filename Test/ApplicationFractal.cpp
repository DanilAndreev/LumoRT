#include <fstream>
#include <cassert>

#include "ApplicationFractal.h"
#include "Utils.h"
#include "FrameDebuggerIntegration.h"
#include <fractal/fractalTypes.hlsli>

constexpr uint32_t TEXTURE_SIZE_X = 256;
constexpr uint32_t TEXTURE_SIZE_Y = 256;
static_assert(TEXTURE_SIZE_X % EXAMPLE_FRACTAL_THREADGROUP_X == 0);
static_assert(TEXTURE_SIZE_Y % EXAMPLE_FRACTAL_THREADGROUP_Y == 0);

void ApplicationFractal::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();
}

void ApplicationFractal::Logic() noexcept {
    // RDOCIntegration::StartCapture();

    using namespace RHINO;
    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 100, "MeinDescriptorHeap");
    Semaphore* semaphore = m_RHI->CreateSyncSemaphore(0);

    Buffer* constatnsStaging = m_RHI->CreateBuffer(sizeof(FractalSettings), ResourceHeapType::Upload, ResourceUsage::CopySource, 0, "ConstantsStaging");
    auto* mapped = static_cast<FractalSettings*>(m_RHI->MapMemory(constatnsStaging, 0, sizeof(FractalSettings)));
    assert(mapped);
    mapped->leanX = 0.05f;
    mapped->leanY = 1.0f;
    mapped->moveX = -static_cast<float>(TEXTURE_SIZE_X) / 2;
    mapped->moveY = 10.0f;
    m_RHI->UnmapMemory(constatnsStaging);


    Texture2D* color0 = m_RHI->CreateTexture2D(Dim3D{TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 1}, 1, TextureFormat::R32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "color0");
    Texture2D* color1 = m_RHI->CreateTexture2D(Dim3D{64, 64, 1}, 1, TextureFormat::R32_FLOAT, ResourceUsage::UnorderedAccess, "color1");
    Buffer* constants = m_RHI->CreateBuffer(sizeof(FractalSettings), ResourceHeapType::Default, ResourceUsage::ConstantBuffer | ResourceUsage::CopyDest, 0, "Constants");
    SamplerDesc samplerDesc{};
    Sampler* sampler = m_RHI->CreateSampler(samplerDesc);

    heap->WriteCBV(WriteBufferDescriptorDesc{constants, 0, sizeof(FractalSettings), 0, 0});
    heap->WriteSRV(WriteTexture2DDescriptorDesc{color0, 1});
    heap->WriteUAV(WriteTexture2DDescriptorDesc{color0, 2});
    heap->WriteUAV(WriteTexture2DDescriptorDesc{color1, 3});
    heap->WriteSMP(sampler, 10);

    DescriptorRangeDesc space0ranges[] = {
            {DescriptorRangeType::UAV, 2, 2},
            {DescriptorRangeType::CBV, 0, 1},
            {DescriptorRangeType::SRV, 1, 1},
    };
    DescriptorRangeDesc space1ranges[] = {
            {DescriptorRangeType::Sampler, 0, 1},
    };
    DescriptorSpaceDesc spaceDescs[] = {
            {0, 0, std::size(space0ranges), space0ranges},
            {1, 10, std::size(space1ranges), space1ranges},
    };

    RootSignature* rootSignature = m_RHI->SerializeRootSignature({std::size(spaceDescs), spaceDescs, "FractalRootSignature"});

    std::ifstream fractalShaderFile{"fractal.scar", std::ios::binary | std::ios::ate};
    assert(fractalShaderFile.is_open());
    auto fractalShaderBytecode = ReadBinary(fractalShaderFile);
    fractalShaderFile.close();
    ComputePSO* fractalPSO = m_RHI->CompileSCARComputePSO(fractalShaderBytecode.data(), fractalShaderBytecode.size(), rootSignature, "FractalComputePSO");

    std::ifstream blitShaderFile{"blit.scar", std::ios::binary | std::ios::ate};
    assert(blitShaderFile.is_open());
    auto blitShaderBytecode = ReadBinary(blitShaderFile);
    blitShaderFile.close();
    ComputePSO* blitPSO = m_RHI->CompileSCARComputePSO(blitShaderBytecode.data(), blitShaderBytecode.size(), rootSignature, "BlitComputePSO");

    CommandList* cmd = m_RHI->AllocateCommandList("CommandList");
    cmd->CopyBuffer(constatnsStaging, constants, 0, 0, sizeof(FractalSettings));

    cmd->SetRootSignature(rootSignature);
    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(fractalPSO);
    cmd->Dispatch({TEXTURE_SIZE_X / EXAMPLE_FRACTAL_THREADGROUP_X, TEXTURE_SIZE_Y / EXAMPLE_FRACTAL_THREADGROUP_Y, 1});

    ResourceBarrierDesc barrier{};
    barrier.resource = color0;
    barrier.type = ResourceBarrierType::UAV;
    cmd->ResourceBarrier(barrier);

    cmd->SetComputePSO(blitPSO);
    cmd->Dispatch({2, 2, 1});
    m_RHI->SubmitCommandList(cmd);
    m_RHI->SignalFromQueue(semaphore, 1);

    m_RHI->SemaphoreWaitFromHost(semaphore, 1, ~0);

    // RDOCIntegration::EndCapture();
}

void ApplicationFractal::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}
