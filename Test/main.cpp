#include "RHINO.h"

#include <iostream>

int main() {
    using namespace RHINO;
    RHINOInterface* rhi = CreateRHINO(BackendAPI::Vulkan);
    rhi->Initialize();
    DescriptorHeap* heap = rhi->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 10, "Heap");

    Buffer* buf = rhi->CreateBuffer(64, ResourceHeapType::Default, ResourceUsage::ConstantBuffer, 0, "ConstantB");

    WriteBufferSRVDesc desc{};
    desc.offsetInHeap = 0;
    desc.bufferOffset = 0;
    desc.buffer = buf;
    heap->WriteCBV(desc);
}