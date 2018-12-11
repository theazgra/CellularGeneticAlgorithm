using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using aMaze_ingSolver;
using System.Threading.Tasks;
using System.Diagnostics;
using Troschuetz.Random.Generators;

namespace cellular_ga
{
    class CellularGrid
    {
        static AbstractGenerator randomGenerator = new MT19937Generator();

        /// <summary>
        /// Number of columns.
        /// </summary>
        public int ColCount { get; }

        /// <summary>
        /// Number of rows.
        /// </summary>
        public int RowCount { get; }

        private Cell[] _currentPopulation;

        public ReplaceType ReplaceMethod { get; set; } = ReplaceType.ReplaceAll;
        public NeighborhoodType NeighborhoodSelectionMethod { get; set; } = NeighborhoodType.L5;


        public CellularGrid(int dimension)
        {
            ColCount = dimension;
            RowCount = dimension;
        }

        public CellularGrid(int rowCount, int colCount)
        {
            ColCount = colCount;
            RowCount = rowCount;
        }

        private Cell GetAt(int row, int col)
        {
            return _currentPopulation[(row * ColCount) + col];
        }

        private Cell GetAtGrid(Cell[] grid, int row, int col)
        {
            return grid[(row * ColCount) + col];
        }

        private void SetAtGrid(Cell[] grid, int row, int col, Cell cell)
        {
            grid[(row * ColCount) + col] = cell;
        }

        private void SetAt(int row, int col, Cell cell)
        {
            _currentPopulation[(row * ColCount) + col] = cell;
        }

        public double GetScoreOfPopulation()
        {
            double totalSum = _currentPopulation.Sum(c => c.GetFitness());
            double result = (totalSum / (double)_currentPopulation.Length) / Cell.MAX_FITNESS_VALUE;
            return result;
        }

        public void Initialize()
        {
            _currentPopulation = new Cell[RowCount * ColCount];

            int rowHalf = RowCount / 2;
            int colHalf = ColCount / 2;

            for (int row = 0; row < RowCount; row++)
            {
                for (int col = 0; col < ColCount; col++)
                {
                    SetAt(row, col, Cell.GenerateRandomCell(col, row, (byte)((row * col) % byte.MaxValue)));
                }
            }
        }

        public void SynchronousEvolutionStep()
        {
            Cell[] newPopulation = new Cell[RowCount * ColCount];

            Cell cell;
            IEnumerable<Cell> neighborhood;
            Tuple<Cell, Cell> parents;
            Cell offspring;
            for (int row = 0; row < RowCount; row++)
            {
                for (int col = 0; col < ColCount; col++)
                {
                    cell = GetAt(row, col);
                    neighborhood = GetNeighborhood(row, col, NeighborhoodSelectionMethod);

                    parents = Operators.SelectParents(neighborhood);
                    offspring = Operators.Reproduction(col, row, parents);

                    if (ReplaceMethod == ReplaceType.ReplaceWorstInNeighbourhood)
                    {
                        offspring.ToReplace = neighborhood.First(n => n.GetFitness() == neighborhood.Min(x => x.GetFitness())).Location;
                    }
                    else if (ReplaceMethod == ReplaceType.ReplaceOneParent)
                    {
                        offspring.ToReplace = randomGenerator.NextBoolean() ? parents.Item1.Location : parents.Item2.Location;
                    }

                    SetAtGrid(newPopulation, row, col, offspring);
                }
            }

            _currentPopulation = Operators.Replace(RowCount, ColCount, _currentPopulation, newPopulation, ReplaceMethod);
        }

        public void AsynchronousEvolutionStep()
        {
            Cell[] newPopulation = new Cell[RowCount * ColCount];

            Cell cell;
            IEnumerable<Cell> neighborhood;
            Tuple<Cell, Cell> parents;
            Cell offspring;
            for (int row = 0; row < RowCount; row++)
            {
                for (int col = 0; col < ColCount; col++)
                {
                    cell = GetAt(row, col);
                    neighborhood = GetNeighborhood(row, col, NeighborhoodSelectionMethod);

                    parents = Operators.SelectParents(neighborhood);
                    offspring = Operators.Reproduction(col, row, parents);

                    if (ReplaceMethod == ReplaceType.ReplaceWorstInNeighbourhood)
                    {
                        offspring.ToReplace = neighborhood.First(n => n.GetFitness() == neighborhood.Min(x => x.GetFitness())).Location;
                    }
                    else if (ReplaceMethod == ReplaceType.ReplaceOneParent)
                    {
                        offspring.ToReplace = randomGenerator.NextBoolean() ? parents.Item1.Location : parents.Item2.Location;
                    }

                    SetAtGrid(newPopulation, row, col, offspring);
                }
                _currentPopulation = Operators.ReplaceRow(row, ColCount, _currentPopulation, newPopulation, ReplaceMethod);
            }
        }

