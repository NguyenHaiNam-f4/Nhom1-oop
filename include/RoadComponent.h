#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Mau Composite: nut (RoadNetwork) va la (Lane, Intersection) cung giao dien.
class RoadComponent {
public:
    virtual ~RoadComponent() = default;

    virtual void add(std::shared_ptr<RoadComponent> /*child*/) {
        throw std::logic_error("Khong the add() vao nut la.");
    }

    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderTarget& target) const = 0;
    virtual std::string name() const = 0;
};
