#include "cell.h"
#include <vector>
#include <random>

Cell reproduction(const Cell &a, const Cell &b)
{
    Cell offspring = {};

    offspring.R = (uchar)(((int)a.R + (int)a.R) / 2);
    offspring.G = (uchar)(((int)a.G + (int)a.G) / 2);
    offspring.B = (uchar)(((int)a.B + (int)a.B) / 2);

    return offspring;
}

std::pair<Cell, Cell> select_parents(const std::vector<Cell> &neighborhood)
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());

    std::vector<float> weights;
    weights.reserve(neighborhood.size());

    for (size_t i = 0; i < neighborhood.size(); i++)
    {
        weights.push_back(neighborhood[i].get_selection_probability());
    }

    std::discrete_distribution<int> discreteDistribution = std::discrete_distribution<int>(std::begin(weights), std::end(weights));
    int indexA = discreteDistribution(randomGenerator);
    int indexB = discreteDistribution(randomGenerator);

    auto result = std::make_pair(neighborhood[indexA], neighborhood[indexB]);
    return result;
}