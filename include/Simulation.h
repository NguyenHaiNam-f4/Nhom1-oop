#pragma once

#include "RoadNetwork.h"
#include "Vehicle.h"

#include <SFML/Graphics.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <random>
#include <vector>

class Simulation {
public:
    static constexpr unsigned kWidth = 1280;
    static constexpr unsigned kHeight = 720;
    static constexpr int kLaneCount = 3;
    static constexpr float kLaneHeight = 58.f;

    Simulation();
    ~Simulation();

    void run();

    // Duoc goi tu luong cua tung xe (da khoa worldMutex_ trong Simulation::vehicleLoop).
    Vehicle* findLeader(const Vehicle& self) const;
    void markFinishedIfOffscreen(Vehicle& v);

    std::mutex& worldMutex() { return worldMutex_; }
    bool isRunning() const { return running_; }

private:
    void spawnIfPossible(float dt);
    void reapFinished();
    void drawHud(sf::RenderTarget& target) const;

    sf::RenderWindow window_;
    sf::Font font_;
    bool hasFont_ = false;

    std::shared_ptr<RoadNetwork> network_;
    std::shared_ptr<TrafficLight> light_;
    std::vector<std::shared_ptr<Vehicle>> vehicles_;

    std::mutex worldMutex_;
    std::atomic<bool> running_{true};

    std::mt19937 rng_{std::random_device{}()};
    float spawnTimer_ = 0.f;
};
