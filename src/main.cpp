#include "templateGeometry.hpp"
#include "tower_sim.hpp"

#include <cstdlib>

int main(int argc, char** argv)
{
    std::srand(std::time(NULL));

    //test_generic_points();
    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}