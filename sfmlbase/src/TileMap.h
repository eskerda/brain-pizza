#pragma once

#include <math/utils.h>
#include <math/Vector2D.h>
#include <math/Bounds.h>
#include <SFML/Graphics.hpp>

struct TileCoords {
    TileCoords() { }
    TileCoords(int _x, int _y) : x(_x), y(_y) {}
    int x, y;
};

class TileMap {
public:
    TileMap() : _bounds(-MaxInt/2, -MaxInt/2, MaxInt, MaxInt) { }

    virtual ~TileMap() { }

    void LoadTileSheet(const std::string& textureFile, int tileSize) {
        _texture.loadFromFile("media/"+textureFile);
        _sprite.setTexture(_texture);
        _tileSize = tileSize;
    }

    virtual TileCoords GetTileCoords(int x, int y) = 0;

    int GetTileSize() { return _tileSize; }

    const Bounds& GetBoundsInTiles() { return _bounds; }
    Bounds GetBoundsInPixels() { return Bounds(_bounds.left,_bounds.top,_bounds.width*_tileSize, _bounds.height * _tileSize); }

    void Draw(sf::RenderTarget& target);

protected:
    sf::Texture _texture;
    sf::Sprite _sprite;
    int _tileSize;

    Bounds _bounds;
};

