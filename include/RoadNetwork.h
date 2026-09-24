#pragma once

#include "RoadComponent.h"
#include "Lane.h"
#include "Intersection.h"

#include <memory>
#include <string>
#include <vector>

class RoadNetwork : public RoadComponent {
public:
    RoadNetwork(float left, float top, float width, float height);

    void add(std::shared_ptr<RoadComponent> child) override;
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;
    std::string name() const override { return "RoadNetwork"; }

    const std::vector<std::shared_ptr<Lane>>& lanes() const { return lanes_; }
    std::shared_ptr<Intersection> intersection() const { return intersection_; }

    float left() const { return left_; }
    float top() const { return top_; }
    float width() const { return width_; }
    float height() const { return height_; }

private:
    float left_;
    float top_;
    float width_;
    float height_;
    std::vector<std::shared_ptr<RoadComponent>> children_;
    std::vector<std::shared_ptr<Lane>> lanes_;
    std::shared_ptr<Intersection> intersection_;
};
