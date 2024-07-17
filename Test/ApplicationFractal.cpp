#include <fstream>
#include <cassert>

#include "ApplicationFractal.h"
#include "Utils.h"
#include "FrameDebuggerIntegration.h"

void ApplicationFractal::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();
}

void ApplicationFractal::Logic() noexcept {
    RDOCIntegration::StartCapture();

    using namespace RHINO;
    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 100, "MeinDescriptorHeap");
    Semaphore* semaphore = m_RHI->CreateSyncSemaphore(0);

    Texture2D* color0 = m_RHI->CreateTexture2D(Dim3D{256, 256, 1}, 1, TextureFormat::R32_FLOAT, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, "color0");
    heap->WriteUAV(WriteTexture2DDescriptorDesc{color0, 0});

    DescriptorRangeDesc space0RangeUAV {DescriptorRangeType::UAV, 0, 1};
    DescriptorSpaceDesc space0Desc{};
    space0Desc.space = 0;
    space0Desc.offsetInDescriptorsFromTableStart = 0;
    space0Desc.rangeDescCount = 1;
    space0Desc.rangeDescs = &space0RangeUAV;
    RootSignature* rootSignature = m_RHI->SerializeRootSignature({1, &space0Desc, "FractalRootSignature"});


    std::ifstream fractalShaderFile{"fractal.scar", std::ios::binary | std::ios::ate};
    assert(fractalShaderFile.is_open());
    auto fractalShaderBytecode = ReadBinary(fractalShaderFile);
    fractalShaderFile.close();
    ComputePSO* fractalPSO = m_RHI->CompileSCARComputePSO(fractalShaderBytecode.data(), fractalShaderBytecode.size(), rootSignature, "FractalComputePSO");

    CommandList* cmd = m_RHI->AllocateCommandList("CommandList");
    cmd->SetRootSignature(rootSignature);
    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(fractalPSO);
    cmd->Dispatch({4, 4, 1});
    m_RHI->SubmitCommandList(cmd);
    m_RHI->SignalFromQueue(semaphore, 1);

    m_RHI->SemaphoreWaitFromHost(semaphore, 1, ~0);

    RDOCIntegration::EndCapture();
}

void ApplicationFractal::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}
