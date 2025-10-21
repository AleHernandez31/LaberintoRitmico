#pragma once
#include <SFML/Graphics.hpp>
#include <array>

class Map {
public:
    static constexpr float CELL   = 256.f; //marque la distancia entre cada interseccion de las calles (centro). si lo cambian se modifica todo el mapa.
    static constexpr float STREET = 50.f; //marque el ancho de la calle pero afecta a las manzanas.

    Map(); //cargo las texturas con el constructor

    void draw2x2(sf::RenderTarget& rt, const sf::Vector2f& centerWorld,  //dibuja el mapa 2x2 centrado en la posicion del pj.
                 int gridX, int gridY) const; //coordenadas de la interseccion.

    // meti dos sobrecargas, una acepta el centerwolrd + las coordenadas para las llamadas anteriores y la otra acepta solo las coordenadas y simplifica el main.
    void draw2x2Framed(sf::RenderTarget& rt, const sf::Vector2f& centerWorld,
                       int gridX, int gridY) const;

    void draw2x2Framed(sf::RenderTarget& rt, const sf::Vector2i& grid) const;

public:
    sf::Texture texRoad; //repito la textura de la calle.
    std::array<sf::Texture,4> texBlocks; // meto 4 png para las manzanas, se pueden meter mas si quieren.

    bool loadTextures(); //cargo los assets desde la ruta.
    int  pickBlockTexId(int gx, int gy) const; //eligo el spray segun la coordenada de cada manzana.

    //helpers para el dibujo (horizontal, vertical y para la manzana drawblock.
    void drawRoadH(sf::RenderTarget& rt, float y, float leftX,
                   float length, float streetW) const;
    void drawRoadV(sf::RenderTarget& rt, float x, float topY,
                   float length, float streetW) const;
    void drawBlock(sf::RenderTarget& rt, const sf::FloatRect& area,
                   int texId) const;
};
