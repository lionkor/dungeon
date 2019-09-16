#ifndef RENDERER_H
#define RENDERER_H

#include "Global.h"
#include <SFML/Graphics.hpp>
#include <map>

using RenderId = std::uintptr_t;

// "Safely" converts void* to RenderId.
class VoidPtrWrapper
{
private:
    void* m_ptr;
public:
    template<typename _T>
    VoidPtrWrapper(_T* ptr) : m_ptr(static_cast<void*>(ptr)) { }
    
    inline RenderId as_render_id() const { return RenderId(m_ptr); }
};

class Renderer
{
public:
    Renderer(sf::RenderWindow* window);
    
    // FIXME: 'submit' should take refcounted pointer, to make sure the resource cannot go out of scope.
    RenderId submit(VoidPtrWrapper, const class PrimitiveRectangle&);
    void render();
private:
    sf::RenderWindow* m_window;
    std::map<RenderId, class PrimitiveRectangle> m_rectangles;
};

inline std::unique_ptr<Renderer> g_renderer;

#endif//RENDERER_H
