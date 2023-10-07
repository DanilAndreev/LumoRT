export module D3D12Backend;

import D3D12BackendTypes;

#ifdef ENABLE_API_D3D12

#define RHINO_VALIDATE_D3D_RESULT(expr) expr
#define RHINO_GPU_DEBUG(expr) expr

namespace RHINO::APID3D12 {
    using namespace std::string_literals;

    D3D12_HEAP_TYPE ToD3D12HeapType(ResourceHeapType value) noexcept {
        switch (value) {
            case ResourceHeapType::Default:
                return D3D12_HEAP_TYPE_DEFAULT;
            case ResourceHeapType::Upload:
                return D3D12_HEAP_TYPE_UPLOAD;
            case ResourceHeapType::Readback:
                return D3D12_HEAP_TYPE_READBACK;
            default:
                assert(0);
                return D3D12_HEAP_TYPE_DEFAULT;
        }
    }

    D3D12_RESOURCE_FLAGS ToD3D12ResourceFlags(ResourceUsage value) noexcept {
        D3D12_RESOURCE_FLAGS nativeFlags = D3D12_RESOURCE_FLAG_NONE;
        if (bool(value & ResourceUsage::UnorderedAccess))
            nativeFlags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
        return nativeFlags;
    }

    D3D12_DESCRIPTOR_HEAP_TYPE ToD3D12DescriptorHeapType(DescriptorHeapType type) noexcept
    {
        switch (type)
        {
            case DescriptorHeapType::SRV_CBV_UAV:
                return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
            case DescriptorHeapType::RTV:
                return D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            case DescriptorHeapType::DSV:
                return D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
            case DescriptorHeapType::Sampler:
                return D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
            default:
                assert(0);
                return D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        }
    }

    export class D3D12Backend : public RHINOInterface {
    private:
        ID3D12Device* m_Device;

    public:
        D3D12Backend() noexcept : m_Device(nullptr) {}

    public:
        RTPSO* CompileRTPSO() noexcept final {
            return nullptr;
        }

        void ReleaseRTPSO(RTPSO* pso) noexcept final {
            delete pso;
        }

        ComputePSO* CompileComputePSO() noexcept final {
            return nullptr;
        }

        void ReleaseComputePSO(ComputePSO* pso) noexcept final {
        }

    public:
        D3D12Buffer* CreateBuffer(size_t size, ResourceHeapType heapType, ResourceUsage usage, size_t structuredStride, const char* name) noexcept final {
            RHINO_UNUSED_VAR(structuredStride);

            auto* result = new D3D12Buffer{};

            switch (heapType) {
                case ResourceHeapType::Upload:
                    result->currentState = D3D12_RESOURCE_STATE_GENERIC_READ;
                    break;
                case ResourceHeapType::Readback:
                    result->currentState = D3D12_RESOURCE_STATE_COPY_DEST;
                    break;
                default:
                    result->currentState = D3D12_RESOURCE_STATE_COMMON;
            }

            D3D12_HEAP_PROPERTIES heapProperties{};
            heapProperties.Type = ToD3D12HeapType(heapType);
            heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
            heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

            D3D12_RESOURCE_DESC resourceDesc{};
            resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
            resourceDesc.Alignment = 0;
            resourceDesc.Height = 1;
            resourceDesc.DepthOrArraySize = 1;
            resourceDesc.MipLevels = 1;
            resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
            resourceDesc.SampleDesc.Count = 1;
            resourceDesc.SampleDesc.Quality = 0;
            resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

            resourceDesc.Width = RHINO_CEIL_TO_MULTIPLE_OF(size, 256);
            resourceDesc.Flags = ToD3D12ResourceFlags(usage);

            RHINO_VALIDATE_D3D_RESULT(m_Device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &resourceDesc, result->currentState, nullptr, IID_PPV_ARGS(&result->buffer)));
            result->desc = result->buffer->GetDesc();

            RHINO_GPU_DEBUG(SetDebugName(result->buffer, name));
            return result;
        }

