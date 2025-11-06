#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <map>
#include <random>

class Map {
public:
    static constexpr float CELL   = 256.f; //marque la distancia entre cada interseccion de las calles (centro). si lo cambian se modifica todo el mapa.
    static constexpr float STREET = 50.f;  //marque el ancho de la calle pero afecta a las manzanas.

    Map(); //cargo las texturas con el constructor

    void draw2x2(sf::RenderTarget& rt, const sf::Vector2f& centerWorld,  //dibuja el mapa 2x2 centrado en la posicion del pj.
                 int gridX, int gridY) const; //coordenadas de la interseccion.

    // meti dos sobrecargas, una acepta el centerwolrd + las coordenadas para las llamadas anteriores y la otra acepta solo las coordenadas y simplifica el main.
    void draw2x2Framed(sf::RenderTarget& rt, const sf::Vector2f& centerWorld,
                       int gridX, int gridY) const;

    void draw2x2Framed(sf::RenderTarget& rt, const sf::Vector2i& grid) const;

private:
    // Calles
    sf::Texture texRoadH;
    sf::Texture texRoadV;

    // spray para interseccion (el cruce de las calles)
    sf::Texture texCross; // spray de la interseccion (cuadrado STREET x STREET)

    // Manzanas: vector para >4 sprays
    std::vector<sf::Texture> texBlocks; // meto 4 png para las manzanas, se pueden meter mas si quieren.

    bool loadTextures(); //cargo los assets desde la ruta.

    // cache por manzana (no por chunk).
    mutable std::map<std::pair<int,int>, int> m_blockTexByCell;
    unsigned m_sessionSeed = 0u;

    int getOrCreateBlockTexId(int bx, int by) const; //eligo el spray segun la coordenada de cada manzana.

    //helpers para el dibujo (horizontal, vertical y para la manzana drawblock.
    void drawRoadH(sf::RenderTarget& rt, float y, float leftX,
                   float length, float streetW) const;
    void drawRoadV(sf::RenderTarget& rt, float x, float topY,
                   float length, float streetW) const;
    void drawBlock(sf::RenderTarget& rt, const sf::FloatRect& area,
                   int texId) const;

    // añadi un helper para dibujar la interseccion (spray cuadrado STREET x STREET)
    void drawIntersection(sf::RenderTarget& rt, float cx, float cy, float streetW) const;
};
