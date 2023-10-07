export module D3D12BackendTypes;

#ifdef RHINO_PLATFORM_WINDOWS

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

    export class D3D12DescriptorHeap {
    public:
    };

    export class D3D12RTPSO {
    public:
    };

    export class D3D12ComputePSO {
    public:
    };

    export class D3D12CommandList {
    public:
    };
}// namespace RHINO::APID3D12

#endif// RHINO_PLATFORM_WINDOWS