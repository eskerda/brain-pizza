#include "TileMap.h"

#include "InputManager.h"

void TileMap::Draw(sf::RenderTarget& rt)
{
    sf::IntRect rect = sf::IntRect(0,0,_tileSize,_tileSize);

    sf::Vector2f center = Input::GetCameraCenter();
    sf::Vector2f halfSize = Input::GetWindowSize()/(Input::GetZoom()*2.f);
    for (int iy = ceil(center.y - halfSize.y)/_tileSize; iy < floor(center.y + halfSize.y)/_tileSize; iy++)
    {
        for (int ix = ceil(center.x - halfSize.x)/_tileSize; ix < floor(center.x + halfSize.x)/_tileSize; ix++)
        {
            if (_bounds.IsInside(Vector2D(ix,iy)))
            {
                TileCoords p = GetTileCoords(ix,iy);
                rect.left = p.x;
                rect.top = p.y;
                _sprite.setTextureRect(rect);
                _sprite.setPosition(ix*_tileSize,iy*_tileSize);
                rt.draw(_sprite);
            }
        }
    }
}
