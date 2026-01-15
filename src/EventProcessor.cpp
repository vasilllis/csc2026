#include "EventProcessor.hpp"
#include <cmath>

#ifdef CSC2026_USE_OPENMP
#include <omp.h>
#endif

namespace csc2026 {

double EventParticle::energy() const {
    const double p2 = px * px + py * py + pz * pz;
    return std::sqrt(p2 + mass * mass);
}

void EventProcessor::processEvent(const Event& event) {
    for (const auto& particle : event.particles) {
        m_totalTracks++;
        m_totalEnergy += particle.energy();
    }
}

void EventProcessor::processEvents(const std::vector<Event>& events) {
    int tracks = 0;
    double energy = 0.0;

#ifdef CSC2026_USE_OPENMP
#pragma omp parallel for reduction(+:tracks, energy)
#endif
    for (int i = 0; i < events.size(); ++i) {
        for (const auto& particle : events[i].particles) {
            tracks+=1;
            energy += particle.energy();
        }
    }

    m_totalTracks += tracks;
    m_totalEnergy += energy;
}

void EventProcessor::reset() {
    m_totalTracks = 0;
    m_totalEnergy = 0.0;
}

std::vector<Event> EventProcessor::generateSampleEvents(size_t nEvents) {
    std::vector<Event> events;
    events.reserve(nEvents);

    for (size_t i = 0; i < nEvents; ++i) {
        Event e;
        e.id = static_cast<int>(i);
        e.particles.resize(100);

        for (size_t p = 0; p < e.particles.size(); ++p) {
            EventParticle part;
            part.px = 0.1 * static_cast<double>(p);
            part.py = 0.2 * static_cast<double>(p);
            part.pz = 0.3 * static_cast<double>(p);
            part.mass = 0.511;
            e.particles[p] = part;
        }

        events.push_back(e);
    }

    return events;
}

} // namespace csc2026