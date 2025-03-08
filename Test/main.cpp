#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Application.h"
#include "ApplicationRT.h"
#include "ApplicationFractal.h"
#include <RHINOSwapchainPlatform.h>

int main() {
#ifdef EXAMPLE_ID_RT
    ApplicationRT app;
#elif EXAMPLE_ID_Compute
    Application app;
#elif EXAMPLE_ID_Fractal
    ApplicationFractal app;
#else
#error "Invalid EXAMPLE_ID cmake cache var"
#endif

#ifdef EXAMPLE_API_D3D12
    constexpr auto api = RHINO::BackendAPI::D3D12;
#elif EXAMPLE_API_VULKAN
    constexpr auto api = RHINO::BackendAPI::Vulkan;
#elif EXAMPLE_API_METAL
    constexpr auto api = RHINO::BackendAPI::Metal;
#else
#error "Unsupported API. Invalid EXAMPLE_API cmake cache var"
#endif

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* glfwWindow = glfwCreateWindow(800, 600, "RHINO Example", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    HWND win32Window = glfwGetWin32Window(glfwWindow);


    app.Init(api);
#if defined(EXAMPLE_ID_Fractal) || defined(EXAMPLE_ID_RT)
    RHINOWin32SurfaceDesc surfaceDesc{};
    surfaceDesc.hWnd = win32Window;
    surfaceDesc.hInstance = GetModuleHandle(NULL);

    app.InitSwapchain(&surfaceDesc);
#endif

    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        app.Logic();
    }

#ifdef defined(EXAMPLE_ID_Fractal) || defined(EXAMPLE_ID_RT)
    app.ReleaseSwapchain();
#endif
    app.Release();
}
