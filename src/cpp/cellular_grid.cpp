#include "cellular_grid.h"
#include "image.cpp"

Cell &CellularGrid::at(uint row, uint col)
{
    //std::lock_guard<std::mutex> lock(currentPopulationMutex);
    return currentPopulation[(row * colCount) + col];
}

CellularGrid::CellularGrid(const uint dimension)
{
    colCount = dimension;
    rowCount = dimension;
}
CellularGrid::CellularGrid(const uint width, const uint height)
{
    rowCount = height;
    colCount = width;
}
CellularGrid::~CellularGrid()
{
    currentPopulation.clear();
    currentPopulation.shrink_to_fit();
}

void CellularGrid::initialize(const NeighborhoodType neighborhoodType, const PopulationMergeType mergeMethod)
{
    this->neighborhoodMethod = neighborhoodType;
    this->mergeMethod = mergeMethod;

    currentPopulation = std::vector<Cell>();
    currentPopulation.reserve(rowCount * colCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255); // Both ranges are inclusive.

    uchar r, g, b;
    int index = 0;
    uchar discrimination;
    for (uint row = 0; row < rowCount; row++)
    {
        for (uint col = 0; col < colCount; col++)
        {

            discrimination = (uchar)((row * col) % UCHAR_MAX_AS_INT);
            r = (uchar)dis(gen);
            g = (uchar)dis(gen);
            b = (uchar)dis(gen);

            r = (r > discrimination) ? (uchar)(r - discrimination) : r;
            g = (g > discrimination) ? (uchar)(g - discrimination) : g;
            b = (b > discrimination) ? (uchar)(b - discrimination) : b;

            currentPopulation.push_back(Cell(Point(col, row), r, g, b));
        }
    }
}

double CellularGrid::get_score_of_generation() const
{
    double sum = 0.0;

    for (size_t i = 0; i < currentPopulation.size(); i++)
    {
        assert(currentPopulation[i].get_fitness() <= MAX_FITNESS_VALUE);
        sum += currentPopulation[i].get_fitness();
    }
    double result = (sum / (double)currentPopulation.size()) / MAX_FITNESS_VALUE;
    return result;
}

void CellularGrid::dump_current_population_to_image(const std::string &folder, uint generation)
{
    cimg_library::CImg<uchar> gridImage = create_color_image(colCount, rowCount);

    for (uint row = 0; row < rowCount; row++)
    {
        for (uint col = 0; col < colCount; col++)
        {
            Cell cell = at(row, col);
            set_pixel(gridImage, row, col, RgbPixel(cell.R, cell.G, cell.B));
        }
    }

    std::string filename = folder + "/generation_" + std::to_string(generation) + ".bmp";
    gridImage.save_bmp(filename.c_str());
}

void CellularGrid::evolve(const int maxGenerationCount, const bool multiThreaded, const int threadCount, const bool saveImages, const std::string &folder)
{
    double generationScore = get_score_of_generation();
    printf("Chosen neighborhood: %s\nChosen merge method: %s\n", std::to_string(neighborhoodMethod).c_str(), std::to_string(mergeMethod).c_str());
    printf("Initial generation score: %f\n", generationScore);

    StopwatchData s;
    double time;
    for (int generation = 1; generation <= maxGenerationCount; generation++)
    {
        start_stopwatch(s);
        if (multiThreaded)
            multithreaded_evolution_step(threadCount);
        else
            synchronous_evolution_step();
        stop_stopwatch(s);

        time = elapsed_milliseconds(s);
        generationScore = get_score_of_generation();

        printf("Completed generation %i; Score: %f; Iteration time %f ms\n", generation, generationScore, time);

        if (saveImages)
            dump_current_population_to_image(folder, generation);

        if (generationScore == 1.0)
        {
            printf("Target objective was reached.\n");
            return;
        }
    }
}

void CellularGrid::synchronous_evolution_step()
{
    std::vector<Cell> newPopulation;
    newPopulation.resize(rowCount * colCount);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis012(0, 2); // Both ranges are inclusive.
    std::uniform_int_distribution<int> dis01(0, 1);  // Both ranges are inclusive.
    int random;
    for (uint row = 0; row < rowCount; row++)
    {
        for (uint col = 0; col < colCount; col++)
        {
            std::vector<Cell> neighborhood = get_neighborhood(row, col);
            std::pair<Cell, Cell> parents = select_parents(neighborhood);
            Cell offspring = reproduction(col, row, parents, dis012(gen));

            if (mergeMethod == ReplaceWorstInNeighborhood)
            {
                offspring.cellToReplaceLocation = get_worst_cell(neighborhood).cellLocation;
            }
            else if (mergeMethod == ReplaceOneParent)
            {
                random = dis01(gen);
                offspring.cellToReplaceLocation = (random == 0) ? parents.first.cellLocation : parents.second.cellLocation;
            }
            newPopulation[(row * colCount) + col] = offspring;
        }
    }
    currentPopulation = replace(rowCount, colCount, currentPopulation, newPopulation, mergeMethod);
}

