#pragma once

#ifdef ENABLE_API_D3D12

namespace RHINO::APID3D12 {
    class D3D12CommandList : public CommandList {
    public:
        ID3D12CommandAllocator* allocator = nullptr;
        ID3D12GraphicsCommandList* cmd = nullptr;

    public:
        void Dispatch(const DispatchDesc& desc) noexcept final;

        void Draw() noexcept final;

        void SetHeap(DescriptorHeap* heap) noexcept final;
    };
}// namespace RHINO::APID3D12

#endif// ENABLE_API_D3D12
