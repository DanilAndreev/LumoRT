#include "Application.h"
#include "ApplicationRT.h"
#include "ApplicationFractal.h"

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

    app.Init(api);
    app.Logic();
    app.Release();
}
