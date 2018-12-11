using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using Troschuetz.Random.Generators;

namespace cellular_ga
{
    class Operators
    {
        static AbstractGenerator defaultGenerator = new MT19937Generator();

        public static Cell[] Replace(int rowCount, int colCount, Cell[] currentPopulation, Cell[] newPopulation, ReplaceType replaceType)
        {
            switch (replaceType)
            {
                case ReplaceType.ReplaceAll:
                    return newPopulation;
                case ReplaceType.ReplaceWorstInNeighbourhood:
                case ReplaceType.ReplaceOneParent:
                    {
                        for (int row = 0; row < rowCount; row++)
                        {
                            for (int col = 0; col < colCount; col++)
                            {
                                Cell offSpring = GetAtGrid(newPopulation, row, col, colCount);
                                Point toReplace = offSpring.ToReplace;

                                offSpring.Location = toReplace;
                                SetAtGrid(currentPopulation, toReplace.Y, toReplace.X, colCount, offSpring);
                            }
                        }
                        return currentPopulation;
                    }
                default:
                    throw new Exception("BAD REPLACE TYPE.");
            }
        }

        public static Cell[] ReplaceRow(int row, int colCount, Cell[] currentPopulation, Cell[] newPopulation, ReplaceType replaceType)
        {
            switch (replaceType)
            {
                case ReplaceType.ReplaceAll:
                    {
                        for (int col = 0; col < colCount; col++)
                        {
                            SetAtGrid(currentPopulation, row, col, colCount, GetAtGrid(newPopulation, row, col, colCount));
                        }
                        return currentPopulation;
                    }
                case ReplaceType.ReplaceWorstInNeighbourhood:
                case ReplaceType.ReplaceOneParent:
                    {
                        for (int col = 0; col < colCount; col++)
                        {
                            Cell offSpring = GetAtGrid(newPopulation, row, col, colCount);
                            Point toReplace = offSpring.ToReplace;

                            offSpring.Location = toReplace;
                            SetAtGrid(currentPopulation, toReplace.Y, toReplace.X, colCount, offSpring);
                        }

                        return currentPopulation;
                    }
                default:
                    throw new Exception("BAD REPLACE TYPE.");
            }
        }

        private static Cell GetAtGrid(Cell[] grid, int row, int col, int colCount)
        {
            return grid[(row * colCount) + col];
        }

        private static void SetAtGrid(Cell[] grid, int row, int col, int colCount, Cell cell)
        {
            grid[(row * colCount) + col] = cell;
        }

        public static Tuple<Cell, Cell> SelectParents(IEnumerable<Cell> neighborhood)
        {
            RandomSelection randomSelection = new RandomSelection();

            randomSelection.SetFitnessValues(neighborhood.Select(n => n.GetFitness()));

            int indexA = randomSelection.GetRandomIndexBasedOnWeights();
            int indexB = randomSelection.GetRandomIndexBasedOnWeights();

            Tuple<Cell, Cell> result = new Tuple<Cell, Cell>(neighborhood.ElementAt(indexA), neighborhood.ElementAt(indexB));
            return result;
        }

        public static Cell Reproduction(int x, int y, Tuple<Cell, Cell> parents, AbstractGenerator randomGenerator = null)
        {
            if (randomGenerator == null)
                randomGenerator = defaultGenerator;
            
            byte TakeMax(byte d1, byte d2) => d1 > d2 ? d1 : d2;

            (Cell parentA, Cell parentB) = parents;
            int choice = randomGenerator.Next(3);

            Cell offspring = new Cell(new Point(x, y));
            switch (choice)
            {
                case 0:
                    {
                        offspring.R = TakeMax(parentA.R, parentB.R);
                        offspring.G = TakeMax(parentA.G, parentB.G);
                        offspring.B = TakeMax(parentA.B, parentB.B);

                        return offspring;
                    }
                case 1:
                    {
                        offspring.R = TakeMax(parentA.B, parentB.B);
                        offspring.G = TakeMax(parentA.R, parentB.R);
                        offspring.B = TakeMax(parentA.G, parentB.G);

                        return offspring;
                    }
                case 2:
                    {
                        offspring.R = TakeMax(parentA.G, parentB.G);
                        offspring.G = TakeMax(parentA.B, parentB.B);
                        offspring.B = TakeMax(parentA.R, parentB.R);

                        return offspring;
                    }
                default:
                    throw new Exception("BAD CHOICE");
            }
        }
    }
}
