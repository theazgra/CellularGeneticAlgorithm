#include "cellular_grid.h"

int main(int, char **)
{
    CellularGrid cg(1000);
    cg.initialize(NeighborhoodType::L9, PopulationMergeType::ReplaceWorstInNeighborhood);

    cg.evolve(100, true, 12, true, "bw");
    return 0;
}
