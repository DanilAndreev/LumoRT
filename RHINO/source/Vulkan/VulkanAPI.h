#pragma once

#ifdef ENABLE_API_VULKAN

#define RHINO_VULKAN_API_FUNCS()    \
    RHINO_APPLY(vkGetDescriptorEXT) \
    RHINO_APPLY(vkCreateBuffer)     \
    RHINO_APPLY(vkCmdBindDescriptorBuffersEXT)


namespace RHINO::APIVulkan {
#define RHINO_APPLY(f) ::PFN_##f f;
    RHINO_VULKAN_API_FUNCS()
#undef RHINO_APPLY

    void LoadVulkanAPI(VkInstance instance, PFN_vkGetInstanceProcAddr getProcAddr) noexcept;
}// namespace RHINO::APIVulkan

#endif// ENABLE_API_VULKAN