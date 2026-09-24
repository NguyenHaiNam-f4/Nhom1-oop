#include "RoadNetwork.h"

RoadNetwork::RoadNetwork(float left, float top, float width, float height)
    : left_(left), top_(top), width_(width), height_(height) {}

void RoadNetwork::add(std::shared_ptr<RoadComponent> child) {
    if (auto lane = std::dynamic_pointer_cast<Lane>(child)) {
        lanes_.push_back(lane);
    }
    if (auto inter = std::dynamic_pointer_cast<Intersection>(child)) {
        intersection_ = inter;
    }
    children_.push_back(std::move(child));
}

void RoadNetwork::update(float dt) {
    for (auto& child : children_) {
        child->update(dt);
    }
}

void RoadNetwork::draw(sf::RenderTarget& target) const {
    sf::RectangleShape curbTop({width_, 10.f});
    curbTop.setPosition({left_, top_ - 10.f});
    curbTop.setFillColor(sf::Color(90, 90, 90));
    target.draw(curbTop);

    sf::RectangleShape curbBot({width_, 10.f});
    curbBot.setPosition({left_, top_ + height_});
    curbBot.setFillColor(sf::Color(90, 90, 90));
    target.draw(curbBot);

    sf::RectangleShape asphalt({width_, height_});
    asphalt.setPosition({left_, top_});
    asphalt.setFillColor(sf::Color(48, 48, 52));
    target.draw(asphalt);

    sf::RectangleShape edge({width_, 3.f});
    edge.setFillColor(sf::Color(230, 230, 230));
    edge.setPosition({left_, top_});
    target.draw(edge);
    edge.setPosition({left_, top_ + height_ - 3.f});
    target.draw(edge);

    for (const auto& child : children_) {
        child->draw(target);
    }
}
