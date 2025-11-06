#include "Mapa.h"
#include <cmath>

// inicie un mezclador estable para semillas por celda (igual que antes.
static inline unsigned mix3(int a, int b, unsigned seed) {
    unsigned h = 2166136261u;
    h ^= static_cast<unsigned>(a) + 0x9e3779b9u + (h<<6) + (h>>2);
    h ^= static_cast<unsigned>(b) + 0x85ebca6bu + (h<<6) + (h>>2);
    h ^= seed + 0x27d4eb2du + (h<<6) + (h>>2);
    return h;
}

Map::Map() {
    std::random_device rd;
    m_sessionSeed = rd();
    loadTextures();
}

bool Map::loadTextures() {
    bool ok = true;

    //rutas de assets.
    ok &= texRoadH.loadFromFile("assets/mapa/roadh.png"); // horizontal
    if (ok) texRoadH.setRepeated(true), texRoadH.setSmooth(true);

    bool okV = texRoadV.loadFromFile("assets/mapa/roadv.png"); // vertical
    if (okV) texRoadV.setRepeated(true), texRoadV.setSmooth(true);
    ok &= okV;

    //ruta para el spray de la interseccion.
    bool okX = texCross.loadFromFile("assets/mapa/intersection.png");
    if (okX) texCross.setSmooth(true);
    ok &= okX;

    // Manzanas (podemos listar la cantidad que quieran)
    const char* files[] = {
        "assets/mapa/block.png",
        "assets/mapa/block2.png",
        "assets/mapa/block3.png",
        "assets/mapa/block4.png",

    };

    texBlocks.clear();
    for (const char* f : files) {
        sf::Texture t;
        if (t.loadFromFile(f)) {
            t.setSmooth(true);
            texBlocks.push_back(std::move(t));
        }
    }

    return ok && !texBlocks.empty();
}

int Map::getOrCreateBlockTexId(int bx, int by) const {
    auto key = std::make_pair(bx, by);
    auto it = m_blockTexByCell.find(key);
    if (it != m_blockTexByCell.end()) return it->second;

    int chosen = -1;
    if (!texBlocks.empty()) {
        std::mt19937 rng(mix3(bx, by, m_sessionSeed));
        std::uniform_int_distribution<int> dist(0, static_cast<int>(texBlocks.size()) - 1);
        chosen = dist(rng);
    }
    m_blockTexByCell.emplace(key, chosen);
    return chosen;
}

void Map::drawRoadH(sf::RenderTarget& rt, float y, float leftX,
                    float length, float streetW) const {
    if (texRoadH.getSize().x == 0) return; // sin textura => no dibujar

    sf::RectangleShape r({length, streetW});
    r.setPosition(std::round(leftX), std::round(y - streetW * 0.5f));
    r.setTexture(&texRoadH);
    r.setTextureRect({0, 0, static_cast<int>(length), static_cast<int>(streetW)});
    rt.draw(r);
}

void Map::drawRoadV(sf::RenderTarget& rt, float x, float topY,
                    float length, float streetW) const {
    if (texRoadV.getSize().x == 0) return; // sin textura => no dibujar

    sf::RectangleShape r({streetW, length});
    r.setPosition(std::round(x - streetW * 0.5f), std::round(topY));
    r.setTexture(&texRoadV);
    r.setTextureRect({0, 0, static_cast<int>(streetW), static_cast<int>(length)});
    rt.draw(r);
}

void Map::drawBlock(sf::RenderTarget& rt, const sf::FloatRect& area, int texId) const {
    if (texId < 0 || texId >= static_cast<int>(texBlocks.size())) return; // sin textura => no dibujar

    sf::RectangleShape r({area.width, area.height});
    r.setPosition(std::round(area.left), std::round(area.top));
    r.setTexture(&texBlocks[texId]);
    r.setTextureRect({0, 0, static_cast<int>(area.width), static_cast<int>(area.height)});
    rt.draw(r);
}

//dibujo el spray de la interseccion centrado en (cx, cy).
void Map::drawIntersection(sf::RenderTarget& rt, float cx, float cy, float streetW) const {
    if (texCross.getSize().x == 0) return; // sin textura ya no dibuja.

    sf::RectangleShape r({streetW, streetW});
    r.setPosition(std::round(cx - streetW * 0.5f), std::round(cy - streetW * 0.5f));
    r.setTexture(&texCross);
    r.setTextureRect({0, 0, static_cast<int>(streetW), static_cast<int>(streetW)});
    rt.draw(r);
}

void Map::draw2x2(sf::RenderTarget& rt, const sf::Vector2f& C,
                  int gridX, int gridY) const {
    const float halfCell  = CELL * 0.5f;
    const float halfSt    = STREET * 0.5f;
    const float blockSide = CELL - STREET;

    //Calles (tres lineas horizontales y verticales alrededor).
    const float span   = 3.f * CELL + STREET; // largo total.
    const float leftX  = C.x - 1.5f * CELL - halfSt;
    const float topY   = C.y - 1.5f * CELL - halfSt;

    for (int k = -1; k <= 1; ++k) {
        drawRoadH(rt, C.y + k * CELL, leftX, span, STREET);
        drawRoadV(rt, C.x + k * CELL, topY,  span, STREET);
    }

    //Intersecciones visibles (k en -1,0,1) => 3x3 puntos.
    for (int ix = -1; ix <= 1; ++ix) {
        for (int iy = -1; iy <= 1; ++iy) {
            const float cx = C.x + ix * CELL;
            const float cy = C.y + iy * CELL;
            drawIntersection(rt, cx, cy, STREET);
        }
    }

    //Manzanas interiores (2x2) – fijas por celda (bx,by).
    auto blockArea = [&](float cx, float cy){
        return sf::FloatRect(
            cx - halfCell + halfSt,   // left
            cy - halfCell + halfSt,   // top
            blockSide,                // width
            blockSide                 // height
        );
    };

    struct B { int dx, dy; };
    B cells[4] = { {-1,-1}, {+1,-1}, {-1,+1}, {+1,+1} }; // NW, NE, SW, SE

    for (auto b : cells) {
        const float cx = C.x + b.dx * halfCell; // centro de la manzana en mundo
        const float cy = C.y + b.dy * halfCell;

        // esquina NW absoluta de la manzana
        const int bx = (gridX + (b.dx == -1 ? -1 : 0));
        const int by = (gridY + (b.dy == -1 ? -1 : 0));

        const int texId = getOrCreateBlockTexId(bx, by);
        drawBlock(rt, blockArea(cx, cy), texId);
    }
}

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
