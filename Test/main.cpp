#include "RHINO.h"

int main() {
    using namespace RHINO;
    RHINOInterface* rhi = CreateRHINO(BackendAPI::Vulkan);
    rhi->Initialize();
    DescriptorHeap* heap = rhi->CreateDescriptorHeap(DescriptorHeapType::SRV_CBV_UAV, 10, "Heap");
}