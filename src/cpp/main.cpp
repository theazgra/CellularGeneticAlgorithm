#include "cellular_grid.h"

int main(int, char **)
{
    CellularGrid cg(200);
    cg.initialize(NeighborhoodType::C9, PopulationMergeType::ReplaceWorstInNeighborhood);

    cg.evolve(100, true, 12, false, "bw");
    return 0;
}
