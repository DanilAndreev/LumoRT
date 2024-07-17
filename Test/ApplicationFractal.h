#pragma once

#include <RHINO.h>

class ApplicationFractal {
public:
    void Init(RHINO::BackendAPI api) noexcept;
    void Logic() noexcept;
    void Release() noexcept;
protected:
    RHINO::RHINOInterface* m_RHI = nullptr;
};
