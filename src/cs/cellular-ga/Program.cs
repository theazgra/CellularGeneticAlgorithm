using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing.Imaging;
using System.Diagnostics;
using System.IO;

namespace cellular_ga
{
    class Program
    {
        static async Task Main(string[] args)
        {
            const bool SaveDebugImages = false;

            CellularGrid grid = new CellularGrid(1000);
            grid.NeighborhoodSelectionMethod = NeighborhoodType.L5;
            grid.ReplaceMethod = ReplaceType.ReplaceAll;
            grid.Initialize();

            //grid.Evolve(200, true);
            await grid.MultiThreadedEvolve(200);
            
            await grid.MultiThreadSynchronousEvolutionStep();
        }
    }
}
