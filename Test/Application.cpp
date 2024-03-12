#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "Application.h"

#include "FrameDebuggerIntegration.h"
#include "Utils/Common.h"

static std::vector<uint8_t> ReadBinary(std::istream& stream) noexcept {
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (stream.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        /* worked! */
    }
    return buffer;
}

void Application::Init() noexcept {
    using namespace RHINO;
    m_RHI = CreateRHINO(BackendAPI::Vulkan);
    m_RHI->Initialize();
}
void Application::Logic() noexcept {
    using namespace RHINO;
    RDOCIntegration::StartCapture();

    DescriptorHeap* heap = m_RHI->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 10, "Heap");

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
    // desc.buffer = bufCBV;
    // heap->WriteCBV(desc);


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
    // assert(bytecode.size() % 4 == 0);

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

    CommandList* cmd = m_RHI->AllocateCommandList("CMDList");
    cmd->SetComputePSO(pso);
    cmd->SetHeap(heap, nullptr);
    cmd->Dispatch({1, 1, 1});

    m_RHI->SubmitCommandList(cmd);
    m_RHI->ReleaseCommandList(cmd);

    CommandList* cmd2 = m_RHI->AllocateCommandList("CMDList");
    cmd2->CopyBuffer(destUAV1, rbkUAV1, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV2, rbkUAV2, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV3, rbkUAV3, 0, 0, sizeof(int) * 64);
    m_RHI->SubmitCommandList(cmd2);
    m_RHI->ReleaseCommandList(cmd2);


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

    m_RHI->ReleaseComputePSO(pso);
    m_RHI->ReleaseDescriptorHeap(heap);
    m_RHI->ReleaseBuffer(bufCBV);
    m_RHI->ReleaseBuffer(rbkUAV1);
    m_RHI->ReleaseBuffer(rbkUAV2);
    m_RHI->ReleaseBuffer(rbkUAV3);
    m_RHI->ReleaseBuffer(destUAV1);
    m_RHI->ReleaseBuffer(destUAV2);
    m_RHI->ReleaseBuffer(destUAV3);

    RDOCIntegration::EndCapture();
}
void Application::Release() noexcept {
    using namespace RHINO;
    m_RHI->Release();
    delete m_RHI;
}