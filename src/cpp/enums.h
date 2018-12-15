#pragma once
enum PopulationMergeType
{
    ReplaceAll,
    ReplaceWorstInNeighborhood,
    ReplaceOneParent
};

enum NeighborhoodType
{
    L5,
    L9,
    C9,
    C13
};

enum InitializationType
{
    RandomWithDiscrimination,
    FitBorders,
    FitCorner
};