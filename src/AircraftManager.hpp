#pragma once

#include "aircraft.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class AircraftManager : public GL::DynamicObject
{
private:
    std::unordered_set<std::unique_ptr<Aircraft>> aircrafts;

public:
    void move() override
    {
        for (auto it = aircrafts.begin(); it != aircrafts.end();)
        {
            if ((*it)->can_be_del())
            {
                it = aircrafts.erase(it);
            }
            else
            {
                (*it)->move();
                it++;
            }
        }
    }

    void add(std::unique_ptr<Aircraft> aircraft) { aircrafts.emplace(std::move(aircraft)); }
};