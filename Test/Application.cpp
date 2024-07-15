#include <cassert>
#include <fstream>
#include <iostream>

#include <Math3D.h>
#include "Application.h"

#include "FrameDebuggerIntegration.h"

#include <RaytracingHlslCompat.hlsli>
#include "Utils.h"

void Application::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();
}

void Application::Logic() noexcept {
    using namespace Math3D;
    using namespace RHINO;
    RDOCIntegration::StartCapture();

    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 100, "Heap");
    CommandList* cmd = m_RHI->AllocateCommandList("CMD");

    Buffer* bufCBV = m_RHI->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::ConstantBuffer, 0, "ConstantB");
    Buffer* destUAV1 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV1");
    Buffer* destUAV2 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV2");
    Buffer* destUAV3 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV3");
    Buffer* rbkUAV1 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV1");
    Buffer* rbkUAV2 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV2");
    Buffer* rbkUAV3 = m_RHI->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV3");

    WriteBufferDescriptorDesc desc{};
    desc.offsetInHeap = 0;
    desc.bufferOffset = 0;

    desc.buffer = destUAV1;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 0;
    heap->WriteUAV(desc);
    desc.buffer = destUAV2;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 1;
    heap->WriteUAV(desc);
    desc.buffer = destUAV3;
    desc.size = sizeof(int) * 64;
    desc.bufferStructuredStride = sizeof(int);
    desc.offsetInHeap = 5;
    heap->WriteUAV(desc);

    std::ifstream shaderFile{"out.scar", std::ios::binary | std::ios::ate};
    assert(shaderFile.is_open());
    auto bytecode = ReadBinary(shaderFile);
    shaderFile.close();

    // const DescriptorRangeDesc space0rd[] = {
    //     DescriptorRangeDesc{DescriptorRangeType::UAV, 0, 2},
    // };
    //
    // const DescriptorRangeDesc space1rd[] = {
    //     DescriptorRangeDesc{DescriptorRangeType::UAV, 1, 1},
    // };
    //
    // const DescriptorSpaceDesc spaces[] = {
    //     DescriptorSpaceDesc{0, 0, RHINO_ARR_SIZE(space0rd), space0rd},
    //     DescriptorSpaceDesc{1, 4, RHINO_ARR_SIZE(space1rd), space1rd},
    // };
    //
    //
    // ComputePSODesc psoDesc{};
    // psoDesc.CS.entrypoint = "main";
    // psoDesc.CS.bytecodeSize = bytecode.size();
    // psoDesc.CS.bytecode = bytecode.data();
    // psoDesc.debugName = "TestCPSO";
    // psoDesc.spacesCount = RHINO_ARR_SIZE(spaces);
    // psoDesc.spacesDescs = spaces;
    // ComputePSO* pso = m_RHI->CompileComputePSO(psoDesc);

    ComputePSO* pso = m_RHI->CompileSCARComputePSO(bytecode.data(), bytecode.size(), "TestCPSO");

    // RDOCIntegration::StartCapture();

    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(pso);
    cmd->Dispatch({1, 1, 1});

    m_RHI->SubmitCommandList(cmd);
    cmd->Release();

    CommandList* cmd2 = m_RHI->AllocateCommandList("CMDList");
    cmd2->CopyBuffer(destUAV1, rbkUAV1, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV2, rbkUAV2, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV3, rbkUAV3, 0, 0, sizeof(int) * 64);
    m_RHI->SubmitCommandList(cmd2);
    cmd2->Release();


    auto* data1 = static_cast<int*>(m_RHI->MapMemory(rbkUAV1, 0, sizeof(int) * 64));
    auto* data2 = static_cast<int*>(m_RHI->MapMemory(rbkUAV2, 0, sizeof(int) * 64));
    auto* data3 = static_cast<int*>(m_RHI->MapMemory(rbkUAV3, 0, sizeof(int) * 64));

    std::vector<int> vdata1{};
    vdata1.resize(64);
    std::vector<int> vdata2{};
    vdata2.resize(64);
    std::vector<int> vdata3{};
    vdata3.resize(64);

    memcpy(vdata1.data(), data1, sizeof(int) * 64);
    memcpy(vdata2.data(), data2, sizeof(int) * 64);
    memcpy(vdata3.data(), data3, sizeof(int) * 64);

    pso->Release();
    heap->Release();
    bufCBV->Release();
    rbkUAV1->Release();
    rbkUAV2->Release();
    rbkUAV3->Release();
    destUAV1->Release();
    destUAV2->Release();
    destUAV3->Release();

    RDOCIntegration::EndCapture();
}
void Application::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}