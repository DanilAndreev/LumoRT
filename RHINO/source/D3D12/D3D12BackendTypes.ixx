export module D3D12BackendTypes;

#ifdef ENABLE_API_D3D12

namespace RHINO::APID3D12 {
    export class D3D12Buffer : public Buffer {
    public:
        ID3D12Resource* buffer = nullptr;
        D3D12_RESOURCE_DESC desc;
        D3D12_RESOURCE_STATES currentState = D3D12_RESOURCE_STATE_COMMON;
    };

    export class D3D12Texture2D : public Texture2D {
    public:
        ID3D12Resource* texture = nullptr;
        D3D12_RESOURCE_DESC desc;
        void* mapped = nullptr;
    };

    export class D3D12DescriptorHeap : public DescriptorHeap {
    public:
        [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHeapCPUHandle(UINT index) const noexcept
        {
            assert(CPUHeapCPUStartHandle.ptr != 0);
            D3D12_CPU_DESCRIPTOR_HANDLE result = CPUHeapCPUStartHandle;
            result.ptr += static_cast<SIZE_T>(index) * descriptorHandleIncrementSize;
            return result;
        }
        [[nodiscard]] D3D12_CPU_DESCRIPTOR_HANDLE GetGPUHeapCPUHandle(UINT index) const noexcept
        {
            assert(GPUHeapCPUStartHandle.ptr != 0);
            D3D12_CPU_DESCRIPTOR_HANDLE result = GPUHeapCPUStartHandle;
            result.ptr += static_cast<SIZE_T>(index) * descriptorHandleIncrementSize;
            return result;
        }
        [[nodiscard]] D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHeapGPUHandle(UINT index) const noexcept
        {
            assert(GPUHeapGPUStartHandle.ptr != 0);
            D3D12_GPU_DESCRIPTOR_HANDLE result = GPUHeapGPUStartHandle;
            result.ptr += static_cast<UINT64>(index) * descriptorHandleIncrementSize;
            return result;
        }

        ID3D12DescriptorHeap* CPUDescriptorHeap = nullptr;
        ID3D12DescriptorHeap* GPUDescriptorHeap = nullptr;
        UINT descriptorHandleIncrementSize = 0;
        D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUStartHandle = {};
        D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUStartHandle = {};
        D3D12_GPU_DESCRIPTOR_HANDLE GPUHeapGPUStartHandle = {};
    };

    export class D3D12RTPSO : public RTPSO {
    public:
    };

    export class D3D12ComputePSO : public ComputePSO {
    public:
        ID3D12RootSignature* rootSignature = nullptr;
        ID3D12PipelineState* PSO = nullptr;
    };

    export class D3D12CommandList : public CommandList {
    public:
        ID3D12CommandAllocator* allocator = nullptr;
        ID3D12GraphicsCommandList* cmd = nullptr;
    public:
        void Dispatch(const DispatchDesc& desc) noexcept final {
            auto* d3d12ComputePSO = static_cast<D3D12ComputePSO*>(desc.pso);
            cmd->SetComputeRootSignature(d3d12ComputePSO->rootSignature);
            cmd->SetPipelineState(d3d12ComputePSO->PSO);
            cmd->Dispatch(desc.dimensionsX, desc.dimensionsY, desc.dimensionsZ);
        }

        void Draw() noexcept final {
        }

        void SetHeap(DescriptorHeap* heap) noexcept final {
            auto* d3d12DescriptorHeap = static_cast<D3D12DescriptorHeap*>(heap);
            cmd->SetDescriptorHeaps(1, &d3d12DescriptorHeap->GPUDescriptorHeap);
        }
    };
}// namespace RHINO::APID3D12

#endif // ENABLE_API_D3D12
