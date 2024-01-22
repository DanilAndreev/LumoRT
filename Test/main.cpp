#include "RHINO.h"
#include "Utils/Common.h"
#include "RDOCIntegration.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<uint8_t> ReadBinary(std::istream& stream) noexcept {
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);
    std::vector<uint8_t> buffer(size);
    if (stream.read(reinterpret_cast<char*>(buffer.data()), size))
    {
        /* worked! */
    }
    return buffer;
}

int main() {
    using namespace RHINO;

    RHINOInterface* rhi = CreateRHINO(BackendAPI::Vulkan);
    rhi->Initialize();

    RDOCIntegration::StartCapture();

    DescriptorHeap* heap = rhi->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 10, "Heap");

    Buffer* bufCBV = rhi->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::ConstantBuffer, 0, "ConstantB");

    Buffer* destUAV1 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV1");
    Buffer* destUAV2 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV2");
    Buffer* destUAV3 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess | ResourceUsage::CopySource, 0, "DestUAV3");
    Buffer* rbkUAV1 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV1");
    Buffer* rbkUAV2 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV2");
    Buffer* rbkUAV3 = rhi->CreateBuffer(sizeof(int) * 64, ResourceHeapType::Readback, ResourceUsage::CopyDest, 0, "DestUAV3");

    WriteBufferSRVDesc desc{};
    desc.offsetInHeap = 0;
    desc.bufferOffset = 0;
    // desc.buffer = bufCBV;
    // heap->WriteCBV(desc);


    desc.buffer = destUAV1;
    desc.offsetInHeap = 0;
    heap->WriteUAV(desc);
    desc.buffer = destUAV2;
    desc.offsetInHeap = 1;
    heap->WriteUAV(desc);
    desc.buffer = destUAV3;
    desc.offsetInHeap = 3;
    heap->WriteUAV(desc);

    std::ifstream shaderFile{"layouts.compute.spirv", std::ios::binary | std::ios::ate};
    assert(shaderFile.is_open());
    auto bytecode = ReadBinary(shaderFile);
    shaderFile.close();
    assert(bytecode.size() % 4 == 0);

    const DescriptorRangeDesc space0rd[] = {
        DescriptorRangeDesc{DescriptorRangeType::UAV, 0, 2},
        DescriptorRangeDesc{DescriptorRangeType::UAV, 3, 1},
    };

    const DescriptorSpaceDesc spaces[] = {
        DescriptorSpaceDesc{0, 0, RHINO_ARR_SIZE(space0rd), space0rd},
    };


    ComputePSODesc psoDesc{};
    psoDesc.CS.entrypoint = "main";
    psoDesc.CS.bytecodeSize = bytecode.size();
    psoDesc.CS.bytecode = bytecode.data();
    psoDesc.debugName = "TestCPSO";
    psoDesc.spacesCount = RHINO_ARR_SIZE(spaces);
    psoDesc.spacesDescs = spaces;
    ComputePSO* pso = rhi->CompileComputePSO(psoDesc);

    CommandList* cmd = rhi->AllocateCommandList("CMDList");
    cmd->SetComputePSO(pso);
    cmd->SetHeap(heap, nullptr);
    cmd->Dispatch({1, 1, 1});

    rhi->SubmitCommandList(cmd);
    rhi->ReleaseCommandList(cmd);

    CommandList* cmd2 = rhi->AllocateCommandList("CMDList");
    cmd2->CopyBuffer(destUAV1, rbkUAV1, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV2, rbkUAV2, 0, 0, sizeof(int) * 64);
    cmd2->CopyBuffer(destUAV3, rbkUAV3, 0, 0, sizeof(int) * 64);
    rhi->SubmitCommandList(cmd2);
    rhi->ReleaseCommandList(cmd2);


    auto* data1 = static_cast<int*>(rhi->MapMemory(rbkUAV1, 0, sizeof(int) * 64));
    auto* data2 = static_cast<int*>(rhi->MapMemory(rbkUAV2, 0, sizeof(int) * 64));
    auto* data3 = static_cast<int*>(rhi->MapMemory(rbkUAV3, 0, sizeof(int) * 64));

    std::vector<int> vdata1{};
    vdata1.resize(64);
    std::vector<int> vdata2{};
    vdata2.resize(64);
    std::vector<int> vdata3{};
    vdata3.resize(64);

    memcpy(vdata1.data(), data1, sizeof(int) * 64);
    memcpy(vdata2.data(), data2, sizeof(int) * 64);
    memcpy(vdata3.data(), data3, sizeof(int) * 64);


    RDOCIntegration::EndCapture();

    rhi->ReleaseComputePSO(pso);
    rhi->ReleaseDescriptorHeap(heap);
    rhi->ReleaseBuffer(bufCBV);
    rhi->ReleaseBuffer(rbkUAV1);
    rhi->ReleaseBuffer(rbkUAV2);
    rhi->ReleaseBuffer(rbkUAV3);
    rhi->ReleaseBuffer(destUAV1);
    rhi->ReleaseBuffer(destUAV2);
    rhi->ReleaseBuffer(destUAV3);
    rhi->Release();

    delete rhi;
}