std::vector<Cell> CellularGrid::get_neighborhood(const uint row, const uint col)
{
    std::vector<Cell> neighborhood;
    switch (neighborhoodMethod)
    {
    case L5:
    {
        neighborhood.reserve(5);
        //        std::lock_guard<std::mutex> lock(currentPopulationMutex);

        neighborhood.push_back(at(row, col));

        neighborhood.push_back(at(row, mod(col - 1, colCount))); // Left
        neighborhood.push_back(at(mod(row - 1, rowCount), col)); // Top
        neighborhood.push_back(at(row, mod(col + 1, colCount))); // Right
        neighborhood.push_back(at(mod(row + 1, rowCount), col)); // Bottom

        return neighborhood;
    }
    break;
    case L9:
    {
        neighborhood.reserve(9);

        //      std::lock_guard<std::mutex> lock(currentPopulationMutex);

        neighborhood.push_back(at(row, col));
        neighborhood.push_back(at(row, mod(col - 1, colCount))); // Left
        neighborhood.push_back(at(row, mod(col - 2, colCount))); // Left 2
        neighborhood.push_back(at(mod(row - 1, rowCount), col)); // Top
        neighborhood.push_back(at(mod(row - 2, rowCount), col)); // Top 2
        neighborhood.push_back(at(row, mod(col + 1, colCount))); // Right
        neighborhood.push_back(at(row, mod(col + 2, colCount))); // Right 2
        neighborhood.push_back(at(mod(row + 1, rowCount), col)); // Bottom
        neighborhood.push_back(at(mod(row + 2, rowCount), col)); // Bottom 2

        return neighborhood;
    }
    break;
    case C9:
    {
        neighborhood.reserve(9);
        //    std::lock_guard<std::mutex> lock(currentPopulationMutex);
        for (int nRow = row - 1; nRow <= row + 1; nRow++)
        {
            for (int nCol = col - 1; nCol <= col + 1; nCol++)
            {
                neighborhood.push_back(at(mod(nRow, rowCount), mod(nCol, colCount)));
            }
        }
        return neighborhood;
    }
    break;
    case C13:
    {
        neighborhood.reserve(13);

        //  std::lock_guard<std::mutex> lock(currentPopulationMutex);
        for (int nRow = row - 1; nRow <= row + 1; nRow++)
        {
            for (int nCol = col - 1; nCol <= col + 1; nCol++)
            {
                neighborhood.push_back(at(mod(nRow, rowCount), mod(nCol, colCount)));
            }
        }
        neighborhood.push_back(at(row, mod(col - 2, colCount))); // Left 2
        neighborhood.push_back(at(mod(row - 2, rowCount), col)); // Top 2
        neighborhood.push_back(at(row, mod(col + 2, colCount))); // Right 2
        neighborhood.push_back(at(mod(row + 2, rowCount), col)); // Bottom 2

        return neighborhood;
    }
    break;
    default:
    {
        assert(false && "Wrong method");
    }
    }
}

void CellularGrid::worker_job(int rowFrom, int rowTo, std::vector<Cell> &result)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis012(0, 2); // Both ranges are inclusive.
    std::uniform_int_distribution<int> dis01(0, 1);  // Both ranges are inclusive.

    Cell offspring;
    std::vector<Cell> neighborhood;
    std::pair<Cell, Cell> parents;

    //  std::vector<Cell> result;

    result.reserve((rowTo - rowFrom) * colCount);
    int index = 0;
    int random;

    for (uint row = rowFrom; row < rowTo; row++)
    {
        for (uint col = 0; col < colCount; col++)
        {
            neighborhood = get_neighborhood(row, col);
            parents = select_parents(neighborhood);
            offspring = reproduction(col, row, parents, dis012(gen));

            if (mergeMethod == ReplaceWorstInNeighborhood)
            {
                offspring.cellToReplaceLocation = get_worst_cell(neighborhood).cellLocation;
            }
            else if (mergeMethod == ReplaceOneParent)
            {
                random = dis01(gen);
                offspring.cellToReplaceLocation = (random == 0) ? parents.first.cellLocation : parents.second.cellLocation;
            }

            result.push_back(offspring);
        }
    }
}

void CellularGrid::multithreaded_evolution_step(const int threadCount)
{
    std::vector<std::vector<Cell>> workersResults;
    workersResults.reserve(threadCount);
    {
        std::vector<std::thread> workers;

        workers.reserve(threadCount);

        int workerRowCount = rowCount / threadCount;

        int workerRowFrom, workerRowTo;
        for (int workerId = 0; workerId < threadCount; workerId++)
        {
            workerRowFrom = workerId * workerRowCount;
            workerRowTo = (workerId == threadCount - 1) ? rowCount : workerRowFrom + workerRowCount;

            workersResults.push_back(std::vector<Cell>());

            workers[workerId] = std::thread(&CellularGrid::worker_job, this, workerRowFrom, workerRowTo, std::ref(workersResults[workerId]));
        }

        std::vector<Cell> newPopulation;
        newPopulation.reserve(rowCount * colCount);

        int offset = 0;
        for (int workerId = 0; workerId < threadCount; workerId++)
        {
            workers[workerId].join();
            //printf("Thread %i reportedly completed.\n", workerId);
            newPopulation.insert((newPopulation.begin() + offset), workersResults[workerId].begin(), workersResults[workerId].end());
            offset += workersResults[workerId].size();
        }

        //std::lock_guard<std::mutex> lock(currentPopulationMutex);
        currentPopulation = replace(rowCount, colCount, currentPopulation, newPopulation, mergeMethod);
    }
}
