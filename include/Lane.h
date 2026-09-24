#pragma once

#include "RoadComponent.h"

#include <string>

class Lane : public RoadComponent {
public:
    Lane(int id, float yCenter, float height, float roadLeft, float roadRight);

    void update(float /*dt*/) override {}
    void draw(sf::RenderTarget& target) const override;
    std::string name() const override;

    int id() const { return id_; }
    float yCenter() const { return yCenter_; }
    float height() const { return height_; }
    float spawnX() const { return roadLeft_ - 40.f; }

private:
    int id_;
    float yCenter_;
    float height_;
    float roadLeft_;
    float roadRight_;
};
