#pragma once

#include "IObserver.h"
#include "TrafficLightState.h"

#include <SFML/Graphics.hpp>

#include <memory>
#include <mutex>
#include <vector>

// Subject trong mau Observer. Dong thoi giu State hien tai (State Pattern).
class TrafficLight : public ISubject {
public:
    TrafficLight(float stopLineX, float poleX, float poleY);

    void update(float dt);
    void draw(sf::RenderTarget& target) const;

    void changeTo(std::unique_ptr<TrafficLightState> next);
    void resetTimer() { elapsed_ = 0.f; }
    float elapsed() const { return elapsed_; }

    LightColor color() const;
    float stopLineX() const { return stopLineX_; }

    void attach(const std::shared_ptr<IObserver>& observer) override;
    void detach(const std::shared_ptr<IObserver>& observer) override;
    void notify() override;

    static constexpr float kGreenDuration = 5.0f;
    static constexpr float kYellowDuration = 1.6f;
    static constexpr float kRedDuration = 4.2f;

private:
    float stopLineX_;
    float poleX_;
    float poleY_;
    float elapsed_ = 0.f;

    std::unique_ptr<TrafficLightState> state_;
    std::vector<std::weak_ptr<IObserver>> observers_;
    mutable std::mutex observerMutex_;
};
