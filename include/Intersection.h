#pragma once

#include "RoadComponent.h"
#include "TrafficLight.h"

#include <memory>
#include <string>

class Intersection : public RoadComponent {
public:
    Intersection(float left, float right, float top, float bottom,
                 std::shared_ptr<TrafficLight> light);

    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;
    std::string name() const override { return "Intersection"; }

    std::shared_ptr<TrafficLight> light() const { return light_; }

private:
    float left_;
    float right_;
    float top_;
    float bottom_;
    std::shared_ptr<TrafficLight> light_;
};
