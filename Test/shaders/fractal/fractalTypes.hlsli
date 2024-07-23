#ifndef _example_fractal_types_h_
#define _example_fractal_types_h_

#define EXAMPLE_FRACTAL_THREADGROUP_X 32
#define EXAMPLE_FRACTAL_THREADGROUP_Y 32

#define EXAMPLE_FRACTAL_BLIT_THREADGROUP_X 32
#define EXAMPLE_FRACTAL_BLIT_THREADGROUP_Y 32

#ifndef __HLSL_VERSION
using uint = uint32_t;
#endif

struct FractalSettings {
      uint backbufferWidth;
      uint backbufferHeight;
      float time;

};

#endif // _example_fractal_types_h_
