#ifndef RESOURCE_H
#define RESOURCE_H

#include "Global.h"
#include <filesystem>

template<typename _ResT>
class Resource // DEPRECATE
{
public:
    template<typename... _Args>
    Resource(_Args... args)
    {
        m_res = std::make_unique<_ResT>(args...);
    }

    const _ResT get() const { return *m_res; }

private:
    std::unique_ptr<_ResT> m_res;
    // Add "is_valid" or "is_loaded" flag.
};

#endif // RESOURCE_H
