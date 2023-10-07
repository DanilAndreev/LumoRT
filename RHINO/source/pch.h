#pragma once

#include "RHINO.h"
#include "RHINOTypes.h"

#include <cstdlib>
#include <vector>
#include <string>
#include <cassert>

#include <iostream>

#include "Utils/PlatfromBase.h"
#include "Utils/Common.h"
#include "RHINOTypesImpl.h"


#ifdef RHINO_PLATFORM_WINDOWS
#include <d3d12.h>
#endif // RHINO_PLATFORM_WINDOWS
