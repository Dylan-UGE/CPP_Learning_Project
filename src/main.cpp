#include "tower_sim.hpp"

#include <cstdlib>

int main(int argc, char** argv)
{
    std::srand(std::time(NULL));

    TowerSimulation simulation { argc, argv };
    simulation.launch();

    return 0;
}