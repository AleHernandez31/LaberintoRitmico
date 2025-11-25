#include "ParticleSystemNodo.h"
#include <cmath>
#include <cstdlib>

ParticleSystemNodo::ParticleSystemNodo()
    : _origin(0.f, 0.f), _color(sf::Color::White) {}

void ParticleSystemNodo::setBaseColor(const sf::Color& color) {
    _color = color;
}

void ParticleSystemNodo::setOrigin(const sf::Vector2f& origin) {
    _origin = origin;
}

void ParticleSystemNodo::triggerBurst(int cantidad,
                                      float minSpeed,
                                      float maxSpeed,
                                      float minLife,
                                      float maxLife) {
    if (cantidad <= 0) return;

    for (int i = 0; i < cantidad; ++i) {
        Particle p;

        // Partículas de pixel
        float size = 3.f + static_cast<float>(std::rand() % 3); // entre 3 y 5 px
        p.shape.setSize(sf::Vector2f(size, size));
        p.shape.setOrigin(size * 0.5f, size * 0.5f);
        p.shape.setPosition(_origin);
        p.shape.setFillColor(_color);

        // Dirección aleatoria
        float angle = static_cast<float>(std::rand() % 360) * 3.1415926f / 180.f;

        // Velocidad aleatoria entre minSpeed y maxSpeed
        float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        float speed = minSpeed + t * (maxSpeed - minSpeed);

        p.velocity.x = std::cos(angle) * speed;
        p.velocity.y = std::sin(angle) * speed;

        // Vida aleatoria entre minLife y maxLife
        float tLife = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        p.maxLife = minLife + tLife * (maxLife - minLife);
        p.life    = p.maxLife;

        _particles.push_back(p);
    }
}

void ParticleSystemNodo::update(float dtSegundos) {
    if (_particles.empty()) return;

    for (std::size_t i = 0; i < _particles.size(); ++i) {
        Particle& p = _particles[i];

        p.life -= dtSegundos;
        if (p.life <= 0.f) {
            // Se maneja después con erase-remove
            continue;
        }

        // Movimiento
        p.shape.move(p.velocity * dtSegundos);

        // Fade out (alpha proporcional a la vida restante)
        float factor = p.life / p.maxLife;
        if (factor < 0.f) factor = 0.f;
        if (factor > 1.f) factor = 1.f;

        sf::Color c = _color;
        c.a = static_cast<sf::Uint8>(255.f * factor);
        p.shape.setFillColor(c);
    }

    // Eliminar las partículas muertas
    _particles.erase(
        std::remove_if(
            _particles.begin(),
            _particles.end(),
            [](const Particle& p) { return p.life <= 0.f; }
        ),
        _particles.end()
    );
}

void ParticleSystemNodo::draw(sf::RenderTarget& target) const {
    for (const Particle& p : _particles) {
        target.draw(p.shape);
    }
}

bool ParticleSystemNodo::isEmpty() const {
    return _particles.empty();
}
