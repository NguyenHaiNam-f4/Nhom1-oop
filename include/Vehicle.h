#pragma once

#include "IObserver.h"

#include <SFML/Graphics.hpp>

#include <atomic>
#include <memory>
#include <string>
#include <thread>

class Simulation;

enum class VehicleType {
    Car,
    Motorcycle
};

class Vehicle : public IObserver, public std::enable_shared_from_this<Vehicle> {
public:
    Vehicle(VehicleType type, int laneId, float x, float y, sf::Color color);
    virtual ~Vehicle();

    Vehicle(const Vehicle&) = delete;
    Vehicle& operator=(const Vehicle&) = delete;

    void startThread(Simulation& simulation);
    void requestStop();
    void joinThread();

    void step(float dt, Simulation& simulation);
    void draw(sf::RenderTarget& target) const;

    void onTrafficLightChanged(LightColor color, float stopLineX) override;

    float x() const { return x_; }
    float y() const { return y_; }
    float width() const { return width_; }
    float height() const { return height_; }
    float speed() const { return speed_; }
    int laneId() const { return laneId_; }
    bool finished() const { return finished_; }
    void setFinished() { finished_ = true; running_ = false; }
    VehicleType type() const { return type_; }

    static std::shared_ptr<Vehicle> createRandom(int laneId, float x, float y);

protected:
    virtual float maxSpeed() const = 0;
    virtual float maxAccel() const = 0;
    virtual float maxBrake() const = 0;

    VehicleType type_;
    int laneId_;
    float x_;
    float y_;
    float width_;
    float height_;
    float speed_;
    sf::Color color_;

    std::atomic<int> lightColor_{static_cast<int>(LightColor::Green)};
    std::atomic<float> stopLineX_{0.f};
    bool passedStopLine_ = false;
    bool finished_ = false;

    std::atomic<bool> running_{false};
    std::thread worker_;
};

class Car : public Vehicle {
public:
    Car(int laneId, float x, float y, sf::Color color);
    float maxSpeed() const override { return 170.f; }
    float maxAccel() const override { return 85.f; }
    float maxBrake() const override { return 220.f; }
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(int laneId, float x, float y, sf::Color color);
    float maxSpeed() const override { return 210.f; }
    float maxAccel() const override { return 110.f; }
    float maxBrake() const override { return 260.f; }
};
