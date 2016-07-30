#pragma once

#include "math/Bounds.h"
#include "math/utils.h"

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

    ~BNode() {
        if (left != nullptr)
            delete left;
        if (right != nullptr)
            delete right;
    }

    void divide() {
        bool horizontal = h>w;
        float ratio = RandInRange(0.30,0.60);
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

    BNode * grow_room() {
        // Full room
        int _x = x + 1;
        int _y = y + 1;
        int _w = w - 1;
        int _h = h - 1;
        int diff_x = 0;
        int diff_y = 0;

        // Random decrease
        _w *= RandInRange(0, 1.5);
        diff_x = (w - _w) * RandInRange(0, 1);
        _h *= RandInRange(0, 1.5);
        diff_y = (h - _h) * RandInRange(0, 1);

        BNode * room = new BNode(_x + diff_x, _y + diff_y, _w, _h);
        return room;
    }
};

class World {
public:
    static std::vector< std::vector<char> > generate(int w, int h, int steps) {
        BNode * room_tree = new BNode(0, 0, w-1, h-1);

        room_tree->divide(steps);

        std::vector<BNode*> leafs;
        room_tree->leafs(leafs);

        BNode * room;
        std::vector< std::vector<char> > tiles = std::vector< std::vector<char> >(w, std::vector<char>(h, ' '));
        for (BNode* node: leafs) {
            for (int x = node->x; x < node->x + node->w + 1; x++) {
                tiles[x][node->y] = 'a';
                tiles[x][node->y+node->h] = 'a';
            }
            for (int y = node->y; y < node->y + node->h + 1; y++) {
                tiles[node->x][y] = 'a';
                tiles[node->x+node->w][y] = 'a';
            }
        }
        for (BNode* node: leafs) {
            room = node->grow_room();
            for (int x = room->x; x < room->w + room->x; x++) {
                for (int y = room->y; y < room->h + room->y; y++) {
                    if (x < tiles.size() && y < tiles[x].size()) {
                        tiles[x][y] = 'r';
                    }
                }
            }
        }

        delete room_tree;
        delete room;

        return tiles;
    }
};
