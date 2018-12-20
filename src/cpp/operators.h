#include "cell.h"
#include "enums.h"
#include <vector>
#include <random>
#include <omp.h>

inline int mod(const int x, const int mod)
{
    return ((x >= 0) ? (x % mod) : (x + mod));
}

Cell get_worst_cell(const std::vector<Cell> &neighborhood)
{
    Cell worst = Cell(Point(-1, -1));
    worst.R = 255;
    worst.G = 255;
    worst.B = 255;

    for (size_t i = 0; i < neighborhood.size(); i++)
    {
        if (neighborhood[i].get_fitness() <= worst.get_fitness())
        {
            worst = neighborhood[i];
        }
    }
    assert(worst.cellLocation.x != -1 && worst.cellLocation.y != -1);
    return worst;
}

std::vector<Cell> replace(int rowCount, int colCount, std::vector<Cell> &currentPopulation, std::vector<Cell> &newPopulation, PopulationMergeType method)
{
    switch (method)
    {
    case ReplaceAll:
    {
        return newPopulation;
    }
    case ReplaceWorstInNeighborhood:
    case ReplaceOneParent:
    {
#pragma omp parallel for
        for (int row = 0; row < rowCount; row++)
        {
            for (int col = 0; col < colCount; col++)
            {
                Cell offspring = newPopulation[(row * colCount) + col];
                Point toReplaceLocation = offspring.cellToReplaceLocation;
                offspring.cellLocation = toReplaceLocation;

#pragma omp critical
                {
                    currentPopulation[(toReplaceLocation.y * colCount) + toReplaceLocation.x] = offspring;
                }
            }
        }
        return currentPopulation;
    }
    default:
    {
        assert(false && "Wrong merge method.");
    }
    }
}

std::vector<Cell> replace_row(int row, int colCount, std::vector<Cell> &currentPopulation, std::vector<Cell> &newPopulation, PopulationMergeType method)
{
    switch (method)
    {
    case ReplaceAll:
    {
        for (int col = 0; col < colCount; col++)
        {
            int index = (row * colCount) + col;
            Cell offspring = newPopulation[index];
            currentPopulation[index] = offspring;
        }
        return currentPopulation;
    }
    case ReplaceWorstInNeighborhood:
    case ReplaceOneParent:
    {
        for (int col = 0; col < colCount; col++)
        {
            Cell offspring = newPopulation[(row * colCount) + col];
            Point replaceLocation = offspring.cellToReplaceLocation;

            offspring.cellLocation = replaceLocation;
            currentPopulation[(replaceLocation.y * colCount) + replaceLocation.x] = offspring;
        }

        return currentPopulation;
    }
    default:
    {
        assert(false && "Wrong merge method.");
    }
    }
}

inline uchar max(const uchar a, const uchar b)
{
    return (a > b) ? a : b;
}

Cell reproduction(int x, int y, std::pair<Cell, Cell> parents, int randomValue)
{
    Cell offspring(Point(x, y));
    switch (randomValue)
    {
    case 0:
    {
        offspring.R = max(parents.first.R, parents.second.R);
        offspring.G = max(parents.first.G, parents.second.G);
        offspring.B = max(parents.first.B, parents.second.B);

        return offspring;
    }
    case 1:
    {
        offspring.R = max(parents.first.B, parents.second.B);
        offspring.G = max(parents.first.R, parents.second.R);
        offspring.B = max(parents.first.G, parents.second.G);

        return offspring;
    }
    case 2:
    {
        offspring.R = max(parents.first.G, parents.second.G);
        offspring.G = max(parents.first.B, parents.second.B);
        offspring.B = max(parents.first.R, parents.second.R);

        return offspring;
    }
    default:
        assert(false && "Only random values allowed are 0, 1, and 2.");
    }
}

std::pair<Cell, Cell> select_parents(const std::vector<Cell> &neighborhood)
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());

    std::vector<float> weights;
    weights.reserve(neighborhood.size());

    for (size_t i = 0; i < neighborhood.size(); i++)
    {
        weights.push_back((neighborhood[i].get_fitness() / MAX_FITNESS_VALUE));
    }

    std::discrete_distribution<int> discreteDistribution = std::discrete_distribution<int>(std::begin(weights), std::end(weights));
    int indexA = discreteDistribution(randomGenerator);
    int indexB = discreteDistribution(randomGenerator);

    while (indexA == indexB)
    {
        indexB = discreteDistribution(randomGenerator);
    }

    auto result = std::make_pair(neighborhood[indexA], neighborhood[indexB]);
    return result;
}