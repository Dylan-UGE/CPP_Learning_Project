#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <memory>

using namespace std::string_literals;

TowerSimulation::TowerSimulation(int argc, char** argv) :
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) },
    context_initializer { ContextInitializer { argc, argv } }
{
    create_keystrokes();

    GL::move_queue.emplace(&aircraft_manager);
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });

    GL::keystrokes.emplace('c',
                           [this]() { aircraft_factory.create_random_aircraft(&aircraft_manager, airport); });

    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });

    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });

    GL::keystrokes.emplace('u', []() { GL::ticks_per_sec++; });
    GL::keystrokes.emplace('i', []() { GL::ticks_per_sec = std::max(1u, GL::ticks_per_sec - 1); });
    GL::keystrokes.emplace('p', []()
                           { GL::ticks_per_sec = (GL::ticks_per_sec == 0) ? DEFAULT_TICKS_PER_SEC : 0; });

    for (auto l = '0'; l < '8'; l++)
    {
        auto airline = aircraft_factory.get_airline(l - '0');
        GL::keystrokes.emplace(l,
                               [this, airline]() {
                                   std::cout << airline << " : "
                                             << aircraft_manager.count_planes_of_airline(airline)
                                             << std::endl;
                               });
    }

    GL::keystrokes.emplace(
        'm',
        [this]() { std::cout << "Plane crash : " << aircraft_manager.get_crash_counter() << std::endl; });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [first, second] : GL::keystrokes)
    {
        std::cout << first << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { one_lane_airport, &aircraft_manager, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();

    GL::loop();
}