        void ReleaseBuffer(Buffer* buffer) noexcept final {
            if (!buffer) return;
            auto* d3d12Buffer = static_cast<D3D12Buffer*>(buffer);
            d3d12Buffer->buffer->Release();
            delete d3d12Buffer;
        }

        D3D12Buffer* CreateTexture2D() noexcept final {
            return nullptr;
        }

        void ReleaseTexture2D(Texture2D* texture) noexcept final {
            if (!texture) return;
            auto* d3d12Texture = static_cast<D3D12Texture2D*>(texture);
            d3d12Texture->texture->Release();
            delete d3d12Texture;
        }

        D3D12DescriptorHeap* CreateDescriptorHeap(DescriptorHeapType heapType, const PipelineLayoutDesc& pipelineLayoutDesc, const char* name) noexcept final {
            auto* result = new D3D12DescriptorHeap{};

            size_t elementsCount = 0;
//            for (size_t i = 0; i < pipelineLayoutDesc.setsDescsCount; ++i)
//            {
//                UINT maxBindingSlot = 0;
//                for (size_t j = 0; j < pipelineLayoutDesc.setDescs[i].bindingDescsCount; ++j)
//                {
//                    const DescriptorBindingDesc& bindingDesc = pipelineLayoutDesc.setDescs[i].bindingDescs[j];
//                    maxBindingSlot = std::max(maxBindingSlot, bindingDesc.slot);
//                }
//                elementsCount += maxBindingSlot + 1;
//            }

            D3D12_DESCRIPTOR_HEAP_TYPE nativeHeapType = ToD3D12DescriptorHeapType(heapType);
            D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
            heapDesc.NumDescriptors = elementsCount;
            heapDesc.Type = nativeHeapType;
            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            RHINO_VALIDATE_D3D_RESULT(m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&result->CPUDescriptorHeap)));

            heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
            RHINO_VALIDATE_D3D_RESULT(m_Device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&result->GPUDescriptorHeap)));

            result->descriptorHandleIncrementSize = m_Device->GetDescriptorHandleIncrementSize(nativeHeapType);

            result->CPUHeapCPUStartHandle = result->CPUDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

            result->GPUHeapCPUStartHandle = result->GPUDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            result->GPUHeapGPUStartHandle = result->GPUDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

            SetDebugName(result->CPUDescriptorHeap, name + ".CPUDescriptorHeap"s);
            SetDebugName(result->GPUDescriptorHeap, name + ".GPUDescriptorHeap"s);
            return result;
        }
        void ReleaseDescriptorHeap(DescriptorHeap* heap) noexcept final {
            if (!heap) return;
            delete heap;
        }

        D3D12CommandList* AllocateCommandList(const char* name) noexcept final {
            auto* result = new D3D12CommandList{};

            RHINO_VALIDATE_D3D_RESULT(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&result->allocator)));
            RHINO_VALIDATE_D3D_RESULT(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, result->allocator, nullptr,
                                                                  IID_PPV_ARGS(&result->cmd)));
            result->cmd->Close();

            SetDebugName(result->allocator, "CMDAllocator_"s + name);
            SetDebugName(result->cmd, "CMD_"s + name);
            return result;
        }

        void ReleaseCommandList(CommandList* commandList) noexcept final {
            if (!commandList) return;
            auto* d3d12CommandList = static_cast<D3D12CommandList*>(commandList);
            d3d12CommandList->cmd->Release();
            d3d12CommandList->allocator->Release();
            delete d3d12CommandList;
        }

    public:
        void DispatchCompute() noexcept final {
        }
        void DispatchComputeIndirect() noexcept final {
        }
        void TraceRays() noexcept final {
        }

    private:
        void SetDebugName(ID3D12DeviceChild* resource, const std::string& name) noexcept {
            resource->SetPrivateData(WKPDID_D3DDebugObjectName, name.length(), name.c_str());
        }
    };
}// namespace RHINO::APID3D12

#endif // ENABLE_API_D3D12
