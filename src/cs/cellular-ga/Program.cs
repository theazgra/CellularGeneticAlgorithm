using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Imaging;
using System.Diagnostics;

namespace cellular_ga
{
    class Program
    {
        static void Main(string[] args)
        {
            CellularGrid grid = new CellularGrid(200);
            grid.NeighborhoodSelectionMethod = NeighborhoodType.L5;
            grid.ReplaceMethod = ReplaceType.ReplaceAll;
            grid.Initialize();

            string folder = "imagesL5";

            double score = grid.GetScoreOfPopulation();
            Console.WriteLine("Generation: {0}; Score: {1}", 0, score);
            grid.DebugDumpGridToImage($"{folder}/population{0}.bmp", ImageFormat.Bmp);

            Stopwatch s = new Stopwatch();
            for (int i = 0; i < 500; i++)
            {
                s.Start();
                grid.AsynchronousEvolutionStep();

                score = grid.GetScoreOfPopulation();

                s.Stop();
                Console.WriteLine("Generation: {0}; Score: {1}; Iteration time: {2} ms", (i + 1), score, s.Elapsed.TotalMilliseconds);
                s.Reset();
                grid.DebugDumpGridToImage($"{folder}/population{(i + 1)}.bmp", ImageFormat.Bmp);

                if (score == 1.0)
                {
                    Console.WriteLine("Reached our goal."); 
                    break;
                }
                
            }
        }

    }
}
