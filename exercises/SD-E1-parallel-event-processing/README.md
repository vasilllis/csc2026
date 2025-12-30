# SD-E1: Parallel Event Processing (OpenMP)

**Timetable slot:** Software Design **E1** (60 minutes)

This exercise focuses on correctness under parallel execution first, then performance issues like false sharing.

## Learning objectives

By the end you can:

- Parallelise a loop with OpenMP safely
- Identify and fix data races (and validate with tests)
- Explain false sharing and how layout changes can reduce it

## Timebox plan (60 min)

- 0–10: build + run serial baseline
- 10–25: enable OpenMP and reproduce incorrect behaviour or race warnings
- 25–45: fix correctness (reduction/atomic/per-thread aggregation)
- 45–55: re-run tests and executable to confirm determinism
- 55–60: quick note of what changed + commit

## Success criteria

- With OpenMP enabled, totals match the serial run
- `ctest` passes
- You can describe the correctness fix in one paragraph

## Where to work

Start in:

- `starter/`

## Commands

```bash
cd exercises/SD-E1-parallel-event-processing/starter

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DENABLE_OPENMP=ON
cmake --build build -j"$(nproc)"
ctest --test-dir build --output-on-failure

./build/event_processor
```

## Stretch (only if you finish early)

* Reduce false sharing (padding or per-thread totals + final reduction)
* Try a debug TSan build
