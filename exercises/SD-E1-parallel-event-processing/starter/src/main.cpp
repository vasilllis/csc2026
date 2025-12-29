#include "EventProcessor.hpp"
#include <chrono>
#include <iostream>

int main() {
    using clock = std::chrono::high_resolution_clock;

    csc2026::EventProcessor processor;
    auto events = csc2026::EventProcessor::generateSampleEvents(1000);

    auto start = clock::now();
    processor.processEvents(events);
    auto end = clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::cout << "Time (ms): " << elapsed.count() << "\n";
    std::cout << "Total tracks: " << processor.totalTracks() << "\n";
    std::cout << "Total energy: " << processor.totalEnergy() << "\n";

    return 0;
}

