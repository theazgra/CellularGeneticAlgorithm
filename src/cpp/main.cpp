#include "cellular_grid.h"

int main(int, char **)
{
    CellularGrid cg(200);
    cg.initialize(NeighborhoodType::C13, PopulationMergeType::ReplaceWorstInNeighborhood);

    cg.evolve(1000, true, 12, false, "bw");
    return 0;
}
