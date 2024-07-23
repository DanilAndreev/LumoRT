#include "ApplicationFractal.h"

#define GLFW_INCLUDE_NONE
#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>
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

    ApplicationFractal app;

    constexpr auto api = RHINO::BackendAPI::Metal;
    app.Init(api);
    RHINOAppleSurfaceDesc surfaceDesc{};
    surfaceDesc.window = metalWindow;
    app.InitSwapchain(&surfaceDesc);

    while (!glfwWindowShouldClose(glfwWindow)) {
        glfwPollEvents();
        app.Logic();
    }

    app.ReleaseSwapchain();
    app.Release();

    glfwTerminate();
}
