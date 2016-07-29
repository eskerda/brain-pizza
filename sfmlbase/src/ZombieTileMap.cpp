#include "ZombieTileMap.h"

const char road  = 'a';
const char house = 'r';
const char empty = ' ';

ZombieTileMap::ZombieTileMap(const std::vector< std::vector<char> >& tiles, const std::string& textureFile, int tileSize)
{
    _texture.loadFromFile("media/"+textureFile);
    _tileSize = tileSize;
    _tilemapShader.loadFromFile("zombietilemap.vert","zombietilemap.frag");
    _tilemapShader.setParameter("tileGraphics",_texture);
    _tilemapShader.setParameter("tileCount",sf::Vector2f(_texture.getSize()/_tileSize));
    _tilemapShader.setParameter("tileSize",_tileSize);

    LoadTileMap(tiles);
}

void ZombieTileMap::LoadTileMap(const std::vector< std::vector<char> >& unpaddedTiles)
{

    std::cout << unpaddedTiles.size() << "x" << unpaddedTiles[0].size() << std::endl;

    //Add padding
    std::vector< std::vector<char> > paddedTiles = std::vector< std::vector<char> >(unpaddedTiles.size() + 2, std::vector<char>(unpaddedTiles[0].size() + 2, empty));
    for (uint column=0;column<unpaddedTiles.size();column++)
    {
        for (uint row=0;row<unpaddedTiles[0].size();row++)
        {
            paddedTiles[column+1][row+1] = unpaddedTiles[column][row];
            std::cout << unpaddedTiles[column][row] << std::flush;
        }
        std::cout << std::endl;
    }


    _tileMapImage.create(paddedTiles.size(), paddedTiles[0].size(), sf::Color(16, 16, 17, 255));
    for (uint column=1;column<paddedTiles.size()-1;column++)
    {
        for (uint row=1;row<paddedTiles[1].size()-1;row++)
        {
            // 1
            //2 4
            // 8
            char index = 0;
            if (paddedTiles[column][row] == road) {
                if (paddedTiles[column][row-1] == road) index += 1;
                if (paddedTiles[column][row+1] == road) index += 8;
                if (paddedTiles[column-1][row] == road) index += 2;
                if (paddedTiles[column+1][row] == road) index += 4;
            } else {
                index = 16;
            }
            std::cout << (int)index << " ";
            sf::Color tileIndex(index, index, index, 255);
            _tileMapImage.setPixel(column, row, tileIndex);
        }
        std::cout << std::endl;
    }
    _tileMapTexture.loadFromImage(_tileMapImage);

    // Add the texture as a usable shader parameter
    _tilemapShader.setParameter("tilemap",_tileMapTexture);
    _tilemapShader.setParameter("tilemapSize",sf::Vector2f(_tileMapTexture.getSize()));

    _tileMapSprite.setTexture(_tileMapTexture);
    _tileMapSprite.setTextureRect(sf::IntRect(0,0,_tileMapTexture.getSize().x*_tileSize,_tileMapTexture.getSize().y*240));

}

void ZombieTileMap::Draw(sf::RenderTarget& target)
{
    sf::RenderStates _tilemapStates;
    _tilemapStates.shader=&_tilemapShader;
    target.draw(_tileMapSprite,_tilemapStates);
}

