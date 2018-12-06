# Cellular evolutionary (genetic) algorithm

- [Wikipedia](https://en.wikipedia.org/wiki/Cellular_evolutionary_algorithm)
- [Python example](https://github.com/gyfis/pycea)
- [Interesting website](http://neo.lcc.uma.es/cEA-web/index.htm)
- [Genetic algorithm process diagram](http://neo.lcc.uma.es/cEA-web/introduction.htm)


### One point crossover

Doesn't introduce new information into the `new` population.
[1,1,0,1|0,0] --> [1,1,0,1|0,1]
[1,0,1,0|0,1] --> [1,0,1,0|0,0]

### Bit-flipping mutation

Does introduce new information into the `new` population.
[1,0,1,0|0|1] --> [1,0,1,0|1|1]

### Gaussian mutation

Gaussian mutation consists in adding a random value from a Gaussian distribution to each element of an individual’s vector to create a new offspring.
[1.3,0.4,1.8,0.2,0.0,1.0] --> [1.2,0.7,1.6,0.2,0.1,1.2]

### Intermediate recombination

Parents values are averages together.
[1.2,0.3,0.0,1.7,0.8,1.2]
[0.8,0.5,1.0,1.1,0.2,1.2] --> [1.0,0.4,0.5,1.4,0.5,1.2] 

## Pseudocode of simple C-EA

```
# petri_dish is a toroidal grid of out population.
# Toroidal grid is a grid which connects vertices at the ends.

procedure cea(petri_dish):
  for s = 1 to NUMBER_OF_STEPS:
    for x = 1 to WIDTH:
      for y = 1 to HEIGHT:
        neighbours   = GetNeighbours(petri_dish, petri_dish.at(x,y))
        selectedInds = SelectParents(neighboursList)
        offsprings   = ApplyReproductionOperators(selectedInds)
    petri_dish = Replace(petri_dish, aux_pop)
    EvaluatePopulation(petri_dish)
```

## Operators

Fitness value is associated based on how close is the invidual to the problem solution.
Three genetic operators:

1. Selection
  - Probability of being selected is based on invidual's fitness value.
  - Selection process guide the algorithm to better solutions.
  - Every invidual reside inside its local neighborhood, and can interact only with neighbors in this area.
    - Neighborhood is area of potentional mating partners.
  - **Linear Rank Selection**
    - Neighborhood is sorted (increasing order) according to the fitness values of inviduals.
    - Target sampling rate, TSR(x) = Min + (Max - Min) * rank(x)/(N-1)
      - rank(x) is the position of invidual in sorted neighborhood
      - 0 <= TSR(X),1<=Max<=2, Min + Max = 2, (sum of all TSR(x) == 1)
      - The TSR is the number of times an individual should be chosen as a parent for every N sampling operations.
  - **Roulette Wheel Selection**
    - The individuals are mapped to contiguous segments of a line, such that each individual's segment is equal in size to its fitness. 
    - A random number is generated and the individual whose segment spans the random number is selected.
    - Process is repeated until the mating population is filled.
2. Reproduction
  - Reproduction is the mechanism that allows us to obtain offsprings from one (asexual) or more (sexual) parents. Among reproduction operators the following stand out:
  - **Recombination**
    - One point crossover - doesn't introduce new information. Switch the bits after |
      - [1,1,0,1|0,0] --> [1,1,0,1|0,1]
      - [1,0,1,0|0,1] --> [1,0,1,0|0,0]
    - **Bit-flipping mutation**
      - [1,0,1,0|0|1] --> [1,0,1,0|1|1]
    - **Gaussian mutation**
      - Gaussian mutation consists in adding a random value from a Gaussian distribution to each element of an individual’s vector to create a new offspring.
      - [1.3,0.4,1.8,0.2,0.0,1.0] --> [1.2,0.7,1.6,0.2,0.1,1.2]
    - **Intermediate recombination**
      - Parents values are averages together.
      - [1.2,0.3,0.0,1.7,0.8,1.2]
      - [0.8,0.5,1.0,1.1,0.2,1.2] --> [1.0,0.4,0.5,1.4,0.5,1.2] 
3. Replacement
  - Replacement schemes are used by evolutionary algorithms to determine how the new individuals will be assimilated into the population.
  - **Replace worst**
    - The offspring replaces the worst invidual in the neighborhood.
  - **Replace most similar**
    - The offspring replaces the most similiar invidual in neighborhood, based on their chromosome similarity.
  - **Replace parent**
    - The offspring replaces one of its parent.
  - *Replace best, Replace random, Replace if better*

## Neighborhoos
  - [Types](http://neo.lcc.uma.es/cEA-web/neighborhood.htm)
  - [Influence of neighborhood size](http://neo.lcc.uma.es/cEA-web/ratio.htm)


## Synchronous and Asynchronous C-EA
In synchronous C-EA the whole population is replaced be the new offspring population at the end of each generation. The new generation is stored into temporary one, which later replaces the current generation.

In asynchronous C-EA the inviduals are replaced by the new offspring, when they are generated. The invidual can mate with the offspring of his neighbors.

  - Fixed line sweep (LS) - cells are updated line by line.
  - Fixed random sweep (FRS) - Random cells are chosen to create offsprings and then those random celss are used to place the new cells.
  - [More...](neo.lcc.uma.es/cEA-web/sync.htm)
