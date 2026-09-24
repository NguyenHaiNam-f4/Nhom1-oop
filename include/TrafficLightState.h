#pragma once

#include "IObserver.h"

class TrafficLight;

// Mau State: moi lop con bieu dien mot trang thai den (Xanh / Vang / Do).
class TrafficLightState {
public:
    virtual ~TrafficLightState() = default;
    virtual void update(TrafficLight& light, float dt) = 0;
    virtual LightColor color() const = 0;
    virtual const char* name() const = 0;
};

class GreenState : public TrafficLightState {
public:
    void update(TrafficLight& light, float dt) override;
    LightColor color() const override { return LightColor::Green; }
    const char* name() const override { return "GREEN"; }
};

class YellowState : public TrafficLightState {
public:
    void update(TrafficLight& light, float dt) override;
    LightColor color() const override { return LightColor::Yellow; }
    const char* name() const override { return "YELLOW"; }
};

class RedState : public TrafficLightState {
public:
    void update(TrafficLight& light, float dt) override;
    LightColor color() const override { return LightColor::Red; }
    const char* name() const override { return "RED"; }
};
