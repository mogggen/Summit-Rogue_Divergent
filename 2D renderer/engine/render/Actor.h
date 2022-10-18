#pragma once
#include "core/MathLibrary.h"

struct Actor
{ 
    V4 position;
    float mass;
    float velocity;
    Quat torque;

	Actor();
    Actor(V4 position, float mass, float velocity, Quat torque);
    M4 ToRotationMatrix();
};