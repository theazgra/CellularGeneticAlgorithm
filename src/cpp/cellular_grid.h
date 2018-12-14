#pragma once
#include <stdio.h>
#include <random>
#include <vector>
#include <string>
#include "operators.h"
#include "stopwatch.h"
#include <thread>
#include <mutex>

class CellularGrid
{
private:
  uint rowCount;
  uint colCount;

  std::vector<Cell> currentPopulation;
  std::mutex currentPopulationMutex;

  NeighborhoodType neighborhoodMethod;
  PopulationMergeType mergeMethod;

  Cell &at(uint row, uint col);
  void synchronous_evolution_step();
  void multithreaded_evolution_step(const int threadCount);
  void openmp_evolution_step(const int threadCount);
  std::vector<Cell> get_neighborhood(const uint row, const uint col);
  void worker_job(int rowFrom, int rowTo, std::vector<Cell> &result);

public:
  CellularGrid(const uint dimension);
  CellularGrid(const uint width, const uint height);

  void initialize(const NeighborhoodType neighborhoodType, const PopulationMergeType mergeMethod);
  double get_score_of_generation() const;

  void dump_current_population_to_image(const std::string &folder, uint generation = 0, bool bw = false);
  void evolve(const int maxGenerationCount, const bool multiThreaded, const int threadCount = 12, const bool saveImages = false, const std::string &folder = "");

  ~CellularGrid();
};
#include "cellular_grid.cpp"