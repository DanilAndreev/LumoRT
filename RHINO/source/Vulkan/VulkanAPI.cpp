#ifdef ENABLE_API_VULKAN

#include "VulkanAPI.h"

namespace RHINO::APIVulkan {
    namespace EXT {
#define RHINO_APPLY(f) ::PFN_##f f;
        RHINO_VULKAN_API_FUNCS()
#undef RHINO_APPLY
    }

    void LoadVulkanAPI(VkInstance instance, PFN_vkGetInstanceProcAddr getProcAddr) noexcept {
#define RHINO_APPLY(f) EXT::f = reinterpret_cast<PFN_##f>(getProcAddr(instance, #f));
        RHINO_VULKAN_API_FUNCS()
#undef RHINO_APPLY
    }
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN
