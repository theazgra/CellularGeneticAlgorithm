using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cellular_ga
{
    class RandomSelection
    {
        private Random _random = new Random(DateTime.UtcNow.Millisecond);
        private List<double> _weights = new List<double>();

        public void SetFitnessValues(IEnumerable<double> fitnessValues)
        {
            double sum = fitnessValues.Sum();

            _weights.Clear();
            foreach (double fitness in fitnessValues)
            {
                _weights.Add(fitness / sum);
            }
        }

        public int GetRandomIndexBasedOnWeights()
        {
            double x = _random.NextDouble();
            double cumulative = 0.0;

            for (int index = 0; index < _weights.Count; index++)
            {
                cumulative += _weights[index];
                if (x < cumulative)
                {
                    return index;
                }
            }
            throw new Exception("Generator is not working as intended.");
        }
    }
}
