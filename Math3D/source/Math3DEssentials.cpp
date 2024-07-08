#include "Math3DEssentials.h"

#define _USE_MATH_DEFINES
#include <cmath>

namespace Math3D {
    double rad2deg(double radians) noexcept {
        return (radians * 180.0) / M_PI;
    }

    double deg2rad(double degrees) noexcept {
        return (degrees * M_PI) / 180.0;
    }

}