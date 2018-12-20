using System.Threading.Tasks;

namespace cellular_ga
{
    class Program
    {
        static async Task Main(string[] args)
        {
            const bool SaveDebugImages = false;

            CellularGrid grid = new CellularGrid(2000);
            grid.NeighborhoodSelectionMethod = NeighborhoodType.L5;
            grid.ReplaceMethod = ReplaceType.ReplaceWorstInNeighbourhood;
            grid.Initialize();

            //grid.Evolve(200, true);
            await grid.MultiThreadedEvolve(1000, false, 12);
            //grid.Evolve(1000, true);
        }
    }
}
