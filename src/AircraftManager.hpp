#pragma once

#include "aircraft.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    void move() override
    {
        aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                       [](std::unique_ptr<Aircraft>& ref) { return !ref->move(); }),
                        aircrafts.end());
    }

    void add(std::unique_ptr<Aircraft> aircraft) { aircrafts.emplace_back(std::move(aircraft)); }
};