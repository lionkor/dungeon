#include "Renderer.h"
#include <assert.h>
#include "PrimitiveRectangle.h"

Renderer::Renderer(sf::RenderWindow* window)
    : m_window(window)
{
    window->setFramerateLimit(60);  
}

RenderId Renderer::submit(VoidPtrWrapper self, const PrimitiveRectangle& rect)
{
    m_rectangles.emplace(self.as_render_id(), rect);
    return self.as_render_id();
}

void Renderer::render()
{
    m_window->clear();
    
    for (auto& rect : m_rectangles)
    {
        sf::VertexArray arr(sf::PrimitiveType::Quads, 4);
        for (int i = 0; i < 4; ++i)
        {
            arr[i] = rect.second.m_vertices[i];
        }
        m_window->draw(arr);
    }
    
    m_window->display();
}
