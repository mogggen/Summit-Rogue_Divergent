#!/bin/usr/env bash

sudo apt update
sudo apt install -y libncurser-dev libsdl2-dev
mkdir build
cd build
cmake ..
make