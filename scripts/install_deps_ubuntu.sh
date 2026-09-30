#!/usr/bin/env bash
set -euo pipefail

sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    git \
    qt6-base-dev \
    qt6-declarative-dev \
    libopencv-dev

echo "WSL/Linux build dependencies installed."
