#include "Lane.h"

#include <sstream>

Lane::Lane(int id, float yCenter, float height, float roadLeft, float roadRight)
    : id_(id), yCenter_(yCenter), height_(height),
      roadLeft_(roadLeft), roadRight_(roadRight) {}

void Lane::draw(sf::RenderTarget& target) const {
    if (id_ == 0) {
        return;
    }
    const float y = yCenter_ - height_ * 0.5f;
    const float dash = 28.f;
    const float gap = 18.f;
    for (float x = roadLeft_; x < roadRight_; x += dash + gap) {
        sf::RectangleShape d({dash, 3.f});
        d.setFillColor(sf::Color(240, 240, 240));
        d.setPosition({x, y - 1.5f});
        target.draw(d);
    }
}

std::string Lane::name() const {
    std::ostringstream oss;
    oss << "Lane-" << id_;
    return oss.str();
}
