#pragma once

#ifdef WIN32

#ifdef WIN32
#include <Windows.h>
#endif // WIN32


#include <RHINO.h>

class ApplicationRT {
public:
    void Init(RHINO::BackendAPI api) noexcept;
    void Logic() noexcept;
    void Release() noexcept;
protected:
    RHINO::RHINOInterface* m_RHI = nullptr;

};

#endif // WIN32
