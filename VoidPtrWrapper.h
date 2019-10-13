#ifndef VOIDPTRWRAPPER_H
#define VOIDPTRWRAPPER_H

#include "Global.h"

// "Safely" converts void* to RenderId.
class VoidPtrWrapper
{
private:
    void* m_ptr;

public:
    template<typename _T>
    VoidPtrWrapper(_T* ptr) : m_ptr(static_cast<void*>(ptr))
    {
    }

    [[deprecated]] inline RenderId as_render_id() const { return RenderId(m_ptr); }

    template<class _T>
    inline _T reinterpret_as() const
    {
        return reinterpret_cast<_T>(m_ptr);
    }
};

#endif // VOIDPTRWRAPPER_H
