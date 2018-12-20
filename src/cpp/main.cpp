#include "cellular_grid.h"

int main(int, char **)
{
    const uint MaxIterationCount = 1000;
    const bool Parallel = true;
    const bool saveImages = false;
    const uint ThreadCount = 12;

    CellularGrid cg(500);
    cg.initialize(NeighborhoodType::L5, PopulationMergeType::ReplaceAll, InitializationType::RandomWithDiscrimination);
    cg.evolve(MaxIterationCount, Parallel, ThreadCount, saveImages, "bw");

    return 0;
}
