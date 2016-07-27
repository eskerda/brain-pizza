#include "ZombieTileMap.h"

const char road  = 'a';
const char house = 'r';
const char empty = ' ';

ZombieTileMap::ZombieTileMap(const std::vector< std::vector<char> >& tiles, const std::string& textureFile, int tileSize)
{
    _tilemapShader.loadFromFile("zombietilemap.vert","zombietilemap.frag");
    LoadTileMap(tiles);
    LoadTileSheet(textureFile, tileSize);
}

void ZombieTileMap::LoadTileMap(const std::vector< std::vector<char> >& unpaddedTiles)
{

    //Add padding
    std::vector< std::vector<char> > paddedTiles = std::vector< std::vector<char> >(unpaddedTiles.size() + 2, std::vector<char>(unpaddedTiles[0].size() + 2, empty));
    for (uint column=0;column<unpaddedTiles.size();column++)
    {
        for (uint row=0;row<unpaddedTiles[0].size();row++)
        {
            paddedTiles[column+1][row+1] = unpaddedTiles[column][row];
        }
    }

    _tileMapImage.create(paddedTiles.size(),paddedTiles[0].size());
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
    _tileMapSprite.setTextureRect(sf::IntRect(0,0,_tileMapTexture.getSize().x*240,_tileMapTexture.getSize().y*240));
    //_tileMapSprite.setOrigin(128*32,128*240);

}

void ZombieTileMap::LoadTileSheet(const std::string& textureFile, int tileSize) {
    _texture.loadFromFile("media/"+textureFile);
    _tileSize = tileSize;
    _tilemapShader.setParameter("tileGraphics",_texture);
    _tilemapShader.setParameter("tileCount",sf::Vector2f(_texture.getSize()/_tileSize));
    //_tilemapShader.setParameter("tileSize",_tileSize));
}

void ZombieTileMap::Draw(sf::RenderTarget& target)
{
    sf::RenderStates _tilemapStates;
    _tilemapStates.shader=&_tilemapShader;
    target.draw(_tileMapSprite,_tilemapStates);
}

