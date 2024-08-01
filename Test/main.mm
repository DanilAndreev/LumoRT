#include "ApplicationFractal.h"
#include "Application.h"

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#import <Foundation/Foundation.h>
#import <RHINOSwapchainPlatform.h>


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* glfwWindow = glfwCreateWindow(800, 600, "RHINO Example", NULL, NULL);
    if (!glfwWindow) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    NSWindow* metalWindow = glfwGetCocoaWindow(glfwWindow);

    constexpr auto api = RHINO::BackendAPI::Metal;
    RHINOAppleSurfaceDesc surfaceDesc{};
    surfaceDesc.window = metalWindow;
#ifdef EXAMPLE_ID_Compute
    Application app;
    app.Init(api);
#elif EXAMPLE_ID_Fractal
    ApplicationFractal app;
    app.Init(api);
    app.InitSwapchain(&surfaceDesc);
#else
#error "Invalid example id"
#endif


    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        app.Logic();
    }

#ifdef EXAMPLE_ID_Fractal
    app.ReleaseSwapchain();
#endif
    app.Release();

    glfwTerminate();
}
