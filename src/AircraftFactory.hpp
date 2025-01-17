#pragma once

#include "AircraftManager.hpp"
#include "aircraft.hpp"
#include "airport.hpp"

#include <array>
#include <set>

class AircraftFactory
{
private:
    size_t NUM_AIRCRAFT_TYPES;
    AircraftType* aircraft_types[3];
    const std::array<std::string, 8> airlines = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

    std::set<std::string> _flight_numbers;

public:
    AircraftFactory()
    {
        aircraft_types[0] = new AircraftType { .02f, .05f, .02f, MediaPath { "l1011_48px.png" } };
        aircraft_types[1] = new AircraftType { .02f, .05f, .02f, MediaPath { "b707_jat.png" } };
        aircraft_types[2] = new AircraftType { .02f, .1f, .02f, MediaPath { "concorde_af.png" } };
    }

    void create_aircraft(AircraftManager* aircraft_manager, Airport* airport, const AircraftType& type)
    {
        assert(airport); // make sure the airport is initialized before creating aircraft
        assert(aircraft_manager);

        std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        while (!_flight_numbers.insert(flight_number).second)
        {
            flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
        }

        const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
        const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
        const Point3D direction = (-start).normalize();

        auto aircraft_ptr =
            std::make_unique<Aircraft>(type, flight_number, start, direction, airport->get_tower());
        aircraft_manager->add(std::move(aircraft_ptr));
    }

    void create_random_aircraft(AircraftManager* aircraft_manager, Airport* airport)
    {
        create_aircraft(aircraft_manager, airport, *(aircraft_types[rand() % 3]));
    }

    std::string get_airline(int index)
    {
        assert(index >= 0 && index < 8);
        return airlines[index];
    }
};