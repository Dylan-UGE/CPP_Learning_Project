#pragma once

#include "aircraft.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

class AircraftManager : public GL::DynamicObject
{
private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;

public:
    void move() override
    {
        std::sort(aircrafts.begin(), aircrafts.end(),
                  [](std::unique_ptr<Aircraft>& ref1, std::unique_ptr<Aircraft>& ref2)
                  { return ref1->has_terminal() || ref1->fuel_quantity() < ref2->fuel_quantity(); });
        aircrafts.erase(std::remove_if(aircrafts.begin(), aircrafts.end(),
                                       [](std::unique_ptr<Aircraft>& ref) { return !ref->move(); }),
                        aircrafts.end());
    }

    void add(std::unique_ptr<Aircraft> aircraft) { aircrafts.emplace_back(std::move(aircraft)); }

    int count_planes_of_airline(std::string airline)
    {
        return std::count_if(aircrafts.begin(), aircrafts.end(),
                             [airline](std::unique_ptr<Aircraft>& ref)
                             { return ref->get_flight_num().substr(0, 2) == airline; });
    }

    int get_required_fuel()
    {
        return std::accumulate(aircrafts.begin(), aircrafts.end(), 0,
                               [](int accu, std::unique_ptr<Aircraft>& ref) {
                                   return accu + ((ref->is_low_on_fuel() || ref->is_at_airport())
                                                      ? (3000 - ref->fuel_quantity())
                                                      : 0);
                               });
    }
};