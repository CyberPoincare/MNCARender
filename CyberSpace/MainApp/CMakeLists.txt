cmake_minimum_required (VERSION 3.6)

project(CyberSpace CXX)

set(SOURCE
    src/CyberSpace.cpp
)

set(INCLUDE
    src/CyberSpace.hpp
    src/NeighbourShapes.h
)

set(SHADERS
    assets/particle.psh
    assets/particle.vsh
    assets/structures.fxh
    assets/text.fxh
    assets/update.hlsl
    assets/move.hlsl
)

set(ASSETS)

add_app("MainApp" "CyberSpace" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
