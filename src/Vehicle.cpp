#include "Vehicle.h"
#include "Simulation.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <random>

namespace {
sf::Color randomColor() {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<int> dist(40, 230);
    return sf::Color(
        static_cast<std::uint8_t>(dist(rng)),
        static_cast<std::uint8_t>(dist(rng)),
        static_cast<std::uint8_t>(dist(rng)));
}
}  // namespace

Vehicle::Vehicle(VehicleType type, int laneId, float x, float y, sf::Color color)
    : type_(type), laneId_(laneId), x_(x), y_(y), color_(color) {
    if (type_ == VehicleType::Car) {
        width_ = 48.f;
        height_ = 26.f;
        speed_ = 90.f;
    } else {
        width_ = 28.f;
        height_ = 16.f;
        speed_ = 110.f;
    }
}

Vehicle::~Vehicle() {
    requestStop();
    joinThread();
}

void Vehicle::startThread(Simulation& simulation) {
    running_ = true;
    worker_ = std::thread([this, &simulation]() {
        using clock = std::chrono::steady_clock;
        auto last = clock::now();
        while (running_ && simulation.isRunning()) {
            const auto now = clock::now();
            float dt = std::chrono::duration<float>(now - last).count();
            last = now;
            if (dt > 0.05f) {
                dt = 0.05f;
            }
            {
                std::lock_guard<std::mutex> lock(simulation.worldMutex());
                if (!finished_) {
                    step(dt, simulation);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    });
}

void Vehicle::requestStop() {
    running_ = false;
}

void Vehicle::joinThread() {
    if (worker_.joinable()) {
        worker_.join();
    }
}

void Vehicle::onTrafficLightChanged(LightColor color, float stopLineX) {
    lightColor_.store(static_cast<int>(color));
    stopLineX_.store(stopLineX);
}

void Vehicle::step(float dt, Simulation& simulation) {
    const float vmax = maxSpeed();
    float accel = maxAccel() * (1.f - speed_ / vmax);

    if (Vehicle* leader = simulation.findLeader(*this)) {
        const float gap = leader->x() - (x_ + width_);
        const float safeGap = 18.f + speed_ * 0.55f;
        if (gap < safeGap) {
            const float urgency = std::min(1.f, (safeGap - gap) / std::max(8.f, safeGap));
            accel = -maxBrake() * (0.35f + 0.65f * urgency);
            if (gap < 8.f) {
                accel = -maxBrake();
                if (gap < 2.f) {
                    x_ = leader->x() - width_ - 2.f;
                    speed_ = std::min(speed_, leader->speed() * 0.5f);
                }
            }
        }
    }

    if (!passedStopLine_) {
        const auto color = static_cast<LightColor>(lightColor_.load());
        const float stopX = stopLineX_.load();
        const float dist = stopX - (x_ + width_);
        if (dist < -8.f) {
            passedStopLine_ = true;
        } else if (dist > 0.f) {
            const bool mustStop =
                (color == LightColor::Red) ||
                (color == LightColor::Yellow && dist > 70.f);
            if (mustStop) {
                const float stoppingDist = (speed_ * speed_) / (2.f * maxBrake() + 1.f);
                if (dist < stoppingDist + 40.f) {
                    float brake = (speed_ * speed_) / (2.f * std::max(12.f, dist));
                    brake = std::min(maxBrake() * 1.2f, std::max(brake, 40.f));
                    accel = -brake;
                    if (dist < 6.f) {
                        speed_ = 0.f;
                        accel = 0.f;
                        x_ = stopX - width_ - 2.f;
                    }
                }
            }
        }
    }

    speed_ += accel * dt;
    if (speed_ < 0.f) {
        speed_ = 0.f;
    }
    if (speed_ > vmax) {
        speed_ = vmax;
    }

    x_ += speed_ * dt;
    simulation.markFinishedIfOffscreen(*this);
}

void Vehicle::draw(sf::RenderTarget& target) const {
    sf::RectangleShape body({width_, height_});
    body.setOrigin({0.f, height_ * 0.5f});
    body.setPosition({x_, y_});
    body.setFillColor(color_);
    body.setOutlineColor(sf::Color(20, 20, 20));
    body.setOutlineThickness(1.f);
    target.draw(body);

    if (type_ == VehicleType::Car) {
        sf::RectangleShape cabin({width_ * 0.42f, height_ * 0.55f});
        cabin.setFillColor(sf::Color(30, 30, 30, 160));
        cabin.setPosition({x_ + width_ * 0.28f, y_ - height_ * 0.28f});
        target.draw(cabin);
    }
}

std::shared_ptr<Vehicle> Vehicle::createRandom(int laneId, float x, float y) {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::bernoulli_distribution isMoto(0.35);
    const sf::Color c = randomColor();
    if (isMoto(rng)) {
        return std::make_shared<Motorcycle>(laneId, x, y, c);
    }
    return std::make_shared<Car>(laneId, x, y, c);
}

Car::Car(int laneId, float x, float y, sf::Color color)
    : Vehicle(VehicleType::Car, laneId, x, y, color) {}

Motorcycle::Motorcycle(int laneId, float x, float y, sf::Color color)
    : Vehicle(VehicleType::Motorcycle, laneId, x, y, color) {}
