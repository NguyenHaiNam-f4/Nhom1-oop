#pragma once

#include <memory>
#include <vector>
#include <algorithm>

// Mau Observer: Observer nhan thong bao khi Subject (den giao thong) doi trang thai.
enum class LightColor {
    Green,
    Yellow,
    Red
};

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onTrafficLightChanged(LightColor color, float stopLineX) = 0;
};

class ISubject {
public:
    virtual ~ISubject() = default;
    virtual void attach(const std::shared_ptr<IObserver>& observer) = 0;
    virtual void detach(const std::shared_ptr<IObserver>& observer) = 0;
    virtual void notify() = 0;
};
