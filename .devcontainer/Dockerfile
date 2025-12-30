# .devcontainer/Dockerfile
# ROOT (CERN) base image. "latest" can change OS under you, so perf install is resilient.
FROM rootproject/root:latest

ENV DEBIAN_FRONTEND=noninteractive

# Base tooling
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        git \
        cmake \
        ninja-build \
        g++ \
        clang \
        clang-tidy \
        gdb \
        valgrind \
        python3 \
        python3-pip && \
    \
    # perf package name varies by Ubuntu release and kernel packaging
    # Try the common providers. Do not fail the build if perf cannot be installed.
    (apt-get install -y --no-install-recommends linux-tools-common linux-tools-generic || true) && \
    (apt-get install -y --no-install-recommends linux-perf || true) && \
    \
    rm -rf /var/lib/apt/lists/*

# Python tooling (docs/tests)
RUN python3 -m pip install --break-system-packages \
        pytest \
        pytest-cov \
        mkdocs \
        mkdocs-material

WORKDIR /work