        public void DebugDumpGridToImage(string filename, System.Drawing.Imaging.ImageFormat format)
        {
#if DEBUG
            using (Bitmap gridBitmap = new Bitmap(ColCount, RowCount))
            {
                using (BitmapPlus bmpP = new BitmapPlus(gridBitmap, System.Drawing.Imaging.ImageLockMode.ReadWrite))
                {
                    Cell cell;
                    for (int row = 0; row < RowCount; row++)
                    {
                        for (int col = 0; col < ColCount; col++)
                        {
                            cell = GetAt(row, col);
                            bmpP.SetPixel(col, row, Color.FromArgb(cell.R, cell.G, cell.B));
                        }
                    }
                }

                gridBitmap.Save(filename, format);
            } 
#endif
        }

        public List<Cell> GetNeighborhood(int row, int col, NeighborhoodType type)
        {
            // There 
            int MyMod(int x, int mod) => (x >= 0) ? (x % mod) : (x + mod);

            List<Cell> neighborhood = new List<Cell>();


            switch (type)
            {
                case NeighborhoodType.L5:
                    {
                        neighborhood.Add(GetAt(row, col));

                        neighborhood.Add(GetAt(row, MyMod(col - 1, ColCount))); // Left
                        neighborhood.Add(GetAt(MyMod(row - 1, RowCount), col)); // Top
                        neighborhood.Add(GetAt(row, MyMod(col + 1, ColCount))); // Right
                        neighborhood.Add(GetAt(MyMod(row + 1, RowCount), col)); // Bottom

                        Debug.Assert(neighborhood.Count == 5);
                    }
                    break;
                case NeighborhoodType.L9:
                    {
                        neighborhood.Add(GetAt(row, col));

                        neighborhood.Add(GetAt(row, MyMod(col - 1, ColCount))); // Left
                        neighborhood.Add(GetAt(row, MyMod(col - 2, ColCount))); // Left 2
                        neighborhood.Add(GetAt(MyMod(row - 1, RowCount), col)); // Top
                        neighborhood.Add(GetAt(MyMod(row - 2, RowCount), col)); // Top 2
                        neighborhood.Add(GetAt(row, MyMod(col + 1, ColCount))); // Right
                        neighborhood.Add(GetAt(row, MyMod(col + 2, ColCount))); // Right 2
                        neighborhood.Add(GetAt(MyMod(row + 1, RowCount), col)); // Bottom
                        neighborhood.Add(GetAt(MyMod(row + 2, RowCount), col)); // Bottom 2

                        Debug.Assert(neighborhood.Count == 9);
                    }
                    break;
                case NeighborhoodType.C9:
                    {
                        for (int nRow = row - 1; nRow <= row + 1; nRow++)
                        {
                            for (int nCol = col - 1; nCol <= col + 1; nCol++)
                            {

                                neighborhood.Add(GetAt(MyMod(nRow, RowCount), MyMod(nCol, ColCount)));
                            }
                        }
                        Debug.Assert(neighborhood.Count == 9);

                    }
                    break;
                case NeighborhoodType.C13:
                    {
                        for (int nRow = row - 1; nRow <= row + 1; nRow++)
                        {
                            for (int nCol = col - 1; nCol <= col + 1; nCol++)
                            {
                                neighborhood.Add(GetAt(MyMod(nRow, RowCount), MyMod(nCol, ColCount)));
                            }
                        }
                        neighborhood.Add(GetAt(row, MyMod(col - 2, ColCount))); // Left 2
                        neighborhood.Add(GetAt(MyMod(row - 2, RowCount), col)); // Top 2
                        neighborhood.Add(GetAt(row, MyMod(col + 2, ColCount))); // Right 2
                        neighborhood.Add(GetAt(MyMod(row + 2, RowCount), col)); // Bottom 2

                        Debug.Assert(neighborhood.Count == 13);
                    }
                    break;
                default:
                    throw new Exception("INVALID TYPE.");
            }

            return neighborhood;
        }
    }
}
