#ifdef ENABLE_API_D3D12

#include "D3D12CommandList.h"
#include "D3D12BackendTypes.h"
#include "D3D12DescriptorHeap.h"

namespace RHINO::APID3D12 {
    void D3D12CommandList::Dispatch(const DispatchDesc& desc) noexcept {
        auto* d3d12ComputePSO = static_cast<D3D12ComputePSO*>(desc.pso);
        cmd->SetComputeRootSignature(d3d12ComputePSO->rootSignature);
        cmd->SetPipelineState(d3d12ComputePSO->PSO);
        cmd->Dispatch(desc.dimensionsX, desc.dimensionsY, desc.dimensionsZ);
    }

    void D3D12CommandList::Draw() noexcept {
    }

    void D3D12CommandList::SetHeap(DescriptorHeap* heap) noexcept {
        auto* d3d12DescriptorHeap = static_cast<D3D12DescriptorHeap*>(heap);
        cmd->SetDescriptorHeaps(1, &d3d12DescriptorHeap->GPUDescriptorHeap);
    }
}// namespace RHINO::APID3D12

#endif// ENABLE_API_D3D12
