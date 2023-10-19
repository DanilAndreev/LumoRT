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
        void WriteSRV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* d3d12Buffer = static_cast<D3D12Buffer*>(desc.buffer);

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = DXGI_FORMAT_UNKNOWN;
            srvDesc.Buffer.NumElements = 0;
            srvDesc.Buffer.StructureByteStride = 0;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateShaderResourceView(d3d12Buffer->buffer, &srvDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteUAV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* d3d12Buffer = static_cast<D3D12Buffer*>(desc.buffer);

            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
            uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            uavDesc.Format = DXGI_FORMAT_UNKNOWN;
            uavDesc.Buffer.NumElements = 0;
            uavDesc.Buffer.StructureByteStride = 0;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateUnorderedAccessView(d3d12Buffer->buffer, nullptr, &uavDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteCBV(const WriteBufferSRVDesc& desc) noexcept final {
            auto* d3d12Buffer = static_cast<D3D12Buffer*>(desc.buffer);

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
            cbvDesc.SizeInBytes = d3d12Buffer->desc.Width;
            cbvDesc.BufferLocation = d3d12Buffer->buffer->GetGPUVirtualAddress();

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateConstantBufferView(&cbvDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteSRV(const WriteTexture2DSRVDesc& desc) noexcept final {
            auto* d3d12Texture = static_cast<D3D12Texture2D*>(desc.texture);

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            // srvDesc.Format = CalculateSRVFormat(d3d12Texture->desc.Format);
            srvDesc.Texture2D.MipLevels = -1;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateShaderResourceView(d3d12Texture->texture, &srvDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteUAV(const WriteTexture2DSRVDesc& desc) noexcept final {
            auto* d3d12Texture = static_cast<D3D12Texture2D*>(desc.texture);

            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
            uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
            // uavDesc.Format = CalculateUAVFormat(d3d12Texture->desc.Format);
            uavDesc.Texture2D.MipSlice = 0;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateUnorderedAccessView(d3d12Texture->texture, nullptr, &uavDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteSRV(const WriteTexture3DSRVDesc& desc) noexcept final {
            auto* d3d12Texture = static_cast<D3D12Texture2D*>(desc.texture);

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            // srvDesc.Format = CalculateSRVFormat(d3d12Texture->desc.Format);
            srvDesc.Texture2D.MipLevels = -1;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateShaderResourceView(d3d12Texture->texture, &srvDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
        void WriteUAV(const WriteTexture3DSRVDesc& desc) noexcept final {
            auto* d3d12Texture = static_cast<D3D12Texture2D*>(desc.texture);

            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
            uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
            // uavDesc.Format = CalculateUAVFormat(d3d12Texture->desc.Format);
            uavDesc.Texture2D.MipSlice = 0;

            D3D12_CPU_DESCRIPTOR_HANDLE CPUHeapCPUHandle = GetCPUHeapCPUHandle(desc.offsetInHeap);
            D3D12_CPU_DESCRIPTOR_HANDLE GPUHeapCPUHandle = GetGPUHeapCPUHandle(desc.offsetInHeap);
            device->CreateUnorderedAccessView(d3d12Texture->texture, nullptr, &uavDesc, CPUHeapCPUHandle);
            device->CopyDescriptorsSimple(1, GPUHeapCPUHandle, CPUHeapCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }

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

        ID3D12Device* device;
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
