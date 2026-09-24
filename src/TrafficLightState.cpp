#include "TrafficLightState.h"
#include "TrafficLight.h"

void GreenState::update(TrafficLight& light, float dt) {
    if (light.elapsed() + dt >= TrafficLight::kGreenDuration) {
        light.changeTo(std::make_unique<YellowState>());
    }
}

void YellowState::update(TrafficLight& light, float dt) {
    if (light.elapsed() + dt >= TrafficLight::kYellowDuration) {
        light.changeTo(std::make_unique<RedState>());
    }
}

void RedState::update(TrafficLight& light, float dt) {
    if (light.elapsed() + dt >= TrafficLight::kRedDuration) {
        light.changeTo(std::make_unique<GreenState>());
    }
}
