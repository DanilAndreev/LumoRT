#include "Math3DEssentials.h"
#include <cmath>

namespace Math3D {
    double rad2deg(double radians) noexcept {
        return (radians * 180.0) / M_PI;
    }

    double deg2rad(double degrees) noexcept {
        return (degrees * M_PI) / 180.0;
    }

}