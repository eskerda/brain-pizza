#pragma once

#include "math/Bounds.h"

const float w_ratio = 0.45;
const float h_ratio = 0.45;

struct BNode {
    BNode * left;
    BNode * right;
    int x, y, w, h;

    BNode(int _x, int _y, int _w, int _h)
        : left(nullptr)
        , right(nullptr)
        , x(_x)
        , y(_y)
        , w(_w)
        , h(_h)
    {}

    void divide() {
        bool horizontal = RandBool();
        float ratio = 0.45;
        if (horizontal) {
            int _h = h*ratio;
            left = new BNode(x, y, w, _h);
            right = new BNode(x, y+_h, w, h-_h);
        } else {
            int _w = w*ratio;
            left = new BNode(x, y, _w, h);
            right = new BNode(x+_w, y, w-_w, h);
        }
    }

    void divide(int steps) {
        if (!steps)
            return;
        divide();
        left->divide(steps-1);
        right->divide(steps-1);
    }

    void leafs(std::vector<BNode*> &stack) {
        if (!left && !right) {
            stack.push_back(this);
        } else {
            left->leafs(stack);
            right->leafs(stack);
        }
    }
};

class World {
public:
    static std::vector< std::vector<char> > generate(int w, int h, int steps) {
        BNode * room_tree = new BNode(0, 0, w-1, h-1);
        std::cout << "Room tree created" << std::endl;
        std::cout << "Dividing shit for " << steps << std::endl;
        room_tree->divide(steps);
        std::cout << "Divided" << std::endl;


        std::vector<BNode*> leafs;
        room_tree->leafs(leafs);

        /** TODO **/
        // for (BNode* room: rooms) {
        //     room
        // }

        std::vector< std::vector<char> > tiles = std::vector< std::vector<char> >(w, std::vector<char>(h, ' '));
        for (BNode* node: leafs) {
            for (int x = node->x; x < node->x + node->w + 1; x++) {
                tiles[node->y][x] = 'a';
                tiles[node->y+node->h][x] = 'a';
            }
            for (int y = node->y; y < node->y + node->h + 1; y++) {
                tiles[y][node->x] = 'a';
                tiles[y][node->x+node->w] = 'a';
            }
        }
        return tiles;
    }
};
