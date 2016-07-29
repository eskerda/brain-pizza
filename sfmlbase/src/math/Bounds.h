#pragma once

#include "constants.h"

#include <math.h>
#include <iosfwd>
#include <limits>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "math/Vector2D.h"
#include "math/utils.h"

struct Bounds : public sf::Rect<float>
{
    explicit Bounds() : sf::Rect<float>(0,0,0,0) { }
    explicit Bounds(const Vector2D& v) : sf::Rect<float>(0,0,v.x,v.y) { }
    explicit Bounds(float x, float y, float w, float h) : sf::Rect<float>(x,y,w,h) { }

    //Expands arround the center by an amount
    void Expand(float x, float y) {
        width += x;
        left -= x/2;
        height += y;
        top -= y/2;
    }

    void Expand(const Vector2D& v) { Expand(v.x, v.y); }

    //Expands arround the center by a factor
    Bounds operator*(float f) {
        Vector2D center = Center();
        Bounds ret = *this;
        ret.width *= f;
        ret.height *= f;
        ret.SetCenter(center);
        return ret;
    }

    Vector2D Center() const {
		return Vector2D(left + width/2, top + height/2);
	}

    void SetCenter(float x, float y) {
        left = x - width/2;
        top = y - height/2;
    }

    void SetCenter(const Vector2D& center) {
        SetCenter(center.x, center.y);
    }

	void Draw(sf::RenderTarget &rt, sf::Color color = sf::Color::Red, sf::Color fillColor = sf::Color::Transparent) {
		sf::RectangleShape rs;

		rs.setSize(Vector2D(width,height));
		rs.setPosition(Vector2D(left,top));

		rs.setFillColor(fillColor);
		rs.setOutlineColor(color);
		rs.setOutlineThickness(1);

		rt.draw(rs);
	}

    float Top() const {
        return top;
    }

    float Bottom() const {
        return top + height;
    }

    float Left() const {
        return left;
    }

    float Right() const {
        return left + width;
    }

	bool IsInside(Vector2D point) const {
        if (point.x < left) return false;
        if (point.x >= left+width) return false;
        if (point.y < top) return false;
        if (point.y >= top+height) return false;
		return true;
    }

    //TODO: Copiar de VIG
    //void ExpandToInclude(Vector2D point);


};

inline std::ostream& operator<<(std::ostream& os, const Bounds& rhs)
{
    os << " " << rhs.left << " " << rhs.top << " " << rhs.width << " " << rhs.height;
    return os;
}

inline Vector2D RandInBounds(const Bounds& b, float padding = 0.0f)
{
    return Vector2D(RandInRange(b.left + padding, b.width - padding), RandInRange(b.top + padding, b.height - padding));
}
