#include "RHINO.h"

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
    DescriptorHeap* heap = rhi->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 10, "Heap");

    Buffer* bufCBV = rhi->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::ConstantBuffer, 0, "ConstantB");
    Buffer* bufUAV = rhi->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::UnorderedAccess, 0, "ConstantB");

    WriteBufferSRVDesc desc{};
    desc.offsetInHeap = 0;
    desc.bufferOffset = 0;
    desc.buffer = bufCBV;
    heap->WriteCBV(desc);
    desc.buffer = bufUAV;
    desc.offsetInHeap = 1;
    heap->WriteUAV(desc);

    std::ifstream shaderFile{"res.spirv", std::ios::binary | std::ios::ate};
    assert(shaderFile.is_open());
    auto bytecode = ReadBinary(shaderFile);
    shaderFile.close();
    assert(bytecode.size() % 4 == 0);

    ComputePSODesc psoDesc{};
    psoDesc.CS.entrypoint = "main";
    psoDesc.CS.bytecodeSize = bytecode.size();
    psoDesc.CS.bytecode = bytecode.data();
    psoDesc.visibleCBVSRVUAVDescriptorCount = 2;
    psoDesc.visibleSamplerDescriptorCount = 0;
    psoDesc.debugName = "TestCPSO";
    ComputePSO* pso = rhi->CompileComputePSO(psoDesc);

    CommandList* cmd = rhi->AllocateCommandList("CMDList");
    cmd->SetComputePSO(pso);
    cmd->SetHeap(heap);
    cmd->Dispatch({1, 1, 1});

    rhi->SubmitCommandList(cmd);

    rhi->ReleaseDescriptorHeap(heap);
    rhi->ReleaseBuffer(bufCBV);
    rhi->ReleaseBuffer(bufUAV);
    rhi->Release();
    delete rhi;
}