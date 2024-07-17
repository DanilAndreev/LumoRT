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
    mapped->leanX = 1.0f;
    mapped->leanY = 1.0f;
    mapped->moveX = -static_cast<float>(TEXTURE_SIZE_X) / 2;
    mapped->moveY = 10.0f;
    m_RHI->UnmapMemory(constatnsStaging);


    Texture2D* color0 = m_RHI->CreateTexture2D(Dim3D{TEXTURE_SIZE_X, TEXTURE_SIZE_Y, 1}, 1, TextureFormat::R32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "color0");
    Buffer* constants = m_RHI->CreateBuffer(sizeof(FractalSettings), ResourceHeapType::Default, ResourceUsage::ConstantBuffer | ResourceUsage::CopyDest, 0, "Constants");

    heap->WriteUAV(WriteTexture2DDescriptorDesc{color0, 0});
    heap->WriteCBV(WriteBufferDescriptorDesc{constants, 0, sizeof(FractalSettings), 0, 1});

    DescriptorRangeDesc space0RangeUAV {DescriptorRangeType::UAV, 0, 1};
    DescriptorRangeDesc space0RangeCBV {DescriptorRangeType::CBV, 1, 1};

    DescriptorRangeDesc space0ranges[] = {space0RangeUAV, space0RangeCBV};
    DescriptorSpaceDesc space0Desc{};
    space0Desc.space = 0;
    space0Desc.offsetInDescriptorsFromTableStart = 0;
    space0Desc.rangeDescCount = std::size(space0ranges);
    space0Desc.rangeDescs = space0ranges;
    RootSignature* rootSignature = m_RHI->SerializeRootSignature({1, &space0Desc, "FractalRootSignature"});


    std::ifstream fractalShaderFile{"fractal.scar", std::ios::binary | std::ios::ate};
    assert(fractalShaderFile.is_open());
    auto fractalShaderBytecode = ReadBinary(fractalShaderFile);
    fractalShaderFile.close();
    ComputePSO* fractalPSO = m_RHI->CompileSCARComputePSO(fractalShaderBytecode.data(), fractalShaderBytecode.size(), rootSignature, "FractalComputePSO");

    CommandList* cmd = m_RHI->AllocateCommandList("CommandList");
    cmd->CopyBuffer(constatnsStaging, constants, 0, 0, sizeof(FractalSettings));

    cmd->SetRootSignature(rootSignature);
    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(fractalPSO);
    cmd->Dispatch({TEXTURE_SIZE_X / EXAMPLE_FRACTAL_THREADGROUP_X, TEXTURE_SIZE_Y / EXAMPLE_FRACTAL_THREADGROUP_Y, 1});
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
