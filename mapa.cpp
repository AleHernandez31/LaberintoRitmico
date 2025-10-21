#include "Mapa.h"
#include <cmath>

Map::Map() {
    loadTextures();
}

bool Map::loadTextures() {
    bool ok = true;

    // prepare la ruta para los spray.
    ok &= texRoad.loadFromFile("aca van los assets/spray de la calle"); //road.png
    texRoad.setRepeated(true); //esto es clave para que no se estire la imagen, hace que la textura se tilee cuando el rectangulo es mas grande que la imagen.
    texRoad.setSmooth(true);

    const char* files[4] = { //4 sprays para las manzanas.
        "assets/block_a.png",
        "assets/block_b.png",
        "assets/block_c.png",
        "assets/block_d.png"
    };
    for (int i=0; i<4; ++i) {
        ok &= texBlocks[i].loadFromFile(files[i]);
        texBlocks[i].setSmooth(true); //es para suavizar el escalado.
    }
    return ok;
}

int Map::pickBlockTexId(int gx, int gy) const { //arme un hash para mezclar gx y gy y lo convierto en un indice para mejorar la eleccion de sprays para las manzanas.
    unsigned h = static_cast<unsigned>(gx) * 73856093u
               ^ static_cast<unsigned>(gy) * 19349663u;
    return static_cast<int>(h % texBlocks.size());
}

void Map::drawRoadH(sf::RenderTarget& rt, float y, float leftX, //centro las calles en base a la interseccion y x.
                    float length, float streetW) const {
    sf::RectangleShape r({length, streetW});
    r.setPosition(std::round(leftX), std::round(y - streetW * 0.5f));

    if (texRoad.getSize().x > 0) { // al poner la textura la tilea al tamaño de la calle por el setrepeated(true).
        r.setTexture(&texRoad);
        r.setTextureRect({0, 0, static_cast<int>(length), static_cast<int>(streetW)});
    } else {
        r.setFillColor(sf::Color(65, 69, 74)); //sin textura es solo gris.
    }
    rt.draw(r);
}

void Map::drawRoadV(sf::RenderTarget& rt, float x, float topY,
                    float length, float streetW) const {
    sf::RectangleShape r({streetW, length});
    r.setPosition(std::round(x - streetW * 0.5f), std::round(topY));

    if (texRoad.getSize().x > 0) {
        r.setTexture(&texRoad);
        r.setTextureRect({0, 0, static_cast<int>(streetW), static_cast<int>(length)});
    } else {
        r.setFillColor(sf::Color(65, 69, 74));
    }
    rt.draw(r);
}

void Map::drawBlock(sf::RenderTarget& rt, const sf::FloatRect& area, int texId) const { //seteo el espacio asi no piso las calles.
    sf::RectangleShape r({area.width, area.height});
    r.setPosition(std::round(area.left), std::round(area.top)); //evito lineas borrosas.

    if (0 <= texId && texId < (int)texBlocks.size() && texBlocks[texId].getSize().x > 0) { //aplico el spray que eligo.
        r.setTexture(&texBlocks[texId]);
        // Tileo 1:1 para que no se deforme; la textura se repite si el rect es mayor.
        r.setTextureRect({0, 0, static_cast<int>(area.width), static_cast<int>(area.height)});
    } else {
        r.setFillColor(sf::Color(130, 150, 130)); // color en caso de no tener spray verde/gris
    }

    // meti un borde finoli para separar las manzanas de las calles. sirve por si algun spray no tiene borde (sino lo borramos)
    r.setOutlineThickness(2.f);
    r.setOutlineColor(sf::Color(35, 35, 35));
    rt.draw(r);
}

void Map::draw2x2(sf::RenderTarget& rt, const sf::Vector2f& C, //C funciona como la posicion mundo de la interseccion central (player.getworldpos()).
                  int gridX, int gridY) const { //funciona como coordenada de la interseccion (player.get.grid().
    const float halfCell  = CELL * 0.5f;
    const float halfSt    = STREET * 0.5f;
    const float blockSide = CELL - STREET;

    //dibuje tres lineas horizontales y tres verticales (k = -1, 0, 1) para dar la sensacion de calles continuas, quiza por el zoom 2x2 no lo noten.
    const float span   = 3.f * CELL + STREET; // largo total
    const float leftX  = C.x - 1.5f * CELL - halfSt;
    const float topY   = C.y - 1.5f * CELL - halfSt;

    for (int k = -1; k <= 1; ++k) {
        drawRoadH(rt, C.y + k * CELL, leftX, span, STREET);
        drawRoadV(rt, C.x + k * CELL, topY,  span, STREET);
    }

    //calculo el rectangulo de la manzana segun su interseccion central para dejar las calles a cada lado de la manzana.
    auto blockArea = [&](float cx, float cy){
        return sf::FloatRect(
            cx - halfCell + halfSt,   // left
            cy - halfCell + halfSt,   // top
            blockSide,                // width
            blockSide                 // height
        );
    };

    struct B { int dx, dy; };
    B cells[4] = { {-1,-1}, {+1,-1}, {-1,+1}, {+1,+1} }; //para cada cuadrante (NW, NE, SW, SE):

    for (auto b : cells) {
        const float cx = C.x + b.dx * halfCell; //(cx, cy) centro de la manzana.
        const float cy = C.y + b.dy * halfCell; // interseccion central de la manzana.

        const int gx = gridX + b.dx;
        const int gy = gridY + b.dy;

        const int texId = pickBlockTexId(gx, gy); //sirve para dejar el spray en la misma manzana si la cruzas dos veces.
        drawBlock(rt, blockArea(cx, cy), texId);
    }
}

// meti sobrecarga para mantener llamados existentes (centerwolrd) y (gridX, gridY).
void Map::draw2x2Framed(sf::RenderTarget& rt,
                        const sf::Vector2f& centerWorld,
                        int gridX, int gridY) const
{
    draw2x2(rt, centerWorld, gridX, gridY);
}

// si ya tengo gx, gy calculo directamente el centro como multiplos de CELL.
void Map::draw2x2Framed(sf::RenderTarget& rt, const sf::Vector2i& grid) const {
    const sf::Vector2f centerWorld(
        static_cast<float>(grid.x) * CELL,
        static_cast<float>(grid.y) * CELL
    );

    draw2x2(rt, centerWorld, grid.x, grid.y);
}
