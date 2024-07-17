#ifndef _example_fractal_types_h_
#define _example_fractal_types_h_

#define EXAMPLE_FRACTAL_THREADGROUP_X 32
#define EXAMPLE_FRACTAL_THREADGROUP_Y 32

struct FractalSettings {
      float moveX;
      float moveY;

      float leanX;
      float leanY;
};

#endif // _example_fractal_types_h_
