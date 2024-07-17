#include <cassert>
#include <fstream>
#include <iostream>

#include <Math3D.h>
#include "Application.h"

#include "FrameDebuggerIntegration.h"

#include "Utils.h"

template<class T>
void PrintVec(std::vector<T> vec, const std::string& separator = " ") {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size()) {
            std::cout << separator;
        }
    }
    std::cout << std::endl;
}

void Application::Init(RHINO::BackendAPI api) noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(api);
    m_RHI->Initialize();
}

void Application::Logic() noexcept {
    using namespace Math3D;
    using namespace RHINO;
    // RDOCIntegration::StartCapture();

    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 100, "Heap");
    CommandList* cmd = m_RHI->AllocateCommandList("CMD");
    Semaphore* semaphore = m_RHI->CreateSyncSemaphore(0);

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

    std::ifstream shaderFile{"compute.scar", std::ios::binary | std::ios::ate};
    assert(shaderFile.is_open());
    auto bytecode = ReadBinary(shaderFile);
    shaderFile.close();

     const DescriptorRangeDesc space0rd[] = {
         DescriptorRangeDesc{DescriptorRangeType::UAV, 0, 2},
     };

     const DescriptorRangeDesc space1rd[] = {
         DescriptorRangeDesc{DescriptorRangeType::UAV, 1, 1},
     };

     const DescriptorSpaceDesc spaces[] = {
         DescriptorSpaceDesc{0, 0, std::size(space0rd), space0rd},
         DescriptorSpaceDesc{1, 4, std::size(space1rd), space1rd},
     };

     RootSignatureDesc rootSignatureDesc{};
     rootSignatureDesc.spacesCount = std::size(spaces);
     rootSignatureDesc.spacesDescs = spaces;
     rootSignatureDesc.debugName = "RootSignature";

     RootSignature* rootSignature = m_RHI->SerializeRootSignature(rootSignatureDesc);


//     ComputePSODesc psoDesc{};
//     psoDesc.rootSignature = rootSignature;
//     psoDesc.CS.entrypoint = "main";
//     psoDesc.CS.bytecodeSize = bytecode.size();
//     psoDesc.CS.bytecode = bytecode.data();
//     psoDesc.debugName = "TestCPSO";
//     ComputePSO* pso = m_RHI->CompileComputePSO(psoDesc);

     ComputePSO* pso = m_RHI->CompileSCARComputePSO(bytecode.data(), bytecode.size(), rootSignature, "ComputePSO");

    cmd->SetRootSignature(rootSignature);
    cmd->SetHeap(heap, nullptr);
    cmd->SetComputePSO(pso);

    cmd->Dispatch({1, 1, 1});

    m_RHI->SubmitCommandList(cmd);
    m_RHI->SignalFromQueue(semaphore, 1);
    cmd->Release();

    CommandList* cmd2 = m_RHI->AllocateCommandList("CMDList");
    cmd2->CopyBuffer(destUAV1, rbkUAV1, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV2, rbkUAV2, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV3, rbkUAV3, 0, 0, sizeof(int) * 64);
    m_RHI->SemaphoreWaitFromQueue(semaphore, 1);
    m_RHI->SubmitCommandList(cmd2);
    m_RHI->SignalFromQueue(semaphore, 2);
    cmd2->Release();

    m_RHI->SemaphoreWaitFromHost(semaphore, 2, ~0);
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

    std::cout << "UAV1 result data: ";
    PrintVec(vdata1);
    std::cout << "UAV2 result data: ";
    PrintVec(vdata2);
    std::cout << "UAV3 result data: ";
    PrintVec(vdata3);

    pso->Release();
    heap->Release();
    bufCBV->Release();
    rbkUAV1->Release();
    rbkUAV2->Release();
    rbkUAV3->Release();
    destUAV1->Release();
    destUAV2->Release();
    destUAV3->Release();

    // RDOCIntegration::EndCapture();
}
void Application::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}
