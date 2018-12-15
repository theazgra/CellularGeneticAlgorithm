#include "cellular_grid.h"

int main(int, char **)
{
    CellularGrid cg(300);
    cg.initialize(NeighborhoodType::L5, PopulationMergeType::ReplaceAll, InitializationType::FitCorner);
    //cg.dump_current_population_to_image("bw", 0, true);
    cg.evolve(1000, true, 12, true, "bw");

    return 0;
}
