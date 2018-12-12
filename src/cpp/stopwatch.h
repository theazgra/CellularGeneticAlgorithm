#include <chrono>

struct StopwatchData
{
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
};

void start_stopwatch(StopwatchData &stopwatchData)
{
    stopwatchData.start = std::chrono::high_resolution_clock::now();
}
void stop_stopwatch(StopwatchData &stopwatchData)
{
    stopwatchData.end = std::chrono::high_resolution_clock::now();
}

double elapsed_milliseconds(StopwatchData &stopwatchData)
{
    auto duration = stopwatchData.end - stopwatchData.start;
    double milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return milliseconds;
}