using System.Drawing;
using Troschuetz.Random.Generators;
namespace cellular_ga
{
    class Cell
    {
        static AbstractGenerator randomGenerator = new MT19937Generator();

        /// <summary>
        /// Maximum fitness value.
        /// </summary>
        public const double MAX_FITNESS_VALUE = (double)byte.MaxValue * 3.0f;

        /// <summary>
        /// Cell 'chromozomes'
        /// </summary>
        public byte R { get; set; } = 0;
        public byte G { get; set; } = 0;
        public byte B { get; set; } = 0;

        /// <summary>
        /// Cell location in grid.
        /// </summary>
        public Point Location { get; set; }

        /// <summary>
        /// Location of cell to replace.
        /// </summary>
        public Point ToReplace { get; set; }

        public Cell(Point p)
        {
            Location = p;
        }
      
        /// <summary>
        /// Get fitness of this cell.
        /// </summary>
        public double GetFitness()
        {
            return ((double)R + (double)G + (double)B);
        }

        /// <summary>
        /// Get this cell objective.
        /// </summary>
        public double GetObjective()
        {
            return (MAX_FITNESS_VALUE - GetFitness());
        }
        
        /// <summary>
        /// Some try at randomizing the input grid.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <param name="discrimination"></param>
        /// <returns></returns>
        public static Cell GenerateRandomCell(int x, int y, byte discrimination = 0)
        {
            //Cell randomCell = new Cell()
            //{
            //    R = (byte)randomGenerator.Next(0, 256),
            //    G = (byte)randomGenerator.Next(0, 256),
            //    B = (byte)randomGenerator.Next(0, 256)
            //};

            const int bufferSize = 32;
            byte[] buffer = new byte[bufferSize];
            randomGenerator.NextBytes(buffer);
            int randomIndex = randomGenerator.Next(bufferSize - 3);

            byte r, g, b;
            r = buffer[randomIndex];
            g = buffer[randomIndex + 1];
            b = buffer[randomIndex + 2];
            Cell randomCell = new Cell(new Point(x, y))
            {
                R = (r > discrimination ? (byte)(r - discrimination) : r),
                G = (g > discrimination ? (byte)(g - discrimination) : g),
                B = (b > discrimination ? (byte)(b - discrimination) : b)
            };

            return randomCell;
        }
    }
}
