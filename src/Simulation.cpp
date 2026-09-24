#include "Simulation.h"

#include <algorithm>
#include <optional>
#include <sstream>

Simulation::Simulation()
    : window_(sf::VideoMode({kWidth, kHeight}),
              "Mo phong vi mo giao thong do thi da tac tu",
              sf::Style::Close | sf::Style::Titlebar) {
    window_.setFramerateLimit(60);

    if (font_.openFromFile("C:/Windows/Fonts/arial.ttf") ||
        font_.openFromFile("C:/Windows/Fonts/segoeui.ttf")) {
        hasFont_ = true;
    }

    const float roadHeight = kLaneCount * kLaneHeight;
    const float roadTop = (static_cast<float>(kHeight) - roadHeight) * 0.5f;
    const float roadLeft = 0.f;
    const float roadWidth = static_cast<float>(kWidth);

    network_ = std::make_shared<RoadNetwork>(roadLeft, roadTop, roadWidth, roadHeight);

    for (int i = 0; i < kLaneCount; ++i) {
        const float y = roadTop + (i + 0.5f) * kLaneHeight;
        network_->add(std::make_shared<Lane>(i, y, kLaneHeight, roadLeft, roadLeft + roadWidth));
    }

    const float stopLineX = 780.f;
    const float interLeft = 790.f;
    const float interRight = 920.f;
    light_ = std::make_shared<TrafficLight>(stopLineX, 930.f, roadTop);
    network_->add(std::make_shared<Intersection>(
        interLeft, interRight, roadTop, roadTop + roadHeight, light_));
}

Simulation::~Simulation() {
    running_ = false;
    for (auto& v : vehicles_) {
        v->requestStop();
    }
    for (auto& v : vehicles_) {
        v->joinThread();
    }
}

void Simulation::run() {
    sf::Clock clock;
    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window_.close();
                }
            }
        }

        const float dt = clock.restart().asSeconds();

        {
            std::lock_guard<std::mutex> lock(worldMutex_);
            network_->update(dt);
            spawnIfPossible(dt);
        }

        reapFinished();

        window_.clear(sf::Color(28, 36, 32));

        {
            std::lock_guard<std::mutex> lock(worldMutex_);
            network_->draw(window_);
            for (const auto& v : vehicles_) {
                v->draw(window_);
            }
            drawHud(window_);
        }

        window_.display();
    }

    running_ = false;
}

void Simulation::spawnIfPossible(float dt) {
    spawnTimer_ += dt;
    if (spawnTimer_ < 0.85f) {
        return;
    }
    spawnTimer_ = 0.f;

    if (!network_ || network_->lanes().empty()) {
        return;
    }

    std::uniform_int_distribution<int> laneDist(0, static_cast<int>(network_->lanes().size()) - 1);
    const int laneId = laneDist(rng_);
    const auto& lane = network_->lanes()[static_cast<size_t>(laneId)];
    const float spawnX = -60.f;

    for (const auto& v : vehicles_) {
        if (v->laneId() == laneId && v->x() < spawnX + 90.f) {
            return;
        }
    }

    auto vehicle = Vehicle::createRandom(laneId, spawnX, lane->yCenter());
    vehicles_.push_back(vehicle);
    if (light_) {
        light_->attach(vehicle);
        vehicle->onTrafficLightChanged(light_->color(), light_->stopLineX());
    }
    vehicle->startThread(*this);
}

void Simulation::reapFinished() {
    std::vector<std::shared_ptr<Vehicle>> dead;
    {
        std::lock_guard<std::mutex> lock(worldMutex_);
        for (auto it = vehicles_.begin(); it != vehicles_.end();) {
            if ((*it)->finished()) {
                dead.push_back(*it);
                if (light_) {
                    light_->detach(*it);
                }
                it = vehicles_.erase(it);
            } else {
                ++it;
            }
        }
    }
    for (auto& v : dead) {
        v->requestStop();
        v->joinThread();
    }
}

Vehicle* Simulation::findLeader(const Vehicle& self) const {
    Vehicle* leader = nullptr;
    float bestX = 1e9f;
    for (const auto& v : vehicles_) {
        if (v.get() == &self) {
            continue;
        }
        if (v->laneId() != self.laneId()) {
            continue;
        }
        if (v->x() > self.x() && v->x() < bestX) {
            bestX = v->x();
            leader = v.get();
        }
    }
    return leader;
}

void Simulation::markFinishedIfOffscreen(Vehicle& v) {
    if (v.x() > static_cast<float>(kWidth) + 80.f) {
        v.setFinished();
    }
}

void Simulation::drawHud(sf::RenderTarget& target) const {
    if (!hasFont_) {
        return;
    }
    sf::Text title(font_, "Mo phong vi mo giao thong do thi da tac tu", 20);
    title.setFillColor(sf::Color(230, 230, 230));
    title.setPosition({24.f, 18.f});
    target.draw(title);

    std::ostringstream oss;
    oss << "Xe: " << vehicles_.size()
        << "  |  Den: ";
    if (light_) {
        switch (light_->color()) {
            case LightColor::Green: oss << "XANH"; break;
            case LightColor::Yellow: oss << "VANG"; break;
            case LightColor::Red: oss << "DO"; break;
        }
    }
    oss << "  |  ESC/Close de thoat";

    sf::Text info(font_, oss.str(), 16);
    info.setFillColor(sf::Color(200, 200, 200));
    info.setPosition({24.f, 48.f});
    target.draw(info);

    sf::Text hint(font_,
                  "O to / xe may: hinh chu nhat.  Theo sat xe truoc, phanh khi den DO.",
                  14);
    hint.setFillColor(sf::Color(160, 160, 160));
    hint.setPosition({24.f, static_cast<float>(kHeight) - 40.f});
    target.draw(hint);
}
