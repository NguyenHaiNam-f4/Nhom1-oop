#include "TrafficLight.h"

#include <algorithm>

TrafficLight::TrafficLight(float stopLineX, float poleX, float poleY)
    : stopLineX_(stopLineX), poleX_(poleX), poleY_(poleY),
      state_(std::make_unique<GreenState>()) {}

void TrafficLight::update(float dt) {
    elapsed_ += dt;
    if (state_) {
        state_->update(*this, dt);
    }
}

void TrafficLight::changeTo(std::unique_ptr<TrafficLightState> next) {
    state_ = std::move(next);
    elapsed_ = 0.f;
    notify();
}

LightColor TrafficLight::color() const {
    return state_ ? state_->color() : LightColor::Red;
}

void TrafficLight::attach(const std::shared_ptr<IObserver>& observer) {
    std::lock_guard<std::mutex> lock(observerMutex_);
    observers_.push_back(observer);
}

void TrafficLight::detach(const std::shared_ptr<IObserver>& observer) {
    std::lock_guard<std::mutex> lock(observerMutex_);
    observers_.erase(
        std::remove_if(observers_.begin(), observers_.end(),
                       [&](const std::weak_ptr<IObserver>& w) {
                           auto s = w.lock();
                           return !s || s == observer;
                       }),
        observers_.end());
}

void TrafficLight::notify() {
    std::vector<std::shared_ptr<IObserver>> alive;
    {
        std::lock_guard<std::mutex> lock(observerMutex_);
        for (auto it = observers_.begin(); it != observers_.end();) {
            if (auto s = it->lock()) {
                alive.push_back(s);
                ++it;
            } else {
                it = observers_.erase(it);
            }
        }
    }
    const LightColor c = color();
    for (auto& obs : alive) {
        obs->onTrafficLightChanged(c, stopLineX_);
    }
}

void TrafficLight::draw(sf::RenderTarget& target) const {
    sf::RectangleShape pole({6.f, 78.f});
    pole.setFillColor(sf::Color(40, 40, 40));
    pole.setPosition({poleX_ + 12.f, poleY_});
    target.draw(pole);

    sf::RectangleShape housing({30.f, 78.f});
    housing.setFillColor(sf::Color(25, 25, 25));
    housing.setOutlineColor(sf::Color(180, 180, 180));
    housing.setOutlineThickness(1.f);
    housing.setPosition({poleX_, poleY_ - 78.f});
    target.draw(housing);

    const LightColor c = color();
    const sf::Color dim(50, 50, 50);
    auto lamp = [&](float oy, sf::Color onColor, bool on) {
        sf::CircleShape circle(9.f);
        circle.setPosition({poleX_ + 6.f, poleY_ - 72.f + oy});
        circle.setFillColor(on ? onColor : dim);
        target.draw(circle);
    };
    lamp(0.f, sf::Color(220, 40, 40), c == LightColor::Red);
    lamp(24.f, sf::Color(230, 200, 40), c == LightColor::Yellow);
    lamp(48.f, sf::Color(40, 200, 70), c == LightColor::Green);

    sf::RectangleShape stopLine({6.f, 174.f});
    stopLine.setFillColor(sf::Color(245, 245, 245));
    stopLine.setPosition({stopLineX_, poleY_});
    target.draw(stopLine);
}
