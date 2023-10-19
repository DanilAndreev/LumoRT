#ifdef ENABLE_API_D3D12
import D3D12Backend;
import VulkanBackend;
#endif // ENABLE_API_D3D12

#ifdef ENABLE_API_METAL
#include "Metal/AllocateMetalBackend.h"
#endif // ENABLE_API_METAL



namespace RHINO {
    RHINOInterface* CreateRHINO(BackendAPI backendApi) noexcept {
        switch (backendApi) {
#ifdef ENABLE_API_D3D12
            case BackendAPI::D3D12: {
                return new APID3D12::D3D12Backend{};
            }
#endif // ENABLE_API_D3D12
#ifdef ENABLE_API_VULKAN
            case BackendAPI::Vulkan: {
                return new APIVulkan::VulkanBackend{};
            }
#endif // ENABLE_API_VULKAN
#ifdef ENABLE_API_METAL
            case BackendAPI::Metal:
                assert(0 && "Metal is not supported. UNDER CONSTRUCTION");
                return APIMetal::AllocateMetalBackend();
#endif // ENABLE_API_METAL
            default:
                assert(0 && "Invalid API or selected API is not supported on this platform.");
                return nullptr;
        }
    }
}// namespace RHINO
