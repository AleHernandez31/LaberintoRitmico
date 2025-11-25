#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class ParticleSystemNodo {
public:
    ParticleSystemNodo();

    // Color base de las partículas (mismo color que el nodo correcto)
    void setBaseColor(const sf::Color& color);

    // Posición de origen (normalmente la posición del nodo en el mundo)
    void setOrigin(const sf::Vector2f& origin);

    // Lanza una “explosión” de partículas
    // cantidad: cuántas partículas
    // minSpeed/maxSpeed: rango de velocidad
    // minLife/maxLife: rango de vida en segundos
    void triggerBurst(int cantidad,
                      float minSpeed,
                      float maxSpeed,
                      float minLife,
                      float maxLife);

    // Actualizar todas las partículas
    void update(float dtSegundos);

    // Dibujar todas las partículas
    void draw(sf::RenderTarget& target) const;

    bool isEmpty() const;

private:
    struct Particle {
        sf::RectangleShape shape;
        sf::Vector2f velocity;
        float life;      // tiempo restante
        float maxLife;   // vida total para hacer el fade
    };

    std::vector<Particle> _particles;
    sf::Vector2f _origin;
    sf::Color _color;
};
