#include "Intersection.h"

Intersection::Intersection(float left, float right, float top, float bottom,
                           std::shared_ptr<TrafficLight> light)
    : left_(left), right_(right), top_(top), bottom_(bottom), light_(std::move(light)) {}

void Intersection::update(float dt) {
    if (light_) {
        light_->update(dt);
    }
}

void Intersection::draw(sf::RenderTarget& target) const {
    sf::RectangleShape box({right_ - left_, bottom_ - top_});
    box.setPosition({left_, top_});
    box.setFillColor(sf::Color(70, 70, 74, 80));
    target.draw(box);

    if (light_) {
        light_->draw(target);
    }
}
