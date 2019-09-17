#include <SFML/Graphics.hpp>
#include "Global.h"

class EditableTileLayer
{
public:
    EditableTileLayer() {}
    
    void change_texture(const std::string& new_texture)
    {
        m_texture_name = new_texture;
        m_texture.loadFromFile("../res/textures/" + m_texture_name); // FIXME: Scuffed.
    }
    
    sf::Texture* texture() { return &m_texture; }
private:
    sf::Texture m_texture;
    std::string m_texture_name {""};
};

class EditableTile
{
public:
    EditableTile() {}
    EditableTile(sf::Vector2f pos)
        : pos(pos)
    {
    }
    
    void change_texture(const std::string& texture, unsigned char layer)
    {
        m_layers[layer].change_texture(texture);
    }
    
    void draw(sf::RenderWindow* window)
    {
        for (auto& layer : m_layers)
        {
            sf::VertexArray arr { sf::PrimitiveType::Quads, 4 };
            arr[0] = (sf::Vertex(pos, { 0, 0 }));
            arr[1] = (sf::Vertex(pos + sf::Vector2f(g_tile_size, 0), { g_texture_size, 0 }));
            arr[2] = (sf::Vertex(pos + sf::Vector2f(g_tile_size, g_tile_size), { g_texture_size, g_texture_size }));
            arr[3] = (sf::Vertex(pos + sf::Vector2f(0, g_tile_size), { 0, g_texture_size }));
            window->draw(arr, sf::RenderStates(layer.texture()));
            // FIXME: Add "No Texture" texture.
        }
    }
private:
    std::array<EditableTileLayer, g_layer_count> m_layers;
    sf::Vector2f pos;
};

class EditableCell
{
public:
    EditableCell() 
    {
        for (int i = 0; i < g_cell_size; ++i)
        for (int k = 0; k < g_cell_size; ++k)
        {
            m_tiles[i][k] = EditableTile(sf::Vector2f(i*g_tile_size, k*g_tile_size));
        }
    }
    
    void draw(sf::RenderWindow* window)
    {
        for (int i = 0; i < g_cell_size; ++i)
        for (int k = 0; k < g_cell_size; ++k)
        {
            m_tiles[i][k].draw(window);
        }
    }
private:
    std::array<std::array<EditableTile, g_cell_size>, g_cell_size> m_tiles;
};

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(1280, 720), "Cell Editor");
    sf::Event event;
    
    EditableCell current_cell;
    
    while (window->isOpen())
    {
        window->clear();
        
        current_cell.draw(window);
        
        window->display();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window->close();
        }
        
        while (window->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window->close();
                break;
            }
        }
    }
}

