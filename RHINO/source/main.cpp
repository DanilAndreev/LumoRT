import D3D12Backend;

namespace RHINO {
    RHINOInterface* CreateRHINO(BackendAPI backendApi) noexcept {
        switch (backendApi) {
            case BackendAPI::D3D12: {
                return new APID3D12::D3D12Backend{};
            }
            case BackendAPI::Metal:
                assert(0 && "Metal is not supported. UNDER CONSTRUCTION");
                return nullptr;
            default:
                assert(0 && "Invalid API or selected API is not supported on this platform.");
                return nullptr;
        }
    }
}// namespace RHINO
