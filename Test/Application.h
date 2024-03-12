#pragma once

#include "RHINO.h"

class Application {
public:
    void Init() noexcept;
    void Logic() noexcept;
    void Release() noexcept;
protected:
    RHINO::RHINOInterface* m_RHI = nullptr;
};
