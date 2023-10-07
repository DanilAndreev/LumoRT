#ifndef RHINO_PLATFORM_APPLE
import D3D12Backend;
#else
#include "Metal/AllocateMetalBackend.h"
#endif // RHINO_PLATFORM_APPLE



namespace RHINO {
    RHINOInterface* CreateRHINO(BackendAPI backendApi) noexcept {
        switch (backendApi) {
#ifdef RHINO_PLATFORM_WINDOWS
            case BackendAPI::D3D12: {
                return new APID3D12::D3D12Backend{};
            }
#endif // RHINO_PLATFORM_WINDOWS
#ifdef RHINO_PLATFORM_APPLE
            case BackendAPI::Metal:
                assert(0 && "Metal is not supported. UNDER CONSTRUCTION");
                return APIMetal::AllocateMetalBackend();
#endif // RHINO_PLATFORM_APPLE
            default:
                assert(0 && "Invalid API or selected API is not supported on this platform.");
                return nullptr;
        }
    }
}// namespace RHINO
